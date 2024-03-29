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

// Data collection
short totalElapsedTime;

// Counts the number of time the limit switch has been pressed
void launchCount(void){
  timesLaunched++; // Adds one to the number of times the button has been pressed
  
  // Update number on controller:
  Controller1.Screen.clearLine(3); // Clear line
  Controller1.Screen.print("Count %d", timesLaunched); // change number
}


void checkLaunch(void){
  if (cataDist.objectDistance(inches) <= 5) {
    Controller1.rumble("....");
    Cata.spinFor(fwd, 360, deg, 100, velocityUnits::pct, false);
  }
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
  /*
    Checks the value of runAuton and displays messages on the screen that 
    shows what the number correlates to. Additionally, changing the color 
    of the text for better indication
  */

  if (runAuton == 0){ 
    Brain.Screen.setPenColor("#E60026"); // sets the color of the text to red
    Brain.Screen.printAt(6, 20, "!!! NO Auton selected !!!"); // 0 = No auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

  } else if (runAuton == 1){
    Brain.Screen.setPenColor("#39FF14"); // sets the color of the text to green
    Brain.Screen.printAt(6, 20, "Auton %d has been selected", runAuton); // 1 = 1st auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

  } else if (runAuton == 2){
    Brain.Screen.setPenColor("#C5E90B"); // sets the color of the text to a yellow-green
    Brain.Screen.printAt(6, 20, "Auton %d has been selected", runAuton); // 2 = 2nd Auton
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white 

  } else if (runAuton == 3){
    Brain.Screen.setPenColor("#00B3CA"); // sets the color of the text to blue
    Brain.Screen.printAt(6, 20, "Skills Auton has been selected (%d)", runAuton); // 3 = Skills Auto
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

  }else { // if there's an error then it prints the number it gets too
    Brain.Screen.setPenColor("#E60026"); // sets the color of the text to red 
    Brain.Screen.print("ERROR, NUMBER AT: %d", runAuton); // print the error number
    Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

  }

}






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
  topInertial.calibrate();
  // While it is calibrating it indicates it on the controller screen
  while (Inertial.isCalibrating() && topInertial.isCalibrating()){
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
const float kP = 0.06f; // error gain  
const float kD = 0.2f; // derivative gain

int error = 0; // the difference from where the goal is to where you are 
int derivative = 0; // the difference from current error and prevError
int prevError = 0; // the last error
short targetDist = 0; // Goal distance (Lateral Movement)


//* Rotational Variables
// Gain variables - Deals with controller sensitivity (for turning)
const float Turn_kP = 0.325f; // turn error gain
const float Turn_kD = 1.0f; // turn derivative gain 

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

    Brain.Screen.clearScreen();

    Brain.Screen.printAt(6, 60, "Error %d", error);
    Brain.Screen.printAt(6, 80, "Avg Position %d", avg);

    derivative = error - prevError; // derivative

    Brain.Screen.printAt(6, 100, "Prev Error %d", prevError);

    // Lateral(Lat) Motor(Mtr) Power(Pwr) equation
    double LatMtrPwr = (error * kP)+(derivative * kD); 

    //----------------------------- Rotation -----------------------------//

    // Makes the Inertial sensor a variable with decimal places (readability)
    double InertPos = (Inertial.heading(degrees) + topInertial.heading(degrees))/2;
    
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
    leftFront.spin(fwd, LatMtrPwr + RotMtrPwr, volt);
    leftMid.spin(fwd, LatMtrPwr + RotMtrPwr, volt);
    leftBack.spin(fwd, LatMtrPwr + RotMtrPwr, volt);
    rightBack.spin(fwd, LatMtrPwr - RotMtrPwr, volt);
    rightMid.spin(fwd, LatMtrPwr - RotMtrPwr, volt);
    rightFront.spin(fwd, LatMtrPwr - RotMtrPwr, volt);

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




// An integer function that allows for the distance sensors
// to show how far away we are from a predetermined starting
// position
bool positionOn = true;
int position(void){
  
  // Desired position values
  uint8_t xPos = 19; // inches away horizontally
  uint8_t yPos = 7; // 7 inches away vertically
  uint16_t inertDeg = 350;

  // Use the distance sensors, sideX and sideY, to get the distance to the  
  // perimeters in inches and store them in inputX and inputY variables
  uint8_t inputX = rightSide.objectDistance(inches); 
  uint8_t inputY = rear.objectDistance(inches);
  short inert = Inertial.heading(deg);

  // Definitions of the integer variables diffX and diffY
  // These will hold the difference of a position and the input
  // side, respectively
  short diffX, diffY;

  // While the boolean positionOn is true, 
  // it will loop the code in the body
  while (positionOn){
    

    // for our auton 1 and skills we face a different direction
    // if runAuton does not equal 2 then xPos, yPos, and inertDeg
    // equal different values
    if(runAuton != 2){
      yPos = 6; // 6 inches for yPos
      inertDeg = 31; // 31 degrees to the right
      xPos = 13; // 13 inches for xPos
    } else { 
      // if else, then xPos, yPos, and inertDeg are back to their default
      xPos = 19;
      yPos = 7;
      inertDeg = 350;
    }

    // If the value is not than 0, then it calculates the 
    // difference for that value, otherwise it does not
    if (xPos != 0) {
      Brain.Screen.clearLine(2);
      // Calculate and indicate how far way the position is
      diffX = xPos - inputX;
      Brain.Screen.printAt(6, 40, "Move bot %d horizontally", diffX);
    } 

    // If the value is not than 0, then it calculates the 
    // difference for that value, otherwise it does not
    if (yPos != 0){
      Brain.Screen.clearLine(3);

      // Calculate and indicate how far way the position is
      diffY = yPos - inputY; 
      Brain.Screen.printAt(6, 60, "Move bot %d vertically", diffY);
    }

    if (inertDeg != 0){
      Brain.Screen.clearLine(8);
      Brain.Screen.printAt(0, 160, "Inertial Position %d", inert - inertDeg);
    }
      
    // Update sensor variables
    inputX = rightSide.objectDistance(inches); 
    inputY = rear.objectDistance(inches);
    inert = Inertial.heading(deg);

    task::sleep(50); // save CPU resources
  }
  return 1;
}






void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  
  // Pre-Match motor temps
  
  // Left side
  Brain.Screen.printAt(6, 100, "LF Motor Temp %f", leftFront.temperature(pct));
  Brain.Screen.printAt(6, 120, "LM Motor Temp %f", leftMid.temperature(pct));
  Brain.Screen.printAt(6, 140, "LB Motor Temp %f", leftBack.temperature(pct));
  // right side
  Brain.Screen.printAt(6, 180, "RB Motor Temp %f", rightBack.temperature(pct));
  Brain.Screen.printAt(6, 200, "RM Motor Temp %f", rightMid.temperature(pct));
  Brain.Screen.printAt(6, 220, "RF Motor Temp %f", rightFront.temperature(pct));
  // catapult
  Brain.Screen.printAt(6, 240, "Cata Motor Temp %f", Cata.temperature(pct));
  
  // resets the position to 0
  rightTrack.resetPosition(); 
  leftTrack.resetPosition();;
  Cata.resetPosition();

  Brain.Screen.clearLine(1);
  
  Brain.Screen.setPenColor("#E60026"); // sets the color of the text to red
  Brain.Screen.printAt(6, 20, "!!! NO Auton selected !!!"); // 0 = No auton
  Brain.Screen.setPenColor("#FFFFFF"); // resets the color of the text to white

  // Create a task to assist positioning the robot 
  task checkPlace(position);
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

  task::stop(position);
  Brain.Screen.clearScreen(); // Clears screen on the brain for diagnosis
  
  Controller1.Screen.clearLine(3); // Clears the controller screen for the catapult counter

  task drivePD(PD_Control); // starts the PD controller as a task
  resetSens = true;

  // Depending on the number that runAuton holds that is the code ran
  if (runAuton == 1){ // if 1 then (for now) will run the autonomous skills code    
    targetDist = 2125; // inital push into the side, then move back
    wait(0.4, sec);
    targetTurn = 0;
    wait(2, sec);
    targetTurn = 31;
    wait(0.4, sec);
    targetDist = 0;
    wait(0.1, sec);
    pistonVertical.set(true);

  } else if (runAuton == 2){
    // initial jolt
    Intake.spin(fwd, 100, pct);
    targetDist = 250;
    wait(0.25, sec);
    targetDist = 100;
    wait(0.5, sec);
     
    // drive and score
    targetDist = 2650;
    wait(1, sec);
    targetTurn = 89;
    wait(0.75, sec);
    Intake.spin(reverse, 100, pct);
    resetSens = true;
    targetDist = 1100;
    wait(1, sec);
    
    // go back and push
    targetDist = -1200;
    wait(1, sec);
    pistonHorizontal.set(true);
    targetDist = 1100;
    wait(0.75, sec);
    
    // obtain 3rd ball
    pistonHorizontal.set(false);
    resetSens = true;
    targetDist = -650;
    wait(0.75, sec);
    targetTurn = 240;
    Intake.spin(fwd, 100, pct);

    
    // score 3rd ball
    wait(0.75, sec);
    resetSens = true;
    targetDist = 1300;
    wait(0.75, sec);
    targetDist = 500;
    wait(0.5, sec);
    targetTurn = 90;
    wait(1, sec);
    targetDist = 1300;
    Intake.spin(reverse, 100, pct);
    wait(0.75, sec);
    targetDist = 750;
    
    // turn and get 4th
    wait(0.5, sec);
    targetTurn = -65;
    Intake.spin(fwd, 100, pct);
    wait(1, sec);
    targetDist = 2000;
    wait(0.75, sec);

    // come back and turn
    resetSens = true;
    targetDist = -650;
    wait(0.75, sec);
    targetTurn = 90;
    wait(0.75, sec);
    pistonHorizontal.set(true);
    resetSens = true;
    targetDist = 2100;
    wait(1, sec);
    pistonHorizontal.set(false);
    Intake.stop(coast);

  } else if(runAuton == 3){ // if 3 then Skills Auton will run for autonomous
    targetDist = 2125; // inital push into the side, then move back
    wait(0.4, sec);
    targetTurn = 0;
    wait(1, sec);
    targetDist = 1000;
    wait(1, sec);
    targetTurn = -70;
    wait(0.60, sec);
    pistonVertical.set(true);
    wait(0.15, sec);
    
    // catapult
    Cata.spin(fwd, 54, pct); // spin the catapult motor
    waitUntil(timesLaunched == 50); // wait for the catapult to spin 50 times
    Cata.stop(coast); // stop catapult motor
    pistonVertical.set(false);

    // move across field
    resetSens = true;
    targetDist = -100;
    targetTurn = -120;
    wait(0.5, sec);
    targetDist = 1350;
    wait(0.6,sec);
    targetTurn = -100;
    wait(0.15, sec);
    targetDist = 1100;
    wait(0.15, sec);
    targetTurn = -85;
    wait(0.05, sec);
    targetDist = 5000;
    wait(1.25, sec);
    targetTurn = -50;
    wait(0.5, sec);

    // Score
    resetSens = true;
    targetDist = 2000; 
    wait(2, sec);
    targetTurn = 0;
    wait(1, sec);
    resetSens = true;
    targetDist = 1000;
    wait(2, sec); 
    pistonHorizontal.set(true);
    targetDist = -150;
    wait(0.75, sec);
    targetDist = 1000;
    wait(1.08, sec);
    pistonHorizontal.set(false);
    wait(1.07, sec);
    targetDist = 850;
    wait(0.25, sec);
    targetTurn = -90;
    wait(1, sec); 
    targetDist = -1300;
    wait(0.75, sec);
    targetTurn = -180;
    pistonVertical.set(true);
    wait(1.5, sec);
    resetSens = true;
    targetDist = -500; // -750
    wait(1, sec);
    resetSens = true;
    targetTurn = -230;
    wait(1, sec);
    resetSens = true;
    targetDist = -1800;
    wait(0.35, sec);
    targetTurn = -270;
    wait(1, sec); 
    targetDist = -850;
    wait(2, sec);
    pistonVertical.set(false);
    targetTurn = -1; // 0
    resetSens = true;
    wait(2, sec);
    targetDist = 1000; //1300
    wait(1, sec);
    pistonVertical.set(true);
    targetTurn = -290;
    wait(1, sec);
    targetDist = -1800;
    wait(1, sec);
    targetDist = -1000;
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
  task::stop(position);
  task::stop(PD_Control);

  controlON = false; // Turns off the PD controller
  Brain.Screen.clearScreen(); // Clears the brain screen for driver control prints
  
  // Variables for Driver Layout Toggle
  bool shiftEnable = false; // two-choice
  bool shiftButtonPressed = false; // Place holder 
  short forward = Controller1.Axis3.value(); // Gets the value of Axis 3
  short turn = Controller1.Axis1.value(); // Gets the value of Axis 1
  
  // Motor voltage on a range of -12 to 12
  double leftVolt; // Declare left side voltage 
  double rightVolt; // Declare right side voltage

  // Booleans for Cata toggle
  bool toggleEnabledCata = false;
  bool buttonPressedCata = false;

  // Booleans for Wing toggle 
  bool toggleEnabledVerticalWings = false; // two-choice toggle, so we use bool
  bool buttonPressedVerticalWings = false; // logic variable
  bool toggleEnabledHorizontalWings = false; // two-choice toggle, so we use bool
  bool buttonPressedHorizontalWings = false; // logic variable

  // Booleans for Hang toggle
  bool toggleEnabledSide = false; // two-choice toggle, so we use bool
  bool buttonPressedSide = false; // logic variable
  bool toggleEnabledHang = false; // two-choice toggle, so we use bool
  bool buttonPressedHang = false; // logic variable
  
  // Data variables
  const short previousTime = totalElapsedTime; // Auton time in seconds for offset
  short driverElapsedTime = 0; // elapsed Driver time (in Seconds)

  // Logic booleans to negate repetition when writing data
  bool onCooldown = false; 
  bool ran = false; 

  uint8_t speed = 50;
  if (runAuton == 3) speed = 54;

  while (1) {
    
    // Motor Data

    // Check if the SD card is inserted and a file is open
    if(cardInserted && createdNameExists){

      // Calculate the driver time from the brain's timer minus the auton time in seconds
      // for an offset
      driverElapsedTime = Brain.Timer.value() - previousTime; 

      // If time is a multiple of 15, then set the cooldown to true
      if (driverElapsedTime % 15 == 0){
        onCooldown = true;
      } else { // if its not then set ran and cooldown to false
        ran = false;
        onCooldown = false;
      }

      // if the inverse of ran and cooldown are true, call the data function
      if (!ran && onCooldown) {
        addData(driverElapsedTime, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 

        // prints the elapsed time and onCooldown bool for debugging
        std::cout << "Added Data at " << driverElapsedTime << "seconds"<< std::endl; 
        ran = true;
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

    // Driver Analog Stick Layout Logic
    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonR2 = Controller1.ButtonR2.pressing();

    // Toggle Logic
    if (buttonR2 && !shiftButtonPressed){
      shiftButtonPressed = true; 
      shiftEnable = !shiftEnable;
    }
    else if (!buttonR2) shiftButtonPressed = false;

    // Code For toggle Enabled or Disabled
    if(shiftEnable){
      // These lines set the controllers analog sticks to the left  
      // motors nad right motors using the tank drive layout

      //This is Left Side AXIS 2
      leftFront.spin(fwd, Controller1.Axis3.position(pct), pct);
      leftMid.spin(fwd, Controller1.Axis3.position(pct), pct);
      leftBack.spin(fwd, Controller1.Axis3.position(pct), pct);
      //This is Right Side AXIS 3
      rightBack.spin(fwd, Controller1.Axis2.position(pct), pct);
      rightMid.spin(fwd, Controller1.Axis2.position(pct), pct);
      rightFront.spin(fwd, Controller1.Axis2.position(pct), pct);

    } else {

      // Update variables
      forward = Controller1.Axis3.value();
      turn = Controller1.Axis1.value();

      // Calculate voltage
      leftVolt = 12 * ((forward + turn) / 100.0);
      rightVolt = 12 * ((forward - turn) / 100.0);

      // Apply leftVolt to the left motors
      leftFront.spin(fwd, leftVolt, volt);
      leftMid.spin(fwd, leftVolt, volt);
      leftBack.spin(fwd, leftVolt, volt);
      // Apply rightVolt to the right motors
      rightBack.spin(fwd, rightVolt, volt);
      rightMid.spin(fwd, rightVolt, volt);
      rightFront.spin(fwd, rightVolt, volt);
    }

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
      Cata.spin(fwd, speed, pct); // Spins Cata on a toggle if it needs to be on for longer
    } 
    // Manual Input
    // when L2 is being pressed spin forward
    else if(Controller1.ButtonL2.pressing()) {
      Cata.spin(fwd, speed, pct); 
    } else{
      Cata.stop(coast); // else stop
    } 
    
    // Wings/ blockers Logic
    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonX = Controller1.ButtonX.pressing();

    // Toggle Logic
    if (buttonX && !buttonPressedVerticalWings){
      buttonPressedVerticalWings = true; 
      toggleEnabledVerticalWings = !toggleEnabledVerticalWings;
    } else if (!buttonX) buttonPressedVerticalWings = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledVerticalWings){
      pistonVertical.set(true); // open wings

    } else pistonVertical.set(false); // close wings



    // Wings/ blockers Logic
    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonY = Controller1.ButtonY.pressing();

    // Toggle Logic
    if (buttonY && !buttonPressedHorizontalWings){
      buttonPressedHorizontalWings = true; 
      toggleEnabledHorizontalWings = !toggleEnabledHorizontalWings;
    }
    else if (!buttonY) buttonPressedHorizontalWings = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledHorizontalWings){
      pistonHorizontal.set(true); // open wings

    } else pistonHorizontal.set(false); // close wings



    // Hang Piston Logic
    // boolean to get if the button is pressed (true) or it isn't pressed (false)
    bool buttonDown = Controller1.ButtonDown.pressing();

    // Toggle Logic
    if (buttonDown && !buttonPressedHang){
      buttonPressedHang = true; 
      toggleEnabledHang = !toggleEnabledHang;
    }
    else if (!buttonDown) buttonPressedHang = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledHang){
      hangMiddle.set(true); // close hang

    } else hangMiddle.set(false); // open hang



    bool buttonUp = Controller1.ButtonUp.pressing();

    // Toggle Logic
    if (buttonUp && !buttonPressedSide){
      buttonPressedSide = true; 
      toggleEnabledSide = !toggleEnabledSide;
    }
    else if (!buttonUp) buttonPressedSide = false;

    // Code For toggle Enabled or Disabled
    if(toggleEnabledSide){
      hangSide.set(true); // retract side hang piston

    } else hangSide.set(false); // extend side hang piston 

    // intake logic
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

// Unsigned int array to store data (2500 bytes/ 2.5 Kilobytes)
uint8_t myData[ 2500 ]; 

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  Competition.bStopAllTasksBetweenModes = true;

  // Checks if the SD card is inserted into the Brain
  if(cardInserted){

    // when the function returns the file name it is assigned to the variable fileNew
    std::string fileNew = fileMake();
    
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
  }
  
  // calls to calibrate the inertial when the Brain is on field control
  if(Competition.isFieldControl() || Competition.isCompetitionSwitch()) calibrateInertial(); 

  // Run the pre-autonomous function.
  pre_auton();

  Auton1.pressed(AutoSwitch); // When bumper is pressed it cycles through different autonomous
  
  // When the limit switch is pressed it counts how many times it has gone off
  cataDist.changed(checkLaunch);
  cataLimit.pressed(launchCount); 

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}