// Robot Seguidor de Líneas con Evasión de Obstáculos
// Utiliza Arduino Nano, sensor de seguimiento, ultrasonido y servomotores

// Definición de pines
#define TRACK_SENSOR_PIN 2      // Pin del sensor de seguimiento de línea
#define TRIG_PIN 3              // Pin Trigger del sensor ultrasónico
#define ECHO_PIN 4              // Pin Echo del sensor ultrasónico
#define SERVO_HEAD_PIN 9        // Pin del servo de la cabeza
#define SERVO_ARM_PIN 10        // Pin del servo del brazo

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
    pinMode(TRACK_SENSOR_PIN, INPUT);
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
  // Leer el estado del sensor de línea
    lineState = digitalRead(TRACK_SENSOR_PIN);

  // Medir la distancia con el sensor ultrasónico
    distance = measureDistance();

  // Imprimir información de diagnóstico
    Serial.print("Línea: ");
    Serial.print(lineState);
    Serial.print(" | Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");

  // Verificar si hay un obstáculo
    if (distance < MIN_DISTANCE) {
        // Hay un obstáculo delante
        Serial.println("¡Obstáculo detectado!");

        // Activar LED rojo, apagar LED verde
        setLEDs(false, true);
        obstacleDetected = true;

        // Detener el robot
        stopMotors();
        delay(500);

        // Escanear alrededor para encontrar una ruta alternativa
        int leftDistance = scanDirection(30);   // Escanear izquierda (30°)
        int rightDistance = scanDirection(150); // Escanear derecha (150°)

        // Centrar la cabeza nuevamente
        servoHead.write(90);
        delay(300);

        // Decidir hacia dónde girar
        if (leftDistance > rightDistance && leftDistance > MIN_DISTANCE) {
        // Girar a la izquierda
        turnLeft(TURN_SPEED);
        delay(700);
        } else if (rightDistance > MIN_DISTANCE) {
        // Girar a la derecha
        turnRight(TURN_SPEED);
        delay(700);
        } else {
        // Dar la vuelta
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
  // Limpiar el pin Trigger
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

  // Enviar pulso de 10 microsegundos
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Leer el tiempo de respuesta
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calcular la distancia en cm
    return duration * 0.034 / 2;
}

// Función para escanear en una dirección específica
int scanDirection(int angle) {
    servoHead.write(angle);
    delay(500);  // Esperar a que el servo se posicione
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
    if (lineState == 1) {
        // Sobre la línea, avanzar recto
        moveForward(BASE_SPEED);
    } else {
        // Fuera de la línea, buscar la línea
        findLine();
    }
}

// Función para buscar la línea cuando se ha perdido
void findLine() {
  // Estrategia simple: girar ligeramente a la izquierda y luego a la derecha
  // hasta encontrar la línea nuevamente
    turnLeft(SLOW_SPEED);
    delay(100);

    // Verificar si hemos encontrado la línea
    if (digitalRead(TRACK_SENSOR_PIN) == 1) {
        return;
    }

    // Si no, girar a la derecha
    turnRight(SLOW_SPEED);
    delay(200);

    // Verificar si hemos encontrado la línea
    if (digitalRead(TRACK_SENSOR_PIN) == 1) {
        return;
    }

    // Si aún no, girar a la izquierda nuevamente pero más tiempo
    turnLeft(SLOW_SPEED);
    delay(300);
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