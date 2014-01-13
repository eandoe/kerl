/*
 * actor.cpp
 *
 * Base class for all interactive entities in the game
 *
 *  Created on: 2012/11/09
 *      Author: Bernhard Strauss
 *      		Bloodnamed
 */

#include "actor.h"

#include <iostream>
#include <stdexcept>

#include "world.h"

using namespace std;

/**
 * Default constructor for actors undefined to keep class invariant.
 */

/**
 * Specific constructor for actors.  Includes all setup information.
 *
 * @param x,y,z - Coordinate location in the world.
 * @param a_id - ID to assign to the actor.
 * @param play - Boolean for player vs AI distinction.
 * @param srad - Standard sight radius for actor.
 */
actor::actor(int x, int y, int z, int a_id, int srad, world* _world, actortype::a_type _type, std::string messageName) :
		type(_type), nturn(1), posX(x), posY(y), posZ(z), sradius(srad), theWorld(_world),
		playermap(pmap(_world, z)), inventory(messageName)
{
//	astr = ncstring(); // default constructed
//	type = isplayer ? actortype::HUMAN : actortype::TEST;
	astr.update(type); // instantiate the char to be shown on screen for the actor
	id = a_id;
	hp = hpmax = mp = mpmax = 1;

	// place actor in the world
	tile& destination = theWorld->getArea(z).getTileRef(x, y); // tile alias
	if(destination.isDefault()){
		destination.setActor(this);
	} else {
		throw std::invalid_argument("Trying to place actor on occupied tile."); // maybe this belongs in tile::setActor
	}
}

/**
 * Destructor method
 */
actor::~actor() {
}

/**
 * Returns the ncstring representation of the actor
 */
const ncstring& actor::toNCString() const {
	return astr;
}

/**
 * Returns whether the actor can move into the tile type.
 */
bool actor::canMove(const int& type) const {
	return (type == tiletype::ROOMFLOOR || type == tiletype::STAIRSUP || type == tiletype::STAIRSDOWN);
}

void actor::moveActor(int x, int y, int z) {
//	if(isplayer) {
//		theworld->getArea(a->getZ())->removeVis(a->getX(), a->getY(), a->getSRadius());
//	}
	//TODO: not very efficient assigning of default tile
	tile& oldt = theWorld->getArea(getZ()).getTileRef(getX(), getY());
	tile& dest = theWorld->getArea(z).getTileRef(x, y);
	pickUpItem(dest);
	oldt.removeActor();
	if(oldt.isDefault()){
		tile t;
		t.setType(oldt.getType());
		//if(turn != 1) delete oldt;
	//TODO: first turn the player is on a default tile, shouldn't be like this
		theWorld->getArea(getZ()).setTile(getX(), getY(),t);
	}
	if(dest.isDefault()){
		tile t;
		t.setType(dest.getType());
		t.setActor(this);
		theWorld->getArea(z).setTile(x, y, t);
	}
	else theWorld->getArea(z).getTileRef(x,y).setActor(this);
//	if(isplayer) {
		//Perform exploration record keeping.
//		vision::exploreLight(x, y, a->getSRadius(), a->getMapRef(),
//				theworld->getArea(z));
		// moved call to beginning of playerTurn, so creature positions are in sync with vision
//		spf->FOV_SpiralPath(theworld->getArea(z),  a->getMapRef(), x, y, z, 4, 0, 3.14159265*2, 0, 1);
//	}

	move(x, y, z);
}

void actor::pickUpItem(tile& _tile){
	Item* _item = _tile.takeItem();
	if(_item != NULL){
		inventory.addItem(_item);
	}
}


/**
 * Returns whether the actor is a player.
 */
//bool actor::isPlayer() const {
//	return isplayer;
//}

/**
 * Sets the next turn for the actor.
 */
//void actor::setNTurn(int i) {
//	nturn = i;
//}

/**
 * Gives a map to a character
 */
//void actor::giveMap(const pmap& mop) {
//	//TODO Implement map sharing.
//	playermap = mop;
//}

int actor::getID() const {return id;}
int actor::getX() const {return posX;}
int actor::getY() const {return posY;}
int actor::getZ() const {return posZ;}
int actor::getHP() const {return hp;}
int actor::getHPMax() const {return hpmax;}
int actor::getMP() const {return mp;}
int actor::getMPMax() const {return mpmax;}
int actor::getNTurn() const {return nturn;}
int actor::getSRadius() const {return sradius;}
const pmap& actor::getMap() const {return playermap;}
pmap& actor::getMapRef() {return playermap;}
