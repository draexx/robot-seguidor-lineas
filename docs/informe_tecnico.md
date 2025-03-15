# Informe Técnico del Robot Seguidor de Líneas con Evasión de Obstáculos

---

## 1. Introducción

Este proyecto consiste en un robot autónomo capaz de seguir una línea negra sobre una superficie clara y evitar obstáculos utilizando un sensor ultrasónico. El robot está controlado por una placa Arduino Nano y utiliza servomotores para el movimiento y la detección de obstáculos.

---

## 2. Hardware

### Componentes principales:
- **Arduino Nano**: Microcontrolador principal.
- **Sensor de seguimiento de línea**: Detecta la línea negra sobre la superficie.
- **Sensor ultrasónico HC-SR04**: Detecta obstáculos delante del robot.
- **Motores DC con ruedas**: Para el movimiento del robot.
- **Puente H L298N**: Controla la dirección y velocidad de los motores.
- **Servomotores**: Para mover la cabeza y el brazo del robot.
- **LEDs indicadores**: Verde (avance normal) y rojo (detección de obstáculos).

### Conexiones:
- Ver el archivo `connections.md` en la carpeta `hardware` para el esquema detallado.

---

## 3. Software

### Librerías utilizadas:
- **Servo.h**: Para controlar los servomotores.

### Funcionalidades principales:
- **Seguimiento de línea**: El robot sigue una línea negra utilizando el sensor de seguimiento.
- **Evasión de obstáculos**: Cuando detecta un obstáculo, el robot se detiene, escanea el entorno y decide la dirección para evitarlo.
- **Indicadores visuales**: LEDs que muestran el estado del robot (avance normal o detección de obstáculos).

### Estructura del código:
- **Inicialización**: Configuración de pines, servomotores y LEDs.
- **Bucle principal**: Lectura de sensores, toma de decisiones y control de motores.
- **Funciones auxiliares**: Movimiento, detección de obstáculos y control de LEDs.

---

## 4. Decisiones de Diseño

### Seguimiento de línea:
- Se utiliza un sensor de seguimiento simple para detectar la línea. El robot ajusta su movimiento girando ligeramente cuando pierde la línea.

### Evasión de obstáculos:
- El sensor ultrasónico mide la distancia a los obstáculos. Si la distancia es menor que `MIN_DISTANCE`, el robot escanea el entorno y decide la dirección para evitar el obstáculo.

### Indicadores visuales:
- Los LEDs proporcionan retroalimentación visual del estado del robot, lo que facilita la depuración y el monitoreo.

---

## 5. Resultados

El robot es capaz de seguir una línea negra de manera estable y evitar obstáculos de forma autónoma. Los LEDs indicadores funcionan correctamente, proporcionando información clara sobre el estado del robot.

---

## 6. Mejoras Futuras

### Mejoras Hardware
- **Integración de más sensores**: Añadir sensores infrarrojos para mejorar la detección de líneas.
- **Comunicación inalámbrica**: Implementar Bluetooth o Wi-Fi para control remoto.
- **Implementación de una pantalla LCD**: Para mostrar información de sensores.
- **Autonomía mejorada**: Añadir una batería recargable y un sistema de gestión de energía.
- **Chasis 3D**: Diseño de un chasis optimizado e impreso en 3D.
### Mejoras Software
- **Algoritmo PID**: Para seguimiento de línea más suave.
- **Interfaz Calibración**: Para calibración de sensores.
- **Modos de Operación**: Seleccionables (rápido, preciso, ahorro de energía).
- **Rutas**: Almacenamiento de mapas de rutas en memoria EEPROM.
- **Integración de algoritmos de aprendizaje**: Para mejorar la navegación con el tiempo.

---

## 7. Conclusión

Este proyecto demuestra cómo la robótica educativa puede utilizarse para enseñar conceptos de electrónica, programación y lógica de control. El robot es una plataforma versátil que puede mejorarse y adaptarse para diferentes aplicaciones.

--