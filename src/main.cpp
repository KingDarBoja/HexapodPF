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
extern double gyroMeasureLoop();
extern void gyroMeasureSetting();
extern void gyroCalibrationLoop();
extern void gyroCalibrationSetting();
extern void servoAttachment();
extern void WakeUp();
extern void Parche();
extern void TurnRightSoft(double var_angle);
extern void TurnLeftSoft(double var_angle);
extern void ForwardTripodGait();
extern void PitchWalkingUp();
extern void PitchWalkingDown();
extern void ForwardWaveGait();

// Calculate based on max input size expected for one command
#define INPUT_SIZE 8

// Variables for serial reading.
const byte numChars = 8;
char receivedChars[numChars];
// temporary array for use when parsing
char tempChars[numChars];

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;

boolean newData = false;

//============

// Declare bool variable to check if the hexapod has performed 'WakeUp' Action.
bool awake = false;
bool slope = false;
double result = 0.0;

// unsigned long init_time, end_time, elapsed;

// Declare string variables for the ultrasonic measures.
String stringFX = "0", stringLD = "0", stringRD = "0", stringLX = "0", stringRX = "0";
String msg = "0:0:0:0:0:0:0"; // string to be sent, default.

// ULTRASONIC SENSOR TRIGGER AND ECHO PINS (TP / EP)
// F - Front, L - Left, R - Right, D - Diagonal, X - Side
const int TP_FX = 28, EP_FX = 29,
          TP_LD = 5,  EP_LD = 4,
          TP_RD = 26, EP_RD = 27,
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
  if (mvalue > 100 || mvalue < 1) {
    mvalue = 100;
  }
  return mvalue;
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,"&");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

    strtokIndx = strtok(NULL, "&"); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer
}

//============

// Setup code to run once.
void setup() {
  // Start the communication at the serial ports.
  Serial1.begin(9600);
  Serial.begin(9600);

  // Pin assigment of every servo of the hexapod.
  // In order to edit it, open the movements.cpp file.
  servoAttachment();

  /*
  // ======================= GYROSCOPE CALIBRATION =======================
  // Enables the gyroscope calibration setup. If the MPU6050 needs to be calibrated,
  // you must uncomment the code block labeled like this.
  gyroCalibrationSetting();
  //*/

  //*
  // ======================= GYROSCOPE MEASUREMENT =======================
  // Enables the gyroscope measurement setup. It will test the sensor connection
  // and get starting angles if succesful.
  gyroMeasureSetting();
  //*/

  /*
  // ===================== HEXAPOD CALIBRATOR ROUTINE =====================
  hexcal.HexPawPin(43, 44, 45, 14, 15, 16, 9, 10, 11, 22, 23, 24, 46, 47, 48, 51, 52, 53);
  //*/

  //*
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
  if (awake == false) {
    WakeUp();
    Parche();
    awake = true;
  }
  delay(3000);
  slope = false;
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

  // delay(200);
  //*
  // ========================= HEXAPOD ACTIONS =========================
  // The remote computer will receive the message string and make computational
  // calculus to output a single char. It will be received via serial port,
  // and the board will execute a movement based on that char.

  // New comm protocol here.
  if (result > 2.0 && slope == false) {
    PitchWalkingUp();
    slope = true;
  } else if ((result > -2.0 && result < 2.0) && slope == true) {
    Parche();
    slope = false;
  } else if (result < -2.0 && slope == false) {
    PitchWalkingDown();
    slope = true;
  }
  delay(100);
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    // because strtok() used in parseData() replaces the commas with \0
    parseData();
    // Execute movements based on input command: <REV&###>.
    if (integerFromPC <= 15.00 && integerFromPC >= -15.00) {
      ForwardTripodGait();
      Serial1.println("|ACK|");
    } else {
      signed int n_total = round(integerFromPC * 2.5);
      do {
        if (n_total > 60) {
          for (int i = 0; i < n_total / 60; i++) {
            TurnLeftSoft(60);
            n_total -= 60;
            // Serial.println(n_total);
          }
        } else if (n_total > 0 && n_total <= 60) {
          TurnLeftSoft(n_total);
          n_total = 0;
          // Serial.println(n_total);
        } else if (n_total < -60) {
          for (int i = -1; i >= n_total / 60; i--) {
            TurnLeftSoft(-60);
            n_total += 60;
            // Serial.println(n_total);
          }
        } else if (n_total >= -60 && n_total < 0) {
            TurnLeftSoft(n_total);
            n_total = 0;
            // Serial.println(n_total);
        }
      } while (n_total != 0);
      Serial1.println("|ACK|");
    }
    newData = false;
  } else {
    //*
    // ======================= GYROSCOPE MEASUREMENT =======================
    // Measure raw data from sensor and print the processed data.
    result = round(gyroMeasureLoop());
    //*/

    //*
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
    msg = "MSG:" + stringFX + ":" + stringLD + ":" + stringRD + ":" + stringLX + ":" + stringRX + ":" + String(result);
    Serial1.println(msg);
    delay(100);
    //*/
  }

  //*/
}
