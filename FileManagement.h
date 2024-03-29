#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

// menu functions
int getInputReprompt(std::string, int, int);

//file handling variables
const char FIELD_DELIMITER = '|'; //use this to separate fields
const char ELEMENT_DELIMITER = '^'; //use this to separate fields
const char SAFE_CHAR = '_';//use this char in place of the delimiter characters if one of them is in user input

//file handling functions/
vector<string> ReadLineByID(fstream&, string);//can find any string, not just ID
vector<string> SeparateLineByDelimiter(string, char);
void GoBeginningOfFile(fstream& inputReader);
string generateDisplayName(string);
string generateSingerKey(string);

//substring function to divide up the elements into display name and key
string generateDisplayName(string comb)
{
	string fullString = comb;
	string singerKey;
	char delimiter = '/';
	string displayName;

	singerKey = fullString.substr(0, fullString.find(delimiter));
	displayName = fullString.substr(fullString.find(delimiter) + 1);
	return displayName;
}

//substring function to divide up the elements into display name and key
string generateSingerKey(string comb)
{
	string fullString = comb;
	string singerKey;
	char delimiter = '/';
	string displayName;

	singerKey = fullString.substr(0, fullString.find(delimiter));
	displayName = fullString.substr(fullString.find(delimiter) + 1);
	return singerKey;
}

//sets reader back to start of file //Amy
void GoBeginningOfFile(fstream& inputReader) {
	inputReader.clear();
	inputReader.seekg(0, ios::beg);
};

//turns a string line with delimiter into individual strings, stored in a vector. //Amy
//adapted from http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
vector<string> SeparateLineByDelimiter(string line, char delimiter) {
	vector<string> inputFields;
	std::stringstream ss(line);
	std::string nextField;
	while ( std::getline(ss, nextField, delimiter)) {
		inputFields.push_back(nextField);
	}
	return inputFields;
};

/*Amy's input validating function for integer within a range. Continues reprompting until input is acceptable. Useful for menu input.
int getInputReprompt(std::string, int, int);
int getInputReprompt(std::string promptMessage, int minRange, int maxRange) {};*/
int getInputReprompt(std::string promptMessage, int minRange, int maxRange) {
	int intInput;
	string strInput;
	bool isInvalidInput=false;
	std::string invalidMessage = "Error: Enter an integer between " + std::to_string(minRange) + " and " + std::to_string(maxRange) + ". ";

	do {
		std::cout << promptMessage;

		getline(cin, strInput);
		stringstream ssInput(strInput);
		ssInput>>intInput;	
		if (isInvalidInput = ssInput.fail()) //invalid input 
		{
			std::cout << "Input Type " << invalidMessage;
		}
		else if (intInput<minRange || intInput>maxRange) //outside range
		{
			std::cout << "Outside Range " << invalidMessage;
			isInvalidInput = true;
		}
	//	if (isInvalidInput) {//clear to reprompt (not needed with this version of function that uses getline();
					//std::cin.clear();
		//	std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//}
	} while (isInvalidInput);//reprompt until valid

	return intInput;//return valid input
};


//Mysti's function to ensure that a file opens appropriately for read/write for reuse by the team
//pass by reference (Gaddis p673)
bool openFileInOut(fstream& file, string filenameTXT)
{
	file.open(filenameTXT, ios::out | ios::in);
	if (file.fail())
	{
		return false;
	}
	else
	{
		return true;
	}
}


