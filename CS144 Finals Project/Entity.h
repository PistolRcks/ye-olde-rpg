// Declares the Entity class, which governs players and monsters alike.

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>
#include "Weapon.h"
#include "TurnTracker.h"

using namespace std;

// Enumeration for the alive/dead state of the Entity
enum EntityState {ALIVE, DEAD, SOMEWHERE_IN_BETWEEN};

class Weapon; // Forward declaration required to resolve circular dependency
class TurnTracker;

class Entity {
// Friends
// Operators
friend ostream& operator<<(ostream& out, Entity& entity);

// onTurnStart Effects (brains)
friend void slasherBrain(Entity* parentEntity, Entity* enemy);
friend void sluggishBrain(Entity* parentEntity, Entity* enemy);
friend void lazyBrain(Entity* parentEntity, Entity* enemy);

private:
	string name;					// The name of the entity
	int maxHP;						// The maximum HP of the entity
	int currentHP;					// The current HP of the entity
	EntityState state = ALIVE;		// State of the entity (enums are unscoped but we won't name anything else ALIVE, DEAD or SOMEWHERE_IN_BETWEEN)
	Weapon* equippedWeapon;			// A pointer to the currently equipped Weapon.

	void (*onTurnStart)(Entity*, Entity*);	// A function pointer to the effect which will proc upon the start of a turn. The first parameter is the parent Entity of the effect, and the second is the enemy Entity.
public:
	// Constructors
	Entity(string name, int maxHP, Weapon* weaponToEquip = nullptr, void (*onTurnStartEffect)(Entity*) = nullptr);
	~Entity();

	// Getters/Setters
	string getName();
	void setName(string name);

	int getMaxHP();
	int getCurrentHP();
	void setHP(int HP);
	void takeDamage(int damage); // semi-related

	EntityState getState();

	Weapon* getEquippedWeapon();
	void setEquippedWeapon(Weapon* weapon);

	// Methods
	void makeAttack(Entity* target);
	void beginTurn();
	void endTurn(TurnTracker* turnTracker);

	// Static (for choosing brains in main)
	static void (*entityBrains[3])(Entity*, Entity*) = {
		slasherBrain,
		sluggishBrain,
		lazyBrain
	}
};
#endif

