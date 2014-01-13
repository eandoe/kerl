/*
 * Item.h
 *
 *  Created on: Jan 29, 2013
 *      Author: peterryszkiewicz
 */

#ifndef ITEM_H_
#define ITEM_H_

#include "ncstring.h"

class Inventory;

class Item {
public:
//	enum Item_type {
//		WEAPON, ARMOR, RING, POTION, GOLD
//	};
	Item();
	virtual ~Item();
	virtual void equip();
	virtual void use();
	virtual ncstring toNCString() const=0;
	virtual std::string toMessageString() const=0;
	virtual void addToInventorySideEffect(Inventory*) {}; // must call in Inventory::addItem() (maybe there's a more elegant way)

	bool operator<(const Item& it) const {return (id < it.id);} // required to insert in set of items in inventory

protected:
	int id;
//	static ncstring ncstr;
private:
	static int numOfItems;
};

class Gold : public Item {
public:
	Gold(int value);
	virtual ~Gold(){}
	virtual ncstring toNCString() const;
	virtual std::string toMessageString() const;
	virtual void addToInventorySideEffect(Inventory*); // must call in Inventory::addItem() (maybe there's a more elegant way)
private:
	int value;
//	static ncstring ncstr; // same ncstring for all objects of type Gold
//	static std::string msgString;
};

#endif /* ITEM_H_ */
