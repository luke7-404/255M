#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftBack = motor(PORT18, ratio6_1, false);
motor leftMid = motor(PORT19, ratio6_1, true);
motor leftFront = motor(PORT20, ratio6_1, true);
motor rightBack = motor(PORT12, ratio6_1, true);
motor rightMid = motor(PORT13, ratio6_1, false);
motor rightFront = motor(PORT11, ratio6_1, false);
motor Cata = motor(PORT8, ratio36_1, true);
motor Intake = motor(PORT2, ratio6_1, true);
inertial Inertial = inertial(PORT5);
rotation leftTrack = rotation(PORT10, true);
rotation rightTrack = rotation(PORT1, false);
digital_out piston = digital_out(Brain.ThreeWirePort.A);
limit cataLimit = limit(Brain.ThreeWirePort.B);
led LEDGreen = led(Brain.ThreeWirePort.C);
led LEDRed = led(Brain.ThreeWirePort.D);
limit Auton1 = limit(Brain.ThreeWirePort.E);
digital_out extendIntake = digital_out(Brain.ThreeWirePort.G);
distance sideX = distance(PORT3);
distance sideY = distance(PORT4);
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
// triport Expander13 = triport(PORT13);