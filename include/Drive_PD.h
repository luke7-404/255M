#ifndef DRIVE_PD_H
#define DRIVE_PD_H

extern double kP;
extern double kD;
extern double TkP;
extern double TkD;

extern int error;
extern int prevError;
extern int derivative; 
extern double targetDist;

extern int tError; 
extern int tPrevError; 
extern int tDerivative;
extern double targetTurn;

extern bool resetSens;
extern bool pdON;

int drivePD();  // Function declaration

#endif
