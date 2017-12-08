// ARDUINO MAIN LIBRARY
#include <Arduino.h>
#include <Servo.h>

//GLOBAL VARIABLES
int cc = 0;

// Setting up the angles (obtained from 01_Servo_Calibrator_Hexapod Code) of every PAW (mpS_#PAW_#PHALANX).
//PAW1            //PAW2            //PAW3            //PAW4            //PAW5            //PAW6
int mpS11 = 90;   int mpS21 = 90;   int mpS31 = 88;   int mpS41 = 79;   int mpS51 = 93;   int mpS61 = 99;
int mpS12 = 85;   int mpS22 = 94;   int mpS32 = 86;   int mpS42 = 94;   int mpS52 = 96;   int mpS62 = 92;
int mpS13 = 90;   int mpS23 = 90;   int mpS33 = 91;   int mpS43 = 106;   int mpS53 = 86;   int mpS63 = 87;

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
  //11097N12099N13094N21089N22111N23090N31101N32099N33098N
  //41094N42085N43089N51092N52090N53090N61094N62102N63091N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               99, 111, 99, 85, 90, 102,
               94, 90, 98, 89, 90, 91,
               10);

  //11097N12129N13124N21089N22141N23124N31101N32129N33128N
  //41094N42115N43119N51092N52120N53120N61094N62132N63121N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               129, 141, 129, 115, 120, 132,
               124, 124, 128, 119, 120, 121,
               10);

  //11097N12069N13064N21089N22081N23064N31101N32069N33068N
  //41094N42055N43059N51092N52060N53060N61094N62072N63061N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               69, 81, 69, 55, 60, 72,
               64, 64, 68, 59, 60, 61,
               10);
}


void Adelante() {
  //11097N12069N13064N21089N22141N23124N31101N32069N33068N
  //41094N42115N43119N51092N52060N53060N61094N62132N63121N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               69, 141, 69, 115, 60, 132,
               64, 124, 68, 119, 60, 121,
               10);

  //11097N12079N13094N21089N22141N23124N31101N32079N33058N
  //41094N42115N43119N51072N52070N53060N61094N62132N63121N
  MoveCommand( 97, 89, 101, 94, 72, 94,
               79, 141, 79, 115, 70, 132,
               94, 124, 58, 119, 60, 121,
               10);

  //11097N12129N13124N21089N22081N23064N31101N32129N33128N
  //41094N42055N43059N51092N52120N53120N61094N62072N63061N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               129, 81, 129, 55, 120, 72,
               124, 64, 128, 59, 120, 61,
               10);

  //11097N12129N13124N21109N22081N23064N31101N32129N33128N
  //41094N42065N43089N51092N52120N53120N61094N62082N63051N
  MoveCommand( 97, 109, 101, 94, 92, 94,
               129, 81, 129, 65, 120, 82,
               124, 64, 128, 89, 120, 51,
               10);
}

void Parche() {
  //11097N12069N13064N21089N22081N23064N31101N32069N33068N
  //41094N42055N43059N51092N52060N53060N61094N62072N63061N
  MoveCommand( 92, 90, 88, 79, 91, 99,
               86, 92, 86, 94, 96, 92,
               89, 90, 91, 106, 86, 83,
               10);
}

void Atras() {
  //11097N12069N13064N21089N22141N23124N31101N32069N33068N
  //41094N42115N43119N51092N52060N53060N61094N62132N63121N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               69, 141, 69, 115, 60, 132,
               64, 124, 68, 119, 60, 121,
               10);

  //11097N12069N13044N21089N22141N23124N31101N32069N33083N
  //41094N42115N43119N51112N52060N53060N61094N62132N63121N
  MoveCommand( 97, 89, 101, 94, 112, 94,
               69, 141, 69, 115, 60, 132,
               44, 124, 83, 119, 60, 121,
               10);

  //11097N12129N13124N21089N22081N23064N31101N32129N33128N
  //41094N42055N43059N51092N52120N53120N61094N62072N63061N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               129, 81, 129, 55, 120, 72,
               124, 64, 128, 59, 120, 61,
               10);

  //11097N12129N13124N21069N22081N23064N31101N32129N33128N
  //41094N42065N43049N51092N52120N53120N61094N62072N63081N
  MoveCommand( 97, 69, 101, 94, 92, 94,
               129, 81, 129, 65, 120, 72,
               124, 64, 128, 49, 120, 81,
               10);
}

void Izquierda() {
  //11097N12069N13064N21089N22081N23064N31101N32069N33068N
  //41094N42055N43059N51092N52060N53060N61094N62072N63061N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               69, 81, 69, 55, 60, 72,
               64, 64, 68, 59, 60, 61,
               10);

  //11127N12129N13124N21089N22081N23064N31131N32129N33128N
  //41094N42055N43059N51122N52120N53120N61094N62072N63061N
  MoveCommand( 127, 89, 131, 94, 122, 94,
               129, 81, 129, 55, 120, 72,
               124, 64, 128, 59, 120, 61,
               10);

  //11127N12069N13064N21119N22141N23124N31131N32069N33068N
  //41124N42115N43119N51122N52060N53060N61124N62132N63121N
  MoveCommand( 127, 119, 131, 124, 122, 124,
               69, 141, 69, 115, 60, 132,
               64, 124, 68, 119, 60, 121,
               10);

  //11097N12069N13064N21119N22081N23064N31101N32069N33068N
  //41124N42055N43059N51092N52060N53060N61124N62072N63061N
  MoveCommand( 97, 119, 101, 124, 92, 124,
               69, 81, 69, 55, 60, 72,
               64, 64, 68, 59, 60, 61,
               10);
}

void Derecha() {

  //11097N12069N13064N21089N22081N23064N31101N32069N33068N
  //41094N42055N43059N51092N52060N53060N61094N62072N63061N
  MoveCommand( 97, 89, 101, 94, 92, 94,
               69, 81, 69, 55, 60, 72,
               64, 64, 68, 59, 60, 61,
               10);

  //11067N12129N13124N21089N22081N23064N31071N32129N33128N
  //41094N42055N43059N51062N52120N53120N61094N62072N63061N
  MoveCommand( 67, 89, 71, 94, 62, 94,
               129, 81, 129, 55, 120, 72,
               124, 64, 128, 59, 120, 61,
               10);

  //11067N12069N13064N21059N22141N23124N31071N32069N33068N
  //41064N42115N43119N51062N52060N53060N61064N62132N63121N
  MoveCommand( 67, 59, 71, 64, 62, 64,
               69, 141, 69, 115, 60, 132,
               64, 124, 68, 119, 60, 121,
               10);

  //11097N12069N13064N21059N22081N23064N31101N32069N33068N
  //41064N42055N43059N51092N52060N53060N61064N62072N63061N
  MoveCommand( 97, 59, 101, 64, 92, 64,
               69, 81, 69, 55, 60, 72,
               64, 64, 68, 59, 60, 61,
               10);
}
