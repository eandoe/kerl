/*
 * actortype.h
 *
 *  Created on: Nov 28, 2012
 *      Author: Bloodnamed
 */

#ifndef ACTORTYPE_H_
#define ACTORTYPE_H_

class actortype {
public:
	static const char* actor_t[];
	static const char* wactor_t[];
	enum a_type {
		HUMAN,
		ELF,
		TEST,
		NONE,
	};

	static const char* toChar(actortype::a_type a);
	static const char* toString(actortype::a_type a);
};

#endif /* ACTORTYPE_H_ */
