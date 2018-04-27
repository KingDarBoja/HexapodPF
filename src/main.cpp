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
extern void PitchWalking();
extern void ForwardWaveGait();

// Calculate based on max input size expected for one command
#define INPUT_SIZE 8

// Declare bool variable to check if the hexapod has performed 'WakeUp' Action.
bool awake = false;
double result;

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
  if (mvalue > 100) {
    mvalue = 100;
    return mvalue;
  } else {
    return mvalue;
  }
}

// Setup code to run once.
void setup() {
  // Start the communication at the serial ports.
  Serial1.begin(9600);
  Serial.begin(115200);

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

  //*
  // ======================= GYROSCOPE MEASUREMENT =======================
  // Measure raw data from sensor and print the processed data.
  result = gyroMeasureLoop();
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
  Serial.println(msg);
  //*/

  delay(1000);
  //*
  // ========================= HEXAPOD ACTIONS =========================
  // The remote computer will receive the message string and make computational
  // calculus to output a single char. It will be received via serial port,
  // and the board will execute a movement based on that char.

  // Get next command from Serial (add 1 for final 0)
  char input[INPUT_SIZE + 1];
  byte size = Serial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  input[size] = 0;

  // Read each command pair
  char* separator = strchr(input, '&');
  if (separator != 0)
  {
    // Actually split the string in 2: replace ':' with 0
    *separator = 0;
    ++separator;
    double position = atof(separator);
    Serial.println(position);
    // Do something with servoId and position
    if (position <= 15.00 && position >= -15.00) {
      ForwardTripodGait();
    } else if (position < -15.00 && position >= -60.00) {
      TurnLeftSoft(abs(position));
    } else if (position > 15.00 && position <= 60.00) {
      TurnRightSoft(abs(position));
    } else {
      Parche();
    }
  }

  //*/
}
