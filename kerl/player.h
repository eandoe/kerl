#ifndef PLAYER_H_
#define PLAYER_H_

#include "actor.h"
#include "pfunctions.h"

class spiralfov;
class pet;

class player : public actor {
public:
	enum playerclass {
		none,
		fighter, archer, mage,
		barbarian, masteratarms, paladin,
		ranger, rogue, assassin,
		cleric, wizard, druid
	};
	player(const std::string& n, playerclass c, int id, world* world, actortype::a_type _type); // the world will be deleted by gamemanager
	virtual ~player();
	virtual bool takeTurn();
	bool act();
	void move(int x, int y, int z);
	pfunctions::playeraction getAction();
	bool doAction(pfunctions::playeraction a);
	virtual char toChar() const;
	std::string enumToString() const;
	std::string getName() const;
	int getStrength() const;
	int getDexterity() const;
	int getConstitution() const;
	int getIntelligence() const;
	int getWisdom() const;
	int getCharisma() const;
	int getXP() const;
	int getHunger() const;
	std::string getHungerStatus() const;
	int getLevel() const;
	bool isWaiting() const;
	void decrementWait();
	void increaseWait(int);
	void printHUD() const;
	void givePet(pet*);
	void displacePet();
private:
	playerclass pClass;
	std::string name;
	int strength;
	int dexterity;
	int constitution;
	int intelligence;
	int wisdom;
	int charisma;
	int xp;
	int level;
	int hunger;
	std::string hungerStatus;
	void updateHungerStatus();
	int waiting;
	spiralfov* spf; // object that performs spiral vision discovery
	pet* myPet;
};
#endif /* PLAYER_H */
