# MEMORIA TÉCNICA: SISTEMA DE ALARMA IOT CON ESP32-S3

**Autores:** Iago Esquivel Ramos, Javier Cadenas de Llano Delgado & Francisco Luis Rivero Sánchez.
**Grado:** Ingeniería de Computadores  
**Universidad:** Universidad de Sevilla  
**Fecha:** Enero 2026

---

## 1. INTRODUCCIÓN

### 1.1. Descripción del Proyecto
El proyecto consiste en el diseño e implementación de un sistema de seguridad IoT (Internet of Things) compacto y modular. El dispositivo monitorea un espacio físico mediante ultrasonidos y, al detectar presencia dentro de un rango determinado, envía una alerta inmediata al dispositivo móvil del usuario a través de la plataforma de mensajería **Telegram**.

### 1.2. Objetivos
* **Implementación Hardware:** Integrar una solución de bajo coste utilizando el microcontrolador avanzado **ESP32-S3**.
* **Integridad Eléctrica:** Resolver la incompatibilidad de voltajes entre sensores de 5V y la lógica de 3.3V mediante hardware intermedio (Conversor de Nivel).
* **Software Seguro:** Desarrollar un firmware eficiente, no bloqueante y que proteja las credenciales del usuario mediante buenas prácticas de programación.

---

## 2. ARQUITECTURA HARDWARE

El sistema hardware se ha diseñado priorizando la seguridad eléctrica y la capacidad de procesamiento. A continuación se detallan los componentes seleccionados:

<div align="center">
  <img src="img/diagrama_bloques.png" alt="Diagrama de Bloques" width="500">
  <br>
  <em>Figura 1: Arquitectura del sistema</em>
</div>

### 2.1. Unidad de Procesamiento: ESP32-S3
Se ha seleccionado el modelo **ESP32-S3** (frente al ESP32 estándar o el ESP8266) por sus capacidades superiores para proyectos IoT modernos:
* **Procesador:** Doble núcleo Xtensa® LX7.
* **Conectividad:** WiFi + Bluetooth 5 (LE).
* **Voltaje de operación:** La lógica de los pines GPIO trabaja estrictamente a **3.3V**.

### 2.2. Sensor HC-SR04
Sensor de medición de distancia por ultrasonidos.
* **Alimentación:** Requiere **5V** para operar correctamente.
* **Funcionamiento:** Emite un pulso a 40kHz y mide el tiempo de rebote del sonido.

### 2.3. Conversor de Nivel Lógico (Logic Level Converter)
**Elemento crítico del diseño.** Dado que el sensor HC-SR04 devuelve señales de eco a 5V y el ESP32-S3 solo tolera 3.3V en sus entradas, una conexión directa podría dañar el microcontrolador.
Se ha intercalado un *Bi-Directional Logic Level Converter* que adapta las señales en tiempo real:
* **Lado HV (High Voltage):** Referenciado a 5V.
* **Lado LV (Low Voltage):** Referenciado a 3.3V.

### 2.4. Sistema de Alimentación
Se utiliza una fuente de alimentación externa de **5V** que energiza el bus principal, alimentando el sensor directamente y el microcontrolador (a través de su regulador interno o entrada USB).

---

## 3. DISEÑO DEL SOFTWARE

El firmware ha sido desarrollado en **C++** utilizando el entorno **Arduino IDE**, configurado específicamente para la arquitectura del ESP32-S3.

### 3.1. Librerías y Dependencias
El proyecto hace uso de las siguientes librerías especializadas:
* `WiFi.h` y `WiFiClientSecure.h`: Para gestionar la conexión a internet y la capa de encriptación SSL/TLS (HTTPS).
* `UniversalTelegramBot.h`: Abstracción completa de la API de Bots de Telegram.
* `ArduinoJson.h`: Necesaria para parsear (interpretar) las respuestas en formato JSON que devuelve el servidor de Telegram.

### 3.2. Estructura del Código y Seguridad
Para garantizar la seguridad y robustez del proyecto:

1.  **Segregación de Credenciales (`arduino_secrets.h`):**
    Se ha extraído toda información sensible (SSID, contraseña WiFi, Token del Bot y Chat ID) a un archivo de cabecera independiente. Esto permite compartir el código fuente principal sin exponer claves privadas.

2.  **Lógica No Bloqueante (Multitarea Cooperativa):**
    Se ha evitado el uso de la función `delay()` en el bucle principal (`loop`), ya que detiene la ejecución del procesador. En su lugar, se utiliza `millis()` para gestionar dos tareas asíncronas:
    * **Muestreo:** Lectura del sensor cada 500ms.
    * **Gestión de Alertas:** Periodo de "enfriamiento" de 10 segundos tras una alarma para evitar el *spam* de notificaciones.

---

## 4. MONTAJE E INSTALACIÓN

### 4.1. Esquema de Conexiones
El montaje requiere especial atención al puenteo de voltajes a través del Conversor de Nivel para proteger el ESP32-S3.

<div align="center">
  <img src="img/esquema_conexiones.jpg" alt="Esquema de Conexiones" width="500">
  <br>
  <em>Figura 2: Detalle del conexionado con el conversor de nivel</em>
</div>

**Distribución de Alimentación:**
* **Fuente 5V** $\rightarrow$ Sensor VCC, Pin HV del Conversor, Pin VIN del ESP32.
* **ESP32 3.3V** $\rightarrow$ Pin LV del Conversor.
* **GND** $\rightarrow$ Conectado a GND de todos los componentes (tierra común).

**Ruta de Datos (Señales):**

| Señal | Origen | Entrada Conversor | Salida Conversor | Destino |
| :--- | :--- | :--- | :--- | :--- |
| **Trigger** | ESP32 GPIO 4 (3.3V) | LV1 (Low) | HV1 (High) | Sensor Trig (5V) |
| **Echo** | Sensor Echo (5V) | HV2 (High) | LV2 (Low) | ESP32 GPIO 5 (3.3V) |

### 4.2. Flujo de Despliegue
1.  Realizar el conexionado físico asegurando la correspondencia HV/LV.
2.  Crear el archivo `arduino_secrets.h` con las credenciales personales.
3.  En Arduino IDE, seleccionar la placa **"ESP32S3 Dev Module"**.
4.  Compilar y subir el firmware.

<div align="center">
  <img src="img/foto_montaje.jpg" alt="Montaje Real" width="450">
  <br>
  <em>Figura 3: Montaje físico del prototipo en laboratorio</em>
</div>

---

## 5. CONCLUSIONES

El proyecto ha resultado en un sistema de vigilancia funcional y seguro. La elección del hardware ha sido clave: el uso del **ESP32-S3** garantiza potencia de procesamiento para futuras ampliaciones (como añadir cámara o IA), mientras que la implementación del **Conversor de Niveles** demuestra un diseño electrónico correcto, protegiendo la integridad del sistema ante la disparidad de voltajes. La integración con la API de Telegram ofrece una experiencia de usuario fluida con notificaciones en tiempo real.

<div align="center">
  <img src="img/captura_telegram.png" alt="Notificación Telegram" width="300">
  <br>
  <em>Figura 4: Prueba de funcionamiento de la notificación Push</em>
</div>

---

## 6. BIBLIOGRAFÍA Y REFERENCIAS

Para el desarrollo de este proyecto se han consultado las siguientes fuentes de documentación técnica y hojas de datos:

1.  **SparkFun Electronics.** *Bi-Directional Logic Level Converter Hookup Guide - SparkFun Learn*. Disponible en: [https://learn.sparkfun.com/tutorials/bi-directional-logic-level-converter-hookup-guide](https://learn.sparkfun.com/tutorials/bi-directional-logic-level-converter-hookup-guide)
    * *Referencia principal para la implementación del hardware de adaptación de voltaje (3.3V <-> 5V).*

2.  **Espressif Systems.** *ESP32-S3 Series Datasheet*. Versión 1.1.
    * Documentación oficial del microcontrolador utilizado, especificaciones de pines y consumo.

3.  **Telegram.** *Telegram Bot API Documentation*. Disponible en: [https://core.telegram.org/bots/api](https://core.telegram.org/bots/api)
    * Guía oficial para desarrolladores sobre el uso de la API HTTP para el envío de mensajes.

4.  **Brian Lough.** *UniversalTelegramBot Library*. GitHub Repository.
    * Librería de código abierto utilizada para la abstracción de las peticiones HTTP seguras en Arduino.

5.  **Benoit Blanchon.** *ArduinoJson: Efficient JSON serialization for embedded C++*.
    * Documentación técnica sobre el parseo de objetos JSON en sistemas embebidos.