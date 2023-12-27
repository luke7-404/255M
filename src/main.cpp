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
#include "vex.h" // Vex's included class to be able to interact with vex objects
#include "iostream" // Needed to output items to a computer
#include "fstream" // Utilized so that data can print to a file
#include "math.h" // Allows use for complicated math functions

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

uint8_t myData[ 1000 ]; // Unsigned int array to store data (1000 bytes)
char buffer[ 100 ]; // Character array that stores the collected data (100 bytes)(1 character = 1 byte)

// Declaration of the variable that the file name will be stored at
std::string fileName; 

// gets the state of if the card is inserted or not
bool cardInserted = Brain.SDcard.isInserted(); 
bool createdNameExists = false;

// String function that creates the file name and outputs the name (No parameters)
std::string fileMake(void){

  // For loop that names the verion of the file
  // if its the first file its 0, if its the second then it will be 1, so on and so forth
  for(int i = 0; i<1000;i++){ // i checks if it's less than 1000 (file name relatively unbounded)

    std::stringstream temp; // needed a string stream variable to allow type conversion (int i to string)
    temp << "dataFile_" << i << ".txt"; // stream in the components of the file name
    fileName = temp.str(); // converts the string stream into a complete string

    // checks if that name does NOT exist
    // if NOT then it will make the file and break out of the loop
    // if it DOES then it will skip to the next index (No duplication) 
    if( !(Brain.SDcard.exists(fileName.c_str())) ){
      std::cout << "Making new file: " << fileName << std::endl; // used to debug if it creates
      createdNameExists = true;
      break; // exits the loop

      // if it does exist
    } else {
      std::cout << fileName << " exists" << std::endl; // Used to debug that that name/ file exists
      fileName.clear(); // clears the name of the file for the next iteration
      continue; // skips the current index
    }
     
  }

  return fileName; // Once completed it returns the file name
}






// inital number of times the limit switch is pressed
int launched = 0;

// Counts the number of time the limit switch has been pressed
void launchCount(void){
  launched++; // Adds one to the number of times the button has been pressed

  // Update number on controller:
  Controller1.Screen.clearLine(3); // Clear line
  Controller1.Screen.print("Count %d", launched); // change number
  
  // when the card is inserted and the file has been created then the code enters the if statement
  if(cardInserted && createdNameExists){
    // stores the data into the buffer character array using printf commands
    std::sprintf(buffer,"Launch Count: %d\n", launched);
    // Appends the data to the active file
    Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));
  }
  
}








// Inital number of times the Auton selector switch has been pressed
int runAuton = 0;

// Changes which Auton is run 
void AutoSwitch(void){
  
  // Limit so that runAuton does not keep adding
  // starts at 0 and is capped at 2 so that the total numbers are 3
  
  if (runAuton > 2 ){ // if more than 2 then return to 0 on the next click
    runAuton = 0;
  } else{ // else add 1
    runAuton++;
  }

  // Clears the second line on the brain screen so that the alert can update 
  Brain.Screen.clearLine(1);
  
  // Depending on the number runAuton is, the screen will display which one is chosen 
  // (LED is for better indication)
  
  //Turns off the LEDs
  LEDGreen.off(); 
  LEDRed.off();

  /*
    Checks the value of runAuton and displays messages on the screen that 
    shows what the number correlates to. Additionally, turning on/off 
    LEDs and changing the color of the text for better indication
  */

  if (runAuton == 0){ 
    Brain.Screen.setPenColor("#E60026"); // sets the color of the text to red
    Brain.Screen.printAt(0, 20, "!!! NO Auton selected !!!"); // 0 = No auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
    LEDRed.on(); // LED indication

  } else if (runAuton == 1){
    Brain.Screen.setPenColor("#39FF14"); // sets the color of the text to green
    Brain.Screen.printAt(0, 20, "Auton %d has been selected", runAuton); // 1 = 1st auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

    // LED indication
    LEDGreen.off();
    wait(0.5, sec);
    LEDGreen.on();

  } else if (runAuton == 2){
    Brain.Screen.setPenColor("#C5E90B"); // sets the color of the text to a yellow-green
    Brain.Screen.printAt(0, 20, "Auton %d has been selected", runAuton); // 2 = 2nd Auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white 

    // LED indication
    LEDGreen.off();
    wait(0.5, sec);
    LEDGreen.on();

  } else if (runAuton == 3){
    Brain.Screen.setPenColor("#00B3CA"); // sets the color of the text to blue
    Brain.Screen.printAt(0, 20, "Skills Auton has been selected (%d)", runAuton); // 3 = Skills Auto
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

    // LED indication
    LEDGreen.on();
    LEDRed.on();
  }else { // if there's an error then it prints the number it gets too
    Brain.Screen.setPenColor("#E60026"); // sets the color of the text to red 
    Brain.Screen.print("ERROR, NUMBER AT: %d", runAuton); // print the error number
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
    LEDRed.on(); // LED indication
  }
}








// function called to calibrate the inertial sensor
void calibrateInertial(void){
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
double kP = 0.125385; // error gain 
double kD = 0.1256; // derivative gain

int error = 0; // the difference from where the goal is to where you are 
int derivative = 0; // the difference from current error and prevError
int prevError = 0; // the last error
int targetDist = 0; // Goal distance (Lateral Movement)


//* Rotational Variables
// Gain variables - Deals with controller sensitivity (for turning)
double Turn_kP = 0.66700000595; // turn error gain
double Turn_kD = 0.7; // turn derivative gain

double TurnError = 0; // the difference from where the goal is to where you are
int TurnDerivative = 0; // the difference from current turn error and TurnPrevError
int TurnPrevError = 0; // the last turn error
double targetTurn = 0; // Goal Distance (Rotational Movement)

//* Boolean Variables 
bool controlON = true; // Toggles the while loop for the controller
bool resetSens = false; // Toggles if the sensors are reset to 0

int PD_Control(void){ // Declaration of the integer type function
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

    // Calculate and store the average between both sides in an integer variable 
    int avg = (leftPos + rightPos)/2;

    error = targetDist - avg; // the difference of target and current location

    Brain.Screen.printAt(0, 60, "Error %d", error);
    Brain.Screen.printAt(0, 80, "Avg Position %d", avg);

    derivative = error - prevError; // derivative

    Brain.Screen.printAt(0, 100, "Prev Error %d", prevError);

    // Lateral(Lat) Motor(Mtr) Power(Pwr) equation
    double LatMtrPwr = (error * kP)+(derivative * kD); 

    //----------------------------- Rotation -----------------------------//

    // Makes the Inertial sensor a variable with decimal places (readability)
    double InertPos = Inertial.rotation(degrees);

    // Adds a limit to the sensor so it doesn't go past 360
    // if the absolute value of the float point number is in 
    // between 360-361 then it sets it back to 0
    if(fabs(InertPos) > 360 && fabs(InertPos) < 361){
      InertPos = 0;
    } 

    // Error: difference from target and current location
    TurnError = targetTurn - InertPos;  

    // Print Values to debug when needed
    Brain.Screen.printAt(0, 140, "Turn Error %f", TurnError);
    Brain.Screen.printAt(0, 160, "Inertial Position %f", InertPos);
    
    TurnDerivative = TurnError - TurnPrevError; // derivative
    
    // Print Value to debug when needed
    Brain.Screen.printAt(0, 180, "Turn Prev Error %f", TurnPrevError);

    
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

    /*
     if an SD card is inserted and a created name exists the function will write data onto a file

     The data includes:
     - average lateral position and rotation position (inertial sensor value)
     - lateral error and rotation error 
     - lateral derivative and rotation derivative 
     - previous lateral error and previous rotation error
     - lateral motor power and rotation motor power
     - Motor efficiencies (efficiency is output power / input power)
    */

    if (cardInserted && createdNameExists){
      
      std::sprintf(buffer,"Lat avg: %d | Rot pos: %.4f\n", avg, floorf(InertPos * 10000) / 10000);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

      std::sprintf(buffer,"Lat error: %d | Rot error: %.4f\n", error, floorf(TurnError * 10000) / 10000);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

      std::sprintf(buffer,"Lat Deriv: %d | Rot Deriv: %d\n", derivative, TurnDerivative);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

      std::sprintf(buffer,"Lat PrevE: %d | Rot PrevE: %d\n", prevError, TurnPrevError);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

      float flooredLMP = floorf(LatMtrPwr * 10000) / 10000;
      float flooredRMP = floorf(RotMtrPwr * 10000) / 10000;

      std::sprintf(buffer,"LatMtrPwr: %.4f | RotMtrPwr: %.4f\n\n\n", flooredLMP, flooredRMP);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

      double lfe = leftFront.efficiency(pct);
      double rfe = rightFront.efficiency(pct);
      double lme = leftMid.efficiency(pct);
      double rme = rightMid.efficiency(pct);
      double lbe = leftBack.efficiency(pct);
      double rbe = rightBack.efficiency(pct);

      std::sprintf(buffer,"Left Front eff: %.4f | Right Front eff: %.4f\n", lfe, rfe);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

      std::sprintf(buffer,"Left Mid eff: %.4f | Right Mid eff: %.4f\n", lme, rme);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

      std::sprintf(buffer,"Left Back eff: %.4f | Right Back eff: %.4f\n", lbe, rbe);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));
    }

    // re-assign the error as prevError and TurnPrevError for next cycle
    prevError = error;
    TurnPrevError = TurnError;


    // Refresh time 20 milliseconds
    task::sleep(20);
  }
  return 1; // Integer functions have to return a number
}






void placeCheck(void){
  
  // Declare variables xPos and yPos to store 
  // the X and Y positions of the object
  int xPos = 0, yPos = 0;

  // Declare boolean variables checkX and checkY to check if the 
  // object is within a certain range in the X and Y directions
  bool checkX = false, checkY = false;

  // Use the sideX and sideY distance sensors to get the distance to the  
  // object in millimeters and store them in inputX and inputY variables
  int inputX = sideX.objectDistance(mm), inputY = sideY.objectDistance(mm);

  // If runAuton is equal to 1 set xPos to 90 and yPos to 1480
  if(runAuton == 1){
    xPos = 90;
    yPos = 1480;

    /* assigning checkX and checkY to true if the inputX is 
     within the range of xPos to xPos+3 and if the inputY 
     is within the range of yPos to yPos+6
    */
    checkX = (inputX >= xPos) && (inputX < xPos+3);
    checkY = (inputY >= yPos) && (inputY < yPos+6);

  // If runAuton is equal to 2 set xPos to 0 and yPos to 0 (there is no set auton yet...)  
  } else if (runAuton == 2){
    xPos = 0;
    yPos = 0;

  // If runAuton is equal to 3 set xPos to 0 (No xPos measured) and yPos to 302
  } else if (runAuton == 3){
    xPos = 0;
    yPos = 302;

    // It sets the value of checkX to true (No xPos measured) and checkY checks if
    // inputY is greater than or equal to yPos and inputY is greater than yPos+5
    checkX = true;
    checkY = (inputY >= yPos) && (inputY > yPos+5);
  }

  // Calculate the difference between where the robot 
  // is supposed to be and where the robot is
  int diffX = xPos - inputX;
  int diffY = yPos - inputY;

  Brain.Screen.clearScreen(); // Clear the screen of the Brain device

  // Enter a while loop that continues until both checkX and checkY are true
  while ( !(checkX && checkY)){

    //Print the values of diffX and diffY on the Brain screen
    Brain.Screen.printAt(0, 20, "Move bot %d horizontally", diffX);
    Brain.Screen.printAt(0, 40, "Move bot %d vertically", diffY);

    // Print the values of diffX and diffY on the console (Used to debug)
    std::cout<< inputX << " , " << inputY <<std::endl;
    
    // Update the values of inputX and inputY
    inputX = sideX.objectDistance(mm); 
    inputY = sideY.objectDistance(mm);

    // If both checkX and checkY are at the correct 
    // positions(true), break out of the while loop
    if(checkX && checkY){

      // print the values of diffX and diffY on the Brain screen and console
      Brain.Screen.printAt(0, 20, "Set up completed: %d distance", diffX);
      Brain.Screen.printAt(0, 40, "Set up completed: %d distance", diffY);
      std::cout<< "Finished at: " <<inputX << " , " << inputY <<std::endl; // Used to debug

      break; // exit the loop
    }

    wait(20, msec); // save CPU resources
  }
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
  Brain.Screen.printAt(0, 220, "Cata Motor Temp %f", Cata.temperature(pct));
  
  // resets the position to 0
  rightTrack.setPosition(0, degrees); 
  leftTrack.setPosition(0, degrees);
  Cata.setPosition(0, degrees);

  // Turns off the LEDs
  LEDGreen.off();
  LEDRed.off();


  // Retains the status of the current mode the competition match is in,
  // if it is neither in autonomous nor driver control then that is a
  // sign that the mode is pre-auton 
  bool periodCheck = Competition.isAutonomous() && Competition.isDriverControl();

  // if the status of the controller is disabled (false) that is 
  // another sign that the robot is in pre-auton
  bool statusCheck = Competition.isEnabled();
  
  // While the code detects that the period is in pre-auton, 
  // it calls the function placeCheck
  do{
    placeCheck();
    wait(20, msec); // Saves CPU resources
  } while(!(periodCheck && statusCheck)); // Condition check

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

  // If the file exists and is currently writing to a new file then 
  // the code will collect what autonomous was selected
  if (cardInserted && createdNameExists){
    // attaches the string to the buffer character array
    std::sprintf(buffer,"Auton Selected: %d\n\n", runAuton);
    // writes the buffer array to the data file
    Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));
  }

  Controller1.Screen.clearLine(3); // Clears the controller screen for the catapult counter

  // Turns off the LEDs
  LEDGreen.off();
  LEDRed.off();

  task drivePD(PD_Control); // starts the PD controller as a task

  extendIntake.set(true); // Extends the intake out

  // Depending on the number that runAuton holds that is the code ran
  if (runAuton == 1){ // if 1 then (for now) will run the autonomous skills code    
    LEDGreen.on();
    //targetTurn = -130;
    targetDist = 2300; //1125
    wait(1.5, sec);
    targetDist = 500;
  } else if (runAuton == 2){ // if 2 then nothing for now
    // code

  
  } else if(runAuton == 3){ // if 3 then Skills Auton will run for autonomous
    LEDGreen.on();
    targetDist= -2000; //-2400
    wait(1,sec);
    LEDGreen.off();
    LEDRed.on();
    targetTurn = -30;
    wait(1,sec);
    LEDRed.off();
    LEDGreen.on();
    wait(1.5, sec);
    resetSens = true;
    targetDist=-300;
    Cata.spin(fwd, 80, pct);
    waitUntil(launched == 54);
    LEDRed.on();
    LEDGreen.off();
    Cata.stop(coast);
  }
  // if anything other than 1, 2, or 3 is chosen then default to none selected 
  else {
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
  
  // Booleans for Wing toggle 
  bool toggleEnabledWings = false; // two-choice toggle, so we use bool
  bool buttonPressedWings = false; // logic variable
  
  // Booleans for Cata toggle
  bool toggleEnabledCata = false;
  bool buttonPressedCata = false;

  // Cooldown boolean
  bool onCooldown = false;
  Brain.resetTimer(); // Resets the timer to 0 for the cooldown logic 


  while (1) {
    
    // Motor temp code // 

    // Calculate the average temperature of all 7 motors in Fahrenheit
    int motorTempAvg = (leftFront.temperature(fahrenheit) + 
                        leftMid.temperature(fahrenheit) + 
                        leftBack.temperature(fahrenheit) + 
                        rightBack.temperature(fahrenheit) + 
                        rightMid.temperature(fahrenheit) + 
                        rightFront.temperature(fahrenheit) + 
                        Cata.temperature(fahrenheit))/7;
    
    // Cooldown logic //

    // Capture the current time from the brain's timer in seconds (type declaration)
    int elapsedTime = Brain.Timer.value();  

    // turn on the onCooldown boolean every 15 seconds
    if (elapsedTime % 15 == 0) {

      onCooldown = true; 

      // prints the elapsed time and onCooldown bool for debugging
      std::cout << elapsedTime << onCooldown << std::endl; 

    } else { // if not an interval of 15 turn off cooldown

      onCooldown = false;
      std::cout << elapsedTime << std::endl; // prints the elapsed time
    }

    /* If the SD card is inserted, the file has been created, and the system is on cooldown,
      format a message with the average motor temperature and elapsed time, and write it 
      to the SD card under the specified file name 
    */
    if((cardInserted && createdNameExists) && onCooldown){
      std::sprintf(buffer,"Avg motor temp %d at %d seconds\n", motorTempAvg, elapsedTime);
      Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));

    }
    
    
    // motors go to half speed at 55 degrees celsius and go to quarter at 60 degrees
    // green zone would most likely be anywhere below 40C and they really shouldn't 
    // be going above 60C unless theres a problem

    // if the average is less than 79 degrees fahrenheit the words on the screen
    // will be blue, otherwise the words on the screen will be orange/ tan
    if (motorTempAvg < 79){
      Brain.Screen.setPenColor("#769CBC"); // sets the color to blue
    } else {
      Brain.Screen.setPenColor("#F1B04C"); // sets the color to orange/ tan
    }
    


    // Motor Temp Prints
    // Left side
    Brain.Screen.printAt(0, 60, "LF Motor Temp %f", leftFront.temperature(pct));
    Brain.Screen.printAt(0, 80, "LM Motor Temp %f", leftMid.temperature(pct));
    Brain.Screen.printAt(0, 100, "LB Motor Temp %f", leftBack.temperature(pct));
    
    // right side
    Brain.Screen.printAt(0, 140, "RB Motor Temp %f", rightBack.temperature(pct));
    Brain.Screen.printAt(0, 160, "RM Motor Temp %f", rightMid.temperature(pct));
    Brain.Screen.printAt(0, 180, "RF Motor Temp %f", rightFront.temperature(pct));
    
    // catapult
    Brain.Screen.printAt(0, 220, "Cata Motor Temp %f", Cata.temperature(pct));
  
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

    // Set Velocity of the motors 
    leftBack.setVelocity(100, pct);
    leftFront.setVelocity(100, pct);
    leftMid.setVelocity(100, pct);
    rightBack.setVelocity(100, pct);
    rightFront.setVelocity(100, pct);
    rightMid.setVelocity(100, pct);
    Cata.setVelocity(70, pct);

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
    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonB = Controller1.ButtonB.pressing();

    // Toggle Logic
    if (buttonB && !buttonPressedCata){
      buttonPressedCata = true; 
      toggleEnabledCata = !toggleEnabledCata;
    }
    else if (!buttonB) buttonPressedCata = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledCata){
      LEDRed.on();
      Cata.spin(fwd, 80, pct); // Spins Cata on a toggle if it needs to be on for longer
    } 

    // Manual Input
    // when L2 is being pressed spin forward
    else if(Controller1.ButtonL2.pressing()) {
      Cata.spin(fwd, 80, pct); //100
    } else{
      LEDRed.off();
      Cata.stop(coast); // else stop
    } 



    // Wings/ blockers Logic
    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonX = Controller1.ButtonX.pressing();

    // Toggle Logic
    if (buttonX && !buttonPressedWings){
      buttonPressedWings = true; 
      toggleEnabledWings = !toggleEnabledWings;
    }
    else if (!buttonX) buttonPressedWings = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledWings){
      piston.set(true); // open wings

    } else piston.set(false); // close wings

    if(Controller1.ButtonL1.pressing()){
      Intake.spin(fwd, 100, pct);
    } else if (Controller1.ButtonR1.pressing()) {
      Intake.spin(reverse, 100, pct);
    } else Intake.stop(coast);

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

  Auton1.pressed(AutoSwitch); // When bumper is pressed it cycles through different autonomous
  
  // When the limit switch is pressed it counts how many times it has gone off
  cataLimit.pressed(launchCount); 

  // Checks if the SD card is inserted into the Brain
  if(cardInserted){
    // when the function returns the file name it is assigned to the variable fileNew
    std::string fileNew = fileMake();

    // Creates and saves the file
    Brain.SDcard.savefile(fileNew.c_str(), myData, sizeof(myData));

    // stores the string into the buffer character array using printf commands
    std::sprintf(buffer,"File name %s\n\n\n\n", fileNew.c_str());

    // As a test the file name is written to the file
    Brain.SDcard.appendfile(fileNew.c_str(), (uint8_t *)buffer, strlen(buffer));
  }

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}