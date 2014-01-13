/*
 * pet.h
 *
 *  Created on: Jan 26, 2013
 *      Author: peterryszkiewicz
 */

#ifndef PET_H_
#define PET_H_

#include "creature.h"

class player;

class pet : public creature {
public:
	pet(int x, int y, int z, int i, world* theWorld, actortype::a_type _type, player*);
	virtual ~pet();
	virtual bool takeTurn();
protected:
	player* owner;
};


#endif /* PET_H_ */
