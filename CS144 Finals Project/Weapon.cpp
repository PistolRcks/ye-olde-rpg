#include "Weapon.h"

// Randomly generates a Weapon with a worth of `worth`.
Weapon::Weapon(int worth) {
	// Randomly generate the name
	string adjective = weaponAdjectives[rand() % 10];
	string type = weaponTypes[rand() % 10];
	string descriptor = weaponDescriptors[rand() % 10];

	name = "The " + adjective + " " + type + descriptor;
	this->worth = worth;

	hitPercent = 0;
	critPercent = 0;
	damageBounds[0] = 0; // Lower bound
	damageBounds[1] = 1; // Upper bound
}

// Gets the name of the weapon.
string Weapon::getName()
{
	return name;
}
