/*
 * creature.h
 *
 *  Created on: 2012/11/09
 *      Author: Bernhard Strauss
 *      		Bloodnamed
 */

#ifndef CREATURE_H_
#define CREATURE_H_

#include "actor.h"

class creature : public actor {
public:
	creature(int x, int y, int z, int id, world* theWorld, actortype::a_type _type, std::string messageName="");
	virtual ~creature();
//	virtual bool canMove(const int& type) const;
	virtual void move(int x, int y, int z);
//	unsigned char toChar(); // why unsigned?
	virtual char toChar() const;
	virtual bool takeTurn();
};
#endif /* CREATURE_H */
