# 🚨 Sistema de Alarma IoT con ESP32-S3 y Telegram

![ESP32-S3](https://img.shields.io/badge/Hardware-ESP32--S3-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![Telegram](https://img.shields.io/badge/Telegram-API-26A5E4?style=for-the-badge&logo=telegram&logoColor=white)
![C++](https://img.shields.io/badge/Code-C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

> **Proyecto Universitario - Ingeniería de Computadores** > **Universidad de Sevilla**

Un sistema de seguridad IoT compacto que monitoriza espacios físicos en tiempo real utilizando ultrasonidos. Diseñado específicamente para resolver la incompatibilidad de voltajes entre sensores de 5V y microcontroladores de 3.3V, notificando al usuario instantáneamente a través de un bot de **Telegram**.

---

## 👥 Autores
* **Iago Esquivel Ramos**
* **Javier Cadenas de Llano Delgado**
* **Francisco Luis Rivero Sánchez**

---

## 💡 La Ingeniería (Lógica del Sistema)

**El Reto:**
Crear un sistema de vigilancia doméstico de bajo coste utilizando el potente **ESP32-S3**, integrando hardware de diferentes voltajes de forma segura y eliminando la necesidad de servidores intermedios o aplicaciones propietarias.

**La Solución:**
Un monitor de proximidad conectado que actúa en tres fases:
1.  **Detección Física:** Un sensor ultrasónico mide la distancia.
2.  **Adaptación de Señal:** Un conversor de nivel lógico protege el ESP32 transformando las señales de 5V a 3.3V en tiempo real.
3.  **Notificación Cloud:** El ESP32 gestiona la conexión SSL/TLS para enviar una alerta "Push" a Telegram.

<p align="center">
  <img src="img/diagrama_bloques.png" alt="Arquitectura del Sistema" width="600">
</p>

---

## 🛠️ Hardware y Tecnologías

* **Microcontrolador:** ESP32-S3 Dev Module (Xtensa® LX7).
* **Sensor:** HC-SR04 (Ultrasonido).
* **Seguridad Eléctrica:** Bi-Directional Logic Level Converter (3.3V <-> 5V).
* **Librerías:** `UniversalTelegramBot` (API), `ArduinoJson` (Parsing), `WiFiClientSecure`.

---

## 🔌 Conexionado (Wiring)

⚠️ **IMPORTANTE:** Este proyecto utiliza un **Conversor de Nivel Lógico**. No conectar el sensor directamente al ESP32-S3 o se dañará el microcontrolador.

<p align="center">
  <img src="img/esquema_conexiones.jpg" alt="Esquema de Conexiones" width="500">
</p>

**Tabla de Rutas de Señal:**

| Señal | Pin ESP32-S3 (3.3V) | Entrada Conversor | Salida Conversor | Pin Sensor (5V) |
| :--- | :--- | :--- | :--- | :--- |
| **Trigger** | GPIO 4 | LV1 (Low) | HV1 (High) | Trig |
| **Echo** | GPIO 5 | LV2 (Low) | HV2 (High) | Echo |
| **VCC** | - | - | HV (Ref) | VCC (5V) |
| **Lógica** | 3V3 | LV (Ref) | - | - |

---

## ⚙️ Configuración e Instalación

### Paso 1: Preparar el Entorno
1.  Instala el IDE de Arduino.
2.  Instala las librerías necesarias:
    * `UniversalTelegramBot` by Brian Lough.
    * `ArduinoJson` by Benoit Blanchon.

### ⚠️ Paso 2: Credenciales de Seguridad
Por seguridad, las claves no están en el código principal.
1.  Crea un archivo llamado `arduino_secrets.h` en la misma carpeta que el `.ino`.
2.  Pega el siguiente contenido con tus datos:
    ```cpp
    #define SECRET_SSID "TU_WIFI"
    #define SECRET_PASS "TU_CLAVE_WIFI"
    #define SECRET_BOT_TOKEN "TU_TOKEN_DE_TELEGRAM"
    #define SECRET_CHAT_ID "TU_ID_DE_TELEGRAM"
    ```

### Paso 3: Carga del Firmware
1.  Selecciona la placa: **Tools > Board > ESP32S3 Dev Module**.
2.  Si usas el puerto serie para depurar, activa: **USB CDC On Boot: "Enabled"**.
3.  Sube el código.

---

## 📸 Resultados

El sistema es capaz de detectar movimiento y enviar la alerta en menos de 2 segundos.

<p align="center">
  <img src="img/foto_montaje.jpg" alt="Montaje Real" width="399">
  <img src="img/captura_telegram.png" alt="Notificación Móvil" width="300">
</p>

---

## 📄 Referencias
* [SparkFun Logic Level Converter Guide](https://learn.sparkfun.com/tutorials/bi-directional-logic-level-converter-hookup-guide)
* [Telegram Bot API](https://core.telegram.org/bots/api)

---

## ⚖️ Licencia

Este proyecto está bajo la Licencia **MIT** - mira el archivo [LICENSE](LICENSE) para más detalles.

Copyright (c) 2026 Iago Esquivel, Javier Cadenas & Francisco Luis Rivero.