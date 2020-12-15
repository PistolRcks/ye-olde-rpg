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
	// Sanity checks
	int dmgAfterArmor = damage - equippedWeapon->getArmor(); // Default
	if (damage < 0) { // If damage is negative (i.e. healing), we don't want to subtract armor for extra healing
		dmgAfterArmor = damage;
	}
	else if (damage >= 0 && dmgAfterArmor <= 0) { // If the damage after armor is 0 or less...
		cout << "*TING!* " << (*equippedWeapon) << " completely blocked all incoming damage!" << endl;
		return;
	}
	
	if (currentHP - dmgAfterArmor > maxHP) { // Heal cap from before
		cout << name << " feels much better. "<< name <<" have been healed for " << (-dmgAfterArmor) << " HP!" << endl;
		currentHP = maxHP;
	}
	else if (currentHP - dmgAfterArmor <= 0) { // If damage would cause the player to die
		cout << "The blow for " << dmgAfterArmor << " DMG is fatal!" << endl;
		currentHP = 0;
		state = DEAD;
		onDeath();
	}
	else { // Regular damage taking
		cout << name << " takes " << dmgAfterArmor << " DMG!" << endl;
		currentHP -= dmgAfterArmor;
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
	weapon->setBearer(this);
}

// Makes an attack with the currently equipped weapon. Alias for inventory[currentWeapon]->makeAttack().
void Entity::makeAttack(Entity* target) {
	equippedWeapon->makeAttack(target);
}

// Writes the name of the Entity to an ostream. Alias for Entity::getName().
ostream& operator<<(ostream& out, Entity& entity) {
	out << entity.name;
	return out;
}
