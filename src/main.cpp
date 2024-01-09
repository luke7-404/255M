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

char buffer[ 1000 ]; // Character array that stores the collected data

// Declaration of the variable that the file name will be stored at
std::string fileName; 

// gets the state of if the card is inserted or not
bool cardInserted = Brain.SDcard.isInserted(); 
bool createdNameExists = false;

// String function that creates the file name and outputs the name (No parameters)
std::string fileMake(void){

  // For loop that names the verion of the file
  // if its the first file its 0, if its the second then it will be 1, so on and so forth
  for(short i = 0; i<1000;i++){ // i checks if it's less than 1000 (file name relatively unbounded)

    std::stringstream temp; // needed a string stream variable to allow type conversion (int i to string)
    temp << "dataFile_" << i << ".csv"; // stream in the components of the file name
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
uint8_t timesLaunched = 0;

// Boolean logic to detect if launched
bool isDriverControl = Competition.isDriverControl();

// Data collection
short totalElapsedTime;

// Counts the number of time the limit switch has been pressed
void launchCount(void){
  timesLaunched++; // Adds one to the number of times the button has been pressed
  
  // Update number on controller:
  Controller1.Screen.clearLine(3); // Clear line
  Controller1.Screen.print("Count %d", timesLaunched); // change number
}






// Inital number of times the Auton selector switch has been pressed
uint8_t runAuton = 0;

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
    Brain.Screen.printAt(6, 20, "!!! NO Auton selected !!!"); // 0 = No auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
    LEDRed.on(); // LED indication

  } else if (runAuton == 1){
    Brain.Screen.setPenColor("#39FF14"); // sets the color of the text to green
    Brain.Screen.printAt(6, 20, "Auton %d has been selected", runAuton); // 1 = 1st auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

    // LED indication
    LEDGreen.off();
    wait(0.5, sec);
    LEDGreen.on();

  } else if (runAuton == 2){
    Brain.Screen.setPenColor("#C5E90B"); // sets the color of the text to a yellow-green
    Brain.Screen.printAt(6, 20, "Auton %d has been selected", runAuton); // 2 = 2nd Auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white 

    // LED indication
    LEDGreen.off();
    wait(0.5, sec);
    LEDGreen.on();

  } else if (runAuton == 3){
    Brain.Screen.setPenColor("#00B3CA"); // sets the color of the text to blue
    Brain.Screen.printAt(6, 20, "Skills Auton has been selected (%d)", runAuton); // 3 = Skills Auto
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











// Short hand variable names to compact the code
/*
  l = left
  r = right
  m = mid
  b = back
  f = front
  e or Eff = efficiency
*/
float lfe, rfe, lme, rme, lbe, rbe, cataEff, intakeEff;

// A void function that 
// And writes the data to the SD card
void addData(short driverTime, int avg, double InertPos, int latErr, double rotErr, int latDeriv,  
              double rotDeriv, int latPrevErr, double rotPrevErr, double RotMtrPwr, double LatMtrPwr){

  totalElapsedTime = Brain.Timer.value();

  // Calculate the average temperature of all 8 motors in Fahrenheit

  double* tempPtr = new double; // Create a temporary pointer that points to the average of all the motors
  
  *tempPtr = (leftFront.temperature(fahrenheit) + 
              leftMid.temperature(fahrenheit) + 
              leftBack.temperature(fahrenheit) + 
              rightBack.temperature(fahrenheit) + 
              rightMid.temperature(fahrenheit) + 
              rightFront.temperature(fahrenheit) + 
              Cata.temperature(fahrenheit)+
              Intake.temperature(fahrenheit))/8;

  // Floor the double and float values to the 4th decimal place
  float motorTempAvg = floorf(*tempPtr * 10000) / 10000; 
  delete tempPtr; // delete the pointer so that there is no memory leak
  // more flooring
  float flooredInertPos = floorf(InertPos * 10000) / 10000;
  float flooredRotErr = floorf(rotErr * 10000) / 10000;
  float flooredRotDeriv = floorf(rotDeriv * 10000) / 10000;
  float flooredRotPrevErr = floorf(rotPrevErr * 10000) / 10000;
  float flooredLMP = floorf(LatMtrPwr * 10000) / 10000;
  float flooredRMP = floorf(RotMtrPwr * 10000) / 10000;
  
  // Updating the variables that hold the motor efficiencies
  lfe = leftFront.efficiency(pct);
  rfe = rightFront.efficiency(pct);
  lme = leftMid.efficiency(pct);
  rme = rightMid.efficiency(pct);
  lbe = leftBack.efficiency(pct);
  rbe = rightBack.efficiency(pct);
  cataEff = Cata.efficiency(pct);
  intakeEff = Intake.efficiency(pct);     

  // Use the sprintf function to append all the data to the buffer character array (string)
  std::sprintf(buffer,"%d , %d , %d , %.4f , %d , %d , %.4f , %d , %.4f , %d , %.4f , %d , %.4f , %.4f , %.4f, %.4f , %.4f , %.4f , %.4f , %.4f , %.4f , %.4f , %.4f\n",
   totalElapsedTime, driverTime, timesLaunched, motorTempAvg, runAuton, avg, flooredInertPos, latErr, flooredRotErr, latDeriv, 
   flooredRotDeriv, latPrevErr, flooredRotPrevErr, flooredLMP, flooredRMP, lfe, lme, lbe, rfe, rme, rbe, cataEff, intakeEff);
  
  // append the buffer to the data file
  Brain.SDcard.appendfile(fileName.c_str(), (uint8_t *)buffer, strlen(buffer));
  
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
  Brain.Screen.printAt(6, 20, "Done");

  // Vibrates the controller to indicate calibration has finished
  Controller1.rumble("..");

  wait(10, msec); // Has a buffer time
}






//---------------//
// PD controller //
//---------------//


//* Lateral Variables
// Gain variables - Deals with controller sensitivity
const float kP = 0.125385f; // error gain 
const float kD = 0.1256f; // derivative gain

int error = 0; // the difference from where the goal is to where you are 
int derivative = 0; // the difference from current error and prevError
int prevError = 0; // the last error
short targetDist = 0; // Goal distance (Lateral Movement)


//* Rotational Variables
// Gain variables - Deals with controller sensitivity (for turning)
const double Turn_kP = 0.66700000595; // turn error gain
const float Turn_kD = 0.7f; // turn derivative gain

double TurnError = 0; // the difference from where the goal is to where you are
double TurnDerivative = 0; // the difference from current turn error and TurnPrevError
double TurnPrevError = 0; // the last turn error
float targetTurn = 0; // Goal Distance (Rotational Movement)

//* Boolean Variables 
bool controlON = true; // Toggles the while loop for the controller
bool resetSens = false; // Toggles on to reset sensors to 0


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

    Brain.Screen.printAt(6, 60, "Error %d", error);
    Brain.Screen.printAt(6, 80, "Avg Position %d", avg);

    derivative = error - prevError; // derivative

    Brain.Screen.printAt(6, 100, "Prev Error %d", prevError);

    // Lateral(Lat) Motor(Mtr) Power(Pwr) equation
    double LatMtrPwr = (error * kP)+(derivative * kD); 

    //----------------------------- Rotation -----------------------------//

    // Makes the Inertial sensor a variable with decimal places (readability)
    double InertPos = Inertial.rotation(degrees);

    // Adds a limit to the sensor so it doesn't go past 360
    // if the absolute value of the float point number is in 
    // between 360-361 then it sets it back to 0
    if(fabs(InertPos) > 360 && fabs(InertPos) < 361.5){
      InertPos = 0;
    } 

    // Error: difference from target and current location
    TurnError = targetTurn - InertPos;  

    // Print Values to debug when needed
    Brain.Screen.printAt(6, 140, "Turn Error %f", TurnError);
    Brain.Screen.printAt(6, 160, "Inertial Position %f", InertPos);
    
    TurnDerivative = TurnError - TurnPrevError; // derivative
    
    // Print Value to debug when needed
    Brain.Screen.printAt(6, 180, "Turn Prev Error %f", TurnPrevError);

    
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

    if (cardInserted && createdNameExists){

      // This is a function call that writes the motor 
      // data such as motor efficiency to the SD card
      addData(0, avg, InertPos, error, TurnError, 
              derivative, TurnDerivative, prevError,
              TurnPrevError, RotMtrPwr, LatMtrPwr); 
    }

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
  Brain.Screen.printAt(6, 100, "LF Motor Temp %f", leftFront.temperature(pct));
  Brain.Screen.printAt(6, 120, "LM Motor Temp %f", leftMid.temperature(pct));
  Brain.Screen.printAt(6, 140, "LB Motor Temp %f", leftBack.temperature(pct));
  // right side
  Brain.Screen.printAt(6, 160, "RB Motor Temp %f", rightBack.temperature(pct));
  Brain.Screen.printAt(6, 180, "RM Motor Temp %f", rightMid.temperature(pct));
  Brain.Screen.printAt(6, 200, "RF Motor Temp %f", rightFront.temperature(pct));
  // catapult
  Brain.Screen.printAt(6, 220, "Cata Motor Temp %f", Cata.temperature(pct));
  
  // resets the position to 0
  rightTrack.resetPosition(); 
  leftTrack.resetPosition();;
  Cata.resetPosition();

  // Turns off the LEDs
  LEDGreen.off();
  LEDRed.off();

  Brain.Screen.clearLine(1);
  
  Brain.Screen.setPenColor("#E60026"); // sets the color of the text to red
  Brain.Screen.printAt(6, 20, "!!! NO Auton selected !!!"); // 0 = No auton
  Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white
  LEDRed.on(); // LED indication
  
  // Checks if the game state is enabled and in Driver Control or Autonomous
  // When the state is not enabled the positioning loop will run 
  if(!Competition.isEnabled()){


    /* Declare integer variable arrays xPos and yPos to  
     store the X and Y positions (in Millimeters) 
     The numbers were determined through testing 
     starting positions.
    */
    short xPos[3] = {0, 0, 90};
    short yPos[3] = {302, 0, 1480};

    /*
    xPos[0] and yPos[0] = 1st Auton setup
    xPos[1] and yPos[1] = 2nd Auton setup
    xPos[2] and yPos[2] = Skills Auton setup
    */

    // Use the distance sensors, sideX and sideY, to get the distance to the  
    // perimeters in millimeters and store them in inputX and inputY variables
    short inputX = sideX.objectDistance(mm); 
    short inputY = sideY.objectDistance(mm);

    // Initialization of the integer variables diffX and diffY
    // These will hold the difference of a position and the input
    // Side respective
    short diffX, diffY;
    
    // While the input of X and Y do NOT equal the predetermined 
    // numbers then the code will loop until it does
    // since arrays are indexed starting at 0 we need to subtract 1
    // runAuton = 1 | calculate positions 1-1 = 0: refer to line 486
    while ((inputX != xPos[runAuton-1]) && (inputY != yPos[runAuton-1])){
      
      // Since Auton 2 is not actively used it breaks out 
      // of the loop when runAuton = 2
      /*
      if (runAuton == 2) {
        Brain.Screen.printAt(6, 40, "Set up completed: 0 distance");
        Brain.Screen.printAt(6, 60, "Set up completed: 0 distance");
        break;
      }
      */
      // Some indexes, like 3, do not have an axis used. If the 
      // value is more than 0, then it does calculate the 
      // difference for that value, otherwise it does not

      if (xPos[runAuton-1] != 0) {
        Brain.Screen.clearLine(2);
        // Calculate and indicate how far way the position is
        diffX = xPos[runAuton-1] - inputX;
        Brain.Screen.printAt(6, 40, "Move bot %d horizontally", diffX);
      } else {

        // indicate that this axis is not used
        Brain.Screen.clearLine(2);
        Brain.Screen.printAt(6, 40, "No horizontal"); 
      }
      
      if (yPos[runAuton-1] != 0){
        Brain.Screen.clearLine(3);
        // Calculate and indicate how far way the position is
        diffY = yPos[runAuton-1] - inputY; 
        Brain.Screen.printAt(6, 60, "Move bot %d vertically", diffY);
      } else {

        // indicate that this axis is not used
        Brain.Screen.clearLine(3);
        Brain.Screen.printAt(6, 60, "No vertical");
      }
      
      std::cout<< inputX << " , " << inputY <<std::endl; // Output the values (Used when debugging)
      
      // Update sensor variables
      inputX = sideX.objectDistance(mm); 
      inputY = sideY.objectDistance(mm);
      
      // If BOTH axises have reached their position then the code breaks out of the loop
      if((inputX == xPos[runAuton-1]) && (inputY == yPos[runAuton-1])){

        // print the final values of diffX and diffY on the Brain screen
        Brain.Screen.printAt(6, 40, "Set up completed: %d distance", diffX);
        Brain.Screen.printAt(6, 60, "Set up completed: %d distance", diffY);

        // Output the values (Used when debugging)
        std::cout<< "Finished at: " <<inputX << " , " << inputY <<std::endl;
        break;
      }
      wait(20, msec); // save CPU resources
    }
  }


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
  resetSens = true;
  extendIntake.set(true); // Extends the intake out

  // Depending on the number that runAuton holds that is the code ran
  if (runAuton == 1){ // if 1 then (for now) will run the autonomous skills code    
    LEDGreen.on();
    //targetTurn = -130;
    targetDist = 3000; //1125
    //wait(1.5, sec);
    //targetDist = 500;
  } else if (runAuton == 2){ // if 2 then nothing for now

  
  } else if(runAuton == 3){ // if 3 then Skills Auton will run for autonomous
    LEDGreen.on();
    targetDist= -1940; //-2000
    wait(1,sec);
    LEDGreen.off();
    LEDRed.on();
    targetTurn = -30;
    wait(1,sec);
    LEDRed.off();
    LEDGreen.on();
    wait(.5, sec);
    resetSens = true;
    targetDist=-300;
    Cata.spin(fwd, 80, pct);
    waitUntil(timesLaunched == 48);
    LEDRed.on();
    LEDGreen.off();
    Cata.stop(coast);
    resetSens = true;
    targetDist = 1865;
    targetTurn = -180;
    wait(0.3, sec);
    resetSens = true;
    targetDist = -1865;
    wait(0.3, sec);
    targetTurn = 0;
    wait(0.2, sec);
    piston.set(true);
    targetDist = 2300;
    wait(0.2, sec);
    targetDist = 0;
    piston.set(false);
  }
  // if anything other than 1-3 is chosen then 
  // the default option is to terminate the task
  // and not let the robot move for Auton 
  else {
    task::stop(PD_Control); // terminates the function
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

  // Data variables
  Brain.resetTimer(); // Resets the timer to 0 for the cooldown logic
  short elapsedTime; // elapsed Driver time (in Seconds)

  while (1) {
    
    // Motor temp code // 

    // Capture the current time from the brain's timer in seconds (type declaration)
    elapsedTime = Brain.Timer.value();  

    // Check if the SD card is inserted and a file is open
    if(cardInserted && createdNameExists){

      // if the robot is nearing the end of the match it will do one more file addition
      if (elapsedTime+5 == 180) {

        addData(elapsedTime, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 

        // prints the elapsed time and onCooldown bool for debugging
        std::cout << "Added Data at " << elapsedTime << "seconds"<< std::endl; 
      } 
      // Otherwise, any interval of 15 seconds before the end of the match will be recorded
      else if (elapsedTime % 15 == 0) {
        
        addData(elapsedTime, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 

        // prints the elapsed time and onCooldown bool for debugging
        std::cout << "Added Data at " << elapsedTime << "seconds"<< std::endl; 
      } 
    }
    


    // Motor Temp Prints
    // Left side
    Brain.Screen.printAt(6, 60, "LF Motor Temp %f", leftFront.temperature(pct));
    Brain.Screen.printAt(6, 80, "LM Motor Temp %f", leftMid.temperature(pct));
    Brain.Screen.printAt(6, 100, "LB Motor Temp %f", leftBack.temperature(pct));
    
    // right side
    Brain.Screen.printAt(6, 140, "RB Motor Temp %f", rightBack.temperature(pct));
    Brain.Screen.printAt(6, 160, "RM Motor Temp %f", rightMid.temperature(pct));
    Brain.Screen.printAt(6, 180, "RF Motor Temp %f", rightFront.temperature(pct));
    
    // catapult
    Brain.Screen.printAt(6, 220, "Cata Motor Temp %f", Cata.temperature(pct));
  
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
    bool buttonA = Controller1.ButtonA.pressing();

    // Toggle Logic
    if (buttonA && !buttonPressedCata){
      buttonPressedCata = true; 
      toggleEnabledCata = !toggleEnabledCata;
    }
    else if (!buttonA) buttonPressedCata = false;

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

    uint8_t myData[ 1000 ]; // Unsigned int array to store data (1000 bytes)
    
    // Creates and saves the file
    Brain.SDcard.savefile(fileNew.c_str(), myData, sizeof(myData));

    /*
      A constant character array (string) to use as column 
      titles for the CSV File. The columns are:

      Total Time, Driver Time, Launch Counter, Avg Motor Temp,
      Auton Selected, Drivetrain Avg, Inertial Position, Lat Error,
      Rot Error, Lat Derivative, Rot Derivative, Prev Error,
      Turn Prev Error, Lat Mtr Pwr, Rot Mtr Pwr, Left Front Eff, 
      Left Mid Eff, Left Back Eff, Right Front Eff, Right Mid Eff, 
      Right Back Eff, Cata Eff, Intake Eff
    */
    const char* header = "Total Time , Driver Time , Launch Counter , Avg Mtr Temp , Auton Selected , Drivetrain Avg , Inertial Position , Lat Error , Rot Error , Lat Deriv , Rot Deriv , Prev Error , Turn Prev Error , Lat Mtr Pwr , Rot Mtr Pwr , Left Front Eff, Left Mid Eff, Left Back Eff , Right Front Eff , Right Mid Eff , Right Back Eff , Cata Eff, Intake Eff";
    
    // Append the string to the buffer 
    std::sprintf(buffer,"%s\n", header);

    // Write the buffer to the file
    Brain.SDcard.appendfile(fileNew.c_str(), (uint8_t*)buffer , sizeof(buffer));

    // delete the header to use more RAM space 
    delete header; 
  }

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}