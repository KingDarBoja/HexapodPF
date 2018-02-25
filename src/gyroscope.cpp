/**
 *  @file    gyroscope.cpp
 *  @author  Manuel Bojato (KingDarBoja)
 *  @date    X/XX/XXXX
 *  @version 1.0
 *
 *  @brief Gyroscope output code with calibration and Kalman filter libraries.
 *
 *  @section DESCRIPTION
 *
 *  Program used to measure the gyroscope / accelerometer raw data in order to
 *  apply Kalman filtering to calculate angle, rate and bias of the IMU6050.
 *  It comes with a calibration routine for the sensor.
 *
*/

// ARDUINO MAIN LIBRARY
#include <Arduino.h>

// Libraries to control the MPU6050
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>
#include <Kalman.h>

// Comment out to restrict roll to ±90deg instead
// Please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
#define RESTRICT_PITCH

// Create the Kalman instances
Kalman kalmanX;
Kalman kalmanY;

// Conversion Ratios
#define A_R 16384.0
#define G_R 131.0

double gyroXangle, gyroYangle;  // Angle calculate using the gyro only
double kalAngleX, kalAngleY;    // Calculated angle using a Kalman filter
int16_t tempRaw;
uint32_t timer;

// MPU6050 Address can be either 0x68 or 0x69, based on the AD0 state
// If not specified, 0x68 is the default value.
MPU6050 sensor;

// Raw data from the IMU6050
int ax, ay, az;
int gx, gy, gz;

// Low pass filter variables
long f_ax,f_ay, f_az;
int p_ax, p_ay, p_az;
long f_gx,f_gy, f_gz;
int p_gx, p_gy, p_gz;
int counter=0;

// Offset values
int ax_o, ay_o, az_o;
int gx_o, gy_o, gz_o;

void gyroMeasureSetting()
{
  Wire.begin();               // Initialize I2C
  sensor.initialize();        // Initialize sensor
  if (sensor.testConnection()) {
    Serial.println("MPU6050 ready!");
  } else {
    Serial.println("Error al iniciar el sensor");
  }
  delay(100);
  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
  #ifdef RESTRICT_PITCH // Eq. 25 and 26
    double roll  = atan2(ay, az) * RAD_TO_DEG;
    double pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;
  #else // Eq. 28 and 29
    double roll  = atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
    double pitch = atan2(-ax, az) * RAD_TO_DEG;
  #endif

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;

  timer = micros();
}

void gyroMeasureLoop()
{
  // Raw data readings
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);

  // Calculate delta time
  double dt = (double)(micros() - timer) / 1000000;
  timer = micros();

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
  #ifdef RESTRICT_PITCH // Eq. 25 and 26
    double roll  = atan2(ay, az) * RAD_TO_DEG;
    double pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;
  #else // Eq. 28 and 29
    double roll  = atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
    double pitch = atan2(-ax, az) * RAD_TO_DEG;
  #endif

  double gyroXrate = gx / G_R; // Convert to deg/s
  double gyroYrate = gy / G_R; // Convert to deg/s

  #ifdef RESTRICT_PITCH
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
      kalmanX.setAngle(roll);
      kalAngleX = roll;
      gyroXangle = roll;
    } else
      kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

    if (abs(kalAngleX) > 90)
      gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
  #else
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
      kalmanY.setAngle(pitch);
      kalAngleY = pitch;
      gyroYangle = pitch;
    } else
      kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

    if (abs(kalAngleY) > 90)
      gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
  #endif

  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180)
    gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;


  // Print Data
  #if 0 // Set to 1 to print raw values
  Serial.print(accX); Serial.print("\t");
  Serial.print(accY); Serial.print("\t");
  Serial.print(accZ); Serial.print("\t");

  Serial.print(gyroX); Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print(gyroZ); Serial.print("\t");

  Serial.print("\t");
  #endif

  Serial.print(roll); Serial.print("\t");
  Serial.print(gyroXangle); Serial.print("\t");
  Serial.print(kalAngleX); Serial.print("\t");
  Serial.print("\t");
  Serial.print(pitch); Serial.print("\t");
  Serial.print(gyroYangle); Serial.print("\t");
  Serial.print(kalAngleY); Serial.print("\t");
  Serial.print("\r\n");
  delay(2);
}

/**
    MPU6050 settings function.
    Initialize the sensor and check the connection.
    Also print the initial offset values and await for user interaction to
    start the calibration routine.
*/
void gyroCalibrationSetting()
{
  Wire.begin();               // Initialize I2C
  sensor.initialize();        // Initialize sensor
  if (sensor.testConnection()) Serial.println("MPU6050 ready!");

  // Offset reading
  ax_o=sensor.getXAccelOffset();
  ay_o=sensor.getYAccelOffset();
  az_o=sensor.getZAccelOffset();
  gx_o=sensor.getXGyroOffset();
  gy_o=sensor.getYGyroOffset();
  gz_o=sensor.getZGyroOffset();

  // Print the offset reading values
  Serial.println("Offsets:");
  Serial.print(ax_o); Serial.print("\t");
  Serial.print(ay_o); Serial.print("\t");
  Serial.print(az_o); Serial.print("\t");
  Serial.print(gx_o); Serial.print("\t");
  Serial.print(gy_o); Serial.print("\t");
  Serial.print(gz_o); Serial.print("\t");
  Serial.println("nnPress any character to start calibration");
  // Waiting loop for char.
  while (true){if (Serial.available()) break;}
  Serial.println("Calibrando, no mover IMU");
}

/**
    MPU6050 calibration function.
    Read and print the offset values from all axis and applies low pass filtering
    to set the desired output value.
*/
void gyroCalibrationLoop() {
  // Get accelerometer and gyroscope raw data.
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);

  // Filtering
  f_ax = f_ax-(f_ax>>5)+ax;
  p_ax = f_ax>>5;

  f_ay = f_ay-(f_ay>>5)+ay;
  p_ay = f_ay>>5;

  f_az = f_az-(f_az>>5)+az;
  p_az = f_az>>5;

  f_gx = f_gx-(f_gx>>3)+gx;
  p_gx = f_gx>>3;

  f_gy = f_gy-(f_gy>>3)+gy;
  p_gy = f_gy>>3;

  f_gz = f_gz-(f_gz>>3)+gz;
  p_gz = f_gz>>3;

  // Every 100 readings, correct the offset
  if (counter==100){
    // Print readings like a table.
    Serial.print("promedio:"); Serial.print("t");
    Serial.print(p_ax); Serial.print("\t");
    Serial.print(p_ay); Serial.print("\t");
    Serial.print(p_az); Serial.print("\t");
    Serial.print(p_gx); Serial.print("\t");
    Serial.print(p_gy); Serial.print("\t");
    Serial.println(p_gz);

    // Calibrate the accelerometer to 1g on the z axis (adjust the offset)
    if (p_ax>0) ax_o--;
    else {ax_o++;}
    if (p_ay>0) ay_o--;
    else {ay_o++;}
    if (p_az-16384>0) az_o--;
    else {az_o++;}

    sensor.setXAccelOffset(ax_o);
    sensor.setYAccelOffset(ay_o);
    sensor.setZAccelOffset(az_o);

    // Calibrate the gyro to 0º / s on all axes (adjust the offset)
    if (p_gx>0) gx_o--;
    else {gx_o++;}
    if (p_gy>0) gy_o--;
    else {gy_o++;}
    if (p_gz>0) gz_o--;
    else {gz_o++;}

    sensor.setXGyroOffset(gx_o);
    sensor.setYGyroOffset(gy_o);
    sensor.setZGyroOffset(gz_o);

    counter=0;
  }
  counter++;
}
