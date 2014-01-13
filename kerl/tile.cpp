/*
 * tile.cpp
 *
 *  Created on: Nov 7, 2012
 *      Author: Bloodnamed
 */

#include "tile.h"

#include <cstdlib>
#include <stdlib.h>
#include <stdexcept>
#include <sstream>

#include "actor.h"
#include "Item.h"

tile::tile() {
	actor_t = NULL;
	tile_t = tiletype::NONE;
	tile_s = ncstring();
	posX = 0;
	posY = 0;
	occupied = false;
	item = NULL;
}

tile::~tile() {
}

ncstring tile::toString(bool vis) const {
	if(vis){
		if(actor_t != NULL){
			return actor_t->toNCString(); // print actors only if in vision range
		}
	}
	if(item != NULL){
		return item->toNCString(); // print item, even when not in vision range, but has been explored
	}
	return tile_s;
}

tile& tile::setType(tiletype::t_type t) {
	tile_t = t;
	tile_s.update(t);
	return *this;
}

bool tile::setActor(actor* a){
	if(occupied == false) {
		occupied = true;
		actor_t = a;
		return true;
	}
	return false;
}

bool tile::removeActor(){
	if(occupied == true) {
		occupied = false;
		actor_t = NULL;
		return true;
	}
	return false;
}

actor* tile::getActor() {
	return actor_t;
}

void tile::setStairs(int s){
	switch(s) {
	case -1://no stairs
		if(tile_t == tiletype::STAIRSUP || tile_t == tiletype::STAIRSDOWN) {
			setType(tiletype::RUBBLE);
		}
		break;
	case 0://upstairs
		setType(tiletype::STAIRSUP);
		break;
	case 1://downstairs
		setType(tiletype::STAIRSDOWN);
		break;
	}
}

void tile::setPos(int x, int y){
	posX = x;
	posY = y;
}

tiletype::t_type tile::getType() const {
	return tile_t;
}

bool tile::getStairs() const {
	return tile_t == tiletype::STAIRSUP || tile_t == tiletype::STAIRSDOWN;
}

bool tile::isUpStairs() const {
	return tile_t == tiletype::STAIRSUP;
}

bool tile::isDownStairs() const {
	return tile_t == tiletype::STAIRSDOWN;
}

bool tile::isEmpty() const {
	return !occupied;
}

bool tile::isOccupied() const {
	return occupied;
}

bool tile::isWall() const {
	switch(tile_t){
	case tiletype::TLROOMCORNER:
	case tiletype::TRROOMCORNER:
	case tiletype::BLROOMCORNER:
	case tiletype::BRROOMCORNER:
	case tiletype::VROOMWALL:
	case tiletype::HROOMWALL:
		return true;
	default:
		return false;
	}
}


bool tile::isFloor() const {
	switch(tile_t){
	case tiletype::ROOMFLOOR:
		return true;
	default:
		return false;
	}
}

bool tile::isSolid() const {
	switch(tile_t){
	case tiletype::SOLIDWALL:
		return true;
	default:
		return false;
	}
}

bool tile::isStair() const {
	switch(tile_t){
	case tiletype::STAIRSUP:
	case tiletype::STAIRSDOWN:
		return true;
	default:
		return false;
	}
}

//Returns true if this tile has nothing special about it and is the same as a "default" tile
bool tile::isDefault() const {
	return actor_t == NULL && occupied == false;
}

void tile::setItem(Item* _item){
	if(item == NULL && occupied == false && tile_t == tiletype::ROOMFLOOR){
		item = _item;
	} else {
		// fix memory leak; maybe this belongs elsewhere
		// if setItem is used in places other than world::poulateItems(), delete _item may be dangerous
		delete _item;
		std::stringstream stream;
		stream << "Could not insert item into tile";
		if(item != NULL){
			stream << " (item already exists here)";
		} else if(occupied){
			stream << " (tile is occupied)";
		} else if(tile_t != tiletype::ROOMFLOOR){
			stream << " (tile is not a floor)";
		}
		throw std::invalid_argument(stream.str());
	}
}

Item* tile::takeItem(){
	if(item != NULL){
		Item* temp = item; // transfer ownership of item
		item = NULL;
		return temp;
	}
	return NULL;
}




