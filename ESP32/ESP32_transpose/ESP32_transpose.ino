#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40); // Dirección I2C 0x40

// Configuración del rango de PWM calibrado para los servos
const int SERVO_MIN = 350; // Valor mínimo de PWM para el ángulo inicial calibrado
const int SERVO_MAX = 505; // Valor máximo de PWM para el ángulo final calibrado
const int SERVO_CENTRO = 460; // Valor de PWM para el ángulo central calibrado

// Pines para los motores DC controlados por el puente H
const int motor1A1 = 27;
const int motor1A2 = 26;
const int motor1B1 = 13;
const int motor1B2 = 12;
const int motor2A1 = 17;
const int motor2A2 = 16;
const int motor2B1 = 19; // PWM
const int motor2B2 = 18;

int velocidadMotorPWM = 250; // Velocidad PWM para el motor 2

// Variables para control no bloqueante
unsigned long tiempoAnteriorServos = 0;
unsigned long tiempoAnteriorMotores = 0;
const unsigned long intervaloServo = 10; // Tiempo entre actualizaciones de los servos (ms)
const unsigned long intervaloMotor = 5; // Tiempo entre actualizaciones de los motores (ms)

// Variables para controlar los servos y sus grupos
byte gruposervo[16] = {55, 55, 55, 3, 4, 1, 2, 55, 55, 0, 5, 3, 2, 55, 55, 55}; // Grupo de cada servo
int posiciones[16];
int velocidades[16][5] = {0}; // Velocidades y estados de los servos

float Tiempos[16][3] = {{2.5, 2, 2}, {4, 1.5, 2}, {1.5, 2.5, 3}, {3, 2.5, 1.5}, {2, 0.5, 1}, {1, 0.5, 2}};
byte Bsubidalista[16], Bbajadalista[16];
int ContadorPausa[16];
float Texe = 5.08;
float Textra = 46.7;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando ESP32 para control de motores y servos...");

  // Inicializa la comunicación I2C
  Wire.begin(21, 22); // SDA en GPIO 21, SCL en GPIO 22

  // Inicializa el módulo PCA9685
  pca9685.begin();
  pca9685.setPWMFreq(50);

  // Configura los pines de los motores como salidas
  pinMode(motor1A1, OUTPUT);
  pinMode(motor1A2, OUTPUT);
  pinMode(motor1B1, OUTPUT);
  pinMode(motor1B2, OUTPUT);
  pinMode(motor2A1, OUTPUT);
  pinMode(motor2A2, OUTPUT);
  pinMode(motor2B1, OUTPUT);
  pinMode(motor2B2, OUTPUT);
  desactivarMotores();

  // Inicializa las posiciones de los servos
  for (int i = 0; i < 16; i++) {
    posiciones[i] = SERVO_CENTRO;
  }

  // Configuración de tiempos de subida y bajada
  float Periodo = Texe + Textra;
  for (int i = 0; i <= 5; i++) {
    Bsubidalista[i] = round(Periodo * 0.218 / Tiempos[i][0]);
    ContadorPausa[i] = round(Tiempos[i][1] * 1000 / Periodo) - 1;
    Bbajadalista[i] = round(Periodo * 0.218 / Tiempos[i][2]);
  }

  // Asignación de velocidades
  for (int i = 0; i < 16; i++) {
    velocidades[i][0] = (gruposervo[i] == 55) ? 0 : Bsubidalista[gruposervo[i]];
    velocidades[i][1] = (gruposervo[i] == 55) ? 0 : ContadorPausa[gruposervo[i]];
    velocidades[i][2] = (gruposervo[i] == 55) ? 0 : -Bbajadalista[gruposervo[i]];
  }

  // Mueve los servos a la posición inicial
  for (int i = 3; i < 14; i++) {
    moverServo(i, posiciones[i]);
  }

  activarMotores();
}

void loop() {
  unsigned long tiempoActual = millis();

  // Actualización no bloqueante para servos
  if (tiempoActual - tiempoAnteriorServos >= intervaloServo) {
    actualizarServos();
    tiempoAnteriorServos = tiempoActual;
  }

  // Actualización no bloqueante para motores
  if (tiempoActual - tiempoAnteriorMotores >= intervaloMotor) {
    actualizarMotores();
    tiempoAnteriorMotores = tiempoActual;
  }
}

// Función para actualizar los estados de los servos
void actualizarServos() {
  for (int i = 3; i < 13; i++) {
    switch (velocidades[i][3]) {
      case 0:
        if (posiciones[i] < SERVO_MAX) {
          moverServo(i, posiciones[i]);
          posiciones[i] += velocidades[i][0];
        } else {
          velocidades[i][3]++;
          posiciones[i] = SERVO_MAX;
        }
        break;
      case 1:
        if (velocidades[i][4] < velocidades[i][1]) {
          velocidades[i][4]++;
        } else {
          velocidades[i][3]++;
          velocidades[i][4] = 0;
        }
        break;
      case 2:
        if (posiciones[i] > SERVO_MIN) {
          moverServo(i, posiciones[i]);
          posiciones[i] += velocidades[i][2];
        } else {
          velocidades[i][3] = 0;
          posiciones[i] = SERVO_MIN;
        }
        break;
    }
  }
}

// Función para activar los motores en un solo sentido
void activarMotores() {
  digitalWrite(motor1A1, HIGH);
  digitalWrite(motor1A2, LOW);
  digitalWrite(motor1B1, HIGH);
  digitalWrite(motor1B2, LOW);
  digitalWrite(motor2A1, HIGH);
  digitalWrite(motor2A2, LOW);
  analogWrite(motor2B1, velocidadMotorPWM); // Controla la velocidad con PWM
  digitalWrite(motor2B2, LOW);
}

// Función para actualizar el estado de los motores DC
void actualizarMotores() {
  // Actualmente, se mantiene la activación continua en un solo sentido
  activarMotores();
}

// Función para mover un servo a una posición específica
void moverServo(int numServo, int pwmValue) {
  if (gruposervo[numServo] == 55) return;
  pca9685.setPWM(numServo, 0, pwmValue);
}

// Función para desactivar los motores
void desactivarMotores() {
  digitalWrite(motor1A1, LOW);
  digitalWrite(motor1A2, LOW);
  digitalWrite(motor1B1, LOW);
  digitalWrite(motor1B2, LOW);
  digitalWrite(motor2A1, LOW);
  digitalWrite(motor2A2, LOW);
  analogWrite(motor2B1, 0); // Detiene el PWM
  digitalWrite(motor2B2, LOW);
}
