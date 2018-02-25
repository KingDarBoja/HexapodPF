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
 *
*/

// ARDUINO MAIN LIBRARY
#include <Arduino.h>

// Libraries to control the MPU6050
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>
#include <Kalman.h>

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
int ax_o,ay_o,az_o;
int gx_o,gy_o,gz_o;

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
