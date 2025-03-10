#include <Servo.h>

Servo motor;  // Crea un objeto servo para controlar el motor
int pinServo = 9;  // El pin al que está conectado el servo
int centro = 145;  // Ángulo central calculado
int angulo110 = 110;  // Ángulo inicial
int angulo180 = 180;  // Ángulo final

void setup() {
  motor.attach(pinServo);  // Asigna el pin del servo
  motor.write(centro);  // Coloca el motor en la posición central
  delay(5000);  // Espera 5 segundos en el centro
}

void loop() {
  // Mover suavemente al ángulo 110 grados
  moverSuavemente(centro, angulo110);
  delay(2000);  // Quedarse 2 segundos en 110 grados

  // Mover suavemente al ángulo central
  moverSuavemente(angulo110, centro);
  delay(5000);  // Quedarse 5 segundos en el centro

  // Mover suavemente a 180 grados
  moverSuavemente(centro, angulo180);
  delay(2000);  // Quedarse 2 segundos en 180 grados

  // Mover suavemente al ángulo central
  moverSuavemente(angulo180, centro);
  delay(5000);  // Quedarse 5 segundos en el centro
}

void moverSuavemente(int desde, int hasta) {
  int paso = (desde < hasta) ? 1 : -1;
  for (int pos = desde; pos != hasta; pos += paso) {
    motor.write(pos);
    delay(20);  // Ajusta el valor para un movimiento más suave o rápido
  }
}
