// Main Program: Hexapod_Development Code
// This code includes every ultrasonic sensor as inputs along with the movements of the hexapods as outputs.
// Developed by Andrea Fontalvo (First Hexapod - Uninorte)
// Edited by Manuel Bojato, Augusto Amador (Second Hexapod - Uninorte)

// ARDUINO MAIN LIBRARY
#include <Arduino.h>

// LIBRARIES
// #include <Servo.h>
#include <Wire.h>
#include <HexCalibrator.h>

// Include external files
extern void gyroSetting();
extern void gyroMeasure();
extern void servoAttachment();
extern void WakeUp();
extern void Adelante();
extern void Atras();
extern void Izquierda();
extern void Derecha();
extern void Parche();
extern void Derecha_2();
extern void Izquierda_2();

// ULTRASONIC SENSOR TRIGGER AND ECHO PINS (TP / EP)
// TRIGGER PINS
const int TP_FU = 40;
const int TP_LU = 5;
const int TP_RU = 38;
const int TP_L  = 7;
const int TP_R  = 49;

// ECHO PINS
const int EP_FU = 41;
const int EP_LU = 4;
const int EP_RU = 39;
const int EP_L  = 6;
const int EP_R  = 50;

bool awake = false;

/*
  ····· MAIN VARIABLES DEFINITIONS ·····
  Uncomment the below string variables in order to enable the ultrasonic sensor and
  string conversion from float values (cm) to string.
*/
// String stringFU, stringRU, stringLU, stringR, stringL;

// Servo definition
// Servo myservo;

/*
  ----- Hexapod Calibrator -----
  Uncomment the following line along with the ones labeled as "Hexapod Calibrator"
  in order to start the calibration of the hexapod
*/
// HexCalibrator hexcal(82, 87, 90, 92, 92, 89, 86, 84, 93, 96, 86, 104, 80, 97, 88, 98, 92, 83);

/*
  String message to be sent via Serial1 port.
*/
String msj;

// FUNCTION PING FOR TRIGGER / ECHO PAIR
int ping(int TriggerPin, int EchoPin)
{
   long duration, distanceCm;

   // In order to generate a clean pulse, set-up LOW at 4us.
   // Para generar un pulso limpio ponemos a LOW 4us
   digitalWrite(TriggerPin, LOW);
   delayMicroseconds(4);
   // Generate Trigger (shot) of 10us
   // Generamos Trigger (disparo) de 10us
   digitalWrite(TriggerPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);

   // Measure of time between pulses on microseconds.
   // Medimos el tiempo entre pulsos, en microsegundos.
   duration = pulseIn(EchoPin, HIGH);

   // Distance conversion to cm.
   distanceCm = duration * 10 / 292/ 2;
   return distanceCm;
}

// FUNCTION TO LIMIT ULTRASONIC VALUES
int limitValue(int mvalue)
{
  int rvalue = 0;
  if (mvalue > 100)
  {
    rvalue = 100;
  } else
  {
    rvalue = mvalue;
  }
  return rvalue;
}

void setup() {
    // put your setup code here, to run once:
    servoAttachment();
    // gyroSetting();
    // hexcal.HexPawPin(43, 44, 45, 14, 15, 16, 9, 10, 11, 22, 23, 24, 46, 47, 48, 51, 52, 53);
    Serial1.begin(9600);
    Serial.begin(9600);

    // PIN MODE INITIALIZATION FOR EVERY TRIGGER / ECHO PIN
    // pinMode(TP_FU, OUTPUT);
    // pinMode(TP_RU, OUTPUT);
    // pinMode(TP_LU, OUTPUT);
    // pinMode(TP_R , OUTPUT);
    // pinMode(TP_L , OUTPUT);
    //
    // pinMode(EP_FU, INPUT);
    // pinMode(EP_RU, INPUT);
    // pinMode(EP_LU, INPUT);
    // pinMode(EP_L , INPUT);
    // pinMode(EP_R , INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
    ----- Hexapod Calibrator -----
  */
  // hexcal.ServoCalibrator();

  // gyroMeasure function
  // gyroMeasure();

  // START THE MOVEMENT, BUT VERIFY IF THE PROGRAM IS RUNNING THE FIRST TIME.

  // Checking the values of Ultrasonic sensor and storing them into int variables.
  // int cm_FU = ping(TP_FU, EP_FU);
  // int cm_RU = ping(TP_RU, EP_RU);
  // int cm_LU = ping(TP_LU, EP_LU);
  // int cm_R  = ping(TP_R , EP_R );
  // int cm_L  = ping(TP_L , EP_L );
  //
  // // Conversion from int to string in order to send the package through serial port
  // stringFU =  String(limitValue(cm_FU));
  // stringRU =  String(limitValue(cm_RU));
  // stringLU =  String(limitValue(cm_LU));
  // stringR =  String(limitValue(cm_R));
  // stringL =  String(limitValue(cm_L));
  //
  // msj = stringFU + ":" + stringRU + ":" + stringLU + ":" + stringR + ":" + stringL;
  // Serial1.println(msj);
  // //Serial.println(msj);
  // delay(200);
  //
  delay(200);
  if (awake == false) {
    WakeUp();
    Parche();
    awake = true;
  }
  if (Serial.available()>0)
  {
    char movCase = Serial.read();
    switch(movCase)
    {
      case 'F':
        Serial.println("Adelante");
        break;
      case 'L':
        Serial.println("Izquierda");
        Izquierda_2();
        break;
      case 'R':
        Serial.println("Derecha");
        Derecha_2();
        break;
      case 'B':
        Serial.println("Atras");
        break;
      default:
        Serial.println("Default");
        Parche();
        break;
    }
  }
}
