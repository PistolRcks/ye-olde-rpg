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
	string name;
	int maxHP;
	int currentHP;
	Weapon* currentWeapon;
	EntityState state = ALIVE; // enums are unscoped but that's no issue since a state machine only requires an int
public:
	// Constructors
	Entity(string name, int maxHP);
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

