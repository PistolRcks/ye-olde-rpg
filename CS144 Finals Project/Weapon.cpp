#include "Weapon.h"

int maximum(int a, int b) { // I am not going to include another header for max()
	return (a > b) ? a : b;
}

// Randomly generates a Weapon with a worth of `worth`.
Weapon::Weapon(int worth) {
	// Randomly generate the name
	string adjective = weaponAdjectives[rand() % 10];
	string type = weaponTypes[rand() % 10];
	string descriptor = weaponDescriptors[rand() % 10];

	name = "The " + adjective + " " + type + descriptor;
	this->worth = worth;

	// Starter stats -- Weapon stats will never go below this
	hitPercent = 50;
	critPercent = 0;
	damageBounds[0] = 0; // Lower bound
	damageBounds[1] = 1; // Upper bound
	armor = 0;
	speed = 1;

	// Generate stats -- We randomly assign points (based on worth) to each of the stats (this is also weighted)
	for (int i = 0; i < worth; i++) {
		int randChoice = rand() % 100;	// Choose a random percentage
		if (randChoice < 40) {							// (0-39%) Increase the lower damage bound and range by 1d20
			int range = damageBounds[1] - damageBounds[0] + (rand() % 20) + 1;
			damageBounds[0] += (rand() % 20) + 1;
			damageBounds[1] = damageBounds[0] + range;
		}
		else if (randChoice < 65) {						// (45-64%) Increase to-hit chance by 5-10%
			if (hitPercent >= 100) { // If we already maxed out the hitpercent
				i--; // "Refund" a point
				continue; // Try again
			}
			hitPercent += (rand() % 6) + 5;
		}
		else if (randChoice < 75) {						// (65-74%) Increase speed by 1d8
			speed += (rand() % 8) + 1;
		}
		else if (randChoice < 85) {						// (75-84%) Increase armor by 1d8
			armor += (rand() % 8) + 1;
		}
		else if (randChoice < 95) {						// (85-94%) Increase critical hit chance by 5-10%
			critPercent += (rand() % 6) + 5;
		}
		else if (randChoice < 99) {						// (95-98%) Increase all stats by 1d4
			hitPercent += (rand() % 4) + 1;

		}
		else if (randChoice < 100) {					// (99-100%) Add an on-crit or on-hit effect
			// TODO: Make effects
		}
	}
}

// Gets the name of the weapon.
string Weapon::getName() {
	return name;
}

// Pretty-prints Weapon statistics to cout.
void Weapon::showStats() {
	int midWidth = maximum((10 + name.length()) / 2, 18); // Align the middle divider to the center

	cout << "Stats for " << name << endl;
	cout << setw(10. + name.length()) << setfill('-') << right << "-" << endl; // Make a cool underline below text
	cout << setfill(' '); // reset fill (already right-aligned)
	cout << setw(midWidth) << "Hit % | " << left << hitPercent << "%" << endl << right; // Set width and put in descriptor text; then, align left and put in the statistic; finally, put in a newline and align back to right
	cout << setw(midWidth) << "Critical Hit % | " << left << critPercent << "% (Critical hits deal twice the damage)" << endl << right;
	cout << setw(midWidth) << "Damage | " << left << damageBounds[0] << "-" << damageBounds[1] << " DMG" << endl << right;
	cout << setw(midWidth) << "Armor | " << left << armor << " (Armor reduces damage)" << endl << right;
	cout << setw(midWidth) << "Speed | " << left << armor << " (Speed determines how fast you attack and who attacks first)" << endl << right;

	// TODO: Add in effects
}

// Streams the name of the Weapon to an ostream (alias of Weapon::getName()).
ostream& operator<<(ostream& out, Weapon& weapon) {
	out << weapon.name;
	return out;
}
