#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <vector>
#include <iostream>

#include <SFML\Graphics.hpp>

//Flags for keeping track of turtle actions in script
enum functionAction
{
	NONE, //No action(default)
	MOVE_FORWARD, //Turtle moves in a straight line
	ROTATE //Turtle turns around
};

//Used to store the action corresponding to individual variables
struct function
{
	
	char boundVariable; //Which letter in the script that preforms this action
	functionAction action; //What action should be preformed on reaching boundVariable
	int argument; //The magnitude which action should be done

	//Constructor
	function(char var, functionAction execution = NONE, int parameter = 0);
};

//Used to control the iteration of the script corresponding to a certain variable
struct rule
{	
	char boundVariable; //Which letter in the script that is replaced on iteration
	std::string replaceString; //What the letter is replaced with

	//Constructor
	rule(char var, std::string action);
};

//Used to store functions of a script, preform operations related to iteration, and drawing of itself on the screen
class script
{
	std::vector<char> variables; //Vector of known variables
	std::vector<rule> rules; //Vector of known rules
	std::vector<function> functions; //Vector of known functions
	std::string seed; //The seed string which is used for iteration 0

	std::string rawStorage; //Representation of the script iteration as a string
	sf::VertexArray vArray; //Representation of the script iteration as a set of vertexes

	sf::Vector2f turtlePos; //Current position of the turtle
	float turtleDir; //Direction which the turtle will move

	//Retrieves variables from raw script
	void getVariables(std::string in, unsigned startPos = 0);

	//Retrieves functions from raw script
	void getFunctions(std::string in, unsigned startPos = 0);

	//Retrieves rules from raw script
	void getRules(std::string in, unsigned startPos = 0);

	//Retrieves the seed string from raw script
	void findSeed(std::string in);

	//Get the function corresponding to a variable
	function getFunctionOfVariable(char var);

	//Convert degree to radian
	float degToRad(float deg);

public:

	//Constructor
	script();

	//Parses a raw string into this script
	void parseStringScript(std::string in);

	//Iterate the script one time
	void step();

	//Draw the script to a display
	void draw(sf::RenderWindow *disp);

	//Print the formatted information contained in the script to the console
	void printContent();

	//Gets a copy of the raw script iteration
	std::string getStorage();
};

#endif