/*
 * tiletype.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: Bloodnamed
 */

#include "tiletype.h"

const char* tiletype::tile_t[] = {	"<\0",
								">\0",
								".\0",
								"-\0",
								"|\0",
								"+\0",
								"+\0",
								"+\0",
								"+\0",
								"#\0",
								"\"\0",
								" \0",
								" \0"	};

const char* tiletype::wtile_t[] = {	"<\0  \0",
								">\0  \0",
								".\0  \0",
								"\u2500\0",
								"\u2502\0",
								"\u250c\0",
								"\u2510\0",
								"\u2514\0",
								"\u2518\0",
								"\u2588\0",
								"\u2591\0",
								"    \0",
								"    \0"	};

const char* tiletype::toChar(tiletype::t_type t) {
	return tile_t[t];
}

const char* tiletype::toString(tiletype::t_type t) {
	return wtile_t[t];
}
