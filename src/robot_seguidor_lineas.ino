// Robot Seguidor de Líneas con Evasión de Obstáculos
// Utiliza Arduino Nano, sensor de seguimiento, ultrasonido y servomotores

// Definición de pines
#define TRACK_SENSOR_PIN_1 A3      // Pin del primer sensor de seguimiento de línea
#define TRACK_SENSOR_PIN_2 A2     // Pin del segundo sensor de seguimiento de línea
#define TRIG_PIN 3              // Pin Trigger del sensor ultrasónico
#define ECHO_PIN 4              // Pin Echo del sensor ultrasónico
#define SERVO_HEAD_PIN 9        // Pin del servo de la cabeza
#define SERVO_ARM_PIN 15        // Pin del servo del brazo

// Pines del puente H (L298N o similar)
#define ENA_PIN 5               // Pin Enable A - PWM para controlar velocidad del motor izquierdo
#define IN1_PIN 6               // Pin IN1 para dirección del motor izquierdo
#define IN2_PIN 7               // Pin IN2 para dirección del motor izquierdo
#define ENB_PIN 11              // Pin Enable B - PWM para controlar velocidad del motor derecho
#define IN3_PIN 12              // Pin IN3 para dirección del motor derecho
#define IN4_PIN 13              // Pin IN4 para dirección del motor derecho

// Pines para LEDs indicadores
#define LED_GREEN_PIN A0        // LED verde (avance normal)
#define LED_RED_PIN A1          // LED rojo (detección de obstáculos)

// Librerías necesarias
#include <Servo.h>

// Objetos servo
Servo servoHead;
Servo servoArm;

// Constantes
const int BASE_SPEED = 150;     // Velocidad base de los motores (0-255)
const int TURN_SPEED = 120;     // Velocidad de giro (0-255)
const int SLOW_SPEED = 80;      // Velocidad lenta para giros precisos
const int MIN_DISTANCE = 10;    // Distancia mínima para detectar obstáculo (cm)
const int NUM_READINGS = 5;     // Número de mediciones para promediar la distancia

// Parámetros de tiempo
const int TURN_DELAY = 700;       // Tiempo de giro (ms)
const int SCAN_DELAY = 500;       // Tiempo de escaneo (ms)

// Parámetros de los sensores de línea
const int LINE_THRESHOLD = 1;     // Valor que indica que el sensor está sobre la línea (negro)
const int BACKGROUND_THRESHOLD = 0; // Valor que indica que el sensor está sobre el fondo (blanco)

// Variables globales
int lineState = 0;              // Estado del sensor de línea (0=sobre línea, 1=fuera de línea)
long duration, distance;        // Variables para el cálculo de la distancia
int headPosition = 90;          // Posición inicial de la cabeza (centro)
int armPosition = 90;           // Posición inicial del brazo (centro)
bool obstacleDetected = false;  // Bandera para indicar si se detectó un obstáculo

void setup() {
  // Inicialización de comunicación serial
  Serial.begin(9600);

  // Configuración de pines de entrada/salida
  pinMode(TRACK_SENSOR_PIN_1, INPUT);
  pinMode(TRACK_SENSOR_PIN_2, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configuración de pines del puente H
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

  // Configuración de pines de LEDs
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  // Inicialización de servos
  servoHead.attach(SERVO_HEAD_PIN);
  servoArm.attach(SERVO_ARM_PIN);

  // Posición inicial de los servos
  servoHead.write(headPosition);
  servoArm.write(armPosition);

  // Esperar a que los servos se posicionen
  delay(1000);

  // Iniciar con motores detenidos
  stopMotors();

  // Prueba inicial de LEDs
  testLEDs();

  Serial.println("Robot inicializado");
}

void loop() {
  // Leer sensores de línea
  int lineState1 = digitalRead(TRACK_SENSOR_PIN_1);
  int lineState2 = digitalRead(TRACK_SENSOR_PIN_2);

  // Medir la distancia con el sensor ultrasónico
  distance = measureDistance();

  // Imprimir información de diagnóstico
  Serial.print("Sensor 1: ");
  Serial.print(lineState1);
  Serial.print(" | Sensor 2: ");
  Serial.print(lineState2);
  Serial.print(" | Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Verificar si hay un obstáculo
  if (distance < MIN_DISTANCE && distance > 0) {  // Asegurar que la distancia sea válida
    // Hay un obstáculo delante
    Serial.println("¡Obstáculo detectado!");

    // Activar LED rojo, apagar LED verde
    setLEDs(false, true);
    obstacleDetected = true;

    // Detener el robot
    stopMotors();
    delay(SCAN_DELAY);

    // Escanear alrededor para encontrar una ruta alternativa
    int leftDistance = scanDirection(30);   // Escanear izquierda (30°)
    int rightDistance = scanDirection(150); // Escanear derecha (150°)

    // Centrar la cabeza nuevamente
    servoHead.write(90);
    delay(300);

    // Decidir hacia dónde girar
    if (leftDistance > rightDistance && leftDistance > MIN_DISTANCE) {
      // Girar a la izquierda
      Serial.println("Girando a la izquierda");
      turnLeft(TURN_SPEED);
      delay(TURN_DELAY);
    } else if (rightDistance > MIN_DISTANCE) {
      // Girar a la derecha
      Serial.println("Girando a la derecha");
      turnRight(TURN_SPEED);
      delay(TURN_DELAY);
    } else {
      // Dar la vuelta
      Serial.println("Dando la vuelta");
      turnAround();
    }

    // Restablecer estado de obstáculo
    obstacleDetected = false;
  } else {
    // No hay obstáculo, seguir la línea

    // Activar LED verde, apagar LED rojo (si no hay obstáculo)
    if (!obstacleDetected) {
      setLEDs(true, false);
    }

    followLine();
  }

  // Pequeña pausa para estabilidad
  delay(50);
}

// Función para probar los LEDs al inicio
void testLEDs() {
  // Encender LED verde
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
  delay(500);

  // Encender LED rojo
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
  delay(500);

  // Apagar ambos LEDs
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  delay(500);
}

// Función para controlar los LEDs
void setLEDs(bool greenOn, bool redOn) {
  digitalWrite(LED_GREEN_PIN, greenOn ? HIGH : LOW);
  digitalWrite(LED_RED_PIN, redOn ? HIGH : LOW);
}

// Función para medir la distancia con el sensor ultrasónico
long measureDistance() {
  long total = 0;
  for (int i = 0; i < NUM_READINGS; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration * 0.034 / 2;
    total += distance;
    delay(10);  // Pequeña pausa entre mediciones
  }
  return total / NUM_READINGS;  // Retorna el promedio
}

// Función para escanear en una dirección específica
int scanDirection(int angle) {
  servoHead.write(angle);
  delay(SCAN_DELAY);  // Esperar a que el servo se posicione
  int scannedDistance = measureDistance();
  Serial.print("Distancia a ");
  Serial.print(angle);
  Serial.print("°: ");
  Serial.print(scannedDistance);
  Serial.println(" cm");
  return scannedDistance;
}

// Función para seguir la línea
void followLine() {
  int lineState1 = digitalRead(TRACK_SENSOR_PIN_1);  // Leer primer sensor
  int lineState2 = digitalRead(TRACK_SENSOR_PIN_2);  // Leer segundo sensor

  // Lógica de seguimiento de línea con dos sensores (línea negra sobre fondo blanco)
  if (lineState1 == LINE_THRESHOLD && lineState2 == LINE_THRESHOLD) {
    // Ambos sensores sobre la línea (negro): avanzar recto
    moveForward(BASE_SPEED);
  } else if (lineState1 == LINE_THRESHOLD && lineState2 == BACKGROUND_THRESHOLD) {
    // Primer sensor sobre el fondo (blanco): girar a la izquierda
    turnLeft(SLOW_SPEED);
  } else if (lineState1 == BACKGROUND_THRESHOLD && lineState2 == LINE_THRESHOLD) {
    // Segundo sensor sobre el fondo (blanco): girar a la derecha
    turnRight(SLOW_SPEED);
  } else {
    // Ambos sensores sobre el fondo (blanco): buscar la línea
    findLine();
  }
}

// Función para buscar la línea cuando se ha perdido
void findLine() {
  // Estrategia mejorada con dos sensores
  turnLeft(SLOW_SPEED);
  delay(100);

  // Verificar si hemos encontrado la línea
  if (digitalRead(TRACK_SENSOR_PIN_1) == LINE_THRESHOLD || digitalRead(TRACK_SENSOR_PIN_2) == LINE_THRESHOLD) {
    return;
  }

  // Si no, girar a la derecha
  turnRight(SLOW_SPEED);
  delay(TURN_DELAY);

  // Verificar si hemos encontrado la línea
  if (digitalRead(TRACK_SENSOR_PIN_1) == LINE_THRESHOLD || digitalRead(TRACK_SENSOR_PIN_2) == LINE_THRESHOLD) {
    return;
  }

  // Si aún no, girar a la izquierda nuevamente pero más tiempo
  turnLeft(SLOW_SPEED);
  delay(TURN_DELAY * 1.5);  // Ajuste adicional para búsqueda más larga
}

// Funciones de control de motores con control de velocidad PWM
void moveForward(int speed) {
  Serial.print("Avanzando con velocidad: ");
  Serial.println(speed);

  // Activar LED verde (avance normal)
  setLEDs(true, false);

  // Configurar dirección: motor izquierdo hacia adelante
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  // Configurar dirección: motor derecho hacia adelante
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);

  // Aplicar velocidad PWM a ambos motores
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
}

void moveBackward(int speed) {
  Serial.print("Retrocediendo con velocidad: ");
  Serial.println(speed);

  // Configurar dirección: motor izquierdo hacia atrás
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);

  // Configurar dirección: motor derecho hacia atrás
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);

  // Aplicar velocidad PWM a ambos motores
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
}

void turnLeft(int speed) {
  Serial.print("Girando izquierda con velocidad: ");
  Serial.println(speed);

  // Motor izquierdo parado o hacia atrás (lento)
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);

  // Motor derecho hacia adelante
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);

  // Aplicar velocidad PWM diferencial para un giro suave
  analogWrite(ENA_PIN, speed * 0.7);  // Motor izquierdo más lento
  analogWrite(ENB_PIN, speed);        // Motor derecho a velocidad normal
}

void turnRight(int speed) {
  Serial.print("Girando derecha con velocidad: ");
  Serial.println(speed);

  // Motor izquierdo hacia adelante
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  // Motor derecho parado o hacia atrás (lento)
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);

  // Aplicar velocidad PWM diferencial para un giro suave
  analogWrite(ENA_PIN, speed);        // Motor izquierdo a velocidad normal
  analogWrite(ENB_PIN, speed * 0.7);  // Motor derecho más lento
}

void stopMotors() {
  Serial.println("Deteniendo motores");

  // Detener ambos motores desactivando los enables
  analogWrite(ENA_PIN, 0);
  analogWrite(ENB_PIN, 0);

  // Alternativa: mantener los enables activos pero con los pines de dirección en el mismo estado
  // digitalWrite(IN1_PIN, LOW);
  // digitalWrite(IN2_PIN, LOW);
  // digitalWrite(IN3_PIN, LOW);
  // digitalWrite(IN4_PIN, LOW);
}

void turnAround() {
  Serial.println("Dando la vuelta");

  // Motor izquierdo hacia adelante
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  // Motor derecho hacia atrás
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);

  // Ambos motores a velocidad de giro
  analogWrite(ENA_PIN, TURN_SPEED);
  analogWrite(ENB_PIN, TURN_SPEED);

  // Girar por un tiempo suficiente para dar la vuelta (ajustar según sea necesario)
  delay(1200);
}

// Función para ajuste fino de velocidad (puede usarse para compensar diferencias entre motores)
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  analogWrite(ENA_PIN, leftSpeed);
  analogWrite(ENB_PIN, rightSpeed);
}