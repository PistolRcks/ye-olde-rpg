#include "Entity.h"

/* Main costructor for the Entity class.
 * @param `string` name - The name of the Entity.
 * @param `int` maxHP - The maximum hit points (better known as health) of the Entity. `currentHP` will also be set to this value.
 * @oaram `void (*func)()` onDeathEffect - The function to call when the entity dies.
 */
Entity::Entity(string name, int maxHP, void (*onDeathEffect)()) {
	this->name = name;
	this->maxHP = maxHP;
	currentHP = maxHP;
	currentWeapon = 0;
	for (int i = 0; i < 5; i++) { // Set Weapon pointers to nullptr when we initialize
		inventory[i] = nullptr;
	}
}

// Destructor for the Entity class
Entity::~Entity() {
	for (int i = 0; i < 5; i++) { // Delete weapon pointers in inventory
		if (inventory[i] != nullptr) { // Safety check
			delete inventory[i];
		}
	}
}

// Gets the name of the Entity.
string Entity::getName() {
	return name;
}

// Sets the name of the Entity.
void Entity::setName(string name) {
	this->name = name;
}

// Gets the maximum hit points of the Entity.
int Entity::getMaxHP() {
	return maxHP;
}

// Gets the current hit points of the Entity.
int Entity::getCurrentHP() {
	return currentHP;
}

// Sets the HP of the Entity. Will not go above the Entity's maxHP. Does not change state.
void Entity::setHP(int HP) {
	// Cap the HP to the Entity's maximum HP (if the new total is greater than the max HP, set the current HP to the max)
	currentHP = (currentHP + HP > maxHP) ? maxHP : currentHP + HP;
}

// Inflicts damage upon the Entity. Negative damage will never increase the Entity's HP above the its maxHP. Will not reduce HP past 0. 
// If HP would be reduced to or past 0, state will be changed to DEAD, and triggers onDeath(). 
void Entity::takeDamage(int damage) {
	if (currentHP - damage > maxHP) { // Heal cap from before
		currentHP = maxHP;
	}
	else if (currentHP - damage <= 0) {
		currentHP = 0;
		state = DEAD;
		onDeath();
	}
}

// Gets the current state of the Entity (ALIVE, DEAD, or SOMEWHERE_IN_BETWEEN).
EntityState Entity::getState() {
	return state;
}

