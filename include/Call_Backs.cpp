#include "Call_Backs.h"
#include "vex.h"

using namespace vex;

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

// TODO - Add call back functions when needed