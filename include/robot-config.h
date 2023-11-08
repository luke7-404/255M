using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern controller Controller2;
extern motor leftBack;
extern motor leftMid;
extern motor leftFront;
extern motor rightBack;
extern motor rightMid;
extern motor rightFront;
extern motor Cata;
extern bumper Auton1;
extern inertial Inertial;
extern rotation leftTrack;
extern rotation rightTrack;
extern motor rightMid;
extern digital_out piston;

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
// extern digital_out DigitalOutC; // Pneumatics
// extern controller Controller1;
// extern motor Motor7;
// extern motor Motor8;
// extern triport Expander13;
// extern limit Auton1; // Limit Switch

//* Motor Group
// extern motor_group MotorGroup15;



