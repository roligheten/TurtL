This program parses a custom script file(.txt) and uses it to iterate and draw an L-System onto the screen.

--Setting up--
This program was made using the SFML(v 2.0) library to draw onto the screen, this repo does not include 
these files but they can be downloaded free from http://www.sfml-dev.org/download.php

--Files--
main.cpp
Has the program entrypoint which takes care of console IO, managing input, and initializing and updating the display.
script.h & script.cpp
Contains the script class which takes care of parsing and storing script data, aswell as drawing that data onto the screen.
fileIO.h & fileIP.cpp
Contains functions used to get and simplify raw script data from windows.

--Scripting language--
The scripting language used is very simple, there are four keywords used to define the working if the script: 
var, func, rule and seed.

"var" defines a variable in the script, these variables can be bound to functions and rules, and are the base 
of the script, variables are defined with the syntax "var [variable character];"

"func" defines a function in the script, when drawing the current iterations of the script, the program will run
trough each variable and preform the function bound to that variable, based on the action and the argument 
bound to it. The current allowed function actions are:
	"penDraw" which draws a line from the previous position to a new position by moving [argument]
	pixels in the direction defined by the current facing(rotation) of the turtle.
	"penRotate" which rotates the turtle [argument] degrees.
The syntax for a function is "func [bound variable character]:[function action]([argument]);"

"rule" are used to iterate the script, when iterating, each variable will be replaced with a replace string if a rule has a
bound variable that is equal to that variable. The syntax of a rule is "rule [bound variable character]:([replace string]);"

"seed" is the seed string which is the first iteration of the script. Only one seed should be defined in a script and it's syntax
should be "seed ([seed string])"

See script.txt for an example of a valid script.

--Controls--
Arrow keys move the display around.
The S key iterates the script one time.
Pgup and Pgdn zooms in and out.