#include "Weapon.h"

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
		if (randChoice > 25) {							// (0-24%) Increase to-hit chance by 5-10%
			if (hitPercent >= 100) { // If we already maxed out the hitpercent
				i--; // "Refund" a point
				continue; // Try again
			}
			int increase = (rand() % 6) + 5;
			hitPercent += increase;
		}
		else if (randChoice > 50) {						// (25-49%) Increase damage bounds by 1d20
			int increase = (rand() % 20) + 1;
			damageBounds[0] += increase;
			damageBounds[1] += increase;
		}
		else if (randChoice > 75) {						// (50-74%) Increase speed by 1d4
			int increase = (rand() % 4) + 1;
			speed += increase;
		}
		else if (randChoice > 85) {						// (75-84%) Increase armor by 1d4
			int increase = (rand() % 4) + 1;
			armor += increase;
		}
		else if (randChoice > 95) {						// (85-94%) Increase critical hit chance by 5-10%
			int increase = (rand() % 6) + 5;
			critPercent += increase;
		}
		else if (randChoice > 99) {						// (95-98%) Increase all stats by 1d4
			int increase = (rand() % 4) + 1;
			hitPercent += increase;

		}
		else if (randChoice > 100) {					// (99-100%) Add an on-crit or on-hit effect
			// TODO: Make effects
		}
	}
}

// Gets the name of the weapon.
string Weapon::getName()
{
	return name;
}
