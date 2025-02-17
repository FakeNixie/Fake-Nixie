//====================================================================================
//                                  WIFI
//====================================================================================


//====================================================================================
//                                  DEFINIZOAK
//====================================================================================

#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include "TFT_eSPI.h"
#include <time.h>
#include <Preferences.h>  //  NVS memoria godetrzeko libreria (memoria persistente)

#define digit1 21  // 1xxx Digit Enable Active Low
#define digit2 19  // x2xx Digit Enable Active Low
#define digit3 17  // xx3x Digit Enable Active Low
#define digit4 16  // xxx4 Digit Enable Active Low
#define digit5 5   // irudi finkorako pantaila (bi puntuak)

#define PIN_HIGH true
#define PIN_LOW false

int BOTON = 33;

//====================================================================================
//                                  DIGITOEN DATA
//====================================================================================

#include "nixieFont.h"  // estilo 1
#include "nenonFont.h"  // estilo 2
#include "imagenFija.h" // irduiak (imagenFija y Nns)
  
//====================================================================================
//                                  LIBRERIAK
//====================================================================================
#include <SPI.h>               // Beharrezkoa TFT_eSPI (Displays)
#include "TFT_eSPI.h"          // ESP32rako libreria espezifikoa.
#include "nixieClockConfig.h"  // Artxiboa zure WIFI kredentzialekin.
#include <time.h>              // Denbora funtzio libreria
#include <WiFi.h>              // WiFi libreria

//====================================================================================
//                                    BARIABLEAK ETA KONSTANTEAK
//====================================================================================
//KONEXIO KREDENTZIALAK
//const char* ssid = "ETT IKASLEAK";
//const char* password = "1Kasl3@na1z";
const char* dstRule = "CET-1CEST,M3.5.0,M10.5.0/3";  // Ordu zona gordetzeko kodea -> nixieClockConfig.h
//
int minutesUnits, minutesTens, hoursUnits, hoursTens, nmins, nhour;
int oldMinuteUnits = 99, oldMinutesTens = 99, oldHoursUnits = 99, oldHoursTens = 99;
time_t currentTime;
bool pulsado = false;
bool estiloNixie = true;  // True = Nixie, False = Nenon
bool mostrarImagenFija = true;  // imagenFija edo Nns erakusten duen kontrolatzen du

unsigned long currentMillis;
unsigned long lastUpdateTime;
unsigned long updateInterval = 30000;  // 30s ekibalentea

//  "nixieFont.h" daude definitututa digitoak
static const unsigned short* nixieNumeral[] = { Nx0, Nx1, Nx2, Nx3, Nx4, Nx5, Nx6, Nx7, Nx8, Nx9,};
static const unsigned short* nenonNumeral[] = { Nn0, Nn1, Nn2, Nn3, Nn4, Nn5, Nn6, Nn7, Nn8, Nn9,};

//====================================================================================
//                                   WIFI BARIABLEAK
//====================================================================================
const char* apSSID = "FAKE NIXIE";
const char* apPassword = "12345678";

char wifiSSID[32] = "";
char wifiPassword[32] = "";
bool credentialsReceived = false;

WebServer server(80);
TFT_eSPI tft = TFT_eSPI();
Preferences preferences; // Manejo de memoria persistente

//====================================================================================
//                                   WEB INTERFAZA
//====================================================================================
void startWebInterface() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Iniciando interfaz web para configuración...");

        // Defektuzko ruta"/" → Redirige a "/config"
        server.on("/datetime", HTTP_GET, []() {
          server.send(200, "text/plain", obtenerFechaHora());
        });


        //Erlojua konfiguratzeko orria, estiloa aldatzeko botoiarekin
        server.on("/config", HTTP_GET, []() {
          String html = R"rawliteral(
              <!DOCTYPE html>
              <html lang="en">
              <head>
                  <meta charset="UTF-8">
                  <meta name="viewport" content="width=device-width, initial-scale=1.0">
                  <title>Fake Nixie Clock</title>
                  <style>
                      body {
                          background-color: #111;
                          color: #ff9800;
                          font-family: 'Courier New', Courier, monospace;
                          text-align: center;
                          padding: 20px;
                      }
                      h1 {
                          font-size: 3em;
                          margin-bottom: 10px;
                      }
                      .date {
                          font-size: 2em;
                          font-weight: bold;
                          margin-bottom: 10px;
                      }
                      .clock {
                          font-size: 4em;
                          font-weight: bold;
                          letter-spacing: 10px;
                          text-shadow: 0 0 10px #ff6d00, 0 0 20px #ff9800, 0 0 30px #ff3d00;
                      }
                      .container {
                          display: flex;
                          flex-direction: column;
                          align-items: center;
                          justify-content: center;
                          height: 70vh;
                      }
                      button {
                          background-color: #ff6d00;
                          border: none;
                          color: white;
                          padding: 15px 30px;
                          font-size: 1.2em;
                          cursor: pointer;
                          margin-top: 20px;
                          border-radius: 8px;
                          text-transform: uppercase;
                          transition: all 0.3s ease;
                      }
                      button:hover {
                          background-color: #ff3d00;
                          transform: scale(1.1);
                      }
                  </style>
              </head>
              <body>
                  <h1>FAKE NIXIE</h1>
                  <div class="container">
                      <div id="date" class="date">Loading date...</div> <!-- Aquí se mostrará la fecha -->
                      <div id="clock" class="clock">Loading time...</div> <!-- Aquí se mostrará la hora -->
                      <form action="/cambiar_estilo" method="POST">
                          <button type="submit">Change Style</button>
                      </form>
                  </div>
                  <script>
                      function actualizarFechaHora() {
                          fetch('/datetime')  // Solicita la fecha y hora al ESP32
                          .then(response => response.text())
                          .then(data => {
                              let partes = data.split('|');  // Divide la respuesta en fecha y hora
                              document.getElementById('date').innerText = partes[0]; // Fecha
                              document.getElementById('clock').innerText = partes[1]; // Hora
                          })
                          .catch(error => console.log("Error obteniendo la fecha y hora:", error));
                      }
                      setInterval(actualizarFechaHora, 1000);  // Actualizar cada segundo
                      actualizarFechaHora();  // Llamar al inicio
                  </script>
              </body>
              </html>
          )rawliteral";

          server.send(200, "text/html", html);
      });



        // Ordu zona gordetzeko
        server.on("/update", HTTP_POST, []() {
            if (server.hasArg("timezone")) {
                String newTimezone = server.arg("timezone");

                // Memoria gorde NVS
                preferences.begin("settings", false);
                preferences.putString("timezone", newTimezone);
                preferences.end();

                // Aldaketa aplikatzeko
                setTimezone(newTimezone);

                server.send(200, "text/html", "<h2>Zona horaria actualizada.</h2><a href='/config'>Volver</a>");
            } else {
                server.send(400, "text/plain", "Faltan parámetros.");
            }
        });

        // Erlojuaren estiloa aldatzeko botoia
        server.on("/cambiar_estilo", HTTP_POST, []() {
            Cambio_de_estilo();  // Llama a la función para alternar el estilo
            server.send(200, "text/html", "<h2>Estilo cambiado.</h2><a href='/config'>Volver</a>");
        });

        server.begin();
    }
}
//====================================================================================
//                            WIFI && ACCESS POINT FUNTZIOAK
//====================================================================================

void startAccessPoint() {
    WiFi.softAP(apSSID, apPassword);
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", "<html><body><h2>WiFi Configuration</h2><form action='/connect' method='POST'>"
                    "SSID: <input type='text' name='ssid'><br>"
                    "Password: <input type='password' name='password'><br>"
                    "<input type='submit' value='Connect'></form></body></html>");
    });

    server.on("/connect", HTTP_POST, []() {
    if (server.hasArg("ssid") && server.hasArg("password")) {
        strcpy(wifiSSID, server.arg("ssid").c_str());
        strcpy(wifiPassword, server.arg("password").c_str());
        credentialsReceived = true;
        
        //  NVS irekitzen dugu idazketa moduan 
        preferences.begin("wifi", false);
        preferences.putString("ssid", String(wifiSSID));  // Guardamos el SSID
        preferences.putString("password", String(wifiPassword));  // Guardamos la contraseña
        preferences.end();  // NVS itxitzen dugu modu egokian
        
        Serial.println("Credenciales guardadas en memoria NVS");
        server.send(200, "text/plain", "Received, attempting connection...");
    } else {
        server.send(400, "text/plain", "Missing arguments");
    }
});

    server.begin();
}

void connectToWiFi() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(wifiSSID);

    WiFi.begin(wifiSSID, wifiPassword);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Failed to connect");
    }
}

//====================================================================================
//                                    setup
//====================================================================================
void setup() {
    Serial.begin(115200);
    delay(100);

    pinMode(digit1, OUTPUT);
    pinMode(digit2, OUTPUT);
    pinMode(digit3, OUTPUT);
    pinMode(digit4, OUTPUT);
    pinMode(digit5, OUTPUT);  // Irudi finkorako 5. pantaila (2 puntuak)
    pinMode(BOTON, INPUT);

    // Pantaila nagusia abiaraztea (erlojuarentzat)
    enableDisplays();
    tft.begin();
    tft.setRotation(0);         // 0 & 2 Portrait. 1 & 3 landscape
    tft.fillScreen(TFT_BLACK);  // Pantailak garbitu (Beltzaratu)
    tft.setSwapBytes(true);     // Requred for the 1.14" IPS SPI (8pin) LCD displays from Aliexpress
    tft.setCursor(0, 0, 2);
    tft.setTextColor(TFT_WHITE);
    tft.setTextFont(1);
    tft.setTextFont(2);
    
    // Gordetako WiFi kredentzialak irakurtzeko
    preferences.begin("wifi", true);  //  Irakuketa modua soilik
    String storedSSID = preferences.getString("ssid", "");
    String storedPassword = preferences.getString("password", "");
    preferences.end();  //  Memoria itxi irakurri ondoren 

    if (storedSSID.length() > 0) {  // Kredentzialak gordeta badaude...
        strcpy(wifiSSID, storedSSID.c_str());
        strcpy(wifiPassword, storedPassword.c_str());
        Serial.println("Conectando a WiFi guardado: " + storedSSID);
        connectToWiFi();
        
        // 10 segundotan konektatzea lortzen ez badu, aktibatu AP
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Conectado a WiFi correctamente.");
        } else {
            Serial.println("No se pudo conectar. Activando AP...");
            startAccessPoint();
        }
    } else {
        Serial.println("No se encontraron credenciales guardadas, activando AP...");
        startAccessPoint();  // Ez badago daturik gordeta, aktibatu AP modua
    }

    // 🔹 Irakurri estiloaren konfigurazioa eta ordu-eremua
    preferences.begin("settings", true);  // Irakuketa modua soilik
    String storedEstilo = preferences.getString("estilo", "nixie");
    String storedTimezone = preferences.getString("timezone", "CET-1CEST,M3.5.0,M10.5.0/3");
    preferences.end();

    // Gordetako balioak aplikatzea
    estiloNixie = (storedEstilo == "nixie");
    setTimezone(storedTimezone);
    
    // 🔹 Hasi web interfazea WiFi konektatuta badago
    if (WiFi.status() == WL_CONNECTED) {
        startWebInterface();
    }

    disableDisplays();  // 1-4 pantailak lehenetsita desabilitatu
    initTime(dstRule);  // Londreseko ordu-eremua gorde eta aplikatu (Set timezone and daylight saving for London)?
    lastUpdateTime = millis();
    mostrarImagen();// Hasierako irudi finkoa erakutsi bosgarren pantailan
}

//====================================================================================
//                                 VOID LOOP
//====================================================================================
void loop() {
  server.handleClient(); //WIFI
    if (credentialsReceived) {
        WiFi.softAPdisconnect(true);
        connectToWiFi();
        credentialsReceived = false;
    }

  currentMillis = millis();

  if (digitalRead(BOTON) == LOW) {
    pulsado = true;
  }
  if (digitalRead(BOTON) == HIGH && pulsado == true) {
    pulsado = false;
    Cambio_de_estilo();  //Erloju-estiloa eta irudi finkoa aldatzea
  }

  // Ordua eguneratzea, dagoen kodearen arabera
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;
    initTime(dstRule);  //Eguneratu ordua NTP zerbitzaritik
  }

  timeVariablesSet();  // Funtzio honek nsecs, nmins, nhour aldagaiak ezartzen ditu.

  minutesUnits = nmins % 10;        //atera minutu kopuruaren digitu unitarioan mins
  minutesTens = (nmins / 10) % 10;  // atera minutu kopuruaren digitu unitarioa nmins
  hoursUnits = nhour % 10;          // atera ordu kopuruaren digitu unitarioa nhour
  hoursTens = (nhour / 10) % 10;    // atera ordu kopuruaren digitu unitarioa nhour

  if (oldMinuteUnits != minutesUnits) {
    loadDigit(digit4, minutesUnits);  // Kargatu Digitua erakusteko
    oldMinuteUnits = minutesUnits;
  }

  if (oldMinutesTens != minutesTens) {
    loadDigit(digit3, minutesTens);  // Kargatu Digitua erakusteko
    oldMinutesTens = minutesTens;
  }

  if (oldHoursUnits != hoursUnits) {
    loadDigit(digit2, hoursUnits);  // Kargatu Digitua erakusteko
    oldHoursUnits = hoursUnits;
  }

  if (oldHoursTens != hoursTens) {
    loadDigit(digit1, hoursTens);  // Kargatu Digitua erakusteko
    oldHoursTens = hoursTens;
  }
}

//====================================================================================
//                                 initTime
//====================================================================================
void initTime(String timezone) {
  struct tm timeinfo;
  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");  // Lehenengo konektatu NTP zerbitzariarekin, 0 TZ offset-ekin.
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
  setenv("TZ", timezone.c_str(), 1);  // Egokitu TZ. Erlojuaren konfigurazioa egokitu egin da toki berria erakusteko.
  tzset();
}

//====================================================================================
//                                 Display digit on LCD x
//====================================================================================
void loadDigit(int displayPin, int numeral) {
  digitalWrite(displayPin, PIN_LOW);  // Pantaila eguneratzeko aukera ematen du.
  if (estiloNixie) {
    tft.pushImage(0, 0, 135, 240, nixieNumeral[numeral]);  //Erakutsi irudi zuzena
  } else {
    tft.pushImage(0, 0, 135, 240, nenonNumeral[numeral]);  // Erakutsi irudi zuzena
  }
  digitalWrite(displayPin, PIN_HIGH);  // Eguneratu ondoren, pantaila desaktibatu.
}

//====================================================================================
//                                 BOSTGARREN PANTAILAN IRUDIA ERAKUTSI
//====================================================================================
void mostrarImagen() {
  digitalWrite(digit5, PIN_LOW);  // Bosgarren pantaila gaitu
  if (mostrarImagenFija) {
    tft.pushImage(0, 0, 135, 240, imagenFija);  // Erakutsi imagenFija
  } else {
    tft.pushImage(0, 0, 135, 240, Nns);  // Erakutsi Nns
  }
  digitalWrite(digit5, PIN_HIGH);  // Bosgarren pantaila desgaitu
}

//====================================================================================
//                                 ESTILOA ALDATU
//====================================================================================
void Cambio_de_estilo() {
  estiloNixie = !estiloNixie;  // Nixie eta Nenon artean alternatu
  mostrarImagenFija = !mostrarImagenFija;  //  imagenFija eta  Nns artean alternatu

  // 1-4 pantailetan erlojua eguneratzea
  loadDigit(digit1, hoursTens);
  loadDigit(digit2, hoursUnits);
  loadDigit(digit3, minutesTens);
  loadDigit(digit4, minutesUnits);

  // Eguneratu irudia bosgarren pantailan
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
  digitalWrite(digit1, PIN_HIGH);  // Pantaila guztiak desabilitatu
  digitalWrite(digit2, PIN_HIGH);
  digitalWrite(digit3, PIN_HIGH);
  digitalWrite(digit4, PIN_HIGH);
}

//====================================================================================
//                                 Obtener fecha y hora
//====================================================================================
String obtenerFechaHora() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Error obteniendo la fecha y hora");
        return "Error";
    }
    char fecha[30];
    char hora[10];
    // Formatear la fecha en inglés
    strftime(fecha, sizeof(fecha), "%A, %d %B %Y", &timeinfo); // Ej: "Wednesday, 12 February 2025"
    // Formatear la hora en formato 24h
    strftime(hora, sizeof(hora), "%H:%M:%S", &timeinfo); // Ej: "14:45:30"
    // Crear la cadena completa con separador '|'
    return String(fecha) + "|" + String(hora);
}
