using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor leftBack;
extern motor leftMid;
extern motor leftFront;
extern motor rightBack;
extern motor rightMid;
extern motor rightFront;
extern motor Cata;
extern motor Intake;
extern limit Auton1;
extern inertial Inertial;
extern rotation leftTrack;
extern rotation rightTrack;
extern motor rightMid;
extern digital_out piston;
extern digital_out extendIntake;
extern limit cataLimit;
extern led LEDGreen;
extern led LEDRed;
extern distance sideX;
extern distance sideY;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );

//! Setup if needed
// extern optical Optical10;
// extern distance Distance11;
// extern gps GPS;
// extern potV2 PotentiometerV2A; // Potentiometer 
// extern led LEDB; // __ LED
// extern triport Expander13;



