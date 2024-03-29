#pragma once
#include <string>
#include<vector>
using namespace std;

const string BLANK_FIELD = "Unknown";

//abstract class for objects that are saved in primary maps (could be printed for "catalogs")
class CatalogEntry
{
public: 
	CatalogEntry() {};
	///Pure virtual functions require child class to implement
	//return a string that describes the object in a way that is readable to a user.
	virtual string display() = 0;
	//return a string that can be saved to file. use the const delimiter charactrrs that are in FileManagement.h
	virtual string toFile() = 0;

	//takes a vector iterator from the file, function moves the iterator and assigns values to fields of the class. 
	//The iterator is created when the map is read from the file, the key is read from the first value, then the iterator is moved to the start of the class fields and this function is called.
	virtual void fromFile(std::vector<string>::iterator iter)=0;

	//return the key string from the class
	virtual string getKey() = 0;
};

