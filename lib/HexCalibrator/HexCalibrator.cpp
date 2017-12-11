/*
  HexCalibrator.cpp - Library for hexapod servo calibration.
  Created by Manuel Bojato, December 11, 2017.
  Released into the public domain.
*/

#include <Arduino.h>
#include <HexCalibrator.h>

HexCalibrator::HexCalibrator(int mpS11, int mpS12, int mpS13, int mpS21, int mpS22,
  int mpS23, int mpS31, int mpS32, int mpS33, int mpS41, int mpS42,
  int mpS43, int mpS51, int mpS52, int mpS53, int mpS61, int mpS62,
  int mpS63)
  {
    _mpS11 = mpS11; _mpS12 = mpS12; _mpS13 = mpS13;
    _mpS21 = mpS21; _mpS22 = mpS22; _mpS23 = mpS23;
    _mpS31 = mpS31; _mpS32 = mpS32; _mpS33 = mpS33,
    _mpS41 = mpS41; _mpS42 = mpS42; _mpS43 = mpS43;
    _mpS51 = mpS51; _mpS52 = mpS52; _mpS53 = mpS53;
    _mpS61 = mpS61; _mpS62 = mpS62; _mpS63 = mpS63;
  }

void HexCalibrator::HexPawPin(int pinS11, int pinS12, int pinS13, int pinS21, int pinS22,
  int pinS23, int pinS31, int pinS32, int pinS33, int pinS41, int pinS42, int pinS43,
  int pinS51, int pinS52, int pinS53, int pinS61, int pinS62, int pinS63)
  {
    S11.attach(pinS11); S12.attach(pinS12); S13.attach(pinS13);
    S21.attach(pinS21); S22.attach(pinS22); S23.attach(pinS23);
    S31.attach(pinS31); S32.attach(pinS32); S33.attach(pinS33);
    S41.attach(pinS41); S42.attach(pinS42); S43.attach(pinS43);
    S51.attach(pinS51); S52.attach(pinS52); S53.attach(pinS53);
    S61.attach(pinS61); S62.attach(pinS62); S63.attach(pinS63);
    S11.write(_mpS11); S21.write(_mpS21); S31.write(_mpS31); S41.write(_mpS41); S51.write(_mpS51); S61.write(_mpS61);
    S12.write(_mpS12); S22.write(_mpS22); S32.write(_mpS32); S42.write(_mpS42); S52.write(_mpS52); S62.write(_mpS62);
    S13.write(_mpS13); S23.write(_mpS23); S33.write(_mpS33); S43.write(_mpS43); S53.write(_mpS53); S13.write(_mpS63);
    Serial.println("SERVO MOTOR CALIBRATOR TOOL");
  }

// READING SERIAL PORT FUNCTION
void HexCalibrator::ComRead() {
  if (Serial.available() > 0) {
    input_console = Serial.read();
  }
}

// PRINT FUNCTION TO DISPLAY THE COMMANDS AVAILABLE IN THE SERIAL MONITOR
void HexCalibrator::PrintCommandList()
{
  Serial.println("   TYPE   |     ACTION  ");
  Serial.println("----------------------------");
  Serial.println("    +     |   PLUS GRADE ");
  Serial.println("    -     | SUBTRACT GRADE ");
  Serial.println("    R     |   RECALIBRATE ");
  Serial.println("    Q     |      EXIT ");
}

// PRINT FUNCTION TO DISPLAY ALL THE CURRENT ANGLES VALUES.
void HexCalibrator::PrintAngleList(){
  String row;
  Serial.println("  PAW  |  SERVO  |  MIDDLE POINT");
  Serial.println("---------------------------------");
  row ="   1   |    1    |       " + String(_mpS11);
  Serial.println(row);
  row ="   1   |    2    |       " + String(_mpS12);
  Serial.println(row);
  row ="   1   |    3    |       " + String(_mpS13);
  Serial.println(row);
  Serial.println("---------------------------------");
  row ="   2   |    1    |       " + String(_mpS21);
  Serial.println(row);
  row ="   2   |    2    |       " + String(_mpS22);
  Serial.println(row);
  row ="   2   |    3    |       " + String(_mpS23);
  Serial.println(row);
  Serial.println("---------------------------------");
  row ="   3   |    1    |       " + String(_mpS31);
  Serial.println(row);
  row ="   3   |    2    |       " + String(_mpS32);
  Serial.println(row);
  row ="   3   |    3    |       " + String(_mpS33);
  Serial.println(row);
  Serial.println("---------------------------------");
  row ="   4   |    1    |       " + String(_mpS41);
  Serial.println(row);
  row ="   4   |    2    |       " + String(_mpS42);
  Serial.println(row);
  row ="   4   |    3    |       " + String(_mpS43);
  Serial.println(row);
  Serial.println("---------------------------------");
  row ="   5   |    1    |       " + String(_mpS51);
  Serial.println(row);
  row ="   5   |    2    |       " + String(_mpS52);
  Serial.println(row);
  row ="   5   |    3    |       " + String(_mpS53);
  Serial.println(row);
  Serial.println("---------------------------------");
  row ="   6   |    1    |       " + String(_mpS61);
  Serial.println(row);
  row ="   6   |    2    |       " + String(_mpS62);
  Serial.println(row);
  row ="   6   |    3    |       " + String(_mpS63);
  Serial.println(row);
  Serial.println("---------------------------------");
  Serial.println("");
}

// FUNCTION PARSER: CHAR TO INT
int CharParser(char in) {
  int result;
  String parser;
  parser = in;
  result = parser.toInt();
  return result;
}

void HexCalibrator::ServoCalibrator()
{
  salto1:
    Serial.println("TYPE IN PAW NUMBER (1 - 6):");
  salto2:
    ComRead();
    paw_selected = CharParser(input_console);
    if (paw_selected != 0 && paw_selected < 7) {
      Serial.print("THE PAW SELECTED WAS ");
      Serial.print(paw_selected);
      Serial.println(".");
      Serial.println("");
      goto salto3;
    } else if (paw_selected != 0) {
      Serial.println("SELECTED PAW DOESN'T EXIST, TRY AGAIN.");
      goto salto1;
    }
    goto salto2;
  salto3:
    Serial.println("TYPE IN SERVO MOTOR NUMBER (1 - 3) OR 'Q' TO RETURN");
  salto4:
    ComRead();
    servo_selected = CharParser(input_console);
    delay(3000);
    if (servo_selected== 'Q') {
      Serial.println("");
      goto salto1;
    } else if (isDigit(input_console)) {
      if (servo_selected > 3) {
        Serial.println("SELECTED SERVO DOESN'T EXIST, TRY AGAIN.");
        goto salto3;
      } else {
        Serial.print("THE SERVO MOTOR SELECTED WAS ");
        Serial.print(servo_selected);
        Serial.print(" FROM PAW NUMBER ");
        Serial.print(paw_selected);
        Serial.println(".");
        Serial.println("");
      }
    } else {
      goto salto4;
    }
  salto5:
    Serial.println("CALIBRATION IN PROCCESS... DONT TOUCH");
    Serial.println("SETTING UP THE POSITION TO 90 GRADES... DONT TOUCH");
    Serial.println("");
    delay(1000);
    switch (paw_selected) {
      case 1: //CALIBRATE SERVOS FROM PAW NUMBER 1
        switch (servo_selected) {
          case 1:
            S11.write(_mpS11);
            PrintCommandList();
  salto6:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS11 = _mpS11 + 1;
              S11.write(_mpS11);
              goto salto6;
            } else if (input_console == '-') {
              _mpS11 = _mpS11 - 1;
              S11.write(_mpS11);
              goto salto6;
            } else {
              goto salto6;
            }
            break;
          case 2:
            S12.write(_mpS12);
            PrintCommandList();
  salto7:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS12 = _mpS12 + 1;
              S12.write(_mpS12);
              goto salto7;
            } else if (input_console == '-') {
              _mpS12 = _mpS12 - 1;
              S12.write(_mpS12);
              goto salto7;
            } else {
              goto salto7;
            }
            break;
          case 3:
            S13.write(_mpS13);
            PrintCommandList();
  salto8:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS13 = _mpS13 + 1;
              S13.write(_mpS13);
              goto salto8;
            } else if (input_console == '-') {
              _mpS13 = _mpS13 - 1;
              S13.write(_mpS13);
              goto salto8;
            } else {
              goto salto8;
            }
            break;
        }
        break;
      case 2: //CALIBRATE SERVOS FROM PAW NUMBER 2
        switch (servo_selected) {
          case 1:
            S21.write(_mpS21);
            PrintCommandList();
  salto9:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS21 = _mpS21 + 1;
              S21.write(_mpS21);
              goto salto9;
            } else if (input_console == '-') {
              _mpS21 = _mpS21 - 1;
              S21.write(_mpS21);
              goto salto9;
            } else {
              goto salto9;
            }
            break;
          case 2:
            S22.write(_mpS22);
            PrintCommandList();
  salto10:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS22 = _mpS22 + 1;
              S22.write(_mpS22);
              goto salto10;
            } else if (input_console == '-') {
              _mpS22 = _mpS22 - 1;
              S22.write(_mpS22);
              goto salto10;
            } else {
              goto salto10;
            }
            break;
          case 3:
            S23.write(_mpS23);
            PrintCommandList();
  salto11:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS23 = _mpS23 + 1;
              S23.write(_mpS23);
              goto salto11;
            } else if (input_console == '-') {
              _mpS23 = _mpS23 - 1;
              S23.write(_mpS23);
              goto salto11;
            } else {
              goto salto11;
            }
            break;
        }
        break;
      case 3://CALIBRATE SERVOS FROM PAW NUMBER 3
        switch (servo_selected) {
          case 1:
            S31.write(_mpS31);
            PrintCommandList();
  salto12:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS31 = _mpS31 + 1;
              S31.write(_mpS31);
              goto salto12;
            } else if (input_console == '-') {
              _mpS31 = _mpS31 - 1;
              S31.write(_mpS31);
              goto salto12;
            } else {
              goto salto12;
            }
            break;
          case 2:
            S32.write(_mpS32);
            PrintCommandList();
  salto13:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS32 = _mpS32 + 1;
              S32.write(_mpS32);
              goto salto13;
            } else if (input_console == '-') {
              _mpS32 = _mpS32 - 1;
              S32.write(_mpS32);
              goto salto13;
            } else {
              goto salto13;
            }
            break;
          case 3:
            S33.write(_mpS33);
            PrintCommandList();
  salto14:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS33 = _mpS33 + 1;
              S33.write(_mpS33);
              goto salto14;
            } else if (input_console == '-') {
              _mpS33 = _mpS33 - 1;
              S33.write(_mpS33);
              goto salto14;
            } else {
              goto salto14;
            }
            break;
        }
        break;
      case 4://CALIBRATE SERVOS FROM PAW NUMBER 4
        switch (servo_selected) {
          case 1:
            S41.write(_mpS41);
            PrintCommandList();
  salto15:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS41 = _mpS41 + 1;
              S41.write(_mpS41);
              goto salto15;
            } else if (input_console == '-') {
              _mpS41 = _mpS41 - 1;
              S41.write(_mpS41);
              goto salto15;
            } else {
              goto salto15;
            }
            break;
          case 2:
            S42.write(_mpS42);
            PrintCommandList();
  salto16:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS42 = _mpS42 + 1;
              S42.write(_mpS42);
              goto salto16;
            } else if (input_console == '-') {
              _mpS42 = _mpS42 - 1;
              S42.write(_mpS42);
              goto salto16;
            } else {
              goto salto16;
            }
            break;
          case 3:
            S43.write(_mpS43);
            PrintCommandList();
  salto17:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS43 = _mpS43 + 1;
              S43.write(_mpS43);
              goto salto17;
            } else if (input_console == '-') {
              _mpS43 = _mpS43 - 1;
              S43.write(_mpS43);
              goto salto17;
            } else {
              goto salto17;
            }
            break;
        }
        break;
      case 5://CALIBRATE SERVOS FROM PAW NUMBER 5
        switch (servo_selected) {
          case 1:
            S51.write(_mpS51);
            PrintCommandList();
  salto18:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS51 = _mpS51 + 1;
              S51.write(_mpS51);
              goto salto18;
            } else if (input_console == '-') {
              _mpS51 = _mpS51 - 1;
              S51.write(_mpS51);
              goto salto18;
            } else {
              goto salto18;
            }
            break;
          case 2:
            S52.write(_mpS52);
            PrintCommandList();
  salto19:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS52 = _mpS52 + 1;
              S52.write(_mpS52);
              goto salto19;
            } else if (input_console == '-') {
              _mpS52 = _mpS52 - 1;
              S52.write(_mpS52);
              goto salto19;
            } else {
              goto salto19;
            }
            break;
          case 3:
            S53.write(_mpS53);
            PrintCommandList();
  salto20:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS53 = _mpS53 + 1;
              S53.write(_mpS53);
              goto salto20;
            } else if (input_console == '-') {
              _mpS53 = _mpS53 - 1;
              S53.write(_mpS53);
              goto salto20;
            } else {
              goto salto20;
            }
            break;
        }
        break;
      case 6://CALIBRATE SERVOS FROM PAW NUMBER 6
        switch (servo_selected) {
          case 1:
            S61.write(_mpS61);
            PrintCommandList();
  salto21:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS61 = _mpS61 + 1;
              S61.write(_mpS61);
              goto salto21;
            } else if (input_console == '-') {
              _mpS61 = _mpS61 - 1;
              S61.write(_mpS61);
              goto salto21;
            } else {
              goto salto21;
            }
            break;
          case 2:
            S62.write(_mpS62);
            PrintCommandList();
  salto22:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS62 = _mpS62 + 1;
              S62.write(_mpS62);
              goto salto22;
            } else if (input_console == '-') {
              _mpS62 = _mpS62 - 1;
              S62.write(_mpS62);
              goto salto22;
            } else {
              goto salto22;
            }
            break;
          case 3:
            S63.write(_mpS63);
            PrintCommandList();
  salto23:
            ComRead();
            if (input_console == 'Q') {
              Serial.println("");
            } else if (input_console == 'R') {
              Serial.println("");
              goto salto5;
            } else if (input_console == '+') {
              _mpS63 = _mpS63 + 1;
              S63.write(_mpS63);
              goto salto23;
            } else if (input_console == '-') {
              _mpS63 = _mpS63 - 1;
              S63.write(_mpS63);
              goto salto23;
            } else {
              goto salto23;
            }
            break;
        }
        break;
    }
  Serial.println("TO PRINT THE LIST OF MIDDLE POINTS TYPE 'Y' OR 'N' TO GO ON WITH OTHER SERVO MOTOR CALIBRATION");
  Serial.println("");
  salto24:
    ComRead();
    if (input_console == 'Y') {
      PrintAngleList();
    } else if (input_console == 'N') {
      Serial.println("");
      goto salto1;
    } else {
      goto salto24;
    }
  }
