#include "Drive_PD.h"
#include "vex.h"

using namespace vex;


/**
 * The Following is about PD loops. The form of the loop was developed by
 * Conner from Team 1814D: His tutorial video link https://youtu.be/_Itn-0d340g
 * 
 *  PD or Proportional Derivative Controller is a type of PID Controller that allows
 *  the robot to mathematically make accurate turns and consistant lateral movements
 * 
 *  * the variables that start with the lower-case t stand  
 *  * for turn and are meant for the turning equations
*/


// Tune settings - variables that control how sensitive the controller is 
// 0.5 is a place holder
double kP = 0.5;
double kD = 0.5;
double TkP = 0.5;
double TkD = 0.5;

/**
 * error/ tError             the difference between where the bot is and the end set point is  
 *  
 * prevError/ tPrevError     the past error (20 milli-secs) used for the derivative calculation
 * 
 * derivative/ tDerivative   the derivative manages the speed by using the rate of change
 *                           from both previous and current errors 
 * 
 * targetDist/ targetTurn    the set or end distance of where the ends
*/

int error = 0;
int prevError = 0;
int derivative = 0; 
double targetDist = 0.0;

int tError; 
int tPrevError = 0; 
int tDerivative = 0;
double targetTurn;

// Boolean variables - True or false variables

bool resetSens = false; // Used to rest the tracking wheel position to 0 when turned true
bool pdON = true; // Used to make sure the robot is not correcting durring driver period


int drivePD() {
  while (pdON) {
    if (resetSens) {
      resetSens = false;
      // Reset track positions
      rightTrack.setPosition(0, degrees); 
      leftTrack.setPosition(0, degrees); 
    }

    // Gets the position of where the bot is in degrees
    int leftPosition = leftTrack.position(degrees);
    int rightPosition = rightTrack.position(degrees);
    
    // Calculates the average of the tracking wheel positions
    int avg =  (leftPosition + rightPosition)/2;

    // Calculates the error and the derivative
    error = targetDist - avg; 
    derivative = error - prevError;

    // Lateral Motor Power
    double latMotorPower = error * kP + derivative * kD;

    //////////////////////////////////////////////////////////////////
    
    // Calculates the error and the derivative for turning 
    tError = targetTurn - Inertial.rotation(degrees);
    tDerivative = tError - tPrevError;

    // Rotational Motor Power
    double turnMotorPower = tError * TkP + tDerivative * TkD;

    
    // Set motor speed in voltage
    leftBack.spin(forward, latMotorPower - turnMotorPower, volt);
    leftMid.spin(forward, latMotorPower - turnMotorPower, volt);
    leftFront.spin(forward, latMotorPower - turnMotorPower, volt);
    rightBack.spin(forward, latMotorPower + turnMotorPower, volt);
    rightMid.spin(forward, latMotorPower + turnMotorPower, volt);
    rightFront.spin(forward, latMotorPower + turnMotorPower, volt);
    
    // Update previous errors
    prevError = error; 
    tPrevError = tError;
    vex::task::sleep(20);
  }
    
  return 1; 
}
