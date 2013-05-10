#include "fileIO.h"

using namespace std;

std::string getFromFile(char* filename)
{
	std::string returnString;

	ifstream fileStream;
	fileStream.open(filename);

	if(fileStream.good())
	{
		//If file open success, give content to returnString
		while(fileStream.good())
			returnString += fileStream.get();

		return returnString;
	}

	//If file open failure, return error-string.
	return "ERROR: File not opened!";
}

std::string prepareScriptFile(std::string in)
{
	unsigned curPos = 0;

	while(true)
	{
		//Find first occurance of space in string
		curPos = in.find_first_of(" ",curPos);

		//If end of file, stop
		if(curPos == string::npos)
			break;

		//Remove space
		in.erase(curPos,1);
	}

	//Same procedure as above, only for linebreaks
	curPos = 0;
	while(true)
	{
		curPos = in.find_first_of("\n",curPos);
		if(curPos == string::npos)
			break;


		in.erase(curPos,1);
	}

	//Return string with all spaces and linebreaks removed
	return in;

}