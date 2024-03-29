#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftBack = motor(PORT12, ratio6_1, true);
motor leftMid = motor(PORT3, ratio6_1, true);  
motor leftFront = motor(PORT18, ratio6_1, true);
motor rightBack = motor(PORT20, ratio6_1, false); 
motor rightMid = motor(PORT8, ratio6_1, false);
motor rightFront = motor(PORT13, ratio6_1, false); 
motor Cata = motor(PORT2, ratio36_1, false);
motor Intake = motor(PORT11, ratio6_1, true);
inertial Inertial = inertial(PORT19);
inertial topInertial = inertial(PORT14);
rotation leftTrack = rotation(PORT7, true);
rotation rightTrack = rotation(PORT17, true);
digital_out pistonHorizontal = digital_out(Brain.ThreeWirePort.A);
limit Auton1 = limit(Brain.ThreeWirePort.B);
limit cataLimit = limit(Brain.ThreeWirePort.C); 
digital_out hangMiddle = digital_out(Brain.ThreeWirePort.F);
digital_out hangSide = digital_out(Brain.ThreeWirePort.G);
digital_out pistonVertical = digital_out(Brain.ThreeWirePort.H);
distance rightSide = distance(PORT9);
distance rear = distance(PORT4);
distance cataDist = distance(PORT10);
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