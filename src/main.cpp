/**
 *  @file    main.cpp
 *  @author  Manuel Bojato (KingDarBoja)
 *  @date    X/XX/XXXX
 *  @version 1.0
 *
 *  @brief Hexapod Development Main Code, Electronic Engineering Final Project
 *
 *  @section DESCRIPTION
 *
 *  Program used to execute movements of a six legged robot or "Hexapod"
 *  (Lynxmotion AH3-R - 18 servos) from char inputs obtained by serial port
 *  (wireless / wired) communication from a remote computer.
 *
 *  The program uses five ultrasonic sensors (HC-SR04) for obstacle detection,
 *  whose values are sent through serial com to the remote computer.
 *
*/

// ARDUINO MAIN LIBRARY
#include <Arduino.h>

// LIBRARIES
#include <Wire.h>
#include <HexCalibrator.h>
#include <Kalman.h>

// Include external files
extern void servoAttachment();
extern void WakeUp();
extern void Adelante();
extern void Atras();
extern void Izquierda();
extern void Derecha();
extern void Parche();
extern void Derecha_2();
extern void Izquierda_2();
extern void Adelante_2();

// Declare bool variable to check if the hexapod has performed 'WakeUp' Action.
bool awake = false;

// Declare string variables for the ultrasonic measures.
String stringFX = "0", stringLD = "0", stringRD = "0", stringLX = "0", stringRX = "0";
String msg = "0:0:0:0:0:0"; // string to be sent, default.

// ULTRASONIC SENSOR TRIGGER AND ECHO PINS (TP / EP)
// F - Front, L - Left, R - Right, D - Diagonal, X - Side
const int TP_FX = 40, EP_FX = 41,
          TP_LD = 5,  EP_LD = 4,
          TP_RD = 38, EP_RD = 39,
          TP_LX = 7,  EP_LX  = 6,
          TP_RX = 49, EP_RX  = 50;


/**
    Applies PWM to the specified trigger pin, measure the echo and returns
    the converted centimeter value.

    @param TriggerPin The assigned ultrasonic trigger pin.
    @param EchoPin The assigned ultrasonic echo pin.
    @return Centimeter measure of sensor.
*/
int ping(int TriggerPin, int EchoPin)
{
   long duration, distanceCm;

   // In order to generate a clean pulse, set-up LOW at 4us.
   digitalWrite(TriggerPin, LOW);
   delayMicroseconds(4);

   // Generate Trigger (shot) of 10us
   digitalWrite(TriggerPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);

   // Measure of time between pulses on microseconds.
   duration = pulseIn(EchoPin, HIGH);

   // Distance conversion to cm.
   distanceCm = duration * 10 / 292/ 2;
   return distanceCm;
}

/*
// ===================== HEXAPOD CALIBRATOR ROUTINE =====================
// Uncomment this code block (place '/' at the opening comment tag)
// in order to enable hexapod calibrator routine.
// Note: The other code-blocks are labeled like this one.
HexCalibrator hexcal(82, 87, 90, 92, 92, 89, 86, 84, 93, 96, 86, 104, 80, 97, 88, 98, 92, 83);
//*/

/**
    Limit to '100' ultrasonic processed data in order to make it work for the
    remote computer algorythm.

    @param mvalue proccesed value from ultrasonic measure.
    @return mvalue limited value based on the specified limit.
*/
int limitValue(int mvalue)
{
  if (mvalue > 100) {
    mvalue = 100;
    return mvalue;
  } else {
    return mvalue;
  }
}

// Setup code to run once.
void setup() {
  // Pin assigment of every servo of the hexapod.
  // In order to edit it, open the movements.cpp file.
  servoAttachment();

  /*
  // ======================= GYROSCOPE CALIBRATION =======================
  // Enables the gyroscope calibration setup. If the MPU6050 needs to be calibrated,
  // you must uncomment the code block labeled like this.
  gyroCalibrationSetting();
  //*/

  /*
  // ===================== HEXAPOD CALIBRATOR ROUTINE =====================
  hexcal.HexPawPin(43, 44, 45, 14, 15, 16, 9, 10, 11, 22, 23, 24, 46, 47, 48, 51, 52, 53);
  //*/

  // Start the communication at the serial ports.
  Serial1.begin(9600);
  Serial.begin(9600);

  /*
  // ===================== ULTRASONIC MEASURE =====================
  // Enables all the ultrasonic sensors for obstacle measurement.
  // Warning: Not connected sensors will heavily slow down the code.
  pinMode(TP_FX, OUTPUT);
  pinMode(TP_RD, OUTPUT);
  pinMode(TP_LD, OUTPUT);
  pinMode(TP_RX , OUTPUT);
  pinMode(TP_LX , OUTPUT);
  pinMode(EP_FX, INPUT);
  pinMode(EP_RD, INPUT);
  pinMode(EP_LD, INPUT);
  pinMode(EP_LX , INPUT);
  pinMode(EP_RX , INPUT);
  //*/
}

// put your main code here, to run repeatedly:
void loop() {
  /*
  // ===================== HEXAPOD CALIBRATOR ROUTINE =====================
  hexcal.ServoCalibrator();
  //*/

  /*
  // ======================= GYROSCOPE CALIBRATION =======================
  gyroCalibrationLoop();
  //*/

  /*
  // ========================= ULTRASONIC MEASURE =========================
  // Checking the values of Ultrasonic sensor and storing them into int variables.
  int cm_FX = ping(TP_FX, EP_FX);
  int cm_RD = ping(TP_RD, EP_RD);
  int cm_LD = ping(TP_LD, EP_LD);
  int cm_RX  = ping(TP_RX , EP_RX );
  int cm_LX  = ping(TP_LX , EP_LX );

  // Conversion from int to string in order to send the package through serial port
  stringFX =  String(limitValue(cm_FX));
  stringRD =  String(limitValue(cm_RD));
  stringLD =  String(limitValue(cm_LD));
  stringRX =  String(limitValue(cm_RX));
  stringLX =  String(limitValue(cm_LX));

  // Message string to be send
  msj = stringFX + ":" + stringRD + ":" + stringLD + ":" + stringRX + ":" + stringLX;
  Serial1.println(msj);
  //*/

  delay(200);
  /*
  // ========================= HEXAPOD ACTIONS =========================
  // The remote computer will receive the message string and make computational
  // calculus to output a single char. It will be received via serial port,
  // and the board will execute a movement based on that char.
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
        Adelante_2();
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
      case 'T':
        Serial.println("Derecha vieja");
        Derecha();
        break;
      default:
        Serial.println("Default");
        Parche();
        break;
    }
  }
  //*/
}
