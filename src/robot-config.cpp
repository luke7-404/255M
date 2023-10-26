#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
//controller Controller2 = controller(partner);
motor leftBack = motor(PORT13, ratio6_1, true);
motor leftMid = motor(PORT12, ratio6_1, false);
motor leftFront = motor(PORT11, ratio6_1, true);
motor rightBack = motor(PORT18, ratio6_1, true);
motor rightMid = motor(PORT19, ratio6_1, false);
motor rightFront = motor(PORT20, ratio6_1, true);
//motor Motor7 = motor(PORT7, ratio18_1, false);
//motor Motor8 = motor(PORT8, ratio18_1, false);
bumper Auton1 = bumper(Brain.ThreeWirePort.A);
inertial Inertial = inertial(PORT16);
rotation leftTrack = rotation(PORT9, false);
rotation rightTrack = rotation(PORT8, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}

//! Setup if needed
// optical Optical10 = optical(PORT10);
// distance Distance11 = distance(PORT11);
// gps GPS = gps(PORT12, 0.00, 0.00, mm, 180); // (port, x offset, y offset, measurement, angle offset)
// potV2 PotentiometerV2A = potV2(Brain.ThreeWirePort.A); // Potentiometer 
// led LEDB = led(Brain.ThreeWirePort.B); // __ LED
// digital_out DigitalOutC = digital_out(Brain.ThreeWirePort.C); // Pneumatics
// controller Controller1 = controller(partner);
// motor Motor7 = motor(PORT7, ratio18_1, false);
// motor Motor8 = motor(PORT8, ratio18_1, false);
// triport Expander13 = triport(PORT13);
// limit Auton1 = limit(Brain.ThreeWirePort.A); // Limit Switch

//* Motor Group
// - Motor setup: motor name1 = motor(PORT, ratio, t/f)
// - Motor setup: motor name2 = motor(PORT, ratio, t/f)
// motor_group MotorGroup15 = motor_group(name1, name2);