/*
 * Inventory.cpp
 *
 *  Created on: Jan 29, 2013
 *      Author: peterryszkiewicz
 */

#include "Inventory.h"

#include <stdexcept>
#include <sstream>

#include "Item.h"
#include "gamemanager.h"

Inventory::Inventory(std::string name) // name defaults to empty string
			: nameMessage(name), gold(0), size(0), capacity(20) { // starting capacity
	// Items start empty
}

Inventory::~Inventory(){
	for(Items::iterator iter = items.begin(); iter != items.end(); ++iter){
		delete *iter;
	}
}

void Inventory::addItem(Item* _item){
//	if(_item.getType() == Item::GOLD){
	if(_item != NULL){
		// _item must be put somewhere, or there is a memory leak
		if(size > capacity){
//			return; // state error on terminal, or become encumbered or something
		}
//		if(_item->getGoldValue() > 0){
//			// add message to terminal about picking up x amount of gold
//			gold += _item->getGoldValue(); // take gold value
//			delete _item; // gold "item" no longer needed
//			return; // don't gold "item" add to inventory
//		}
		// consider how to just add only gold value to inventory and delete the gold item, instead of also adding the gold item to inventory
		_item->addToInventorySideEffect(this);
		printAddedItem(_item);
		items.insert(_item);
	}

}

// add to gold count
void Inventory::addGold(int val){
	if(val > 0){
		gold += val;
	} else throw std::invalid_argument("Trying to add negative gold to inventory.");
}


void Inventory::printAddedItem(Item* _item) const{
	display& disp = gamemanager::getDisplay();
	std::stringstream sstr;
	sstr << nameMessage << " picked up " << _item->toMessageString() << ".";
	disp.appendString(sstr.str()); // sometimes causes stray characters in terminal when message too long; find general purpose solution
}


