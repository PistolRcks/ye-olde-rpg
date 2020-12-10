#include "Entity.h"

/* Main costructor for the Entity class.
 * @param `string` name - The name of the Entity.
 * @param `int` maxHP - The maximum hit points (better known as health) of the Entity. `currentHP` will also be set to this value.
 * @param `Weapon*` weaponToEquip - The Weapon to equip to the Entity. (Optional)
 * @oaram `void (*func)()` onDeathEffect - The function to call when the Entity dies. (Optional)
 * @param `void (*func)()` onTurnStartEffect - The function to call when the Entity starts its turn--most of the time, this will provide AI. ((Optional)
 */
Entity::Entity(string name, int maxHP, Weapon* weaponToEquip, void (*onDeathEffect)(), void (*onTurnStartEffect)()) {
	this->name = name;
	this->maxHP = maxHP;
	currentHP = maxHP;

	equippedWeapon = weaponToEquip;

	onDeath = onDeathEffect;
	onTurnStart = onTurnStartEffect;
}

// Destructor for the Entity class
Entity::~Entity() {
	delete equippedWeapon;
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

// Gets a pointer to the currently equipped Weapon
Weapon* Entity::getEquippedWeapon() {
	return equippedWeapon;
}

// Equips a Weapon. The previously eqiupped Weapon (if there is one) will be deleted.
void Entity::setEquippedWeapon(Weapon* weapon) {
	if (equippedWeapon != nullptr) { delete equippedWeapon; }
	equippedWeapon = weapon;
}

// Makes an attack with the currently equipped weapon. Alias for inventory[currentWeapon]->makeAttack().
void Entity::makeAttack() {
	//inventory[currentWeapon]->makeAttack();
}

// Writes the name of the Entity to an ostream. Alias for Entity::getName().
ostream& operator<<(ostream& out, Entity& entity) {
	out << entity.name;
	return out;
}
