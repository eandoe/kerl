/*
 * actor.h
 *
 * Base class for all interactive entities in the game
 *
 *  Created on: 2012/11/09
 *      Author: Bernhard Strauss
 *      		Bloodnamed
 */

#ifndef ACTOR_H_
#define ACTOR_H_

#include "actortype.h"
#include "ncstring.h"
#include "pmap.h"
#include "Inventory.h"
//#include "tile.h"

class tile;
class world;

class actor {
public:
	actor(int x, int y, int z, int a_id, int srad, world* _world, actortype::a_type _type, std::string messageName="");
	virtual ~actor();
	virtual bool takeTurn()=0; // must be implemented by inherited classes
	virtual bool canMove(const int& type) const;
	virtual void move(int x, int y, int z)=0;
	void pickUpItem(tile&);
//	virtual void giveMap(const pmap& mop);
	virtual char toChar() const=0;
	virtual void moveActor(int x, int y, int z);
//	virtual void setNTurn(int i);
	const ncstring& toNCString() const;
	int getX() const;
	int getY() const;
	int getZ() const;
	int getHP() const;
	int getHPMax() const;
	int getMP() const;
	int getMPMax() const;
	int getID() const;
	int getNTurn() const;
	int getSRadius() const;
	int getGold() const {return inventory.getGold();}
	const pmap& getMap() const;
	pmap& getMapRef(); // modifiable pmap
protected:
	ncstring astr; // char representation of actor on screen
	actortype::a_type type;
	int nturn;
	int posX, posY, posZ;
	int id;
	int sradius;
	int hp;
	int hpmax;
	int mp;
	int mpmax;
	world* theWorld;
	pmap playermap;
	Inventory inventory;
};
#endif /* ACTOR_H */
