// Ye Olde RPG
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "Entity.h"
#include "Weapon.h"
#include "TurnTracker.h"

using namespace std;

string enemyNames[10] = {
	"The Devourer of Souls",
	"Bob from Next Door",
	"The Undead Plumber",
	"A Sentient Doorknob",
	"A Zombie",
	"A Shambling Mass",
	"A Skeleton",
	"The Antagonist of This Game",
	"The Monopoly(tm) Man",
	"An Endangered Species of Some Sort"
};

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

	cout << endl; // for niceness

	cout << "The local king asks you to go on a quest." << endl << "\"Hail, Sir " << (*player) << "!\" the wise old man calls. \
\"There is a dungeon off to the east filled with monsters--mind taking care of it? There will be a reward...\"" << endl << endl;
	cout << "Now there's an offer you can't possibly refuse. You agree." << endl << endl;
	cout << "\"May thine journey fare thee well then, Sir " << (*player) << ".\"" << endl << endl;
	cout << "You head off on your journey and enter Ye Olde Dungeon..." << endl << endl;

	string temp;
	cout << "[PRESS ENTER TO START YOUR ADVENTURE]";
	getline(cin, temp);

	cout << endl << endl;

	//   -- Main Game Loop --   //
	bool active = true;
	int battlesWon = 0;
	string playerChoice;
	while (active && (player->getState() == ALIVE)) {
		cout << "You walk deeper into the dungeon..." << endl;

		// Generate an enemy
		Weapon* enemyWeapon = new Weapon(static_cast<int>(10 * pow(2, battlesWon))); // Each weapon generated's worth is 10 * 2 ^ (number of battles won), making this game almost impossible to "beat".
		string enemyName = enemyNames[rand() % 10]; // Give it a random name
		void (*enemyBrain)(Entity*, Entity*) = Entity::entityBrains[rand() % 3]; // Give it a random brain
		Entity enemy(enemyName, static_cast<int>(10 * pow(3, battlesWon)), enemyWeapon, enemyBrain); // Again, this game is most likely not beatable, as the HP of each enemy is 10 * 3 ^ (number of battles won).

		cout << "You come upon " << enemy << " wielding " << (*enemyWeapon) << "! Prepare for battle!" << endl;

		// Start the turn tracker
		TurnTracker turnTracker(player, &enemy);

		// Run through combat
		do {
			turnTracker.printTurn(); // Print start-of-turn info
			turnTracker.getCurrentCombatant()->beginTurn(turnTracker.getNextTurnCombatant()); // Run AI (if there is any)

			// Player Input
			if (turnTracker.getCurrentCombatantIndex() == 0) { // If it's the player's turn, hand the controls off to them; otherwise let the enemy brain do the work
				bool meaningfulActionTaken = false;	// Meaningful actions will take up a player's turn
				while (!meaningfulActionTaken) {
					// Display choices
					cout << "It's your turn! What would you like to do?" << endl;
					cout << "	[ENTER] Attack" << endl;
					cout << "	(A)ppraise your weapon and your own health" << endl;
					cout << "	(I)dentify the enemy's health and weapon" << endl;
					cout << "	(R)un away and live the good life with your loot (Quit)" << endl;

					// Gain sanitized input
					do {
						cout << "Choice: ";
						getline(cin, playerChoice);
						if (cin.fail()) {
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cin.clear();
							cout << "Not a valid option. Please try again." << endl;
						}
					} while (cin.fail());

					cout << endl;

					// Get the player's first char of input
					char playerChoiceChar;
					if (playerChoice.length() > 0) { // If the player typed anything
						playerChoiceChar = playerChoice.at(0);
					}
					else { // The player pressed enter
						playerChoiceChar = ' ';
					}

					// Apply player's input
					switch (toupper(playerChoiceChar)) {
					case 'A':	// Show weapon stats and current health (not meaningful)
						cout << "Looking at yourself, you find you are at " << player->getCurrentHP() << " out of " << player->getMaxHP() << " HP." << endl;
						player->getEquippedWeapon()->showStats();
						break;
					case 'I':	// Show enemy's weapon's stats and current health (not meaningful)
						cout << "Sizing up the enemy, you find that they have " << enemy.getCurrentHP() << " out of " << enemy.getMaxHP() << " HP." << endl;
						enemy.getEquippedWeapon()->showStats();
						break;
					case 'R':	// Retire/Quit/Run Away (meaningful)
						cout << "You run away!" << endl;
						active = false;
						turnTracker.endCombat();
						meaningfulActionTaken = true;
						break;
					default:	// Attack (meaningful)
						player->makeAttack(&enemy);
						meaningfulActionTaken = true;
						break;
					}
				}
			}
			turnTracker.advanceTurnTracker(); // End the current turn and pass it on
		} while (turnTracker.isCombatOngoing());
		battlesWon++; // Either the player won or it doesn't matter (because they're either dead or they ran)
	}

	//   -- Outro Sequence --   //
	cout << endl;
	if (player->getState() == ALIVE) { // If the player ran
		cout << "You exit out of the dungeon, cuts and scars abraising your body." << endl;
		cout << "Upon returning to the King, he tells you that you've won a timeshare in his kingdom. What a letdown." << endl;
		cout << "If only you could get your afternoon back..." << endl;
	}
	else { // If the player died
		cout << "As the last breath of your life escapes your lungs, you can only think of one thing:" << endl;
		cout << "\"Man, I really wish I had done something better with my Saturday afternoon...\"" << endl;
	}
	cout << endl << "FINAL SCORE: " << ((player->getEquippedWeapon()->getWorth() + player->getCurrentHP()) * (battlesWon + 1)) << endl << endl;
	cout << "*****************" << endl;
	cout << "*   GAME OVER   *" << endl;
	cout << "*****************" << endl;
	
	return 0;
}