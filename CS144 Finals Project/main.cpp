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

	cout << endl;

	//   -- Main Game Loop --   //
	bool active = true;
	int battlesWon = 0;
	string playerChoice;
	while (active && (player->getState() == ALIVE)) {
		cout << endl << "(Battles Won: " << battlesWon << ") You walk deeper into the dungeon..." << endl;

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

		// Post-battle
		if (player->getState() == ALIVE && active) { // If the character is alive and didn't run (aka if they won the battle)
			// At the end of the battle, heal back HP
			cout << endl << "------------------------------------------------" << endl << endl; // Spacer for niceness
			cout << "At the end of the battle, you find a health potion! Your HP returns to maximum." << endl;
			player->takeDamage(-(player->getMaxHP())); // Negative damage is healing, remember?

			// Offer the player the enemy's weapon
			cout << endl << "Upon your enemy's bloodied corpse, you find their weapon, " << (*(enemy.getEquippedWeapon())) << "." << endl;
			cout << "Additionally, you can run away with your life." << endl << endl;
			enemy.getEquippedWeapon()->showStats();

			// Gain sanitized input
			do {
				cout << "Would you like to take it or run away? (Y/n/r): ";
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

			switch (toupper(playerChoiceChar)) {
			case 'N':
				cout << "Okay then. Suit yourself, I guess, with your INFERIOR WEAPON." << endl;
				break;
			case 'R':
				cout << "You know what? Running is fine. They may say that running is for quitters, but most quitters are also still alive." << endl;
				active = false;
				break;
			default: // Enter or Y or realistically anything else
				cout << "You pick up the enemy's weapon and toss your old one aside!" << endl;
				Weapon* newWeapon = new Weapon(*enemyWeapon);	// Make a copy of the enemy's weapon (the enemy's will be deleted with the enemy's destructor)
				player->setEquippedWeapon(newWeapon);			// setEquippedWeapon already takes care of deleting the previous weapon
				cout << "You can see your reflection in " << (*(player->getEquippedWeapon())) << "!" << endl;
				break;
			}

			battlesWon++; // Increment battles won

			cout << endl << "[PRESS ENTER TO CONTINUE TO THE NEXT BATTLE]" << endl;
			getline(cin, temp);
		}
	}

	//   -- Outro Sequence --   //
	cout << endl;
	if (player->getState() == ALIVE) { // If the player ran
		cout << "You sprint out of the dungeon, cuts and scars abraising your body." << endl;
		cout << "Upon returning to the King, he tells you that you've won a timeshare in his kingdom. What a letdown." << endl;
		cout << "If only you could get your afternoon back..." << endl << endl;
	}
	else { // If the player died
		cout << "You were killed!" << endl << endl;
		cout << "As the last breath of your life escapes your lungs, you can only think of one thing:" << endl;
		cout << "\"Man, I really wish I had done something better with my Saturday afternoon...\"" << endl << endl;
	}
	cout << "*****************" << endl;
	cout << "*   GAME OVER   *" << endl;
	cout << "*****************" << endl << endl;
	
	cout << "FINAL SCORE: " << ((player->getEquippedWeapon()->getWorth() + player->getCurrentHP()) * (battlesWon + 1)) << endl << endl;
	
	return 0;
}