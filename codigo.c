#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <math.h>

// ===== WIFI ====
const char* ssid = "nome_da_rede";
const char* password = "senha_da_rede";

WebServer server(80);

// ===== INA219 =====
Adafruit_INA219 ina1(0x40);
Adafruit_INA219 ina2(0x44);

// ===== NTC =====
#define NTC1 34
#define NTC2 35
#define BUZZER 27

const float VCC = 3.3;
const float R_FIXO = 9900.0;

// NTC 222
const float R0 = 2200.0;
const float T0_NTC = 298.15;
const float BETA = 3977.0;

// Limites
const float TEMP_LIGA = 27.0;
const float TEMP_DESLIGA = 26.0;

bool alertaLigado = false;

// Variáveis globais
float temp1, temp2;
float V1, V2;
float I1, I2;

// ===== Ler temperatura =====
float lerTemperatura(int pino) {

  int soma = 0;

  for (int i = 0; i < 10; i++) {
    soma += analogRead(pino);
    delay(2);
  }

  int adc = soma / 10;

  if (adc <= 0 || adc >= 4095)
    return -100;

  float Vout = adc * (VCC / 4095.0);

  float R_ntc = R_FIXO * (Vout / (VCC - Vout));

  float tempK = 1.0 / (
      (1.0 / T0_NTC) +
      (1.0 / BETA) * log(R_ntc / R0)
    );

  return tempK - 273.15;
}

// ===== Página WEB =====
void handleRoot() {

  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta http-equiv='refresh' content='2'>
    <meta charset="UTF-8">
    <title>Monitor ESP32</title>

    <style>
      body{
        font-family: Arial;
        background:#111;
        color:white;
        text-align:center;
      }

      .card{
        background:#222;
        margin:20px auto;
        padding:20px;
        width:300px;
        border-radius:15px;
      }

      h1{
        color:#00ff99;
      }

      .alerta{
        color:red;
        font-size:24px;
      }
    </style>
  </head>

  <body>

    <h1>Monitoramento ESP32</h1>

    <div class='card'>
      <h2>Bateria 1</h2>
      <p>Temperatura: )rawliteral";

  html += String(temp1, 1);

  html += R"rawliteral( °C</p>
      <p>Tensão: )rawliteral";

  html += String(V1, 2);

  html += R"rawliteral( V</p>
      <p>Corrente: )rawliteral";

  html += String(I1, 1);

  html += R"rawliteral( mA</p>
    </div>

    <div class='card'>
      <h2>Bateria 2</h2>
      <p>Temperatura: )rawliteral";

  html += String(temp2, 1);

  html += R"rawliteral( °C</p>
      <p>Tensão: )rawliteral";

  html += String(V2, 2);

  html += R"rawliteral( V</p>
      <p>Corrente: )rawliteral";

  html += String(I2, 1);

  html += R"rawliteral( mA</p>
    </div>
  )rawliteral";

  if (alertaLigado) {
    html += "<p class='alerta'>ALERTA DE TEMPERATURA!</p>";
  }

  html += R"rawliteral(
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {

  Serial.begin(115200);

  

  // ADC ESP32
  analogReadResolution(12);

  // I2C
  Wire.begin(21, 22);

  // INA219
  if (!ina1.begin()) {
    Serial.println("Erro INA219 1");
    while (1);
  }

  if (!ina2.begin()) {
    Serial.println("Erro INA219 2");
    while (1);
  }

  pinMode(BUZZER, OUTPUT);

  // ===== WIFI =====
  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // ===== Servidor =====
  server.on("/", handleRoot);

  server.begin();
}

void loop() {

  // ===== Temperaturas =====
  temp1 = lerTemperatura(NTC1);
  temp2 = lerTemperatura(NTC2);

  // ===== INA219 =====
  V1 = ina1.getBusVoltage_V();
  I1 = ina1.getCurrent_mA();

  V2 = ina2.getBusVoltage_V();
  I2 = ina2.getCurrent_mA();

  // ===== Controle buzzer =====
  float tempMax = max(temp1, temp2);

  if (tempMax >= TEMP_LIGA) {
    alertaLigado = true;
  }

  if (tempMax <= TEMP_DESLIGA) {
    alertaLigado = false;
  }

  digitalWrite(BUZZER, alertaLigado);

  // ===== Serial =====
  Serial.print("T1: ");
  Serial.print(temp1);

  Serial.print(" | T2: ");
  Serial.print(temp2);

  Serial.print(" | V1: ");
  Serial.print(V1);

  Serial.print(" | I1: ");
  Serial.print(I1);

  Serial.print(" | V2: ");
  Serial.print(V2);

  Serial.print(" | I2: ");
  Serial.println(I2);

  // Web server
  server.handleClient();

  delay(500);
}
