#include "Weapon.h"

int maximum(int a, int b) { // I am not going to include another header for max()
	return (a > b) ? a : b;
}

/*****************
 * WeaponEffects *
 *****************/

// WeaponEffect default constructor. Sets string values to "" and the function to nullptr.
WeaponEffect::WeaponEffect() {
	effect = nullptr;
	description = "";
	target = "";
}

/* WeaponEffect normal constructor.
 * @param `void (*func)(Weapon*, Entity*)` effect - The effect to trigger. The first parameter is the parent Weapon and the second is the targe Entity.
 * @param `string` description - A description of the effect.
 * @param `string` target - Defines whom the effect targets (either "self" or "opponent")
 */
WeaponEffect::WeaponEffect(void (*effect)(Weapon*, Entity*), string description, string target) {
	this->effect = effect;
	this->description = description;
	this->target = target;
}

// This WeaponEffect heals the wielder for 5% of its maxHealth.
void vampirismEffect(Weapon* parentWeapon, Entity* target) {
	if ((rand() % 100) < 25) { // 25% chance to proc
		cout << (*parentWeapon) << "'s effect, Vampirism, activates to heal its user!" << endl;
		target->takeDamage(target->getMaxHP() * -0.05); // Negative damage is the same as healing
	}
}

// This WeaponEffect has a 15% chance to activate a GigaCrit, which deals max damage * 4.
void gigaCritEffect(Weapon* parentWeapon, Entity* target) {
	if ((rand() % 100) < 15) { // 15% chance to proc
		cout << "A holy light shines upon " << (*parentWeapon) << "! A GigaCrit smites " << (*target) << "!" << endl;
		target->takeDamage(parentWeapon->damageBounds[1] * 4);
	}
}

// This WeaponEffect has a 5% chance to increase the stats of the weapon itself by 1d10.
void selfImprovementEffect(Weapon* parentWeapon, Entity* target) {
	if ((rand() % 100) < 5) { // 5% chance to proc
		cout << (*parentWeapon) << " automatically sharpens itself! The effect of Self Improvement has increased " << (*parentWeapon) << "'s stats!" << endl;
		parentWeapon->upgradeWeapon((rand() % 10) + 1);
	}
}

// Initialize all WeaponEffects
// For some odd reason, an array uses the default constructor, but vectors use the specified constructor??? wack
vector<WeaponEffect> weaponEffectsArray = {
	WeaponEffect(vampirismEffect, "Has a 25% chance to heal the weapon's owner for 5% of their max health on-hit. (Vampirism)", "self"),
	WeaponEffect(gigaCritEffect, "Has a 15% chance to deal max damage times four to a target on-hit. (GigaCrit)", "opponent"),
	WeaponEffect(selfImprovementEffect, "Has a 5% chance to upgrade itself 1d10 times. (Self Improvement)", "self")
};

/*********************
 * Methods of Weapon * 
 *********************/

// Randomly generates a Weapon with a worth of `worth`.
Weapon::Weapon(int worth) {
	// Randomly generate the name
	string adjective = weaponAdjectives[rand() % 10];
	string type = weaponTypes[rand() % 10];
	string descriptor = weaponDescriptors[rand() % 10];

	name = "The " + adjective + type + descriptor;
	this->worth = 0;

	bearer = nullptr;

	// Starter stats -- Weapon stats will never go below this
	hitPercent = 50;
	critPercent = 0;
	damageBounds[0] = 0; // Lower bound
	damageBounds[1] = 1; // Upper bound
	armor = 0;
	speed = 1;

	// Generate stats -- We randomly assign points (based on worth) to each of the stats (this is also weighted). See Weapon::upgradeWeapon for more.
	upgradeWeapon(worth);
}

// Gets the name of the Weapon.
string Weapon::getName() {
	return name;
}

// Gets the armor of the Weapon.
int Weapon::getArmor() {
	return armor;
}

// Gets the speed of the Weapon.
int Weapon::getSpeed() {
	return speed;
}

// Gets the worth of the Weapon (explained in Weapon.h)
int Weapon::getWorth() {
	return worth;
}

// Sets the bearer of the Weapon (i.e. the Entity which has this weapon equipped). This is used for effects.
void Weapon::setBearer(Entity* target) {
	bearer = target;
}

// Pretty-prints Weapon statistics to cout.
void Weapon::showStats() {
	int midWidth = maximum((10 + name.length()) / 2, 18); // Align the middle divider to the center

	cout << "Stats for " << name << endl;
	cout << setw(10. + name.length()) << setfill('-') << right << "-" << endl; // Make a cool underline below text
	cout << setfill(' '); // reset fill (already right-aligned)
	cout << setw(midWidth) << "Hit % | " << left << hitPercent << "%" << endl << right; // Set width and put in descriptor text; then, align left and put in the statistic; finally, put in a newline and align back to right
	cout << setw(midWidth) << "Critical Hit % | " << left << critPercent << "% (Critical hits deal twice the damage)" << endl << right;
	cout << setw(midWidth) << "Damage | " << left << damageBounds[0] << "-" << damageBounds[1] << " DMG" << endl << right;
	cout << setw(midWidth) << "Armor | " << left << armor << " (Armor reduces damage)" << endl << right;
	cout << setw(midWidth) << "Speed | " << left << speed << " (Speed determines how fast you attack and who attacks first)" << endl << right;

	// On-hit effect descriptions
	cout << setw(0) << endl; // Reset cout modifiers, also create a newline for niceness
	if (onHitEffects.size() > 0) {
		for (unsigned int i = 0; i < onHitEffects.size(); i++) {
			cout << "Effect: " << onHitEffects.at(i).description << endl;
		}
	}
}

// Makes an attack with the Weapon at the target Entity.
void Weapon::makeAttack(Entity* target) {
	if (((rand() % 100) + 1) < hitPercent) { // If it hits...
		int dmgRange = damageBounds[1] - damageBounds[0];
		int damage = (rand() % dmgRange) + damageBounds[0] + 1; // Record damage

		if (((rand() % 100) + 1) < critPercent) { // If it crits...
			damage *= 2;						  // Multiply the damage by two
			cout << "CRIT! " << name << " comes down with a mighty strike!" << endl;
			target->takeDamage(damage);
		}
		else { // Normal hit (non-crit)
			cout << name << " lands a direct blow on its foe!" << endl;
			target->takeDamage(damage);
		}

		// On-hit effects
		for (unsigned int i = 0; i < onHitEffects.size(); i++) {
			if (onHitEffects[i].target == "self") { // If it affects the bearer...
				onHitEffects[i].effect(this, bearer);
			} 
			else { // If it affects the target being attacked...
				onHitEffects[i].effect(this, target);
			}
		}
	}
	else { // On miss
		cout << name << " completely whiffed its attack..." << endl;
	}
}

// Adds worth to a weapon, which increases stats. Stats are randomly increased (with weighting) for a number of times equal to `upgradeAmount`. Also increases worth by `upgradeAmount`.
void Weapon::upgradeWeapon(int upgradeAmount) {
	worth += upgradeAmount;
	for (int i = 0; i < upgradeAmount; i++) {
		int randChoice = rand() % 100;	// Choose a random percentage
		if (randChoice < 40) {							// (0-39%) Increase the lower damage bound and range by 1d20
			int range = damageBounds[1] - damageBounds[0] + (rand() % 20) + 1;
			damageBounds[0] += (rand() % 20) + 1;
			damageBounds[1] = damageBounds[0] + range;
		}
		else if (randChoice < 65) {						// (45-64%) Increase to-hit chance by 5-10%
			if (hitPercent >= 100) { // If we already maxed out the hitpercent
				i--; // "Refund" a point
				continue; // Try again
			}
			hitPercent += (rand() % 6) + 5;
		}
		else if (randChoice < 75) {						// (65-74%) Increase speed by 1d8
			speed += (rand() % 8) + 1;
		}
		else if (randChoice < 85) {						// (75-84%) Increase armor by 1d20 * 2
			armor += ((rand() % 20) + 1) * 2;
		}
		else if (randChoice < 95) {						// (85-94%) Increase critical hit chance by 5-10%
			if (critPercent >= 100) { // If we already maxed out the critpercent
				i--;
				continue;
			}
			critPercent += (rand() % 6) + 5;
		}
		else if (randChoice < 99) {						// (95-98%) Increase all stats by 1d4
			hitPercent += (rand() % 4) + 1;
		}
		else if (randChoice < 100) {					// (99-100%) Add an on-hit effect
			onHitEffects.push_back(weaponEffectsArray.at(rand() % 3)); // Add a random effect to the onHitEffects list
		}
	}
}

/***************************
 * Other Friends of Weapon *
 ***************************/

// Streams the name of the Weapon to an ostream (alias of Weapon::getName()).
ostream& operator<<(ostream& out, Weapon& weapon) {
	out << weapon.name;
	return out;
}
