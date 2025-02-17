//====================================================================================
//                                  Definitions
//====================================================================================

#define digit1 21  // 1xxx Digit Enable Active Low
#define digit2 22  // x2xx Digit Enable Active Low
#define digit3 17  // xx3x Digit Enable Active Low
#define digit4 16  // xxx4 Digit Enable Active Low
#define digit5 5   // Quinta pantalla para la imagen fija

#define PIN_HIGH true
#define PIN_LOW false

int BOTON = 33;

//====================================================================================
//                                  Digit data includes
//====================================================================================

#include "nixieFont.h"  // Fuente 1
#include "nenonFont.h"  // Fuente 2
#include "imagenFija.h" // Imágenes (imagenFija y Nns)

//====================================================================================
//                                  Libraries
//====================================================================================
#include <SPI.h>               // required for TFT_eSPI (Displays)
#include "TFT_eSPI.h"          // Hardware-specific library for ESP32
#include "nixieClockConfig.h"  // The file with your WIFI credentials
#include <time.h>              // time library functions
#include <WiFi.h>              // WiFi library

//====================================================================================
//                                    Variables and Constants
//====================================================================================
const char* ssid = "ETT IKASLEAK";
const char* password = "1Kasl3@na1z";
const char* dstRule = "CET-1CEST,M3.5.0,M10.5.0/3";  // daylight saving code from nixieClockConfig.h

int minutesUnits, minutesTens, hoursUnits, hoursTens, nmins, nhour;
int oldMinuteUnits = 99, oldMinutesTens = 99, oldHoursUnits = 99, oldHoursTens = 99;
time_t currentTime;
bool pulsado = false;
bool estiloNixie = true;  // True = Nixie, False = Nenon
bool mostrarImagenFija = true;  // Controla si se muestra imagenFija o Nns

unsigned long currentMillis;
unsigned long lastUpdateTime;
unsigned long updateInterval = 30000;  // this equates to 30 secs

// Digit are defined in "nixieFont.h"
static const unsigned short* nixieNumeral[] = {
  Nx0,
  Nx1,
  Nx2,
  Nx3,
  Nx4,
  Nx5,
  Nx6,
  Nx7,
  Nx8,
  Nx9,
};
static const unsigned short* nenonNumeral[] = {
  Nn0,
  Nn1,
  Nn2,
  Nn3,
  Nn4,
  Nn5,
  Nn6,
  Nn7,
  Nn8,
  Nn9,
};

//====================================================================================
//                                    Initialise Functions
//====================================================================================
WiFiServer server(80);
TFT_eSPI tft = TFT_eSPI();  // Pantalla principal (para el reloj)

//====================================================================================
//                                    Setup
//====================================================================================
void setup() {
  Serial.begin(115200);  // Used for debug messages
  delay(100);

  Serial.println("Fake Nixie Clock");

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  pinMode(digit5, OUTPUT);  // Quinta pantalla para la imagen fija
  pinMode(BOTON, INPUT);

  // Inicializar la pantalla principal (para el reloj)
  enableDisplays();
  tft.begin();
  tft.setRotation(0);         // 0 & 2 Portrait. 1 & 3 landscape
  tft.fillScreen(TFT_BLACK);  // Clear the screens
  tft.setSwapBytes(true);     // Requred for the 1.14" IPS SPI (8pin) LCD displays from Aliexpress
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.println(F("Connecting to "));
  tft.setTextFont(2);
  tft.println(ssid);

  WiFi.setHostname("NIXIE_CLOCK");  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(F("."));
  }
  tft.println("");
  tft.println(F("WiFi connected."));
  tft.println(F("IP address: "));
  tft.println(WiFi.localIP());

  disableDisplays();  // Deshabilitar las pantallas 1-4 por defecto

  initTime(dstRule);  // Set timezone and daylight saving for London

  lastUpdateTime = millis();

  // Mostrar la imagen fija inicial en la quinta pantalla
  mostrarImagen();
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop() {
  currentMillis = millis();

  if (digitalRead(BOTON) == LOW) {
    pulsado = true;
  }
  if (digitalRead(BOTON) == HIGH && pulsado == true) {
    pulsado = false;
    Cambio_de_estilo();  // Cambiar el estilo de reloj y la imagen fija
  }

  // Actualización de hora, según el código existente
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;
    initTime(dstRule);  // Actualizar hora desde el servidor NTP
  }

  timeVariablesSet();  // This function sets the nsecs, nmins, nhour variables from system time

  minutesUnits = nmins % 10;        // extract the unit digit of the number of minutes from nmins
  minutesTens = (nmins / 10) % 10;  // extract the tens digit of the number of minutes from nmins
  hoursUnits = nhour % 10;          // extract the unit digit of the number of hours from nhour
  hoursTens = (nhour / 10) % 10;    // extract the tens digit of the number of hours from nhour

  if (oldMinuteUnits != minutesUnits) {
    loadDigit(digit4, minutesUnits);  // Load Digit to display
    oldMinuteUnits = minutesUnits;
  }

  if (oldMinutesTens != minutesTens) {
    loadDigit(digit3, minutesTens);  // Load Digit to display
    oldMinutesTens = minutesTens;
  }

  if (oldHoursUnits != hoursUnits) {
    loadDigit(digit2, hoursUnits);  // Load Digit to display
    oldHoursUnits = hoursUnits;
  }

  if (oldHoursTens != hoursTens) {
    loadDigit(digit1, hoursTens);  // Load Digit to display
    oldHoursTens = hoursTens;
  }
}

//====================================================================================
//                                 initTime
//====================================================================================
void initTime(String timezone) {
  struct tm timeinfo;
  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");  // First connect to NTP server, with 0 TZ offset
  if (!getLocalTime(&timeinfo)) {
    Serial.println("initTime: Failed to obtain time");
    return;
  }
  setTimezone(timezone);
}

//====================================================================================
//                                 timeVariablesSet
//====================================================================================
void timeVariablesSet() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("timeVariableSet: Failed to obtain time");
    return;
  }

  nmins = (timeinfo.tm_min);
  nhour = (timeinfo.tm_hour);
}

//====================================================================================
//                                 Set Time Zone
//====================================================================================
void setTimezone(String timezone) {
  setenv("TZ", timezone.c_str(), 1);  // Now adjust the TZ. Clock settings are adjusted to show the new local time
  tzset();
}

//====================================================================================
//                                 Display digit on LCD x
//====================================================================================
void loadDigit(int displayPin, int numeral) {
  digitalWrite(displayPin, PIN_LOW);  // Enable the display to be updated
  if (estiloNixie) {
    tft.pushImage(0, 0, 135, 240, nixieNumeral[numeral]);  // display correct image
  } else {
    tft.pushImage(0, 0, 135, 240, nenonNumeral[numeral]);  // display correct image
  }
  digitalWrite(displayPin, PIN_HIGH);  // Disable the display after update
}

//====================================================================================
//                                 Mostrar imagen en la quinta pantalla
//====================================================================================
void mostrarImagen() {
  digitalWrite(digit5, PIN_LOW);  // Habilitar la quinta pantalla
  if (mostrarImagenFija) {
    tft.pushImage(0, 0, 135, 240, imagenFija);  // Mostrar imagenFija
  } else {
    tft.pushImage(0, 0, 135, 240, Nns);  // Mostrar Nns
  }
  digitalWrite(digit5, PIN_HIGH);  // Deshabilitar la quinta pantalla
}

//====================================================================================
//                                 Cambiar el estilo
//====================================================================================
void Cambio_de_estilo() {
  estiloNixie = !estiloNixie;  // Alternar entre Nixie y Nenon
  mostrarImagenFija = !mostrarImagenFija;  // Alternar entre imagenFija y Nns

  // Actualizar el reloj en las pantallas 1-4
  loadDigit(digit1, hoursTens);
  loadDigit(digit2, hoursUnits);
  loadDigit(digit3, minutesTens);
  loadDigit(digit4, minutesUnits);

  // Actualizar la imagen en la quinta pantalla
  mostrarImagen();
}

//====================================================================================
//                                 Enable all displays
//====================================================================================
void enableDisplays() {
  digitalWrite(digit1, PIN_LOW);
  digitalWrite(digit2, PIN_LOW);
  digitalWrite(digit3, PIN_LOW);
  digitalWrite(digit4, PIN_LOW);
}

//====================================================================================
//                                 Disable all displays
//====================================================================================
void disableDisplays() {
  digitalWrite(digit1, PIN_HIGH);  // Deshabilitar todas las pantallas
  digitalWrite(digit2, PIN_HIGH);
  digitalWrite(digit3, PIN_HIGH);
  digitalWrite(digit4, PIN_HIGH);
}