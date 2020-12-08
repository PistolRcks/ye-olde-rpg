// Defines the Entity class, which governs players and monsters alike.

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "Weapon.h"

using namespace std;

// Enumeration for the alive/dead state of the Entity
enum EntityState {ALIVE, DEAD, SOMEWHERE_IN_BETWEEN};

class Entity {
private:
	string name; // The name of the entity
	int maxHP; // The maximum HP of the entity
	int currentHP; // The current HP of the entity
	EntityState state = ALIVE; // State of the entity (enums are unscoped but we won't name anything else ALIVE, DEAD or SOMEWHERE_IN_BETWEEN)
	Weapon* inventory[5]; // An array of pointers to Weapons.
	int currentWeapon; // The index of the currently equipped Weapon.

	void (*onDeath)(); // A function pointer to the effect which will proc on death of the Entity.
public:
	// Constructors
	Entity(string name, int maxHP, void (*onDeathEffect)());
	~Entity();

	// Getters/Setters
	string getName();
	void setName(string name);

	int getMaxHP();
	int getCurrentHP();
	void setHP(int HP);
	void takeDamage(int damage); // semi-related

	EntityState getState();
};
#endif

