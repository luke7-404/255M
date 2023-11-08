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
#include "Drive_PD.cpp"

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
    
  if (runAuton >= 3){
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
  pdON = true;
  resetSens = true;
  vex::task PD(drivePD);
  targetTurn = 90;
  

}
/*
switch (runAuton){
    case 1:
      targetDist = 0;
      targetTurn = 0;
      break;
    
    case 2:
      //Code
      break;

    default:
      Brain.Screen.print("!!! No Auton Selected !!!");
      break;
      
  }
*/
/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop

  bool toggleEnabled = false; // two-choice toggle, so we use bool
  bool buttonPressed = false; // logic variable


  //pdON = false; // Turns off PD function
  while (1) {

    Brain.Screen.printAt(2, 80, "left track %f", leftTrack.position(deg));
    Brain.Screen.printAt(2, 100, "right track %f", rightTrack.position(deg));
    Brain.Screen.printAt(2, 120, "Inertial %f", Inertial.heading(deg));

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
    if(Controller1.ButtonL2.pressing()){
      Cata.spin(fwd);
    }else if(Controller1.ButtonR2.pressing()){
      Cata.spin(reverse);
    }else{
      Cata.stop(hold);
    }


    bool buttonX = Controller1.ButtonX.pressing(); // boolean to get if the button is pressed (true) or it isn't pressed (false)

    // Toggle Logic
    if (buttonX && !buttonPressed){
      buttonPressed = true; 
      toggleEnabled = !toggleEnabled;
    }
    else if (!buttonX) buttonPressed = false;
    /* from the logic above we take the output of if the toggledEnabled is true or false and then we use it to spin the fly wheel motor */

    // Code For toggle Enabled or Disabled
    if(toggleEnabled){

      piston.set(true); // spin the motor
    } else{
      piston.set(false);// stops the motor but keeps the motion die off
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

  Auton1.pressed(AutoBump);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
