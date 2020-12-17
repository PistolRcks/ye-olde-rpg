// Declares the Weapon class, which Entities use to hit each other.

#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
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

class Entity; // Forward declaration required to resolve circular dependency

class Weapon; // Weapon needs to be declared for WeaponEffect

struct WeaponEffect {
	WeaponEffect();
	WeaponEffect(void (*effect)(Weapon*, Entity*), string description, string target); // Normal constructor
	void (*effect)(Weapon*, Entity*); // The effect the weapon will proc. The first parameter is a pointer to the Weapon which causes the effect, and the second parameter is a pointer to the target Entity of the effect.
	string description; // A description of the weapon's effect.
	string target; // The target who will be affected by the effect (either "self" or "opponent").
};


class Weapon {
// Friends
// On-hit Effects
friend void vampirismEffect(Weapon* parentWeapon, Entity* target);
friend void gigaCritEffect(Weapon* parentWeapon, Entity* target);
friend void selfImprovementEffect(Weapon* parentWeapon, Entity* target);

// Operators
friend ostream& operator<<(ostream& out, Weapon& weapon);

private:
	// Metadata
	string name;							// The name of the weapon.
	int worth;								// Determines how many stat increases this weapon has had.
	Entity* bearer;							// Points to the Entity which has this weapon equipped.

	// Attack stats
	int hitPercent;							// The percentage chance that the Weapon will hit
	int critPercent;						// The percentage chance that the Weapon will perform a critical hit (deals 2x damage)
	int damageBounds[2];					// The lower (damageBounds[0]) and upper (damageBounds[1]) bounds of damage that the weapon can deal (not affected by crits)

	// Other stats attributed to a Weapon
	int armor;								// Subtracts damage dealt to the Weapon's bearer
	int speed;								// Determines who goes first

	// Effects
	vector<WeaponEffect> onHitEffects;		// A list of all WeaponEffects which are run when the weapon hits
public:
	// Constructors
	Weapon(int worth);

	// Getters/Setters
	string getName();
	int getArmor();
	int getSpeed();
	int getWorth();
	void setBearer(Entity* target);

	// Methods
	void showStats();
	void makeAttack(Entity* target);
	void upgradeWeapon(int upgradeAmount);
};
#endif

