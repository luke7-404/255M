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
  switch (runAuton){
    case 1:
      // code
      break;

    case 2:
      //Code
      break;

    default:
    
      leftBack.setVelocity(40, pct);
      leftFront.setVelocity(40, pct);
      leftMid.setVelocity(40, pct);
      rightBack.setVelocity(40, pct);
      rightFront.setVelocity(40, pct);
      rightMid.setVelocity(40, pct);

      leftBack.spinFor(fwd, 2880, degrees, false);
      leftFront.spinFor(fwd, 2880, degrees, false);
      leftMid.spinFor(fwd, 2880, degrees, false);
      rightBack.spinFor(fwd, 2880, degrees, false);
      rightFront.spinFor(fwd, 2880, degrees, false);
      rightMid.spinFor(fwd, 2880, degrees, true);
      
      //Cata.spin(fwd, true);

      leftBack.spinFor(reverse, 1200, degrees, false);
      leftFront.spinFor(reverse, 1200, degrees, false);
      leftMid.spinFor(reverse, 1200, degrees, false);
      rightBack.spinFor(reverse, 1200, degrees, false);
      rightFront.spinFor(reverse, 1200, degrees, false);
      rightMid.spinFor(reverse,1200, degrees, true);
      
  
      //Brain.Screen.print("!!! No Auton Selected !!!");
      break;
      
  }
}
/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop

  bool toggleEnabled = false; // two-choice toggle, so we use bool
  bool buttonPressed = false; // logic variable

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

    // Catapult Motor Logic
    // when L2 is being pressed spin forward
    if(Controller1.ButtonL2.pressing()){
      Cata.spin(fwd);

      // when R2 is being pressed spin reverse
    }else if(Controller1.ButtonR2.pressing()){
      Cata.spin(reverse);

      // else stop
    }else{
      Cata.stop(hold);
    }

    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonL1 = Controller1.ButtonL1.pressing();

    // Toggle Logic
    if (buttonL1 && !buttonPressed){
      buttonPressed = true; 
      toggleEnabled = !toggleEnabled;
    }
    else if (!buttonL1) buttonPressed = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabled){
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

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
