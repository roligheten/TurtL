#include "script.h"

void script::parseStringScript(std::string in)
{
	//FIX: Do we really need 4 functions when they all run at the same time anyway?
	getVariables(in);
	getFunctions(in);
	getRules(in);
	findSeed(in);

	rawStorage = seed;
}

script::script()
{
	//Set initial conditions for the turtle
	turtlePos = sf::Vector2f(0.f, 0.f);
	turtleDir = 0;
	vArray.setPrimitiveType(sf::LinesStrip);
	vArray.resize(0);
}

float script::degToRad(float deg)
{
	return deg*0.0174532925;
}

void script::getVariables(std::string in, unsigned startPos)
{
	//Find first instance of "var" in the string
	startPos = in.find("var", startPos);
	if(startPos != std::string::npos)
	{
		//If not at end of string, we have found a variable, find the end of the it by finding the next ";"
		unsigned nameEnd = in.find_first_of(";", startPos); //FIX: Do we need to find ; since it is only one character?
		//Store the variables identity in name
		char name = in.at(nameEnd-1);
		//Put the variable in the vector for variables in the script class
		variables.push_back(name);
		//Function runs itself from the point after the found variable
		getVariables(in, nameEnd);
	}
	//If we are here we have found all the variables in the script and should exit
}

void script::getRules(std::string in, unsigned startPos)
{
	//Find first instance of "rule" in the string
	startPos = in.find("rule", startPos);
	if(startPos != std::string::npos)
	{
		//If not at end of string, we have found a rule, get corresponding variable by the fact that it is always 4 characters away from "rule"
		char varName = in.at(startPos+4);
		//Find the start of the replace string by finding the first "("
		unsigned replaceStart = in.find_first_of("(", startPos);
		//Find the end of the replace string by finding the first ")"
		unsigned replaceEnd = in.find_first_of(")", startPos);
		//Find the replace string by clamping from replaceStart to replaceEnd
		std::string replaceString = in.substr(replaceStart+1, replaceEnd-replaceStart-1);
		//Put the rule in the vector for rules in the script class
		rules.push_back(rule(varName, replaceString));
		//Function runs itself from the point after the found rule
		getRules(in, replaceEnd+1);
	}
	//If we are here we have found all the rules in the script and should exit
}

void script::getFunctions(std::string in, unsigned startPos)
{
	//Find first instance of "func" in the string
	startPos = in.find("func", startPos);
	if(startPos != std::string::npos)
	{
		//If not at end of string, we have found a function, get corresponding variable by the fact that it is always 4 characters away from "func"
		char varName = in.at(startPos+4);
		//Find the start of the function string by finding the first ":"
		unsigned funcStart = in.find_first_of(":", startPos);
		//Find the start of the function string by finding the first ";"
		unsigned funcEnd = in.find_first_of(";", startPos);

		//Find the start of the argument by finding the firt "("
		unsigned argStart = in.find_first_of("(", startPos);
		//Find the end of the argument by finding the firt ")"
		unsigned argEnd = in.find_first_of(")", startPos);

		//Get the string which tells which turtle action this function will preform
		std::string funcName = in.substr(funcStart+1, argStart-funcStart-1);

		//Get the magnitude which to preform the action
		std::string argRaw = in.substr(argStart+1, argEnd-argStart-1);

		//NONE is default for if we cannot find a corresponding action to the string
		functionAction act = NONE;
		if(funcName == "penDraw")
			act = MOVE_FORWARD; //"penDraw" corresponds to the turtle moving forward
		if(funcName == "penRotate")
			act = ROTATE; //"penRotate" corresponds to the turtle turning

		//Convert argument from string to integer
		int arg = atoi(argRaw.data()); //FIX: Allow for floats to be arguments aswell

		//Put the function in the vector for functions in the script class
		functions.push_back(function(varName,act,arg));
		//Function runs itself from the point after the found function
		getFunctions(in, funcEnd);
	}
	//If we are here we have found all the functions in the script and should exit
}

void script::findSeed(std::string in)
{
	//Find first instance of "seed" in the string
	unsigned startPos = in.find("seed");
	if(startPos != std::string::npos)
	{
		//If not at end of string, we have found the seed, now get the first instance of "(" which is the start of the seed string
		unsigned seedStart = in.find_first_of("(", startPos);
		//Find the first instance of ")" which is the end of the seed string
		unsigned seedEnd = in.find_first_of(")", startPos);

		//Get the actual seed string
		seed = in.substr(seedStart+1, seedEnd-seedStart-1);
	}
}

function::function(char var, functionAction execution, int parameter)
{
	boundVariable = var;
	action = execution;
	argument = parameter;
}

rule::rule(char var, std::string action)
{
	boundVariable = var;
	replaceString = action;
}

void script::printContent()
{
	std::cout << "Variables:" << std::endl;
	for(signed int i = 0; i < variables.size(); i++)
	{
		std::cout << "\t" << variables[i] << std::endl;
	}

	std::cout << "Functions:" << std::endl;
	for(signed int i = 0; i < functions.size(); i++)
	{
		std::cout << "\tOn " << functions[i].boundVariable << " do " << functions[i].action << " with parameter " << functions[i].argument << std::endl;
	}

	std::cout << "Rules:" << std::endl;
	for(signed int i = 0; i < rules.size(); i++)
	{
		std::cout << "\tOn var " << rules[i].boundVariable << " iterate with " << rules[i].replaceString << std::endl;
	}
	std::cout << "Seed is:" << seed << std::endl;
}

void script::step()
{
	std::string newStorage; //This is the representation of the next iteration as a string
	//For each variable in the previous string represention
	for(int i = 0; i < rawStorage.size(); i++)
	{
		bool wasReplaced = false; //Used to see if we did change this part of the string in any way
		//For each known rule
		for(int j = 0; j < rules.size(); j++)
		{
			//See if the current variable in the previous string represention has a corresponding rule
			if(rawStorage.at(i) == rules[j].boundVariable)
			{
				//If yes, append the new string representation with the replace string for that rule
				wasReplaced = true;
				newStorage += rules[j].replaceString;
				//Do not check more rules as only one rule can correspond to one variable
				break;
			}
		}
		if(!wasReplaced)
			//If we didn't change anything, append the new string represention with the old variable
			newStorage += rawStorage.at(i);
	}

	//We now have the current iteration in newStorage, put that into rawStorage as it is the current iteration
	rawStorage = newStorage;

	//Use the turtle to make a set of vertexes representing the current iteration

	//Reset the turtle and clear the vertex vector
	turtlePos = sf::Vector2f(0.f, 0.f);
	turtleDir = 0.f;
	vArray.clear();
	int cVertex = 1; //Current vertex
	vArray.append(sf::Vertex(turtlePos)); //Append the vector with origo

	//For each variable in the string representation
	for(int i = 0; i < rawStorage.size(); i++)
	{
		//Match the variable to an action and preform it
		function match = getFunctionOfVariable(rawStorage.at(i));
		if(match.action == MOVE_FORWARD)
		{
			//If moving forward, find the next turtle position by move amount and direction
			cVertex++;
			turtlePos.x = turtlePos.x + cos(turtleDir)*match.argument;
			turtlePos.y = turtlePos.y - sin(turtleDir)*match.argument;
			//Append this position to the vertex vector
			vArray.append(sf::Vertex(turtlePos));
			
		}
		if(match.action == ROTATE)
			//If turning, turn(duh)
			turtleDir += degToRad(match.argument); //FIX: Speed up by converting to radian in the script parsing
	}
}

std::string script::getStorage()
{
	return rawStorage;
}

function script::getFunctionOfVariable(char var)
{
	//For each function
	for(int i = 0; i < functions.size(); i++)
	{
		//See if function has a corresponding variable equal to var
		if(var == functions[i].boundVariable)
			//Return this function if it does
			return functions[i];
	}
	//We have failed to find a corresponding function, return an empty one instead
	return function(' ');
}

void script::draw(sf::RenderWindow *disp)
{
	//Draw the vertex vector to the display
	disp->draw(vArray);
}