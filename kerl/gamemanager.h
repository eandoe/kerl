/*
 * gamemanager.h
 *
 *  Created on: Nov 18, 2012
 *      Author: Bloodnamed
 */

#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

#include <list>

//#include "pfunctions.h"
#include "display.h"
//#include "spiralfov.h"

class actor; // forward declarations
class player;
class creature;
class pet;
class world;

class gamemanager {
public:
	gamemanager();
	virtual ~gamemanager();
	bool run();
	static display& getDisplay() {return disp;} // inline function calls
	static bool getArrowKeysEnabled() {return arrowKeysEnabled;}
	static bool getWidechars() {return widechars;}
	static bool getAllowResize() {return allowresize;}
private:
	std::list<actor*> activeActors;
	int worldXFactor;
	int worldYFactor;
	int worldZFactor;
	int termX;
	int termY;
	int cid;
	world* theworld;
	int turn;

	static display disp;
	static bool arrowKeysEnabled;
	static bool widechars;
	static bool allowresize;

	player* makePlayer();
	creature* makeCreature();
	pet* makePet(player* p);
	bool actorTurn(actor* a);
	bool menu();
	bool newGame();
	bool loadGame();
	bool options();
	void worldOptions();
	void displayOptions();
	void controlsOptions();
};


#endif /* GAMEMANAGER_H_ */
