// Defines the Weapon class, which Entities use to hit each other.

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

struct WeaponEffect {
	void (*effect)(Weapon*, Entity&*); // The effect the weapon will proc. The first parameter is a pointer to the Weapon which causes the effect, and the second parameter is a pointer to the target Entity of the effect.
	string description; // A description of the weapon's effect.
	string target = "self"; // The target who will be affected by the effect (either "self" or "opponent").
};

void initWeaponEffects(WeaponEffect weaponEffectsArray[]);

class Weapon {
// Friends
// On-hit Effects
friend void vampirismEffect(Weapon* ownerWeapon, Entity* target);
friend void gigaCritEffect(Weapon* ownerWeapon, Entity* target);
friend void selfImprovementEffect(Weapon* ownerWeapon, Entity* target);

// Operators
friend ostream& operator<<(ostream& out, Weapon& weapon);

private:
	// Metadata
	string name;
	int worth;
	Entity* bearer;

	// Attack stats
	int hitPercent;
	int critPercent;
	int damageBounds[2];

	// Other stats attributed to a Weapon
	int armor;
	int speed;

	// Effects
	vector<WeaponEffect> onHitEffects;
public:
	// Constructors
	Weapon(int worth);

	// Getters/Setters
	string getName();
	int getArmor();
	int getSpeed();
	void setBearer(Entity* target);

	// Methods
	void showStats();
	void makeAttack(Entity* target);
	void upgradeWeapon(int upgradeAmount);
};
#endif

