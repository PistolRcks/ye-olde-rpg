#include "Entity.h"

/********************************
 * onTurnStart Effects (Brains) *
 ********************************/

// An AI brain. This brain always attacks.
void slasherBrain(Entity* parentEntity, Entity* enemy) {
	cout << (*parentEntity) << " lunges at you!" << endl;
	parentEntity->makeAttack(enemy);
}

// An AI brain. This brain has a 50% chance to skip their turn.
void sluggishBrain(Entity* parentEntity, Entity* enemy) {
	if ((rand() % 2) == 0) { // Attack mode
		cout << (*parentEntity) << " musters up the courage to swing!" << endl;
		parentEntity->makeAttack(enemy);
	}
	else { // Do nothing mode
		cout << (*parentEntity) << " is too scared to attack!" << endl;
	}

}

// An AI brain. This brain ALWAYS skips their turn.
void lazyBrain(Entity* parentEntity, Entity* enemy) {
	cout << (*parentEntity) << " lazes around!" << endl;
}

// Create the array of function pointers
void (*Entity::entityBrains[3])(Entity*, Entity*) = {
	slasherBrain,
	sluggishBrain,
	lazyBrain
};


/*********************
 * Methods of Entity *
 *********************/

/* Main costructor for the Entity class.
 * @param `string` name - The name of the Entity.
 * @param `int` maxHP - The maximum hit points (better known as health) of the Entity. `currentHP` will also be set to this value.
 * @param `Weapon*` weaponToEquip - The Weapon to equip to the Entity. (Optional)
 * @param `void (*func)(Entity*)` onTurnStartEffect - The function to call when the Entity starts its turn--most of the time, this will provide AI. The first parameter is the parent Entity of the effect, and the second is the enemy Entity. (Optional)
 */
Entity::Entity(string name, int maxHP, Weapon* weaponToEquip, void (*onTurnStartEffect)(Entity*, Entity*)) {
	this->name = name;
	this->maxHP = maxHP;
	currentHP = maxHP;

	equippedWeapon = weaponToEquip;
	equippedWeapon->setBearer(this);

	onTurnStart = onTurnStartEffect;
}

// Destructor for the Entity class
Entity::~Entity() {
	if (equippedWeapon != nullptr) {
		delete equippedWeapon;
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
// If HP would be reduced to or past 0, state will be changed to DEAD. 
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

// Makes an attack with the currently equipped weapon. Alias for equippedWeapon->makeAttack(target).
void Entity::makeAttack(Entity* target) {
	equippedWeapon->makeAttack(target);
}

// Begins the turn for the Entity. Runs an onTurnStart effect, if there is one (otherwise does nothing). Requires a enemy Entity.
void Entity::beginTurn(Entity* enemy) {
	if (onTurnStart != nullptr) {
		onTurnStart(this, enemy);
	}
}

/*********************
 * Friends of Entity *
 *********************/

// Writes the name of the Entity to an ostream. Alias for Entity::getName().
ostream& operator<<(ostream& out, Entity& entity) {
	out << entity.name;
	return out;
}