/*
 * tiletype.h
 *
 *  Created on: Nov 28, 2012
 *      Author: Bloodnamed
 */

#ifndef TILETYPE_H_
#define TILETYPE_H_

class tiletype {
public:
	static const char* tile_t[];
	static const char* wtile_t[];
	enum t_type {
		STAIRSUP, STAIRSDOWN,
		ROOMFLOOR, HROOMWALL, VROOMWALL, TLROOMCORNER, TRROOMCORNER, BLROOMCORNER, BRROOMCORNER, SOLIDWALL, RUBBLE,
		NONE
	};

	static const char* toChar(tiletype::t_type t);
	static const char* toString(tiletype::t_type t);
};

#endif /* TILETYPE_H_ */
