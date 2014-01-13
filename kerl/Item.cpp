/*
 * Item.cpp
 *
 *  Created on: Jan 29, 2013
 *      Author: peterryszkiewicz
 */

#include "Item.h"

#include <sstream>

#include "Inventory.h"

Item::Item() : id(numOfItems++) {

}

//Item::Item(Item_type _type)
//		: id(numOfItems++), type(_type) {
//}

Item::~Item(){
}

void Item::equip(){

}

void Item::use(){

}

int Item::numOfItems = 0;


//Gold::Gold(int _value) : Item(GOLD), value(0) {
Gold::Gold(int _value) : Item(), value(0) {
	if(_value > 0){
		value = _value;
	} else {
		// error, nonpositive gold item
	}
}

ncstring Gold::toNCString() const {
	return ncstring('$', "$", true, ncstring::YELLOW);
}

std::string Gold::toMessageString() const {
	std::stringstream sstr;
	sstr << value << " gold";
	return sstr.str();
}

void Gold::addToInventorySideEffect(Inventory* inv) {
	inv->addGold(value);
}
