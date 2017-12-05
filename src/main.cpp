// Main Program: Hexapod_Development Code
// This code includes every ultrasonic sensor as inputs along with the movements of the hexapods as outputs.
// Developed by Andrea Fontalvo (First Hexapod - Uninorte)
// Edited by Manuel Bojato, Augusto Amador (Second Hexapod - Uninorte)

//ARDUINO MAIN LIBRARY
#include <Arduino.h>

// //LIBRARIES
// #include <Servo.h>
//
// //GLOBAL VARIABLES
// int cc = 0;
// void MoveCommand(int s11o, int s21o, int s31o, int s41o, int s51o, int s61o,
//                  int s12o, int s22o, int s32o, int s42o, int s52o, int s62o,
//                  int s13o, int s23o, int s33o, int s43o, int s53o, int s63o,
//                  int DelayTime);
// void WakeUp();
// void Adelante();
// void Parche();
// void Atras();
// void Derecha();
// void Izquierda();

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

// // MAIN VARIABLES DEFINITIONS
// String values of cm conversion for each ultrasonic sensor
String stringFU, stringRU, stringLU, stringR, stringL;

// Servo definition
// Servo myservo;

// String message to be sent via Serial1 port.
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
    Serial1.begin(9600);
    Serial.begin(9600);

    // PIN MODE INITIALIZATION FOR EVERY TRIGGER / ECHO PIN
    pinMode(TP_FU, OUTPUT);
    pinMode(TP_RU, OUTPUT);
    pinMode(TP_LU, OUTPUT);
    pinMode(TP_R , OUTPUT);
    pinMode(TP_L , OUTPUT);

    pinMode(EP_FU, INPUT);
    pinMode(EP_RU, INPUT);
    pinMode(EP_LU, INPUT);
    pinMode(EP_L , INPUT);
    pinMode(EP_R , INPUT);
}

void loop() {
//  put your main code here, to run repeatedly:

  // START THE MOVEMENT, BUT VERIFY IF THE PROGRAM IS RUNNING THE FIRST TIME.

  // Checking the values of Ultrasonic sensor and storing them into int variables.
  int cm_FU = ping(TP_FU, EP_FU);
  int cm_RU = ping(TP_RU, EP_RU);
  int cm_LU = ping(TP_LU, EP_LU);
  int cm_R  = ping(TP_R , EP_R );
  int cm_L  = ping(TP_L , EP_L );

  // Conversion from int to string in order to send the package through serial port
  stringFU =  String(limitValue(cm_FU));
  stringRU =  String(limitValue(cm_RU));
  stringLU =  String(limitValue(cm_LU));
  stringR =  String(limitValue(cm_R));
  stringL =  String(limitValue(cm_L));

  msj = stringFU + ":" + stringRU + ":" + stringLU + ":" + stringR + ":" + stringL;
  Serial1.println(msj);
  Serial.println(msj);
  delay(500);

  if (Serial1.available()>0)
  {
    char movCase = Serial1.read();
    switch(movCase)
    {
      case 'F':
        Serial.println("Adelante");
        break;
      case 'L':
        Serial.println("Izquierda");
        break;
      case 'R':
        Serial.println("Derecha");
        break;
      case 'B':
        Serial.println("Atras");
        break;
      default:
        Serial.println("Default");
        break;
    }
  }

 // msj = "hola:12,3";
 // Serial1.println(msj);
 // delay(500);
 // if (Serial1.available()>0)
 // {
 //   char movCase = Serial1.read();
 //   switch(movCase)
 //   {
 //     case 'F':
 //       Serial.println("Adelante");
 //       break;
 //     default:
 //       Serial.println("Default");
 //       break;
 //   }
 // }
}
