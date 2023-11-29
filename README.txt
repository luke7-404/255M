255M's project for Over Under

255M Coders:
Lucas (4th year in Vex)
Alex (3rd year in Vex)

This is 255M first ever GitHub hosted folder

The contents that are crustal for the Over Under Robot are:

robot-config.cpp
robot-config.h
---
main.cpp
vex.h

The files ending in ".h" are header files which include libraries and declarations for variables 
and functions.

The files ending in ".cpp" are the source files that use/ modify the declared variables

The main reason to use GitHub was so that the Folder and its contents would be backed up in the cloud
and accessible online. Because it is accessible from anywhere anyone of our teammates could look at, 
change, and/or download the code. 

The added benefit to GitHub is that there is version tracking so if something went wrong in 
the current version you can look back at previous ones. There is also blame tracking so if Alex, or I 
wrote bad code our team could look at the blame page and see who committed what version. 

As of 11/28 - 11/29 2023 the changes are:

PD Controller Fix - the .rotation(degrees) method for the inertial sensor did NOT have a capped limit
as a result there is a limit

Next steps: add a detector for left and right turns and tune the Controller

/////

Catapult limit switch additions - I have coded in the launchCount function 
(counts how many times the limit switch has been pressed) and have setup the foundation for the catapult
launch to be at the press of a button instead of holding it down.

Minor additions - Commented the PD controller, general touchups 

Plans:

I would like the program to write the PD controller data (variables and numbers) to an SD
card

Add in ultrasonic/ distance sensors for more accurate setup/lineup during auto

Add in auto selector

