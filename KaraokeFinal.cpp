// KaraokeSongs.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "Song.h"
#include "MapManagement.h"
#include "FileManagement.h"
#include "CatalogEntry.h"
#include "QueueManagement_KJ.h"
#include "Singer.h"
#include <map>
#include "SingerHistory.h"
#include <algorithm>
#include "SongRequest.h"

using namespace std;

//global variables
static QueueManagement_KJ<SongRequest> comboList; //holds combo of everything
SongRequest newRequest;
SongRequest confirm;
//testing 
const bool DEBUGMAIN = false;
const bool DEMO = true;//pre-load some things for demo purposes
//function prototypes
void startup();
bool displayMenu();
void menuManageCatalogue();
void menuDisplayCatalogue();
void exitSaving();
void menuQueueManagement();
void menuSinger();
void addSongReq();
//inline bool operator==( Song& s1,  Song& s2) { return s1.getKey() == s2.getKey(); }

int main()
{
	startup();
	

	while (displayMenu()); //automatically repeats until a false (exit) is returned.
	exitSaving();
	return 0;
};

void startup() {
	cout << "Startup...";
	bool check;
	if (DEBUGMAIN) {
		cout << "DEBUGMAIN is true. Displaying startup messages\n";
	}
	check = openFileInOut(songFstream, songFileTXT);
	if (DEBUGMAIN) { cout << "\nopen song fstream: " << check << endl; }
	check = openFileInOut(artistFstream, artistFileTXT);
	if (DEBUGMAIN) { cout << "\nopen artist fstream: " << check << endl; }
	check = openFileInOut(singerFstream, singerTXT);
	if (DEBUGMAIN) { cout << "\nopen allSingerHistory fstream: " << check << endl; }
	check = openFileInOut(singerHistoryFstream, allSingerHistoryTXT);
	if (DEBUGMAIN) { cout << "\nopen allSingerHistory fstream: " << check << endl; }

	primaryMapFromFile(songMap, songFstream);
	//fill secondary songCatalogByArtist multimap with the contents fo the songMap
	for (auto& e : songMap) {
		Song newSong = e.second;
		addObjectToMap(&songCatalogByArtist,  newSong.getArtistKey(), newSong.getTitle());
	}
	primaryMapFromFile(artistMap, artistFstream);
	primaryMapFromFile(singerMap, singerFstream);
	multiMapFromFile(allSingerHistoryMap, singerHistoryFstream);
	cout << "Done importing map data\n";
	if (DEMO) {
		Song songA;
		Singer singerA;
		bool singercheck = SelectByKey(singerMap, "amy100", singerA);
		bool songcheck = SelectByKey(songMap, "Dreams - Fleetwood Mac", songA);
		if (singercheck && songcheck) {
			SongRequest newRequest = SongRequest(singerA, songA);
			comboList.appendNode(newRequest);
		}
		if (SelectByKey(singerMap, "mystifreed", singerA) && SelectByKey(songMap, "Alone - Heart", songA)) {

			SongRequest newRequest = SongRequest(singerA, songA);
			comboList.appendNode(newRequest);
		}

		if (SelectByKey(singerMap, "anna", singerA) && SelectByKey(songMap, "JOLENE - PARTON, DOLLY", songA)) {

			SongRequest newRequest = SongRequest(singerA, songA);
			comboList.appendNode(newRequest);
		}

		if (SelectByKey(singerMap, "ethanf", singerA) && SelectByKey(songMap, "PANAMA - VAN HALEN", songA)) {

			SongRequest newRequest = SongRequest(singerA, songA);
			comboList.appendNode(newRequest);
		}
		cout << "Loaded demo data into kj queue.\n";
	}
};

// Function displays a menu for user selection of the submenu. Returns true if menu should continue.
bool displayMenu()
{
	int userSelection; //user choice within the top-of-house menu display
	enum roleOptions {NONE, CATALOG, KJ,SINGER,EXIT};
	string prompt = "\n----Karaoke Role Selection Menu----\n ";
	prompt += "  1) Catalogue Management\n "; //this holds the menu options specific to management of the song/artist catalogues
	prompt += "  2) KJ Queue Management\n "; //this menu holds options for the KJ to manage the queue of singers
	prompt += "  3) Singer Menu\n "; //this menu holds singer options - histories, etc
	prompt += "  4) Exit program\n ";
	prompt += "  Please make a selection:\n ";

	//this is having some issues, need to take a look at the getInputReprompt function in FileManagement.h
	userSelection = getInputReprompt(prompt, CATALOG, EXIT);//getInputPreprompt converts any entry to upper for comparison

	//call the MenuMember() menu
	if (toupper(userSelection) == CATALOG)
	{
		menuManageCatalogue();

		return true;//causes main menu to continue
	}

	else if (toupper(userSelection) == KJ)
	{
		menuQueueManagement();
		//linkListTester();
		return true;//causes main menu to continue
	}
	else if (toupper(userSelection) == SINGER)
	{
		menuSinger();
		return true;//causes main menu to continue

	}
	else if (toupper(userSelection) == EXIT)
	{
		cout << "Thank you, program closing." << endl;
		return false;//causes main menu to stop
	}
	else {
		cout << "Main menu error. ";
		return true;
	}
};
//saves current contents of maps to files
void exitSaving() {
	primaryMapToFile(songMap, songFstream);
	primaryMapToFile(artistMap, artistFstream);
	primaryMapToFile(singerMap, singerFstream);
	multiMapToFile(allSingerHistoryMap, singerHistoryFstream);
}
//submenu
void menuManageCatalogue() {
	while (true)
	{
		int userSelection;
		enum catalogOptions {BACK,  ADD_SONG, ADD_ARTIST, COUNTS, VIEW};
		string prompt = "\n----Catalogue Management Menu----\n ";
		prompt += "  "+to_string(BACK)+") Back\n ";
		prompt += "  " + to_string(ADD_ARTIST) + ") Add Multiple Songs by Artist to Catalogue\n "; 
		prompt += "  " + to_string(COUNTS) + ") View Song Popularity Counts\n "; 
		prompt += "  "+to_string(VIEW) + ") View Catalogues\n "; 
		
		prompt += "  Please make a selection:\n ";
		userSelection = getInputReprompt(prompt, BACK, VIEW);//getInputPreprompt converts any entry to upper for comparison

		Artist tempArtist;
		Song tempSong;
		switch (userSelection) {
		case ADD_ARTIST:
			do 
			{
				Artist tempArtist = userInputArtist();
				if (!(tempArtist.getKey() == BLANK_FIELD))//skip if no artist entered/selected
				{
					do {
						userInputSong(tempArtist);
					} while (getInputReprompt("Add another song by this Artist?    0:No,  1:Yes   Enter a selection: \n", 0, 1));
				}
			} while (getInputReprompt("Add another Artist?    0:No,  1:Yes   Enter a selection: \n", 0, 1));
			
			break;
		case ADD_SONG:
			do{
			 userInputSong();
			} while (getInputReprompt("Add another Song?    0:No,  1:Yes   Enter a selection: \n", 0, 1));
			break;
		case VIEW:
			menuDisplayCatalogue();
			break;
		case COUNTS:
			cout << "\n----Popularity counts from all performance history----\n";
			viewSingerHistory(allSingerHistoryMap);
			break;
		default:// BACK OR ERROR GOES HERE, Returns from function
			return;
			break;
		}
	}
}
//submenu
void menuDisplayCatalogue()
{
	while (true)//always continue, use returns to exit function
	{
	//	GET USER INPUT ON VIEW METHOD 
		enum viewOption { BACK_MENU, SCREEN_DISPLAY, PRINT_REPORT};
		
		string prompt = "\n----View Catalogue Menu----\n ";
		prompt += "  "+to_string(BACK_MENU) +") Back to previous menu\n ";
		prompt += "  " + to_string(SCREEN_DISPLAY)+") Display on screen\n ";
		prompt += "  " + to_string(PRINT_REPORT) +") Print catalog\n "; //NEED TO DO - either create this or remove this menu.
		prompt += "  Please make a selection:\n ";
		int viewMethod = getInputReprompt(prompt, BACK_MENU, PRINT_REPORT);//getInputPreprompt converts any entry to upper for comparison
		
		//skip the rest if back selected
		if (viewMethod == BACK_MENU) { return; };
		
		//GET USER INPUT WHICH CATALOG TO VIEW
		int viewCatalog;
		enum catalogSelection { BACK, SONG, SONG_ARTIST, ARTIST };
		string promptb = "\n----View Catalogue Menu - SELECT CATALOG----\n ";
		promptb += "  "+to_string(BACK) +") Back to previous menu\n ";
		promptb += "  " + to_string(SONG) +") Song Catalogue\n ";
		promptb +="  "+to_string(SONG_ARTIST) +") Song by Artist Catalogue\n ";
		promptb +="  "+to_string(ARTIST) +") Artist List\n ";
		
		promptb += "  Please make a selection:\n ";
		viewCatalog = getInputReprompt(promptb, 1, 4);//getInputPreprompt converts any entry to upper for comparison
		
		stringstream report;//fill with contents of map
		char delimiter = '\t';

		string reportTXT;
		switch (viewCatalog) {
		case BACK:
			//allow while loop to continue;
			break;
		case SONG: 
		{
			reportTXT = "SongListReport.txt";
			
				report = reportSongMap(delimiter);
			
			break;
		}
		case SONG_ARTIST:
		{
			reportTXT = "SongListByArtistReport.txt";
			
			report = reportSongByArtistMap(delimiter);
			break;
		}
		case ARTIST:
		{
			reportTXT = "ArtistListReport.txt";
			report = reportArtistMap(delimiter);
			break;
		}
		default:// BACK OR ERROR GOES HERE, Returns from function
			return;
			break;
		}

		//output catalog according to view method selected
		if (viewMethod == SCREEN_DISPLAY)
		{
			cout << report.str();
		}
		else if (viewMethod == PRINT_REPORT) {
			fstream reportFstream;
			reportFstream.open(reportTXT, fstream::out);
			reportFstream << report.str();
			reportFstream.close();
			cout << "\nReport saved\n";
		}
		else {
			cout << "error in viewMethod if statement";

		}
	}//end while loop
}//end menuMDisplayCatalogue
//submenu
void menuQueueManagement()
{
	bool continueMenu = true;
	
	//QueueManagement_KJ<SongRequest> comboList; //holds combo of everything
	//QueueManagement_KJ<string> comboList; //holds combo of everything
	SongRequest newRequest;

	QueueManagement_KJ<string> list; //this holds the list of singers with both their key and display name
	QueueManagement_KJ<string> songList; //this holds all the songs in the queue, this is only the song names
	QueueManagement_KJ<string> displayList; //this holds only the singer's display name, used to pop up on deck singers for the audience


	while (continueMenu)
	{
		int userSelection;
		string prompt = "\n----Queue Management Menu----\n ";
		prompt += "  1) Add Singer and Selection\n "; //add a new singer to the queue - this adds them to the end
		prompt += "  2) Remove Singer\n "; //remove a singer from the queue 
		prompt += "  3) Move Singer in Queue\n "; //move a singer from their current place in the queue to another selected place
		prompt += "  4) Display Pending Singers\n "; //display the next 10 singers pending
		/*prompt += "  5) Display Pending Songs\n "; //display the pending songs (and their corresponding singer)*/
		prompt += "  5) EOD Clear Queue\n "; //clears out any remaining in the queue at the end of the day
		prompt += "  6) Exit to main\n "; //exit to main menu
		prompt += "  Please make a selection:\n ";
		userSelection = getInputReprompt(prompt, 1, 7);//getInputPreprompt converts any entry to upper for comparison

		Singer newSinger; //used to capture a new singer in the queue
		Song newSong; //used to capture a new song in the queue
		Singer toRemove; //singer that will be removed from the queue
		Singer moveIt;
		string displayname; //this is the displayname of the singer that's been added
		string singerKey; //holds the singer key for addition to the map
		string nodeData; //holds the combined node data for removal
		string songTitle; //this is the title of the song pulled from the newSong
		string songKey; //holds the key for the song the singer chooses
		string singerToMove; //this holds the singer to move in the queue
		string singerAfter; //singer who will follow the singer being moved in the queue
		string displayNameMove;
		string displayNameAfter;
		string songToMove;
		string songTitleToMove;
		bool verifyExists; //verify that the singer is in the queue
		char songComplete; //holds the user input for whether the singer completed the song
		SongRequest confirm;
		switch (userSelection) {
		case 1:
			//confirm = addSongReq();
			//set struct contents
			newRequest.reqSinger = userInputSinger();
			newRequest.reqSong = userInputSong();
			//add struct as object of the list node
			comboList.appendNode(newRequest);
			//cout << "Amy's experiment with SongRequest list output: ";
			confirm = comboList.findNode(newRequest);
			cout << confirm.display() <<" added.\n";
			//cin.get();

			//try a different way//
			/*newRequest.reqSinger = userInputSinger(); //use userInputSinger to gather the right singer
			singerKey = newRequest.reqSinger.getKey();  //get the key for the singer that needs removed
			displayname = newRequest.reqSinger.getDisplayName(); //get the display name for the singer to remove
			newRequest.reqSong = userInputSong();
			songKey = newRequest.reqSong.getKey();
			songTitle = newRequest.reqSong.getTitle();
			nodeData = (displayname + " / " + songKey);
			displayname = newSinger.getDisplayName(); //get the display name for the singer to remove
			comboList.appendNode(nodeData);
			cout << confirm.display() << " added.\n";*/
			//lets see//
			break;
		case 2:
			cout << endl;
			cout << "Current queue:" << endl; 
			comboList.displayFullList(); //generate the full list of singers
			cout << endl;
			
			//cout << "Please select the singer to remove by entering the display name.\n";//select the user to remove
			//cin >> displayname; //collect the input*/
			//confirm = comboList.findAndRemoveNode(displayname);
			//cout << confirm.display();
			newRequest.reqSinger = userInputSinger(); //use userInputSinger to gather the right singer
			singerKey = newRequest.reqSinger.getKey();  //get the key for the singer that needs removed
			displayname = newRequest.reqSinger.getDisplayName(); //get the display name for the singer to remove
			newRequest.reqSong = userInputSong();
			songKey = newRequest.reqSong.getKey();
			songTitle = newRequest.reqSong.getTitle();
			cout << singerKey << " is key" << songKey << " is song key";
			nodeData = (displayname + " / " + songKey);
			cout << nodeData << " is node data";
			displayname = newSinger.getDisplayName(); //get the display name for the singer to remove
			//nodeData = (singerKey + "/" + displayname); //concatenate key and name to create node data for the list/removal
			/*verifyExists = list.verifyNameExists(displayname); //make sure that the name is on the list
			if (verifyExists == false) //if not on the list, have them reenter until it is
			{
				cout << "Please enter a valid username:\n"; 
				goto reenter1;
			}*/
			cout << endl;
			cout << "Did the singer complete the song? Y yes, N no.\n"; //find out if they completed a song or just left
			reenter2:cin >> songComplete; 
			if (toupper(songComplete) == 'Y')
			{
				//comboList.displayFullList(); //list all the songs in the queue for review
				//cout << "Please select the name of the song they completed:\n";
				/*reenter3:cin >> songTitle;
				verifyExists = songList.verifyNameExists(songTitle);
				if (verifyExists == false) //if not on the list, have them reenter until it is
				{
					cout << "Please enter a valid song name:\n";
					goto reenter3;
				}*/
				//cin.ignore();
				//newSong = userInputSong(); //use userInputSong to gather the right singer
				//songKey = newSong.getKey(); //get the key for the singer that needs to be added to the map
				//songTitle = newSong.getTitle(); //get the song title for the song to remove
				cout << singerKey << " is key" << songKey << " is song key";
				cout << nodeData << " is node data";
				addToSingerHistory(singerKey, 2019, 12, 13, songKey); //add the song to the singer's history
				comboList.deleteNode(newRequest); //delete the node from the list that has the singer name/username
				//displayList.deleteNode(displayname); //delete the node from the list that only has display names
				//songList.deleteNode(songTitle); //delete the song from the songlist
				cout << "Singer removed and singer history updated with song: " << songTitle << endl; //tell the KJ that the singer has been removed
			}
			else if (toupper(songComplete) == 'N') //don't add to the singer history as they didn't complete the song
			{
				comboList.deleteNode(newRequest); //delete the node from the list that has the singer name/username
				//displayList.deleteNode(displayname); //delete the node from the list that only has display names
				comboList.displayFullList(); //display the songs pending
				//cout << "Please select the name of the song they intended to sing:\n";
				//cin.ignore(); //ignore trailing whitespace
				//newSong = userInputSong();
				//songTitle = newSong.getTitle();
				//songList.deleteNode(songTitle); //delete the song
				cout << "Singer removed.\n"; //tell the KJ that the singer has been removed
			}
			else //resolicit if they type anything but Y or N
			{
				cout << "Please enter either Y or yes, they completed, or N for no, they did not complete a song.\n";
				goto reenter2;
			}
			cout << "Remaining queue:\n"; //display the remaining queue for the KJ
			cout << endl;
			comboList.displayFullList();
			cin.ignore();
			break;
		case 3:
			comboList.displayFullList();
			cout << "Please select the singer to move by entering the full name including the / from the list.\n";
			getline(cin, singerToMove);
			nodeData = singerToMove;
			comboList.displayFullList();
			//cin.ignore();
			//credit to geeksforgeeks: https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
			transform(singerToMove.begin(), singerToMove.end(), singerToMove.begin(), ::toupper); //convert the whole combined string to all caps
			displayNameMove = generateDisplayName(singerToMove);
			cout << "Which song is this singer scheduled to sing?\n";
			getline(cin, songToMove);
			//transform(songToMove.begin(), songToMove.end(), songToMove.begin(), ::toupper); //convert the whole combined string to all caps
			cout << "Move this singer to which position (please enter the singer full name that will follow the moved singer).\n";
			getline(cin, singerAfter);
			//cin.ignore();
			transform(singerAfter.begin(), singerAfter.end(), singerAfter.begin(), ::toupper); //convert the whole combined string to all caps
			displayNameAfter = generateDisplayName(singerAfter);
			//cin.ignore();
			list.deleteNode(singerToMove);
			displayList.deleteNode(displayNameMove);
			list.insertNode(singerToMove, singerAfter);
			displayList.insertNode(displayNameMove, displayNameAfter);
			break;
		case 4:
			cout << "On deck: \n";
			cout << endl;
			comboList.displayList(); //display the top 10 displaynames only for singers pending
			break;
		/*case 5:
			cout << "Songs currently in the queue: \n"; 
			songList.displayFullList(); //display the list of songs in the queue
			break;*/
		case 5:
			cout << "Clearing queue.\n";
			comboList.~QueueManagement_KJ(); //call the destructor to remove all pending singers, send back to the main to avoid duplicate delete[] calls
			//main();
			menuQueueManagement();
		default:
			continueMenu = false; //change the boolean from true to false and exit the menu
			return;
			break;
		}
	}
}

//Mysti's function to add a singer and song to the queue, can be used from either the KJ or singer menus
void addSongReq()
{
	/*newRequest.reqSinger = userInputSinger(); //gather the singer's information
	newRequest.reqSong = userInputSong(); //gather the song information

	comboList.appendNode(newRequest); //add the comboList (which includes the singer object and song object) to the list
	confirm = comboList.findNode(newRequest); //gather the information in the node
	cout << confirm.display() << " added.\n"; //generate the information for the user

	return confirm;*/
}
