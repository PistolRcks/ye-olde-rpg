// Defines the TurnTracker class, which takes care of choosing which Entity goes when.
#include "TurnTracker.h"


/* Default constructor for the TurnTracker class.
 * @param `Entity*` combatantA - A pointer to an Entity which is in combat. (In the case of speed ties, this entity always goes first.)
 * @param `Entity*` combatantB - A pointer to another Entity which is in combat.
 */
TurnTracker::TurnTracker(Entity* combatantA, Entity* combatantB) {
	this->combatantA = combatantA;
	this->combatantB = combatantB;
}

// Gets the Entity pointer of the next turn's combatant.
Entity* TurnTracker::getNextTurnsCombatant() {
	switch (combatantTurn) {
	case 0:		// If it's Combatant A's turn
		return combatantB;	// then the next is Combatant B's
	case 1:		// If it's Combatant B's turn
		return combatantA;	// then the next is Combatant A's
	}
}

// Returns whether or not the Combatants are still fighting.
bool TurnTracker::isCombatOngoing() {
	return isInCombat;
}

// Ends combat for both combatants. Sets isInCombat to false.
void TurnTracker::endCombat() {
	isInCombat = false;
}


// Flips between whose turn it is and increments turn counter. Combatant A always takes priority over Combatant B.
void TurnTracker::advanceTurnTracker() {
	if (turnNumber == 0) { // At the start of combat, determine turn order
		if (combatantA->getEquippedWeapon()->getSpeed() >= combatantB->getEquippedWeapon()->getSpeed()) { // If Combatant A's speed is greater than (or equal to; priority, remember?) Combatant B's...
			combatantTurn = 0;	// ...it's Combatant A's turn
		}
		else { // If Combatant B's speed is greater...
			combatantTurn = 1;	// ...it's Combatant B's turn
		}
		turnNumber++; // It's originally set at 0.
	}

	// Run the turn for the current Combatant
	switch (combatantTurn) {
	case 0: // Combatant A
		cout << "[TURN " << turnNumber << "] It's " << (*combatantA) << "'s turn!" << endl;
		combatantA->beginTurn();
		combatantTurn = 1;	// Set it to Combatant B's turn next
		break;
	case 1:
		cout << "[TURN " << turnNumber << "] It's " << (*combatantB) << "'s turn!" << endl;
		combatantB->beginTurn();
		combatantTurn = 1;	// Set it to Combatant A's turn next
		break;
	}
	turnNumber++;	// Increment the turn counter
}