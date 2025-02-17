#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include "TFT_eSPI.h"
#include "nixieClockConfig.h"
#include <time.h>

// Configuración del servidor web
WebServer server(80);

// Variables para WiFi y zona horaria
String newSSID = "";
String newPassword = "";
String newTimezone = "";

// Variables para el estilo del reloj
bool estiloNixie = true;
bool mostrarImagenFija = true;

// Definiciones de pines
#define digit1 21
#define digit2 22
#define digit3 17
#define digit4 16
#define digit5 5
#define BOTON 33

#define PIN_HIGH true
#define PIN_LOW false

// Variables y constantes
const char* ssid = "ETT IKASLEAK";
const char* password = "1Kasl3@na1z";
const char* dstRule = "CET-1CEST,M3.5.0,M10.5.0/3";

int minutesUnits, minutesTens, hoursUnits, hoursTens, nmins, nhour;
int oldMinuteUnits = 99, oldMinutesTens = 99, oldHoursUnits = 99, oldHoursTens = 99;
time_t currentTime;
bool pulsado = false;

unsigned long currentMillis;
unsigned long lastUpdateTime;
unsigned long updateInterval = 30000;

// Incluir datos de dígitos
#include "nixieFont.h"
#include "nenonFont.h"
#include "imagenFija.h"

static const unsigned short* nixieNumeral[] = {
  Nx0, Nx1, Nx2, Nx3, Nx4, Nx5, Nx6, Nx7, Nx8, Nx9,
};
static const unsigned short* nenonNumeral[] = {
  Nn0, Nn1, Nn2, Nn3, Nn4, Nn5, Nn6, Nn7, Nn8, Nn9,
};

// Inicializar funciones
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  pinMode(digit5, OUTPUT);
  pinMode(BOTON, INPUT);

  enableDisplays();
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.println(F("Connecting to "));
  tft.setTextFont(2);
  tft.println(ssid);

  WiFi.setHostname("NIXIE_CLOCK");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(F("."));
  }
  tft.println("");
  tft.println(F("WiFi connected."));
  tft.println(F("IP address: "));
  tft.println(WiFi.localIP());

  disableDisplays();

  initTime(dstRule);

  lastUpdateTime = millis();

  mostrarImagen();

  // Configurar rutas del servidor web
  server.on("/", handleRoot);
  server.on("/changeWiFi", handleChangeWiFi);
  server.on("/changeTimezone", handleChangeTimezone);
  server.on("/changeStyle", handleChangeStyle);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  currentMillis = millis();

  if (digitalRead(BOTON) == LOW) {
    pulsado = true;
  }
  if (digitalRead(BOTON) == HIGH && pulsado == true) {
    pulsado = false;
    Cambio_de_estilo();
  }

  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;
    initTime(dstRule);
  }

  timeVariablesSet();

  minutesUnits = nmins % 10;
  minutesTens = (nmins / 10) % 10;
  hoursUnits = nhour % 10;
  hoursTens = (nhour / 10) % 10;

  if (oldMinuteUnits != minutesUnits) {
    loadDigit(digit4, minutesUnits);
    oldMinuteUnits = minutesUnits;
  }

  if (oldMinutesTens != minutesTens) {
    loadDigit(digit3, minutesTens);
    oldMinutesTens = minutesTens;
  }

  if (oldHoursUnits != hoursUnits) {
    loadDigit(digit2, hoursUnits);
    oldHoursUnits = hoursUnits;
  }

  if (oldHoursTens != hoursTens) {
    loadDigit(digit1, hoursTens);
    oldHoursTens = hoursTens;
  }

  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "Bienvenido al reloj Nixie");
}

void handleChangeWiFi() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    newSSID = server.arg("ssid");
    newPassword = server.arg("password");

    WiFi.disconnect();
    delay(100);
    WiFi.begin(newSSID.c_str(), newPassword.c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi cambiado correctamente.");
    Serial.println("Nueva IP: " + WiFi.localIP().toString());

    server.send(200, "text/plain", "WiFi cambiado correctamente");
  } else {
    server.send(400, "text/plain", "Faltan parámetros (ssid o password)");
  }
}

void handleChangeTimezone() {
  if (server.hasArg("timezone")) {
    newTimezone = server.arg("timezone");

    setTimezone(newTimezone);
    initTime(newTimezone);

    server.send(200, "text/plain", "Zona horaria cambiada correctamente");
  } else {
    server.send(400, "text/plain", "Falta el parámetro (timezone)");
  }
}

void handleChangeStyle() {
  estiloNixie = !estiloNixie;
  mostrarImagenFija = !mostrarImagenFija;

  Cambio_de_estilo();

  server.send(200, "text/plain", "Estilo cambiado correctamente");
}

void Cambio_de_estilo() {
  loadDigit(digit1, hoursTens);
  loadDigit(digit2, hoursUnits);
  loadDigit(digit3, minutesTens);
  loadDigit(digit4, minutesUnits);

  mostrarImagen();
}

void initTime(String timezone) {
  struct tm timeinfo;
  configTime(0, 0, "pool.ntp.org");
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora");
    return;
  }
  setTimezone(timezone);
}

void timeVariablesSet() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("timeVariableSet: Failed to obtain time");
    return;
  }

  nmins = (timeinfo.tm_min);
  nhour = (timeinfo.tm_hour);
}

void setTimezone(String timezone) {
  setenv("TZ", timezone.c_str(), 1);
  tzset();
}

void loadDigit(int displayPin, int numeral) {
  digitalWrite(displayPin, PIN_LOW);
  if (estiloNixie) {
    tft.pushImage(0, 0, 135, 240, nixieNumeral[numeral]);
  } else {
    tft.pushImage(0, 0, 135, 240, nenonNumeral[numeral]);
  }
  digitalWrite(displayPin, PIN_HIGH);
}

void mostrarImagen() {
  digitalWrite(digit5, PIN_LOW);
  if (mostrarImagenFija) {
    tft.pushImage(0, 0, 135, 240, imagenFija);
  } else {
    tft.pushImage(0, 0, 135, 240, Nns);
  }
  digitalWrite(digit5, PIN_HIGH);
}

void enableDisplays() {
  digitalWrite(digit1, PIN_LOW);
  digitalWrite(digit2, PIN_LOW);
  digitalWrite(digit3, PIN_LOW);
  digitalWrite(digit4, PIN_LOW);
}

void disableDisplays() {
  digitalWrite(digit1, PIN_HIGH);
  digitalWrite(digit2, PIN_HIGH);
  digitalWrite(digit3, PIN_HIGH);
  digitalWrite(digit4, PIN_HIGH);
}