/*
  Prueba de Servomotores con PCA9685 y Rango Calibrado en ESP32

  Este programa controla 16 servomotores utilizando el módulo PCA9685 y un ESP32DevKitV1.
  Los movimientos se adaptan a un rango calibrado debido a las restricciones mecánicas, 
  con ángulos configurados para centro, mínimo y máximo. Al finalizar el movimiento de los servos,
  el programa desactiva los pines de los motores DC para evitar daños en el puente H.

  Conexiones:
  - PCA9685:
    - VCC -> 5V
    - GND -> GND
    - SDA -> GPIO 21 (ESP32)
    - SCL -> GPIO 22 (ESP32)
  - Servomotores:
    - Conectar los servos a los canales 0-15 del PCA9685
  - Puente H (Motores DC):
    - Configura los pines según tu conexión específica para controlar el puente H

  Autor: [Tu Nombre]
  Fecha: [Fecha Actual]
  Licencia: MIT (Licencia de código abierto)
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40); // Dirección I2C 0x40 por defecto

// Configuración del rango de PWM calibrado para los servos
const int SERVO_MIN = 350; // Valor mínimo de PWM para el ángulo inicial calibrado
const int SERVO_MAX = 505; // Valor máximo de PWM para el ángulo final calibrado
const int SERVO_CENTRO = 460; // Valor de PWM para el ángulo central calibrado

// Pines para los motores DC controlados por el puente H
const int motorPin1 = 27; // Ajusta según tu conexión
const int motorPin2 = 26; // Ajusta según tu conexión
const int motorPin3 = 13; // Ajusta según tu conexión
const int motorPin4 = 12; // Ajusta según tu conexión

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando prueba de servomotores con rango calibrado...");

  // Inicializa la comunicación I2C
  Wire.begin(21, 22); // SDA en GPIO 21, SCL en GPIO 22

  // Inicializa el módulo PCA9685
  pca9685.begin();
  pca9685.setPWMFreq(50); // Frecuencia de 50 Hz para control de servos

  // Configura los pines de los motores como salidas y desactívalos
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  desactivarMotores();

  // Coloca todos los servos en la posición central
  for (int i = 0; i < 16; i++) {
    moverServo(i, SERVO_CENTRO);
  }
  delay(2000); // Pausa de 5 segundos en la posición central
}

void loop() {
  // Mover suavemente todos los servos al ángulo mínimo calibrado
  for (int i = 0; i < 16; i++) {
    moverSuavemente(i, SERVO_CENTRO, SERVO_MIN);
  }
  delay(1000); // Quedarse 2 segundos en la posición mínima

  // Mover suavemente todos los servos al ángulo central calibrado
  for (int i = 0; i < 16; i++) {
    moverSuavemente(i, SERVO_MIN, SERVO_CENTRO);
  }
  delay(2000); // Quedarse 5 segundos en el centro

  // Mover suavemente todos los servos al ángulo máximo calibrado
  for (int i = 0; i < 16; i++) {
    moverSuavemente(i, SERVO_CENTRO, SERVO_MAX);
  }
  delay(1000); // Quedarse 2 segundos en la posición máxima

  // Mover suavemente todos los servos al ángulo central calibrado
  for (int i = 0; i < 16; i++) {
    moverSuavemente(i, SERVO_MAX, SERVO_CENTRO);
  }
  delay(2000); // Quedarse 5 segundos en el centro

  // Desactivar los pines de los motores DC al finalizar el ciclo
  desactivarMotores();
}

// Función para mover un servo a una posición específica
void moverServo(int numServo, int pwmValue) {
  pca9685.setPWM(numServo, 0, pwmValue);
  Serial.print("Servo ");
  Serial.print(numServo);
  Serial.print(" movido a valor PWM: ");
  Serial.println(pwmValue);
}

// Función para mover suavemente el servo desde un valor PWM a otro
void moverSuavemente(int numServo, int desde, int hasta) {
  int paso = (desde < hasta) ? 1 : -1;
  for (int pwmValue = desde; pwmValue != hasta; pwmValue += paso) {
    moverServo(numServo, pwmValue);
    delay(5); // Ajusta el valor para un movimiento más suave o rápido
  }
}

// Función para desactivar los motores DC
void desactivarMotores() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  Serial.println("Motores desactivados.");
}
