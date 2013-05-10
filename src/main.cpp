#include <iostream>
#include <fstream>

#include "fileIO.h"
#include "script.h"
#include <SFML\Graphics.hpp>

using namespace std;

int main()
{

	//Get path to script file from user input
	cout << "Path to script:" << endl;
	char input[25]; //FIX: Allow for longer path strings
	cin >> input;
		
	//Load raw textcontent of file into stringScript
	std::string stringScript;
	stringScript = getFromFile(input);
	cout << "Parsing..." << endl;


	//mysScript will be our parsed script file from stringScript
	script myScript;
	myScript.parseStringScript(prepareScriptFile(stringScript));

	//Print a message detailing the functions of the script
	myScript.printContent();

	//Wait for user
	system("pause");

	//Set up render to draw graphics on
	sf::RenderWindow app(sf::VideoMode(800,800,32), "Artist Turtle");
	sf::View myView(sf::Vector2f(0.f,0.f), sf::Vector2f(800.f, 800.f));
	app.setView(myView);

	//Program loop
	while(true)
	{	
		sf::Event evt;

		//Get user input
		while(app.pollEvent(evt))
		{
			if(evt.type == sf::Event::KeyReleased)
			{
				if(evt.key.code == sf::Keyboard::S)
					myScript.step();
				if(evt.key.code == sf::Keyboard::Right)
					myView.move(myView.getSize().x/50, 0);
				if(evt.key.code == sf::Keyboard::Left)
					myView.move(-myView.getSize().x/50, 0);
				if(evt.key.code == sf::Keyboard::Up)
					myView.move(0, -myView.getSize().y/50);
				if(evt.key.code == sf::Keyboard::Down)
					myView.move(0, myView.getSize().y/50);
				if(evt.key.code == sf::Keyboard::PageUp)
					myView.zoom(0.5f);
				if(evt.key.code == sf::Keyboard::PageDown)
					myView.zoom(1.5f);
			}
			app.setView(myView);
		}
		
		//Update screen display
		app.clear();
		myScript.draw(&app);
		app.display();
	}
}


/*

*/