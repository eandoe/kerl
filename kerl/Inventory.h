/*
 * Inventory.h
 *
 *  Created on: Jan 29, 2013
 *      Author: peterryszkiewicz
 */

#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <set>
#include <string>

class Item;

class Inventory {
public:
	Inventory(std::string = "");
	virtual ~Inventory();
	void addItem(Item*);
	void addGold(int); // add to gold count
	int getGold() const {return gold;}
private:
	void printAddedItem(Item*) const;

	std::string nameMessage;
	int gold; // gold representation
	int size;
	int capacity;
	typedef std::set<Item*> Items;
	Items items;
};


#endif /* INVENTORY_H_ */
