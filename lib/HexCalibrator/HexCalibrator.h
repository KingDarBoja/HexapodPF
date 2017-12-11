/*
  HexCalibrator.h - Library for hexapod servo calibration.
  Created by Manuel Bojato, December 11, 2017.
  Released into the public domain.
*/
#ifndef HexCalibrator_h
#define HexCalibrator_h

#include <Arduino.h>
#include <Servo.h>

class HexCalibrator
{
  public:
  HexCalibrator(int mpS11, int mpS12, int mpS13, int mpS21, int mpS22,
    int mpS23, int mpS31, int mpS32, int mpS33, int mpS41, int mpS42,
    int mpS43, int mpS51, int mpS52, int mpS53, int mpS61, int mpS62,
    int mpS63);
    void HexPawPin(int pinS11, int pinS12, int pinS13, int pinS21, int pinS22,
      int pinS23, int pinS31, int pinS32, int pinS33, int pinS41, int pinS42,
      int pinS43, int pinS51, int pinS52, int pinS53, int pinS61, int pinS62,
      int pinS63);
    void ServoCalibrator();
  private:
    void ComRead();
    void PrintAngleList();
    void PrintCommandList();
    int _mpS11, _mpS12, _mpS13, _mpS21, _mpS22, _mpS23, _mpS31, _mpS32, _mpS33,
    _mpS41, _mpS42, _mpS43, _mpS51, _mpS52, _mpS53, _mpS61, _mpS62, _mpS63;
    char input_console;
    int paw_selected = 0;
    int servo_selected = 0;
    //PAW1     //PAW2     //PAW3     //PAW4     //PAW5     //PAW6
    Servo S11; Servo S21; Servo S31; Servo S41; Servo S51; Servo S61;
    Servo S12; Servo S22; Servo S32; Servo S42; Servo S52; Servo S62;
    Servo S13; Servo S23; Servo S33; Servo S43; Servo S53; Servo S63;
};

#endif
