/*
 * pet.cpp
 *
 *  Created on: Jan 26, 2013
 *      Author: peterryszkiewicz
 */

#include "pet.h"

#include <cstdlib>
#include <cmath>

#include "player.h"
#include "rngke.h"
#include "world.h"

pet::pet(int x, int y, int z, int id, world* theWorld, actortype::a_type _type, player* _player)
	: creature(x, y, z, id, theWorld, _type, "Your pet")
{
	// the pet will attempt to follow its player and help fight alongside
	if(_player != NULL)
		owner = _player;
}

pet::~pet(){
}

/**
 * Performs the actions necessary for an AI controlled pet's turn.
 *
 * @return A boolean whether to quit the game.
 */
bool pet::takeTurn() {
	using namespace std;
	bool moved = false;
	int tries = 0;
	// avoiding square roots, so using squared distances
	double distanceThresholdSq = pow(double(3), 2); // if owner if farther than 4 units away, the pet attempts to follow
	double distanceFromOwnerSq = pow(double(owner->getX() - getX()), 2) + pow(double(owner->getY() - getY()), 2);
	bool follow = distanceFromOwnerSq > distanceThresholdSq;
	int followDX = 0, followDY = 0;
	if (follow) {
		followDX = owner->getX() > getX() ? 1 : (owner->getX() < getX() ? -1 : 0); // essentially, a sign function
		followDY = owner->getY() > getY() ? 1 : (owner->getY() < getY() ? -1 : 0);
	}
	while(!moved && tries < 5) {
		// naive follow algorithm; no pathfinding involved, yet
		int dx = follow ? followDX : rngke::uniform(-1, 1); //Random X movement
		int dy = follow ? followDY : rngke::uniform(-1, 1); //Random Y movement
		int n_x = getX()+dx;//new x
		int n_y = getY()+dy;//new y
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

