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
	Weapon* starterWeapon = new Weapon(10);


	//   -- Intro Sequence --   //
	cout << "Welcome to..." << endl;
	cout << "*--------------------------------------*" << endl;
	cout << "|                                      |" << endl;
	cout << "|         Y E  O L D E  R P G          |" << endl;
	cout << "|                                      |" << endl;
	cout << "*--------------------------------------*" << endl << endl;

	// Ask for the player's name (making sure that cin doesn't break)
	do {
		cout << "Please enter your name: ";
		getline(cin, playerName);
		if (cin.fail()) { // If the input is wack
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cin.clear();
			cout << endl << "Excuse me, sire?!? Let's try that again." << endl;
		}
	} while (cin.fail());

	Entity* player = new Entity(playerName, 200, starterWeapon);

	cout << "May thine journey fare thee well, Sir " << (*player) << "." << endl;



	//   -- Main Game Loop --   //
	//   -- Outro Sequence --   //
	return 0;
}