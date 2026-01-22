#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h" // Importamos las claves

// Definición de Pines (HC-SR04)
const int trigPin = 4; // 
const int echoPin = 5; // 

// Configuración
#define DISTANCIA_ALARMA 20        // cm para activar alarma 
#define INTERVALO_LECTURA 500      // Leer sensor cada 500ms
#define INTERVALO_ALARMA 10000     // No repetir alarma en 10s

// Objetos
WiFiClientSecure client;
UniversalTelegramBot bot(SECRET_BOT_TOKEN, client);

// Variables de control de tiempo (Non-blocking)
unsigned long ultimoMensaje = 0;
unsigned long ultimaLectura = 0;
bool alarmaActivada = false;

void setup() {
  Serial.begin(115200);
  
  // Configuración de pines
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); // [cite: 10]

  // Conexión WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  client.setInsecure(); // Necesario para Telegram sin certificados complejos

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado. Sistema armado.");
  bot.sendMessage(SECRET_CHAT_ID, "🟢 Sistema de Seguridad Iniciado", "");
}

void loop() {
  // Reconexión automática si se cae el WiFi
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SECRET_SSID, SECRET_PASS);
  }

  // Lectura del sensor (Sin bloquear el procesador)
  if (millis() - ultimaLectura > INTERVALO_LECTURA) {
    long distancia = obtenerDistancia();
    ultimaLectura = millis();
    
    // Solo imprimimos si hay cambios significativos o para depurar
    // Serial.print("Distancia: "); Serial.println(distancia);

    gestionarAlarma(distancia);
  }
}

// Función auxiliar para leer el sensor
long obtenerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Pulso de 10us 
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH); // Leer eco [cite: 15]
  return duration * 0.034 / 2; // Calcular cm 
}

// Lógica de la alarma
void gestionarAlarma(long distancia) {
  // Si detecta presencia (menos de 20cm y mayor a 0)
  if (distancia > 0 && distancia < DISTANCIA_ALARMA) {
    
    // Verificamos si ha pasado el tiempo de enfriamiento
    if (millis() - ultimoMensaje > INTERVALO_ALARMA) {
      Serial.println("¡INTRUSO DETECTADO! Enviando alerta...");
      
      String mensaje = "🚨 ¡ALERTA DE SEGURIDAD!\nMovimiento detectado a " + String(distancia) + " cm.";
      
      if (bot.sendMessage(SECRET_CHAT_ID, mensaje, "")) {
        ultimoMensaje = millis(); // Reiniciamos contador de spam
      } else {
        Serial.println("Error enviando Telegram");
      }
    }
  }
}