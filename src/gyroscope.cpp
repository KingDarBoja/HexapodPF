// ARDUINO MAIN LIBRARY
#include <Arduino.h>
#include <Wire.h>


// I2C Direction of IMU
#define MPU 0x68

// Conversion Ratios
#define A_R 16384.0
#define G_R 131.0

// Rad to degree Conversion
#define RAD_A_DEG = 57.295779;

// MPU-6050 values are 16 bit integers
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;

// Angles
float Acc[2], Gy[2], Angle[2], Acel[2];

// Settings for the MPU6050
void gyroSetting()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

//Leer los valores del Acelerometro de la IMU
void gyroMeasure()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
  AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();

  //A partir de los valores del acelerometro, se calculan los angulos Y, X
  //respectivamente, con la formula de la tangente.
  Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;

  //Leer los valores del Giroscopio
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true); //A diferencia del Acelerometro, solo se piden 4 registros
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();

  //Calculo del Acelerometro
  Acel[0] = AcX/A_R;
  Acel[1] = AcY/A_R;
  Acel[2] = AcZ/A_R;

  //Calculo del angulo del Giroscopio
  Gy[0] = GyX/G_R;
  Gy[1] = GyY/G_R;
  Gy[2] = GyZ/G_R;

  //Aplicar el Filtro Complementario
  Angle[0] = 0.5 *(Angle[0]+Gy[0]*0.010) + 0.5*Acc[0];
  Angle[1] = 0.5 *(Angle[1]+Gy[1]*0.010) + 0.5*Acc[1];

  // Print the values of the Gyroscope/Acelerometer
  Serial.print("AcX = "); Serial.print(Acel[0]);
  Serial.print(" | AcY = "); Serial.print(Acel[1]);
  Serial.print(" | AcZ = "); Serial.print(Acel[2]);
  Serial.print(" | GyX = "); Serial.print(Gy[0]);
  Serial.print(" | GyY = "); Serial.print(Gy[1]);
  Serial.print(" | GyZ = "); Serial.println(Gy[2]);

  //Mostrar los valores por consola
  Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.print("\n");
  Serial.print("Angle Y: "); Serial.print(Angle[1]); Serial.println("\n------------\n");

  delay(10); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
}
