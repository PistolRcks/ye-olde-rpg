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

	Weapon::initWeaponEffects(); // Initialize weapon effects (for the reason I state in Weapon.cpp)

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

	cout << endl; // for niceness

	cout << "The local king asks you to go on a quest." << endl << "\"Hail, Sir " << (*player) << "!\" the wise old man calls. \
\"There is a dungeon off to the east filled with monsters--mind taking care of it? There will be a reward...\"" << endl << endl;
	cout << "Now there's an offer you can't possibly refuse. You agree." << endl << endl;
	cout << "\"May thine journey fare thee well then, Sir " << (*player) << ".\"" << endl << endl;
	cout << "You head off on your journey and enter Ye Olde Dungeon..." << endl << endl;

	//   -- Main Game Loop --   //
	//   -- Outro Sequence --   //
	return 0;
}