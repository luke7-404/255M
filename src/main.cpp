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

uint8_t myTestData[ 1000 ];
char buffer[50];


std::string fileMake(void){
  std::string fileNew;

  for(int i = 0; i<1000;i++){
    std::stringstream temp;
    temp << "dataFile_" << i << ".txt";
    fileNew = temp.str();

    if( !(Brain.SDcard.exists(fileNew.c_str())) ){
      std::cout << "Making new file: " << fileNew << std::endl; 
      
      break;
    } else {
      std::cout << fileNew << " exists" << std::endl;

      continue;
    }
    fileNew.clear();
  }

  
  return fileNew;
}






// inital number of times the limit switch is pressed
int launched = 0;

// Counts the number of time the limit switch has been pressed
void launchCount(){
  launched++; // Adds one to the number of times the button has been pressed

  // Update number on controller:
  Controller1.Screen.clearLine(3); // Clear line
  Controller1.Screen.print("Count %d", launched); // change number
  
  
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

  if (runAuton == 0){ // If 0, Red LED turns on and indicates no auton has been selected
    Brain.Screen.setPenColor("#E60026"); // sets the color of the text for indication
    Brain.Screen.printAt(0, 20, "!!! NO Auton selected !!!"); // print on the screen
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
    LEDRed.on();

  } else if (runAuton == 1){ // If 1, Green LED turns on and says Auton 1 has been selected
    Brain.Screen.setPenColor("#39FF14"); // sets the color of the text for indication
    Brain.Screen.printAt(0, 20, "Auton %d has been selected", runAuton); // print on the screen
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
    LEDGreen.off();
    wait(0.5, sec);
    LEDGreen.on();

  } else if (runAuton == 2){ // If 2, Green LED turns on and says Auton 2 has been selected
    Brain.Screen.setPenColor("#C5E90B"); // sets the color of the text for indication
    Brain.Screen.printAt(0, 20, "Auton %d has been selected", runAuton); // print on the screen
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white 
    LEDGreen.off();
    wait(0.5, sec);
    LEDGreen.on();

  } else if (runAuton == 3){ // If 3, the Green and Red LEDs turn on and 
                             // says Skills Auton has been selected
    Brain.Screen.setPenColor("#00B3CA"); // sets the color of the text for indication
    Brain.Screen.printAt(0, 20, "Skills Auton has been selected (%d)", runAuton); // print on the screen
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
    LEDGreen.on();
    LEDRed.on();
  }else { // if there's an error then it prints the number it gets too
    Brain.Screen.setPenColor("#E60026"); // sets the color of the text for indication
    Brain.Screen.print("ERROR, NUMBER AT: %d", runAuton); // print on the screen
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
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
double kP = 0.125385; // error gain // 0.5-0.2-0.1255-0.12549-0.12539 // 0.12-0.125
double kD = 0.1256; // derivative gain //0.2-0.5 // 0.1-0.11-0.12-0.125

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

    // re-assign the error as prevError and TurnPrevError for next cycle
    prevError = error;
    TurnPrevError = TurnError;

    // Refresh time 20 milliseconds
    task::sleep(20);
  }
  return 1; // Integer functions have to return a number
}






void placeCheck(void){
  int xPos;
  int yPos;
  bool checkX = false;
  bool checkY = false;
  int inputX = sideX.objectDistance(mm); 
  int inputY = sideY.objectDistance(mm);

  if(runAuton == 1){
    xPos = 90;
    yPos = 1480;
    checkX = (inputX >= xPos) && (inputX < xPos+3);
    checkY = (inputY >= yPos) && (inputY < yPos+6);
  } else if (runAuton == 2){
    xPos = 0;
    yPos = 0;
  } else if (runAuton == 3){
    xPos = 0;
    yPos = 302;
    checkX = true;
    checkY = (inputY >= yPos) && (inputY > yPos+5);
  }
  
  
  Brain.Screen.clearScreen();

  

  while ( !(checkX && checkY)){
    Brain.Screen.printAt(0, 20, "Move bot %d horizontally", inputX);
    Brain.Screen.printAt(0, 40, "Move bot %d vertically", inputY);
    std::cout<< inputX << " , " << inputY <<std::endl;
    inputX = sideX.objectDistance(mm); 
    inputY = sideY.objectDistance(mm);
    if(checkX && checkY){
      Brain.Screen.printAt(0, 20, "Move bot %d horizontally", inputX);
      Brain.Screen.printAt(0, 40, "Move bot %d vertically", inputY);
      std::cout<< "Finished at: " <<inputX << " , " << inputY <<std::endl;
      break;
    }
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

  bool periodCheck = vex::competition::isAutonomous && vex::competition::isDriverControl;
  bool statusCheck = vex::competition::isEnabled;
  
  do{
    placeCheck();
  } while(!(periodCheck && statusCheck));

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

  while (1) {
    

    // gets the average of all 7 motor temps
    int motorTempAvg = (leftFront.temperature(pct) + leftMid.temperature(pct) + leftBack.temperature(pct) + rightBack.temperature(pct)
                       + rightMid.temperature(pct) + rightFront.temperature(pct) + Cata.temperature(pct))/7;
    
    // if the average is less than 50 the words on the screen will be blue
    if (motorTempAvg < 50){
      Brain.Screen.setPenColor("#769CBC"); // sets the color of the text for indication
    } else { // otherwise the words on the screen will be orange/ tan
      Brain.Screen.setPenColor("#F1B04C"); // sets the color of the text for indication
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

  if(Brain.SDcard.isInserted()){
    std::string fileName = fileMake();

    Brain.SDcard.savefile(fileName.c_str(), myTestData, sizeof(myTestData));
    std::sprintf(buffer,"File name %s", fileName.c_str()); 
    Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));
  }

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}