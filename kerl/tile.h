/*
 * tile.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Bloodnamed
 */

#ifndef TILE_H_
#define TILE_H_

#include "ncstring.h"
#include "tiletype.h"

class actor;
class Item;

class tile {
public:
	tile();
	virtual ~tile();
	void setPos(int x, int y);
	tile& setType(tiletype::t_type t); // allow chainable methods
	void setStairs(int s);
	bool setActor(actor* a);
	bool removeActor();
	ncstring toString(bool vis) const;
	tiletype::t_type getType() const;
	bool getStairs() const;
	bool isUpStairs() const;
	bool isDownStairs() const;
	bool isEmpty() const;
	bool isOccupied() const;
	bool isWall() const;
	bool isFloor() const;
	bool isSolid() const;
	bool isStair() const;
	actor* getActor();
	bool isDefault() const;
	void setItem(Item*);
//	bool hasItem() const {return item != NULL;}
	Item* takeItem();
private:
	int posX, posY;
	bool occupied;
	actor* actor_t;
	tiletype::t_type tile_t;
	ncstring tile_s;
	Item* item; // maybe have multiple items on one tile in the future
//	void removeItem() {item = NULL;};
};

#endif /* TILE_H_ */
