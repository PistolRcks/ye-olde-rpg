// Defines the Weapon class, which Entities use to hit each other.

#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>
#include <stdlib.h>
#include "Entity.h"

using namespace std;

// Words for weapon name generation
static string weaponAdjectives[10] = {
	"Threatening",
	"Dirty",
	"Chonkin'",
	"Holy",
	"Demonic",
	"Shoddy",
	"Discotechnical",
	"Magical",
	"Scottish",
	""
};

static string weaponTypes[10] = {
	"Gun",
	"Sword",
	"Pickle",
	"Farm Implement",
	"War-Triangle",
	"War-Lute",
	"Stick",
	"Kunai with Chain",
	"Axe",
	"Spear"
};

static string weaponDescriptors[10] = {
	" of Truth",
	" of the Creator",
	" of Immense Power",
	" from the Underworld",
	" from Somebody's Basement",
	" from Bathrooms Unknown",
	", the Slayer's Assistant",
	", Big Stick of Theodore",
	", Artisanally Crafted",
	""
};


class Weapon {
private:
	struct WeaponEffect {
		void (*effect)();
		string description = "";
	};

	// Metadata
	string name;
	int worth;

	// Attack stats
	int hitPercent;
	int critPercent;
	int damageBounds[2];

	// Other stats attributed to a Weapon
	int armor;
	int speed;

	Entity* bearer; // The bearer of the Weapon

	// Effects
	vector<WeaponEffect> onHitEffects;
	vector<WeaponEffect> onCritEffects;
public:
	// Constructors
	Weapon(int worth);

	// Getters/Setters
	string getName();
};
#endif

