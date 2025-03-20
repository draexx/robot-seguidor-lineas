# Guía de Calibración del Robot Seguidor de Líneas

## Introducción

Esta guía detalla el proceso de calibración necesario para optimizar el funcionamiento del robot seguidor de líneas. Una calibración adecuada garantiza que los sensores detecten correctamente la línea y que el robot responda de manera precisa a los obstáculos.
## Índice
- [Guía de Calibración del Robot Seguidor de Líneas](#guía-de-calibración-del-robot-seguidor-de-líneas)
  - [Introducción](#introducción)
  - [Índice](#índice)
  - [1. Calibración del Sensor de Línea](#1-calibración-del-sensor-de-línea)
    - [Materiales necesarios:](#materiales-necesarios)
    - [Pasos:](#pasos)
  - [2. Calibración del Sensor Ultrasónico](#2-calibración-del-sensor-ultrasónico)
    - [Herramientas necesarias](#herramientas-necesarias)
    - [Procedimiento](#procedimiento)
  - [3. Calibración de los Motores](#3-calibración-de-los-motores)
    - [Materiales necesarios:](#materiales-necesarios-1)
    - [Pasos:](#pasos-1)
  - [4. Calibración de los LEDs Indicadores](#4-calibración-de-los-leds-indicadores)
    - [Procedimiento](#procedimiento-1)
  - [5. Pruebas Finales](#5-pruebas-finales)
  - [6. Resolución de Problemas Comunes](#6-resolución-de-problemas-comunes)
    - [El robot no detecta la línea correctamente](#el-robot-no-detecta-la-línea-correctamente)
    - [El robot no responde a obstáculos](#el-robot-no-responde-a-obstáculos)
    - [Los LEDs no se encienden](#los-leds-no-se-encienden)
  - [7. Valores Óptimos Recomendados](#7-valores-óptimos-recomendados)

---

## 1. Calibración del Sensor de Línea

### Materiales necesarios:
- Superficie con una línea negra sobre fondo claro.
- Destornillador pequeño (si el sensor tiene potenciómetros).

### Pasos:
1. Modifica los valores `LINE_THRESHOLD` y `BACKGROUND_THRESHOLD` según sea necesario:

   ```arduino
    const int LINE_THRESHOLD = 1;     // Valor que indica que el sensor está sobre la línea (negro)
    const int BACKGROUND_THRESHOLD = 0; // Valor que indica que el sensor está sobre el fondo (blanco)
   ```
2. Coloca el robot sobre la superficie de prueba.
3. Asegúrate de que el sensor de línea esté correctamente alineado con la línea negra.
4. Enciende el robot y observa el comportamiento:
   - Si el robot no sigue la línea correctamente, ajusta la sensibilidad del sensor (si tiene potenciómetros).
   - Gira el potenciómetro lentamente hasta que el robot siga la línea de manera estable.
5. Repite el proceso en diferentes superficies para asegurar que el robot funcione en diversos entornos.

---

## 2. Calibración del Sensor Ultrasónico

### Herramientas necesarias
- Regla o cinta métrica
- Obstáculos de prueba

### Procedimiento

1. **Verificación de la distancia medida**
   - Coloca un obstáculo a una distancia conocida (por ejemplo, 10 cm) frente al sensor.
   - Utiliza el siguiente código para verificar la precisión de las mediciones:
   
   ```arduino
   void setup() {
     Serial.begin(9600);
     pinMode(TRIG_PIN, OUTPUT);
     pinMode(ECHO_PIN, INPUT);
   }
   
   void loop() {
     long duration, distance;
     digitalWrite(TRIG_PIN, LOW);
     delayMicroseconds(2);
     digitalWrite(TRIG_PIN, HIGH);
     delayMicroseconds(10);
     digitalWrite(TRIG_PIN, LOW);
     duration = pulseIn(ECHO_PIN, HIGH);
     distance = (duration / 2) / 29.1;
     Serial.print("Distancia: ");
     Serial.print(distance);
     Serial.println(" cm");
     delay(500);
   }
   ```

2. **Ajuste de parámetros de detección**
   - Modifica los valores `MIN_DISTANCE` y `OBSTACLE_CHECK_INTERVAL` según sea necesario:

   ```arduino
   #define MIN_DISTANCE 15      // Distancia mínima antes de considerar un obstáculo (cm)
   #define OBSTACLE_CHECK_INTERVAL 100  // Intervalo para comprobar obstáculos (ms)
   ```

---

## 3. Calibración de los Motores

### Materiales necesarios:
- Superficie plana y lisa.
- Cinta métrica.

### Pasos:
1. Coloca el robot sobre una superficie plana.
2. Enciende el robot y observa si avanza en línea recta.
3. Si el robot se desvía:
   - Ajusta la velocidad de los motores usando la función `setMotorSpeeds()` en el código.
   - Incrementa o reduce la velocidad de uno de los motores para compensar la desviación.
4. Repite el proceso hasta que el robot avance en línea recta.

---

## 4. Calibración de los LEDs Indicadores

### Procedimiento

1. **Verificación de funcionamiento**
   - Ejecuta la función `testLEDs()` incluida en el setup para comprobar que ambos LEDs funcionan correctamente.
   - Los LEDs deberían encenderse y apagarse secuencialmente.

2. **Ajuste de brillo (opcional)**
   - Si los LEDs son demasiado brillantes o tenues, puedes usar PWM para ajustar su intensidad:

   ```arduino
   // En lugar de:
   digitalWrite(LED_GREEN_PIN, HIGH);

   // Usa:
   analogWrite(LED_GREEN_PIN, 150); // Valor entre 0-255
   ```

---

## 5. Pruebas Finales

Una vez completada la calibración, realiza pruebas en diferentes entornos para asegurar que el robot funcione correctamente en todas las condiciones.

---

## 6. Resolución de Problemas Comunes

### El robot no detecta la línea correctamente

1. Coloca el robot sobre la superficie de prueba.
2. Asegúrate de que el sensor de línea esté correctamente alineado con la línea negra.
3. Enciende el robot y observa el comportamiento:
   - Si el robot no sigue la línea correctamente, ajusta la sensibilidad del sensor (si tiene potenciómetros).
   - Gira el potenciómetro lentamente hasta que el robot siga la línea de manera estable.
4. Repite el proceso en diferentes superficies para asegurar que el robot funcione en diversos entornos.

### El robot no responde a obstáculos
1. Comprueba que el sensor ultrasónico esté correctamente conectado.
2. Verifica que `MIN_DISTANCE` sea apropiado para tu aplicación.
3. Asegúrate de que el LED rojo se encienda cuando se detecta un obstáculo.

### Los LEDs no se encienden
1. Verifica las conexiones de los LEDs y resistencias.
2. Comprueba que los pines definidos (`LED_GREEN_PIN` y `LED_RED_PIN`) coincidan con tu montaje físico.
3. Asegúrate de que las resistencias limitadoras sean del valor adecuado (220-330 ohms).

---

## 7. Valores Óptimos Recomendados

Para condiciones estándar (línea negra de 2cm de ancho sobre fondo blanco, iluminación interior normal), recomendamos los siguientes valores iniciales:

```arduino
const int BASE_SPEED = 150;     // Velocidad base de los motores (0-255)
const int TURN_SPEED = 120;     // Velocidad de giro (0-255)
const int SLOW_SPEED = 80;      // Velocidad lenta para giros precisos
const int MIN_DISTANCE = 10;    // Distancia mínima para detectar obstáculo (cm)
const int NUM_READINGS = 5;     // Número de mediciones para promediar la distancia
// Parámetros de tiempo
const int TURN_DELAY = 700;       // Tiempo de giro (ms)
const int SCAN_DELAY = 500;       // Tiempo de escaneo (ms)
```

Recuerda que estos valores pueden requerir ajustes dependiendo de:
- El tipo de superficie
- Las condiciones de iluminación
- El peso del robot
- El estado de la batería

Documenta tus valores calibrados para referencia futura.