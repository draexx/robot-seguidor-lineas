# Robot Seguidor de Líneas con Detección de Obstáculos

![Robot ensamblado](docs/img/robot_assembled.jpg)

## Descripción
Robot móvil autónomo construido con Arduino Nano capaz de seguir líneas negras y detectar/evitar obstáculos utilizando un sensor ultrasónico montado en un cabezal móvil.

## Características
- Seguimiento de líneas mediante sensor infrarrojo
- Detección y evasión de obstáculos con sensor ultrasónico
- Cabezal móvil con servomotor para escaneo del entorno
- Indicadores LED para estado de funcionamiento
- Control preciso de motores mediante puente H

## Materiales Necesarios
- Arduino Nano
- Sensor de seguimiento de línea
- Sensor ultrasónico HC-SR04
- 2 servomotores
- 2 motorreductores
- Driver L298N
- LEDs (verde y rojo)
- Cables, resistencias y otros componentes

## Instalación
1. Ensambla el hardware según el [esquema de conexiones](hardware/connections.md)
2. Instala el [IDE de Arduino](https://www.arduino.cc/en/software)
3. Descarga y abre el archivo `src/robot_seguidor_lineas.ino`
4. Conecta el Arduino Nano vía USB
5. Carga el código al Arduino

## Uso
1. Coloca el robot sobre una superficie con una línea negra
2. Enciende el robot
3. El LED verde indica seguimiento normal
4. El LED rojo indica detección de obstáculo

## Documentación
- [Manual de Usuario](docs/manual.md)
- [Informe Técnico](docs/informe_tecnico.md)
- [Guía de Calibración](docs/calibracion.md)
- [Lista de Materiales](hardware/bom.md)

## Licencia
Este proyecto está licenciado bajo [MIT License](LICENSE).

## Autor
[Tu Nombre]
[Tu Institución/Escuela]