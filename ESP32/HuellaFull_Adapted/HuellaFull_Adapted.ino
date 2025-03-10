#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40); // Dirección I2C 0x40 por defecto

int velocidadMotorPWM = 250;

const byte motor1A1 = 27; // A-1A del driver A
const byte motor1A2 = 26; // A-1B del driver A
const byte motor1B1 = 13; // B1-A del driver A
const byte motor1B2 = 12; // B2-A del driver A

const byte motor2A1 = 17; // A-1A del driver B
const byte motor2A2 = 16; // A-1B del driver B
const byte motor2B1 = 19; // B-1A del driver B (PWM)
const byte motor2B2 = 18; // B-2A del driver B

bool motor1Activado = false;
bool motor2Activado = false;

// Configuración del rango de PWM calibrado para los servos
const int SERVO_MIN = 350; // Valor mínimo de PWM para el ángulo inicial calibrado
const int SERVO_MAX = 505; // Valor máximo de PWM para el ángulo final calibrado
const int SERVO_CENTRO = 460; // Valor de PWM para el ángulo central calibrado

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando prueba de servomotores con rango calibrado...");

  // Inicializa la comunicación I2C
  Wire.begin(21, 22); // SDA en GPIO 21, SCL en GPIO 22

  // Inicializa el módulo PCA9685
  pca9685.begin();
  pca9685.setPWMFreq(50); // Frecuencia de 50 Hz para control de servos

  // Configuración de los pines como salidas
  pinMode(motor1A1, OUTPUT);
  pinMode(motor1A2, OUTPUT);
  pinMode(motor1B1, OUTPUT);
  pinMode(motor1B2, OUTPUT);
  
  pinMode(motor2A1, OUTPUT);
  pinMode(motor2A2, OUTPUT);
  pinMode(motor2B1, OUTPUT);
  pinMode(motor2B2, OUTPUT);

  // Inicialización de los motores apagados
  apagarMotores();

  // Coloca todos los servos en la posición central
  for (int i = 0; i < 11; i++) {
    moverServo(i, SERVO_CENTRO);
  }
  delay(2000); // Pausa de 5 segundos en la posición central

}

void loop() {
  // Ejemplo de prueba: encender los motores si no están activados
  if (!motor1Activado) {
    activarMotor1();  // Activa el motor 1 en un sentido
    motor1Activado = true;
  }
  
  if (!motor2Activado) {
    activarMotor2();  // Activa el motor 2 con PWM en un sentido
    motor2Activado = true;
  }
  
  
  // Mantener los motores encendidos indefinidamente
  delay(1000);

  // Mover suavemente todos los servos al ángulo mínimo calibrado
  for (int i = 0; i < 11; i++) {
    moverSuavemente(i, SERVO_CENTRO, SERVO_MIN);
  }
  delay(1000); // Quedarse 2 segundos en la posición mínima

  // Mover suavemente todos los servos al ángulo central calibrado
  for (int i = 0; i < 11; i++) {
    moverSuavemente(i, SERVO_MIN, SERVO_CENTRO);
  }
  delay(2000); // Quedarse 5 segundos en el centro

  // Mover suavemente todos los servos al ángulo máximo calibrado
  for (int i = 0; i < 11; i++) {
    moverSuavemente(i, SERVO_CENTRO, SERVO_MAX);
  }
  delay(1000); // Quedarse 2 segundos en la posición máxima

  // Mover suavemente todos los servos al ángulo central calibrado
  for (int i = 0; i < 11; i++) {
    moverSuavemente(i, SERVO_MAX, SERVO_CENTRO);
  }
  delay(2000); // Quedarse 5 segundos en el centro
}

// Función para activar el motor 1 en un solo sentido
void activarMotor1() {
  // Activa en un solo sentido
  digitalWrite(motor1A1, HIGH);
  digitalWrite(motor1A2, LOW);
  digitalWrite(motor1B1, HIGH);
  digitalWrite(motor1B2, LOW);
}

// Función para activar el motor 2 con control PWM en un solo sentido
void activarMotor2() {
  // Activa en un solo sentido con control PWM
  digitalWrite(motor2A1, HIGH);
  digitalWrite(motor2A2, LOW);
  analogWrite(motor2B1, velocidadMotorPWM); // Controla la velocidad con PWM
  digitalWrite(motor2B2, LOW);
}

// Función para apagar todos los motores
void apagarMotores() {
  digitalWrite(motor1A1, LOW);
  digitalWrite(motor1A2, LOW);
  digitalWrite(motor1B1, LOW);
  digitalWrite(motor1B2, LOW);
  
  digitalWrite(motor2A1, LOW);
  digitalWrite(motor2A2, LOW);
  digitalWrite(motor2B1, LOW);
  digitalWrite(motor2B2, LOW);
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