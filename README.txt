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

////

Nov 29 - 30

Catapult has a counter and every time the limit switch is pressed then it will count (number is shown on 
controller)

Auton selector is up and running slight bugs will fix

Minor amount of comments have been added

PD Controller:

- has to be tuned
- Check if the controller can handle negative values (rotational especially)

Motor temps are printed throughout various stages

LEDs added for notification

////

Dec 2 - 3

I've been wanting to utilize the micro sd card slot for a while and I started that project this weekend

The outline of what I need to do is 

Create the file
Write data to it
close it 
repeat

So far what has been completed is the first and last part
When the program is ran the string function filename will run and it will name the file
a long this format: Year-Month-day_Hour-minute-second
This name format handles duplication because the position of second will change the rest of the filename
the hour outputs in a 24 hour clock (or military time) so that it distinguishes between AM and PM

The only times left are how to implement the creation, writing, and closing of the file
There are multiple posts on Vex Forum that deal with these but I am having confusion deciphering them

Other Changes:

MASSIVE amounts of comments - anything that could've been commented is

Layout - I've added spaces in between functions for separation purposes 

Minor fixes to code that will get tested to make sure they were the correct fixes or where style changes

If anything else was forgotten look at code changes

///

DEC 24-25

First commit : Wrote the file naming function
Second commit : Wrote a function that lets the person that sets up the robot know the distance from the set up spot

Third commit : Added comments to all uncommented things along with other minor changes

Future plans:

 - Add GPS capabilities 
 - Tie in everything together (distance sensors, file/ data collection, GPS, PD control)
 - Other auton layouts/ routes


