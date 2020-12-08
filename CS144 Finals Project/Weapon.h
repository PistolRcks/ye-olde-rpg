// Defines the Weapon class, which Entities use to hit each other.

#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <stdlib.h>

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
	// Metadata
	string name;
	int worth;

	// Attack stats
	int hitPercent;
	int critPercent;
	int damageBounds[2];

	// Effects
	void (*onHit)();
	string onHitDescription = "";
	void (*onCrit)();
	string onCritDescription = "";
public:
	// Constructors
	Weapon(int worth);

	// Getters/Setters
	string getName();
};
#endif

