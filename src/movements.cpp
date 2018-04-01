// ARDUINO MAIN LIBRARY
#include <Arduino.h>
#include <Servo.h>

//GLOBAL VARIABLES
int cc = 0;

// Setting up the angles (obtained from HexCalibrator routine) of every PAW (mpS_#PAW_#PHALANX).
//PAW1            //PAW2            //PAW3            //PAW4            //PAW5            //PAW6
int mpS11 = 83;   int mpS21 = 92;   int mpS31 = 86;   int mpS41 = 96;   int mpS51 = 80;   int mpS61 = 98;
int mpS12 = 87;   int mpS22 = 90;   int mpS32 = 84;   int mpS42 = 86;   int mpS52 = 97;   int mpS62 = 92;
int mpS13 = 91;   int mpS23 = 91;   int mpS33 = 93;   int mpS43 = 104;   int mpS53 = 88;   int mpS63 = 83;

// Each one of the Paw-Phalanx angles is assigned to temporal variable (initial angle).
int s11i = mpS11; int s21i = mpS21; int s31i = mpS31; int s41i = mpS41; int s51i = mpS51; int s61i = mpS61;
int s12i = mpS12; int s22i = mpS22; int s32i = mpS32; int s42i = mpS42; int s52i = mpS52; int s62i = mpS62;
int s13i = mpS13; int s23i = mpS23; int s33i = mpS33; int s43i = mpS43; int s53i = mpS53; int s63i = mpS63;

//SERVO-PAW DISCRIMINATION
//PAW1    //PAW2    //PAW3    //PAW4    //PAW5    //PAW6
Servo S11; Servo S21; Servo S31; Servo S41; Servo S51; Servo S61;
Servo S12; Servo S22; Servo S32; Servo S42; Servo S52; Servo S62;
Servo S13; Servo S23; Servo S33; Servo S43; Servo S53; Servo S63;

//SERVO-PIN ATTACH
void servoAttachment()
{
  //PAW1            //PAW2            //PAW3            //PAW4            //PAW5            //PAW6
  S11.attach(43);   S21.attach(14);   S31.attach(9);    S41.attach(22);   S51.attach(46);   S61.attach(51);
  S12.attach(44);   S22.attach(15);   S32.attach(10);   S42.attach(23);   S52.attach(47);   S62.attach(52);
  S13.attach(45);   S23.attach(16);   S33.attach(11);   S43.attach(24);   S53.attach(48);   S63.attach(53);
  S11.write(s11i);  S21.write(s21i);  S31.write(s31i);  S41.write(s41i);  S51.write(s51i);  S61.write(s61i);
  S12.write(s12i);  S22.write(s22i);  S32.write(s32i);  S42.write(s42i);  S52.write(s52i);  S62.write(s62i);
  S13.write(s13i);  S23.write(s23i);  S33.write(s33i);  S43.write(s43i);  S53.write(s53i);  S63.write(s63i);
}

//FUNCTION MOVES PER AXIS
void MoveCommand(int s11o, int s21o, int s31o, int s41o, int s51o, int s61o,
                 int s12o, int s22o, int s32o, int s42o, int s52o, int s62o,
                 int s13o, int s23o, int s33o, int s43o, int s53o, int s63o,
                 int DelayTime) {
  //CALCULATE SWEEP ANGLES
  float SweepAngles[18];
  SweepAngles[0] = s11o - s11i;  SweepAngles[1] = s12o - s12i;  SweepAngles[2] = s13o - s13i;
  SweepAngles[3] = s21o - s21i;  SweepAngles[4] = s22o - s22i;  SweepAngles[5] = s23o - s23i;
  SweepAngles[6] = s31o - s31i;  SweepAngles[7] = s32o - s32i;  SweepAngles[8] = s33o - s33i;
  SweepAngles[9] = s41o - s41i;  SweepAngles[10] = s42o - s42i; SweepAngles[11] = s43o - s43i;
  SweepAngles[12] = s51o - s51i; SweepAngles[13] = s52o - s52i; SweepAngles[14] = s53o - s53i;
  SweepAngles[15] = s61o - s61i; SweepAngles[16] = s62o - s62i; SweepAngles[17] = s63o - s63i;
  //CALCULATE FOR LOOP LIMIT
  int Limit = 180;
  for (int j = 0 ; j <= 17 ; j++) {
    if (SweepAngles[j] != 0) {
      Limit = min(abs(SweepAngles[j]), Limit);
    }
  }
  if (Limit == 1) {
    Limit = 30;
  }
  Limit = 30;
  //EXECUTE SERVO MOVES
  for (int k = 0 ; k <= Limit ; k++) {
    if (SweepAngles[0] == 0) {
      S11.write(s11o);
    } else {
      S11.write(s11i + (SweepAngles[0] / Limit)*k);
    }
    if (SweepAngles[1] == 0) {
      S12.write(s12o);
    } else {
      S12.write(s12i + (SweepAngles[1] / Limit)*k);
    }
    if (SweepAngles[2] == 0) {
      S13.write(s13o);
    } else {
      S13.write(s13i + (SweepAngles[2] / Limit)*k);
    }
    if (SweepAngles[3] == 0) {
      S21.write(s21o);
    } else {
      S21.write(s21i + (SweepAngles[3] / Limit)*k);
    }
    if (SweepAngles[4] == 0) {
      S22.write(s22o);
    } else {
      S22.write(s22i + (SweepAngles[4] / Limit)*k);
    }
    if (SweepAngles[5] == 0) {
      S23.write(s23o);
    } else {
      S23.write(s23i + (SweepAngles[5] / Limit)*k);
    }
    if (SweepAngles[6] == 0) {
      S31.write(s31o);
    } else {
      S31.write(s31i + (SweepAngles[6] / Limit)*k);
    }
    if (SweepAngles[7] == 0) {
      S32.write(s32o);
    } else {
      S32.write(s32i + (SweepAngles[7] / Limit)*k);
    }
    if (SweepAngles[8] == 0) {
      S33.write(s33o);
    } else {
      S33.write(s33i + (SweepAngles[8] / Limit)*k);
    }
    if (SweepAngles[9] == 0) {
      S41.write(s41o);
    } else {
      S41.write(s41i + (SweepAngles[9] / Limit)*k);
    }
    if (SweepAngles[10] == 0) {
      S42.write(s42o);
    } else {
      S42.write(s42i + (SweepAngles[10] / Limit)*k);
    }
    if (SweepAngles[11] == 0) {
      S43.write(s43o);
    } else {
      S43.write(s43i + (SweepAngles[11] / Limit)*k);
    }
    if (SweepAngles[12] == 0) {
      S51.write(s51o);
    } else {
      S51.write(s51i + (SweepAngles[12] / Limit)*k);
    }
    if (SweepAngles[13] == 0) {
      S52.write(s52o);
    } else {
      S52.write(s52i + (SweepAngles[13] / Limit)*k);
    }
    if (SweepAngles[14] == 0) {
      S53.write(s53o);
    } else {
      S53.write(s53i + (SweepAngles[14] / Limit)*k);
    }
    if (SweepAngles[15] == 0) {
      S61.write(s61o);
    } else {
      S61.write(s61i + (SweepAngles[15] / Limit)*k);
    }
    if (SweepAngles[16] == 0) {
      S62.write(s62o);
    } else {
      S62.write(s62i + (SweepAngles[16] / Limit)*k);
    }
    if (SweepAngles[17] == 0) {
      S63.write(s63o);
    } else {
      S63.write(s63i + (SweepAngles[17] / Limit)*k);
    }
    delay(DelayTime);
  }
  //ASSIGN FINAL ANGLES AS BEGIN ANGLES
  s11i = s11o;  s21i = s21o;  s31i = s31o;  s41i = s41o;  s51i = s51o;  s61i = s61o;
  s12i = s12o;  s22i = s22o;  s32i = s32o;  s42i = s42o;  s52i = s52o;  s62i = s62o;
  s13i = s13o;  s23i = s23o;  s33i = s33o;  s43i = s43o;  s53i = s53o;  s63i = s63o;
}

void WakeUp() {
  MoveCommand( 97, 89, 101, 94, 92, 94,
               99, 111, 99, 85, 90, 102,
               94, 90, 98, 89, 90, 91,
               10);

  MoveCommand( 97, 89, 101, 94, 92, 94,
               129, 141, 129, 115, 120, 132,
               124, 124, 128, 119, 120, 121,
               10);

  MoveCommand( 97, 89, 101, 94, 92, 94,
               69, 81, 69, 55, 60, 72,
               64, 64, 68, 59, 60, 61,
               10);
}

void Parche() {
  MoveCommand( mpS11, mpS21, mpS31, mpS41, mpS51, mpS61,
               mpS12, mpS22, mpS32, mpS42, mpS52, mpS62,
               mpS13, mpS23, mpS33, mpS43, mpS53, mpS63,
               10);
}

/*
  ----- NEW HEXAPOD MOVEMENTS -----
  Here lies several functions in order to generate more flexible and soft movements
  for the hexapod. I use the difference between start and end angles in order to
  make it more understandable for the users.
*/

// Set the elevation angle for some movements (ForwardTripodGait)
int elev_angle = 60;
// Set the turning angle for some movements (TurnRightSoft, TurnLeftSoft)
int turn_angle_s = 45;

void TurnRightSoft() {
  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i+30, s22i, s32i+30, s42i, s52i+30, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i-turn_angle_s, s21i, s31i-turn_angle_s, s41i, s51i-turn_angle_s, s61i,
              s12i, s22i, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i-30, s22i, s32i-30, s42i, s52i-30, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i+30, s32i, s42i+30, s52i, s62i+30,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i-turn_angle_s, s31i, s41i-turn_angle_s, s51i, s61i-turn_angle_s,
              s12i, s22i, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i-30, s32i, s42i-30, s52i, s62i-30,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i+turn_angle_s, s21i+turn_angle_s, s31i+turn_angle_s, s41i+turn_angle_s, s51i+turn_angle_s, s61i+turn_angle_s,
              s12i, s22i, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);
}

void TurnLeftSoft() {
  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i+30, s22i, s32i+30, s42i, s52i+30, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i+45, s21i, s31i+45, s41i, s51i+45, s61i,
              s12i, s22i, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i-30, s22i, s32i-30, s42i, s52i-30, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i+30, s32i, s42i+30, s52i, s62i+30,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i+45, s31i, s41i+45, s51i, s61i+45,
              s12i, s22i, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i-30, s32i, s42i-30, s52i, s62i-30,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i-45, s21i-45, s31i-45, s41i-45, s51i-45, s61i-45,
              s12i, s22i, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);
}

void ForwardTripodGait() {
  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i+elev_angle, s32i, s42i+elev_angle, s52i, s62i+elev_angle,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i+45, s21i-36, s31i+12, s41i+45, s51i-36, s61i+12,
              s12i-8, s22i-2-elev_angle, s32i-16, s42i-8-elev_angle, s52i-2, s62i-16-elev_angle,
              s13i+10, s23i-10, s33i-45, s43i+10, s53i-10, s63i-45,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i+60, s22i, s32i+60, s42i, s52i+60, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i-45, s21i+36, s31i-12, s41i-45, s51i+36, s61i-12,
              s12i+8, s22i+2, s32i+16, s42i+8, s52i+2, s62i+16,
              s13i-10, s23i+10, s33i+45, s43i-10, s53i+10, s63i+45,
              10);

  MoveCommand(s11i-12, s21i+36, s31i-45, s41i-12, s51i+36, s61i-45,
              s12i-16-elev_angle, s22i-2, s32i-8-elev_angle, s42i-16, s52i-2-elev_angle, s62i-8,
              s13i-45, s23i-10, s33i+10, s43i-45, s53i-10, s63i+10,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i+elev_angle, s32i, s42i+elev_angle, s52i, s62i+elev_angle,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i+12, s21i-36, s31i+45, s41i+12, s51i-36, s61i+45,
              s12i+16, s22i+2, s32i+8, s42i+16, s52i+2, s62i+8,
              s13i+45, s23i+10, s33i-10, s43i+45, s53i+10, s63i-10,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i-elev_angle, s32i, s42i-elev_angle, s52i, s62i-elev_angle,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);
}

void PitchWalking() {
  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i+elev_angle, s22i, s32i, s42i+elev_angle, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i+58, s21i, s31i, s41i+58, s51i, s61i,
              s12i-elev_angle, s22i, s32i, s42i-elev_angle, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i, s32i+elev_angle, s42i, s52i, s62i+elev_angle,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i-58, s41i, s51i, s61i-58,
              s12i, s22i, s32i-elev_angle, s42i, s52i, s62i-elev_angle,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i+74, s22i+32, s32i, s42i, s52i+32, s62i+74,
              s13i+148, s23i+118, s33i, s43i, s53i+118, s63i+148,
              10);
}

void ForwardWaveGait() {
  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i+elev_angle, s22i, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i-10, s21i, s31i, s41i, s51i, s61i,
              s12i-8-elev_angle, s22i, s32i, s42i, s52i, s62i,
              s13i-27, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i+elev_angle, s32i, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i-28, s31i, s41i, s51i, s61i,
              s12i, s22i-elev_angle, s32i, s42i, s52i, s62i,
              s13i, s23i-6, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i, s32i+elev_angle, s42i, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i-25, s41i, s51i, s61i,
              s12i, s22i, s32i-5-elev_angle, s42i, s52i, s62i,
              s13i, s23i, s33i+10, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i, s32i, s42i+elev_angle, s52i, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i+25, s51i, s61i,
              s12i, s22i, s32i, s42i-5-elev_angle, s52i, s62i,
              s13i, s23i, s33i, s43i+10, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i, s32i, s42i, s52i+elev_angle, s62i,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i+28, s61i,
              s12i, s22i, s32i, s42i, s52i-elev_angle, s62i,
              s13i, s23i, s33i, s43i, s53i-6, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
              s12i, s22i, s32i, s42i, s52i, s62i+elev_angle,
              s13i, s23i, s33i, s43i, s53i, s63i,
              10);

  MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i+10,
              s12i, s22i, s32i, s42i, s52i, s62i-8-elev_angle,
              s13i, s23i, s33i, s43i, s53i, s63i-27,
              10);

  MoveCommand(s11i+10, s21i+28, s31i+25, s41i-25, s51i-28, s61i-10,
              s12i+8, s22i, s32i+5, s42i+5, s52i, s62i+8,
              s13i+27, s23i+6, s33i-10, s43i-10, s53i+6, s63i+27,
              10);

  // MoveCommand(s11i, s21i, s31i, s41i, s51i, s61i,
  //             s12i, s22i, s32i, s42i, s52i, s62i,
  //             s13i, s23i, s33i, s43i, s53i, s63i,
  //             10);
}
