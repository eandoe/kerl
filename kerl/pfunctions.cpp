/*
 * pfunctions.cpp
 *
 *  Created on: Dec 8, 2012
 *      Author: Bloodnamed
 */

#include "pfunctions.h"

pfunctions::pfunctions() {
	// TODO Auto-generated constructor stub

}

pfunctions::~pfunctions() {
	// TODO Auto-generated destructor stub
}

bool pfunctions::isMove(playeraction a) {
	return (a >= 0 && a <=8);
}
