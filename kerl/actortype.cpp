/*
 * actortype.cpp
 *
 * Enum wrapper class for actor distinctions.
 *
 *  Created on: Nov 28, 2012
 *      Author: Bloodnamed
 */

#include "actortype.h"

// ASCII Character assignments
const char* actortype::actor_t[] = {	"@\0",
								"@\0",
								"c\0",
								"*\0",
								" \0",
								" \0"	};
// Unicode Character assignments
const char* actortype::wactor_t[] = {	"@\0  \0",
								"@\0  \0",
								"c\0  \0",
								"*\0  \0",
								"    \0",
								"    \0"	};
/**
 * Returns the ASCII character representation of an actor
 */
const char* actortype::toChar(actortype::a_type a) {
	return actor_t[a];
}

/**
 * Returns the Unicode character representation of an actor
 */
const char* actortype::toString(actortype::a_type a) {
	return wactor_t[a];
}
