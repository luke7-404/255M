/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Controller2          controller                    
// leftBack             motor         1               
// leftMid              motor         2               
// leftFront            motor         3               
// rightBack            motor         4               
// rightFront           motor         6               
// Cata                 motor         8               
// Motor8               motor         9               
// Auton1               bumper        C               
// Inertial             inertial      10              
// leftTrack            rotation      11              
// rightTrack           rotation      12              
// rightMid             motor         5               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
int launched = 0;
void launchCount(){
  launched++;
  Controller1.Screen.clearLine(3);
  Controller1.Screen.print("Count %d", launched);
}

int runAuton = 0;

void AutoBump(){
    
  if (runAuton > 3){
    runAuton = 0;
  } else runAuton++;
    
  switch (runAuton){
    case 1:
      Brain.Screen.clearLine(2);
      Brain.Screen.printAt(2, 40, "Auton 1 has been selected");
      break;

    case 2:
      Brain.Screen.clearLine(2);
      Brain.Screen.printAt(2, 40, "Auton 2 has been selected");
      break;  
    
    default:
      Brain.Screen.clearLine(2);
      Brain.Screen.printAt(2, 40, "!!! No Auton selected !!!");
      break;
      
  }
}

void calibrateInertial(){
  Inertial.calibrate();

  while (Inertial.isCalibrating()){
    Controller1.Screen.clearLine(3);
    Controller1.Screen.print("Calibrating");
  } 

  Controller1.Screen.clearLine(3);
  Controller1.Screen.print("Done");
  Brain.Screen.printAt(0, 60, "Done");
  Controller1.rumble("..");
  wait(10, msec);
}


// PD controller

// Lateral Variables

// Gain variables - Deals with controller sensitivity
double kP = 0.0; // error gain
double kD = 0.0; // derivative gain

int error = 0; // the difference from where the goal is to where you are 
int derivative = 0; // the difference from current error and prevError
int prevError = 0; // the last error
int targetDist = 0; // Goal distance (Lateral Movement)


// Rotational Variables

// Gain variables - Deals with controller sensitivity (for turning)
double Turn_kP = 1; // turn error gain
double Turn_kD = 0.0; // turn derivative gain

double TurnError = 0; // the difference from where the goal is to where you are
int TurnDerivative = 0; // the difference from current turn error and TurnPrevError
int TurnPrevError = 0; // the last turn error
double targetTurn = 0; // Goal Distance (Rotational Movement)


// Boolean Variables 
bool controlON = true; // Toggles the while loop for the controller
bool resetSens = false; // Toggles if the sensors are reset to 0

int PD_Control(){ // Declaration
  while(controlON){ // while loop for the controller (Needed to update values)

    // When true the sensors will be reset to 0
    if(resetSens){
      resetSens = false;
      leftTrack.resetPosition();
      rightTrack.resetPosition();
    }

    // Makes the rotation encoders an integer variable 
    int leftPos = leftTrack.position(degrees);
    int rightPos = rightTrack.position(degrees);

    // the average between both sides 
    int avg = (leftPos + rightPos)/2;

    error = targetDist - avg; // the difference of target and current location

    derivative = error - prevError; // derivative

    // Lateral(Lat) Motor(Mtr) Power(Pwr) equation
    double LatMtrPwr = (error * kP)+(derivative * kD); 

    ////////////////////////////////////////////////////////////////////

    // Makes the Inertial sensor a variable with decimal places
    double InertPos = Inertial.rotation(degrees);

    // Adds a limit to the sensor so it doesn't go past 360
    // if less than -180 it wraps to positive 180 and vice versa
    if(InertPos < -180){
      InertPos = 180;
    } else if (InertPos > 180){
      InertPos = -180;
    }

    // Error: difference from target and current location
    TurnError = targetTurn - InertPos;  

    // Print Values to debug when needed
    Brain.Screen.printAt(0, 20, "Turn Error %f", TurnError);
    Brain.Screen.printAt(0, 40, "Inertial Position %f", InertPos);
    
    TurnDerivative = TurnError - TurnPrevError; // derivative
    
    // Print Value to debug when needed
    Brain.Screen.printAt(0, 60, "Turn Prev Error %f", TurnPrevError);

    
    // Rotational(Rot) Motor(Mtr) Power(Pwr) equation
    double RotMtrPwr = (TurnError * Turn_kP) + (TurnDerivative * Turn_kD);
    
    // Application of motor powers 
    leftFront.spin(fwd, LatMtrPwr + RotMtrPwr, pct);
    leftMid.spin(fwd, LatMtrPwr + RotMtrPwr, pct);
    leftBack.spin(fwd, LatMtrPwr + RotMtrPwr, pct);
    rightBack.spin(fwd, LatMtrPwr - RotMtrPwr, pct);
    rightMid.spin(fwd, LatMtrPwr - RotMtrPwr, pct);
    rightFront.spin(fwd, LatMtrPwr - RotMtrPwr, pct);

    // re-assign the error as prevError and TurnPrevError for next cycle
    prevError = error;
    TurnPrevError = TurnError;

    // Refresh time 20 milliseconds
    task::sleep(20);
  }
  return 1;
}




void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  calibrateInertial();
  
  rightTrack.setPosition(0, degrees); 
  leftTrack.setPosition(0, degrees);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  Brain.Screen.clearScreen();
  Controller1.Screen.clearLine(3);
  switch (runAuton){
    case 1:
      // code
      break;

    case 2:
      //Code
      break;

    default:
      Brain.Screen.print("!!! No Auton Selected !!!");
      break;
      
  }
}
/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/




void usercontrol(void) {
  // User control code here, inside the loop
  controlON = false;
  bool toggleEnabledWings = false; // two-choice toggle, so we use bool
  bool buttonPressedWings = false; // logic variable

  bool toggleEnabledCata = false; // two-choice toggle, so we use bool
  bool buttonPressedCata = false; // logic variable
  while (1) {

    leftBack.setVelocity(100, pct);
    leftFront.setVelocity(100, pct);
    leftMid.setVelocity(100, pct);
    rightBack.setVelocity(100, pct);
    rightFront.setVelocity(100, pct);
    rightMid.setVelocity(100, pct);

    // These lines set the controllers analog sticks to the left motors nad right motors using the tank drive layout
    //This is Left Side AXES 3
    leftFront.spin(fwd, Controller1.Axis3.position(pct), pct);
    leftMid.spin(fwd, Controller1.Axis3.position(pct), pct);
    leftBack.spin(fwd, Controller1.Axis3.position(pct), pct);
    //This is Right Side AXES 2
    rightBack.spin(fwd, Controller1.Axis2.position(pct), pct);
    rightMid.spin(fwd, Controller1.Axis2.position(pct), pct);
    rightFront.spin(fwd, Controller1.Axis2.position(pct), pct);


    // Button Assignment //

    bool buttonL2 = Controller1.ButtonL2.pressing();

    // Toggle Logic
    if (buttonL2 && !buttonPressedCata){
      buttonPressedCata = true; 
      toggleEnabledCata = !toggleEnabledCata;
    }
    else if (!buttonL2) buttonPressedCata = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledCata){
      // Fire cata
    } else{
      // reset cata 
    }

    // Catapult Motor Logic
    // when L2 is being pressed spin forward
    if(Controller1.ButtonL2.pressing()){
      Cata.spin(fwd,100,pct);

      // when R2 is being pressed spin reverse
    }else if(Controller1.ButtonR2.pressing()){
      Cata.spin(reverse,100,pct);

      // else stop
    }else{
      Cata.stop(hold);
    }


    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonL1 = Controller1.ButtonL1.pressing();

    // Toggle Logic
    if (buttonL1 && !buttonPressedWings){
      buttonPressedWings = true; 
      toggleEnabledWings = !toggleEnabledWings;
    }
    else if (!buttonL1) buttonPressedWings = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledWings){
      piston.set(true); // open wings
    } else{
      piston.set(false);// close wings
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  Auton1.pressed(AutoBump); // When bumper is pressed it cycles through different autonomous 
  
  // When the limit switch is pressed it counts how many times it has gone off
  cataCount.pressed(launchCount); 

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
