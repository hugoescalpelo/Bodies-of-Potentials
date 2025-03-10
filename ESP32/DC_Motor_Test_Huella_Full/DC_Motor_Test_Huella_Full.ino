/*
  Control de 4 Motores DC con ESP32DevKitV1

  Este programa controla 4 motores DC utilizando un ESP32DevKitV1 y dos drivers H-Bridge. 
  Los motores solo funcionan en una dirección debido a las limitaciones mecánicas del sistema.
  El motor conectado a los pines 18 y 19 funciona con control PWM, permitiendo ajustar su velocidad.

  Conexiones:
  - Driver A:
    - B1-A -> GPIO 13
    - B2-A -> GPIO 12
    - A-1A -> GPIO 27
    - A-1B -> GPIO 26
  - Driver B:
    - A-1B -> GPIO 16
    - A-1A -> GPIO 17
    - B-2A -> GPIO 18 (PWM control)
    - B-1A -> GPIO 19

  Parámetros:
  - Velocidad del motor controlado por PWM: Configurable mediante la variable 'velocidadMotorPWM'.

  Precauciones:
  - Los motores solo se activan en la primera dirección configurada para evitar dañar el mecanismo.
  
  Autor: Hugo 
  Fecha: [Fecha Actual]
  Licencia: MIT (Licencia de código abierto)
*/

// Velocidad del motor controlado por PWM (valor entre 0 y 255)
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

void setup() {
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
