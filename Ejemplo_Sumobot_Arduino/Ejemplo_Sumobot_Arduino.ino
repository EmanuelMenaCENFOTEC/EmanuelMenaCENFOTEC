/*
*Código Simple de las funciones del SumoBot de la Universidad CENFOTEC
*
*Emanuel Mena Araya 2024
*/

#include "SRF05.h"

// Pines para los sensores y motores
const int ir      = 33;
const int trig    = 26;
const int echo    = 25;

const int motor_1[2] = {12, 14}; 
const int motor_2[2] = {13, 15}; 

double  dist   = 0;  // Distancia medida por el sensor de ultrasonidos
bool    sensor = 0;  // Estado del sensor IR

SRF05 SRF(trig, echo);

/**
 * Configura los pines y el sensor de ultrasonidos.
 */
void setup() {
  Serial.begin(9600);

  pinMode(motor_1[0], OUTPUT);
  pinMode(motor_1[1], OUTPUT);
  pinMode(motor_2[0], OUTPUT);
  pinMode(motor_2[1], OUTPUT);
  pinMode(ir, INPUT);

  SRF.setCorrectionFactor(1.035);
}

/**
 * Bucle principal que controla el movimiento del robot en función de las lecturas del sensor.
 */
void loop() {
  dist = SRF.getCentimeter();
  sensor = digitalRead(ir);

  if (dist > 10 && !sensor) {
    adelante();
  } else {
    retroceder();
  }
  
  delay(100);
}

/**
 * Mueve el robot hacia adelante.
 */
void adelante() {
  motor(motor_1, 1);
  motor(motor_2, 1);
  delay(100);
}

/**
 * Mueve el robot hacia atrás y realiza una maniobra de evasión.
 */
void retroceder() {
  int dir = random(1);

  motor(motor_1, 0);
  motor(motor_2, 0);
  delay(500);
  motor(motor_1, -1);
  motor(motor_2, -1);
  delay(2000);
  motor(motor_1, 0);
  motor(motor_2, 0);
  delay(500);

  if (dir == 0) {
    motor(motor_1, 1);
    motor(motor_2, 0);
    delay(2000);
  } else {
    motor(motor_1, 0);
    motor(motor_2, 1);
    delay(2000);
  }

  motor(motor_1, 0);
  motor(motor_2, 0);
  delay(500);
}

/**
 * Mapea un valor de un rango a otro rango.
 * 
 * @param x Valor a mapear
 * @param in_min Valor mínimo del rango de entrada
 * @param in_max Valor máximo del rango de entrada
 * @param out_min Valor mínimo del rango de salida
 * @param out_max Valor máximo del rango de salida
 * @return Valor mapeado en el rango de salida
 */
double mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Controla el movimiento de un motor.
 * 
 * @param motor Arreglo con los pines del motor
 * @param velocidad Velocidad del motor (entre -1 y 1)
 */
void motor(const int motor[2], double velocidad) {
  // Verificar si la velocidad está fuera del rango permitido
  if (velocidad > 1 || velocidad < -1) {
    Serial.println("Error: Velocidad fuera del rango permitido (-1 a 1)");
    while (true); // Detener el programa en un bucle infinito
  }
  
  int v = 0;

  if (velocidad == 0) {
    analogWrite(motor[0], 0);
    analogWrite(motor[1], 0);
  } else if (velocidad > 0.00) {
    v = (int)mapDouble(velocidad, 0, 1, 0, 255);
    analogWrite(motor[0], v);
    analogWrite(motor[1], 0);
  } else if (velocidad < 0.00) {
    v = (int)mapDouble(velocidad, -1, 0, 255, 0);
    analogWrite(motor[0], 0);
    analogWrite(motor[1], v);
  }
}
