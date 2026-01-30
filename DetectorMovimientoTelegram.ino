#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h" 

// Pines sensor
const int trigPin = 4; 
const int echoPin = 5; 

// Ajustes
#define DISTANCIA_ALARMA 20     // cm para saltar
#define INTERVALO_LECTURA 500   // leer cada medio segundo
#define INTERVALO_ALARMA 10000  // 10s entre mensajes

// Objetos telegram
WiFiClientSecure client;
UniversalTelegramBot bot(SECRET_BOT_TOKEN, client);

// Timers para no usar delay
unsigned long ultimoMensaje = 0;
unsigned long ultimaLectura = 0;
bool alarmaActivada = false;

void setup() {
  Serial.begin(115200);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 

  // Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  client.setInsecure(); // saltar certificado SSL

  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nListo.");
  bot.sendMessage(SECRET_CHAT_ID, "🟢 Sistema ON", "");
}

void loop() {
  // Reconectar si se cae
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SECRET_SSID, SECRET_PASS);
  }

  // Leer sensor cada X tiempo
  if (millis() - ultimaLectura > INTERVALO_LECTURA) {
    long distancia = obtenerDistancia();
    ultimaLectura = millis();
    
    gestionarAlarma(distancia);
  }
}

// Lectura del HC-SR04
long obtenerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH); 
  return duration * 0.034 / 2; 
}

void gestionarAlarma(long distancia) {
  // Si detecta algo cerca
  if (distancia > 0 && distancia < DISTANCIA_ALARMA) {
    
    // Evitar spam de mensajes (esperar 10s)
    if (millis() - ultimoMensaje > INTERVALO_ALARMA) {
      Serial.println("¡Intruso! Enviando...");
      
      String mensaje = "🚨 ¡ALERTA!\nMovimiento a " + String(distancia) + " cm.";
      
      if (bot.sendMessage(SECRET_CHAT_ID, mensaje, "")) {
        ultimoMensaje = millis(); // reset timer
      } else {
        Serial.println("Fallo Telegram");
      }
    }
  }
}
