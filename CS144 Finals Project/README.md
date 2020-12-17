# Ye Olde RPG
## What is this for?
Hopefully, this README should help you to understand the inner workings of Ye Olde RPG.

## First, Some Definitions
There are a few specialized words that should be described before going into depth of the game's mechanics.

* `Entity` - A class which keeps track of the stats of enemies and the player. Think of the Entity as a person or character in the game.
* `onStartEffect` (aka a `Brain`) - onStartEffects are triggered on the start of a turn. As of now, onStartEffects are only used for AI (which is why they are also referred to as Brains.).
* `Weapon` - A class which Entities use to attack each other. Every Entity has a Weapon. Weapons also have stats and WeaponEffects, which trigger On-Hit.
* `On-Hit` - A term used to refer to the moment when a Weapon successfully hits a target Entity.
* `Bearer` - A term used to refer to the Entity which has the Weapon currently equipped. A Bearer is referred to only when speaking of the relationship of the Weapon to it's parent Entity, and not the other way around.
* `WeaponEffect` - A struct which Weapons may have. WeaponEffects trigger an effect On-Hit, which may affect the Bearer or another target Entity. Also referred to as an "onHitEffect," although this can be generalized to other triggers.
* `Critical Hit` - A term used to refer to a special attack which deals 2x damage. Critical Hits are checked after a Weapon makes a successful hit. Also referred to as a Crit.
* `TurnTracker` - A class which keeps track of which Entity goes when during combat.
* `Combatant` - A term used to refer to an Entity which is in combat.


## The Structure of the Game
The game is based around battles, which are separated into turns. 

### The Basic Battle
Each turn, a Combatant makes an attack or does nothing. Essentially, this is how a turn plays out:

0. Upon starting the battle, TurnTracker decides who goes first.
1. TurnTracker triggers the Brain of the Combatant whose turn it is (if they have one; the player will never have a brain, as it is handled in `main`).
2. The current Combatant either attacks or does nothing.
	2a. If the current Combatant attacks, run RNG to see if it hits.
	2b. If it hits, run RNG to see if it Crits.
	2c. If it hits, apply damage to the enemy Combatant. (which may cause its state to change to `DEAD`)
	2d. If it hits, apply any and all WeaponEffects that the Entity's equipped Weapon may have.
3. Advance the TurnTracker, switching whose turn it is.
4. Repeat steps 1-3 until one of the combatants are in the DEAD state (or runs away, in the case of the player).

### Additional Player Actions during Battle
This is the basic structure of the game, with the only caveat that the player does not have the choice of doing nothing, but has the choice of:

* Assessing their weapon and health
* Assessing the enemy's weapon and health
* Running away (quitting the game)

### Post-Combat
After combat, the player is healed back to full. They also have the option of taking up their enemy's Weapon, replacing it with their own. Additionally, they may run away in order to quit prematurely.

After these decisions (if they have not run away), another combat begins anew with a stronger enemy with a stronger weapon.

## The Entity Class
The Entity class is the bread and butter of the game. They handle the actions of combat, as well as keep track of important stats.

### The Brain of the Entity
Firstly, let us talk about Brains. They are friend functions of the Entity class which handle the actions of the parent Entity. Here is the basic structure:

```cpp
void exampleBrain(Entity* parentEntity, Entity* enemyEntity) {
	// do stuff here...
}
```

The first parameter, `Entity* parentEntity` refers to the Entity which the brain is controlling. Conversely, the second parameter, `Entity* enemyEntity` refers to the enemy of the parent Entity.
Otherwise, there are little restrictions on what you can do with a Brain.

Brains are stored in the static `entityBrains` array of the Entity class.

### Important Methods
Most methods and properties of Entity are self-explanatory, but there are some which may need some explaining.

#### `takeDamage()`
`takeDamage()` deals damage to an Entity, subtracting that damage from their current HP. There are a few notable mechanics of this.

1. Negative damage is referred to as healing.
2. Healing may not go above the Entity's maximum HP, nor can damage lower an Entity's HP below zero.
3. If an Entity's health is put below zero, they are put in the	`DEAD` state.
4. An Entity's Weapon's armor value subtracts from the damage taken (which can completely deflect damage).
5. Armor deflection cannot heal an Entity.
6. Armor does not affect healing.

The healing rules apply to the `setHP()` method as well.

## The Weapon Class
The Weapon class is what Entities use to deal damage to other Entities. They also house a few important stats.

### Properties of A Weapon
Weapon stats and names are randomly generated (with weighting) at creation. Here are a few important properties.

#### Worth
Worth determines how many times stats have been randomly added to a Weapon. Generally, Weapons with higher worth will be stronger.

#### Damage Bounds
Determines the lower (`damageBounds[0]`) and upper (`damageBounds[1]`) limits to a Weapon's damage.

#### Hit Percent and Crit Percent
Determines how likely a weapon is to hit or Crit (out of 100%).

#### Armor
Determines how much incoming damage is subtracted when an Entity takes damage.

#### Speed
Determines who goes first in combat. Entities wielding Weapons with higher speed will go first. In the case of a tie, the player always goes first.

### The Structure of a WeaponEffect
WeaponEffects, as stated before, are a struct which handles special effects which occur On-Hit. They are structured as follows:

```cpp
WeaponEffect(
	void (*effect)(Weapon*, Entity*), 
	string description, 
	string target
)
```

The `description` describes the effect, and the `target` defines whom the WeaponEffect targets (either `self`, which is the bearer, or `opponent`, which is the opponent).

#### The `effect` Function Pointer
The meat and bones of an effect are within the `effect` function pointer. It is structured as follows:

```cpp
void exampleEffect(Weapon* parentWeapon, Entity* target) {
	// do something...
}

The `parentWeapon` refers to the Weapon which has this WeaponEffect, and the `target` is the target of the effect. Otherwise, like with Brains, there are fairly few restrictions to what an effect can do.