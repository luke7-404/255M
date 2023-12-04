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

#include "sstream" // Used to control strings
#include "vex.h"
#include "iostream" // Needed to output items to a computer
// #include "fstream" // Used to handle files
#include "ctime" // Used to get time (used for file naming)

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

// List of months in abbreviated form 0-11 : Jan - Dec
std::string const Months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", " Jul ", "Aug", "Sep", "Oct", "Nov", "Dec"};

// A function that returns a string (the filename)  
std::string filename(void){
  
  // Get current time in seconds since epoch (Jan 1, 1970 00:00:00 UTC)
  // Stores it into currentTime which is a time_t type
  // time_t type is used to represent time (Not a primitive type)
  std::time_t currentTime = std::time(nullptr);

  // Convert to local timezone and returns to pointer localTime
  // localTime points to the converted current time structure
  // which contains year, moth, day, etc. units
  std::tm* localTime = std::localtime(&currentTime);
   
  // Store date and time components separately

  // The integer variables are appointed from
  // the the converted current time structure
  // (Look at previous lines)
  int year = localTime->tm_year + 1900; // Years since 1900
  
  // Months are from 0-11 (from 0-11 there is a total of 12 numbers)
  int month = localTime->tm_mon; 
  int day = localTime->tm_mday; 
  int hour = localTime->tm_hour;
  int minute = localTime->tm_min;
  int second = localTime->tm_sec;

  // Type conversion (Integer to string)
  // Declare string stream variables
  std::stringstream yearS, dayS, hourS, minuteS, secondS; 
  
  // "<<" in this case is similar to assigning a value with "="
  // Insert the time unit into the stringstream
  yearS << year; 
  dayS << day;  
  hourS << hour;
  minuteS << minute;
  secondS << second;

  // Final type conversion (stringstream to string)
  // takes yearS, dayS, hourS, minuteS, secondS and turns it into strings 
  std::string yearStr, dayStr, hourStr, minuteStr, secondStr;
  
  // sets the string variables equal to the stringstream variables
  // with the .str() method (Overall converts them into strings)
  yearStr = yearS.str();
  dayStr = dayS.str();
  hourStr = hourS.str();
  minuteStr = minuteS.str();
  secondStr = secondS.str();

  // Concatenate the components together to form the name 
  std::string name = yearStr + "-" + Months[month] + "-" + dayStr + "_" + hourStr + "." + minuteStr + "." + secondStr;
  
  // Output the filename to the terminal (for debugging purposes)
  std::cout << name << std::endl; 

  // outputs the final string to be used the filename
  return name; 
}








// inital number of times the limit switch is pressed
int launched = 0;

// Counts the number of time the limit switch has been pressed
void launchCount(){
  launched++; // Adds one to the number of times the button has been pressed
  
  // Update number on controller
  Controller1.Screen.clearLine(3); // Clear line
  Controller1.Screen.print("Count %d", launched); // change number
}








// Inital number of times the Auton selector switch has been pressed
int runAuton = 0;

// Changes which Auton is run 
void AutoBump(void){
  
  // Limit so that runAuton does not keep adding
  // starts at 0 and is capped at 2 so that the total numbers are 3
  
  if (runAuton > 2 ){ // if more than 2 then return to 0
    runAuton = 0;
  } else{ // else add 1
    runAuton++;
  }

  // Clears the second line on the brain screen so that the alert can update 
  Brain.Screen.clearLine(2);
  
  // Depending on the number runAuton is, the screen will display which one is chosen 
  // (LED is for better indication)
  
  //Turns off the LEDs
  LEDGreen.off(); 
  LEDRed.off();

  if (runAuton == 0){ // If 0, Red LED turns on and indicates no auton has been selected
    Brain.Screen.printAt(0, 40, "!!! NO Auton selected !!!");
    LEDRed.on();

  } else if (runAuton == 1){ // If 1, Green LED turns on and says Auton 1 has been selected
    Brain.Screen.printAt(0, 40, "Auton %d has been selected", runAuton);
    LEDGreen.on();

  } else if (runAuton == 2){ // If 2, Green LED turns on and says Auton 2 has been selected
    Brain.Screen.printAt(0, 40, "Auton %d has been selected", runAuton);
    LEDGreen.on();

  } else { // if there's an error then it prints the number it gets too
    Brain.Screen.print("ERROR, NUMBER AT: %d", runAuton);
    LEDRed.on();
  }
}








// function called to calibrate the inertial sensor
void calibrateInertial(){
  Inertial.calibrate(); // Starts the sensor calibration

  // While it is calibrating it indicates it on the controller screen
  while (Inertial.isCalibrating()){
    // Update Screen
    Controller1.Screen.clearLine(3); // clear line
    Controller1.Screen.print("Calibrating"); // print calibrating
  } 

  // when finished it prints done on both the brain and controller screen
  Controller1.Screen.clearLine(3); // clear line on controller 

  // Print on brain and controller Screen done
  Controller1.Screen.print("Done");
  Brain.Screen.printAt(0, 20, "Done");

  // Vibrates the controller to indicate calibration has finished
  Controller1.rumble("..");

  wait(10, msec); // Has a buffer time
}







//---------------//
// PD controller //
//---------------//


//* Lateral Variables
// Gain variables - Deals with controller sensitivity
double kP = 0.16; // error gain
double kD = 0.0; // derivative gain

int error = 0; // the difference from where the goal is to where you are 
int derivative = 0; // the difference from current error and prevError
int prevError = 0; // the last error
int targetDist = 0; // Goal distance (Lateral Movement)


//* Rotational Variables
// Gain variables - Deals with controller sensitivity (for turning)
double Turn_kP = 0.6; // turn error gain
double Turn_kD = 0.0; // turn derivative gain

double TurnError = 0; // the difference from where the goal is to where you are
int TurnDerivative = 0; // the difference from current turn error and TurnPrevError
int TurnPrevError = 0; // the last turn error
double targetTurn = 0; // Goal Distance (Rotational Movement)

//* Boolean Variables 
bool controlON = true; // Toggles the while loop for the controller
bool resetSens = false; // Toggles if the sensors are reset to 0

int PD_Control(){ // Declaration
  while(controlON){ // while loop for the controller (Needed to update values)

    
    // When true the sensors will be reset to 0
    if(resetSens){

      // Sets boolean to false so that the sensors don't always get reset
      resetSens = false; 

      // Resets both rotation encoders to 0
      leftTrack.resetPosition();
      rightTrack.resetPosition();
    }

    // Makes both rotation encoders an integer variable (readability)
    int leftPos = leftTrack.position(degrees);
    int rightPos = rightTrack.position(degrees);

    // the average between both sides 
    int avg = (leftPos + rightPos)/2;

    error = targetDist - avg; // the difference of target and current location

    derivative = error - prevError; // derivative

    // Lateral(Lat) Motor(Mtr) Power(Pwr) equation
    double LatMtrPwr = (error * kP)+(derivative * kD); 

    //----------------------------- Rotation -----------------------------//

    // Makes the Inertial sensor a variable with decimal places (readability)
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
    // when a negative value is applied the signs are flipped
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
  return 1; // Integer functions have to return a number
}







void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  calibrateInertial(); // calls to calibrate the inertial
  
  // Pre-Match motor temps

  // Left side
  Brain.Screen.printAt(0, 60, "LF Motor Temp %f", leftFront.temperature(pct));
  Brain.Screen.printAt(0, 80, "LM Motor Temp %f", leftMid.temperature(pct));
  Brain.Screen.printAt(0, 100, "LB Motor Temp %f", leftBack.temperature(pct));
  // right side
  Brain.Screen.printAt(0, 140, "RB Motor Temp %f", rightBack.temperature(pct));
  Brain.Screen.printAt(0, 160, "RM Motor Temp %f", rightMid.temperature(pct));
  Brain.Screen.printAt(0, 180, "RF Motor Temp %f", rightFront.temperature(pct));
  // catapult
  Brain.Screen.printAt(0, 200, "Cata Motor Temp %f", Cata.temperature(pct));

  // resets the position to 0
  rightTrack.setPosition(0, degrees); 
  leftTrack.setPosition(0, degrees);
  Cata.setPosition(0, degrees);

  // Turns off the LEDs
  LEDGreen.off();
  LEDRed.off();

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
  Brain.Screen.clearScreen(); // Clears screen on the brain for diagnosis

  Controller1.Screen.clearLine(3); // Clears the controller screen for the catapult counter

  // Turns off the LEDs
  LEDGreen.off();
  LEDRed.off();

  task drivePD(PD_Control); // starts the PD controller as a task

  // Depending on the number that runAuton holds that is the code ran
  if (runAuton == 1){ // if 1 then...
    // code
    targetDist = 800; // Moves the robot laterally for 800 units 
    
    

  } else if (runAuton == 2){ // if 2 then...
    // code

  
  }
  // if anything other than 1 or 2 then default to none selected 
  else {
    Brain.Screen.print("!!! No Auton Selected !!!"); // Alerts that no auton is selected
    task::stop(drivePD); // stops the function
  } 
}








/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/


void usercontrol(void) {
  // User control code here, inside the loop

  // Turns off the LEDs
  LEDGreen.off();
  LEDRed.off();

  controlON = false; // Turns off the PD controller
  Brain.Screen.clearScreen(); // Clears the brain screen for driver control prints
  
  // Booleans for wing toggle 
  bool toggleEnabledWings = false; // two-choice toggle, so we use bool
  bool buttonPressedWings = false; // logic variable

  while (1) {

    // Motor Temp Prints
    // Left Side
    Brain.Screen.printAt(0, 60, "LF Motor Temp %f", leftFront.temperature(pct));
    Brain.Screen.printAt(0, 80, "LM Motor Temp %f", leftMid.temperature(pct));
    Brain.Screen.printAt(0, 100, "LB Motor Temp %f", leftBack.temperature(pct));
    // Right Side
    Brain.Screen.printAt(0, 140, "RB Motor Temp %f", rightBack.temperature(pct));
    Brain.Screen.printAt(0, 160, "RM Motor Temp %f", rightMid.temperature(pct));
    Brain.Screen.printAt(0, 180, "RF Motor Temp %f", rightFront.temperature(pct));

    // Catapult Motor
    Brain.Screen.printAt(0, 200, "Cata Motor Temp %f", Cata.temperature(pct));


    // Set Velocity of the motors 
    leftBack.setVelocity(100, pct);
    leftFront.setVelocity(100, pct);
    leftMid.setVelocity(100, pct);
    rightBack.setVelocity(100, pct);
    rightFront.setVelocity(100, pct);
    rightMid.setVelocity(100, pct);
    Cata.setVelocity(100, pct);

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
      Cata.spin(fwd,100,pct);

      // when R2 is being pressed spin reverse
    } else if(Controller1.ButtonR2.pressing()){
      Cata.spin(reverse,100,pct);

      
    } else Cata.stop(hold); // else stop
    


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

    } else piston.set(false); // close wings

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
  cataLimit.pressed(launchCount); 

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}