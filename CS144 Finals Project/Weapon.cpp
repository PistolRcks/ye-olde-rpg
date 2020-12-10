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

	bearer = nullptr;

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
			if (critPercent >= 100) { // If we already maxed out the critpercent
				i--;
				continue;
			}
			critPercent += (rand() % 6) + 5;
		}
		else if (randChoice < 99) {						// (95-98%) Increase all stats by 1d4
			hitPercent += (rand() % 4) + 1;
		}
		else if (randChoice < 100) {					// (99-100%) Add an on-hit effect
			// TODO: Make effects
		}
	}
}

// Gets the name of the Weapon.
string Weapon::getName() {
	return name;
}

// Gets the armor of the Weapon.
int Weapon::getArmor() {
	return armor;
}

// Gets the speed of the Weapon.
int Weapon::getSpeed() {
	return speed;
}

// Sets the bearer of the Weapon (i.e. the Entity which has this weapon equipped). This is used for effects.
void Weapon::setBearer(Entity* target) {
	bearer = target;
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

	// TODO: Add in effect text
}

// Makes an attack with the Weapon at the target Entity.
void Weapon::makeAttack(Entity* target) {
	if (((rand() % 100) + 1) < hitPercent) { // If it hits...
		int dmgRange = damageBounds[1] - damageBounds[0];
		int damage = (rand() % dmgRange) + damageBounds[0] + 1; // Record damage

		if (((rand() % 100) + 1) < critPercent) { // If it crits...
			damage *= 2;						  // Multiply the damage by two
			target->takeDamage(damage);
			cout << "CRIT! " << (*this) << " comes down with a mighty strike, dealing " << damage << " DMG!" << endl;
		}
		else {
			target->takeDamage(damage);
			cout << (*this) << " lands a blow on its foe, dealing " << damage << " DMG!" << endl;
		}

		if (onHitEffects.size() > 0) { // If there's an on-hit effect...
			for (unsigned int i = 0; i < onHitEffects.size(); i++) { // Proc all effects
				if (onHitEffects[i].target == "self") { // If it affects the bearer...
					onHitEffects[i].effect(bearer);
				} 
				else { // If it affects the target being attacked...
					onHitEffects[i].effect(target);
				}
			}
		}
	}
	else {
		cout << (*this) << " completely whiffed its attack..." << endl;
	}
}



// Streams the name of the Weapon to an ostream (alias of Weapon::getName()).
ostream& operator<<(ostream& out, Weapon& weapon) {
	out << weapon.name;
	return out;
}
