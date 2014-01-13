/*
 * creature.cpp
 *
 *  Created on: 2012/11/09
 *      Author: Bernhard Strauss
 *      		Bloodnamed
 */

#include "creature.h"
#include "rngke.h"
#include "world.h"

using namespace std;


creature::creature(int x, int y, int z, int _id, world* _theWorld, actortype::a_type _type, std::string messageName) :
		actor(x, y, z, _id, 4, _theWorld, _type, messageName) { // creature given sight radius of 4

}

creature::~creature() {
}

//bool creature::canMove(const int& type) const {
//	return (type == tiletype::ROOMFLOOR || type == tiletype::STAIRSUP || type == tiletype::STAIRSDOWN);
//}

void creature::move(int x, int y, int z) {
	posX = x;
	posY = y;
	posZ = z;
}

char creature::toChar() const {
	return 'c';
}

/**
 * Performs the actions necessary for an AI controlled creature's turn.
 *
 * @return A boolean whether to quit the game.
 */
bool creature::takeTurn() {
	bool moved = false;
	int tries = 0;
	while(!moved && tries < 5) {
		int x = rngke::uniform(-1, 1); //Random X movement
		int y = rngke::uniform(-1, 1); //Random Y movement
		int n_x = getX()+x;//new x
		int n_y = getY()+y;//new y
		//Make sure movement isn't out of bounds.
		if(n_x >= 0 && n_x <= theWorld->getWidth() && n_y >= 0 && n_y <= theWorld->getHeight()) {
			level& clevel = theWorld->getArea(getZ());
			//Check to see the tile is empty and enterable.
			if( (clevel.getTile(n_x,n_y).isEmpty() &&
					 canMove(clevel.getTile(n_x,n_y).getType()))
					|| (this == clevel.getTileRef(n_x,n_y).getActor())) // check if moving to the same tile
			{ //If the spot is empty and the creature can move there
//				(lev.getTile(x,y).isEmpty())
//										// for if the player "skips" and the tile has a pointer to the player
//										|| (p == dynamic_cast<player*>(lev.getTileRef(x,y).getActor()))
				moveActor(n_x, n_y, getZ());
				moved = true;
			}//Otherwise get a new direction.
			else ++tries;
		} else ++tries;
	} // end of while
	return false;
}

