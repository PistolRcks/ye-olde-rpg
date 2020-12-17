// Declares the TurnTracker class, which takes care of choosing which Entity goes when.

#ifndef TURNTRACKER_H
#define TURNTRACKER_H
#include <iostream>
#include "Entity.h"
#include "Weapon.h"

// Forward declaration
class Entity; 
class Weapon;

class TurnTracker {
private:
	Entity* combatantA;
	Entity* combatantB;
	int combatantTurn = -1;		// Which Combatant's turn it is (0 for A and 1 for B)
	int turnNumber = 0;		// How many turns have passed
	bool isInCombat = true;	// Whether it the Combatants are in combat or not
public:
	// Constructors
	TurnTracker(Entity* combatantA, Entity* combatantB);

	// Getters and Setters
	int getCurrentCombatantIndex();
	Entity* getNextTurnsCombatant();
	bool isCombatOngoing();
	void endCombat();

	// Methods
	void advanceTurnTracker();
};

#endif


