//This is the abstract class Artist.h. This class can be instantiated through either the Band.h file or IndividualArtist.h files. This is the
//basic structure shared between the band or the individual artists.
//Mysti Freed
//4-NOV-2019
//mrfreed@dmacc.edu

#pragma once
#ifndef ARTIST_H
#define ARTIST_H
#include <string>
#include "CatalogEntry.h"
#include "FileManagement.h"
#include <map>
#include <sstream>

using namespace std;

class Artist : public CatalogEntry
{
private:
	//int id; holds the identifier for the artist
	string alphaName=BLANK_FIELD; //holds the artist' name alphabetically
	string displayName = BLANK_FIELD; //holds the name of the artist
	//Genre primaryGenre; //holds the primary genre for the artist
public:
	//default constructor
	Artist() : CatalogEntry() {
		
	}

	//constructor that allows for setup without a Genre, when it's not known
	Artist(string an, string dn) : CatalogEntry()
	{
		alphaName = an;
		displayName = dn; 
	}

	//constructor where there's only an alphabetical name (for example, Prince)
	//and no genre selected
	Artist(string an) : CatalogEntry()
	{
		

		alphaName = an;
		displayName = alphaName; //sets the alphaName to the displayName
	}
	//copy constructor
	Artist(const Artist& a2) {
		

		alphaName = a2.alphaName;
		displayName = a2.displayName;
		//primaryGenre = a2.primaryGenre;
	}
	//https://en.cppreference.com/w/cpp/language/copy_assignment
	Artist& operator=(const Artist& a2)
	{
		

		alphaName = a2.alphaName;
		displayName = a2.displayName;
		//primaryGenre = a2.primaryGenre;
		return *this;
	}
	//setters/mutators
	void setAlphaName(string an);		//{ alphaName = an; }
	void setDisplayName(string dn);	//{ displayName = dn; }
	//void setGenre(Genre g);	//{ primaryGenre = g; }
										//
	//accessors (getters)				//
	string getAlphaName(); /*const*/		//{ return alphaName; }
	string getDisplayName(); /*const*/	//{ return displayName; }

	virtual string display(); //from CatalogEntry.h
	virtual string toFile(); //from CatalogEntry.h
	virtual void fromFile(vector<string>::iterator iter); //from CatalogEntry.h
	virtual string getKey(); //from CatalogEntry.h
	virtual void updateKey(); //from CatalogEntry.h
};
#endif ARTIST_H


//Class Implementation

void Artist::setAlphaName(string an)	{ alphaName = an; };
void Artist::setDisplayName(string dn)	{ displayName = dn; };

string Artist::getAlphaName()			{ return alphaName; };
string Artist::getDisplayName()			{ return displayName; };
//Genre Artist::getPrimaryGenre()			{ return primaryGenre; };

void Artist::updateKey()
{
	cout << "pending functionality stub";
}

string Artist::display()
{
	return displayName;
}

string Artist::toFile()
{
	return alphaName + FIELD_DELIMITER+ displayName+ FIELD_DELIMITER; //not sure if we're using the same delimiter for all?
}

void Artist::fromFile(std::vector<string>::iterator iter)
{
	alphaName = *iter;
	displayName = *++iter;//assign next field//I think this is all we need here, but lets chat, I want to make sure
}

string Artist::getKey() {
	return alphaName;

}

map<string, Artist> artistMap;
string artistFileTXT = "Artists.txt";
fstream artistFstream;// (artistFileTXT, ios::in | ios::out);



