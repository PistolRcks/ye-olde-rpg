// Ye Olde RPG
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "Entity.h"
#include "Weapon.h"

using namespace std;

int main() {
	// Set random seed
	srand(time(NULL));

	// Useful Variables
	string playerName;
	Weapon starterWeapon(10);


	// Intro Sequence
	cout << "Welcome to..." << endl;
	cout << "*--------------------------------------*" << endl;
	cout << "|                                      |" << endl;
	cout << "|         Y E  O L D E  R P G          |" << endl;
	cout << "|                                      |" << endl;
	cout << "*--------------------------------------*" << endl << endl;

	// Ask for the player's name (making sure that cin doesn't break)
	while (cin.fail()) {
		cout << "Please enter your name: ";
		getline(cin, playerName);
		if (cin.fail()) { // If the input is wack
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cin.clear();
			cout << endl << "Excuse me?!? Let's try that again." << endl;
		}
	}

	cout << "Thine weapon, sire... its name is " << starterWeapon.getName() << endl;
	// Main Game Loop
	// Outro Sequence
	return 0;
}