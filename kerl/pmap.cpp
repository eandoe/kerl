/*
 * pmap.cpp
 *
 *  Created on: Dec 5, 2012
 *      Author: Bloodnamed
 */

#include "pmap.h"

#include <stdlib.h>

#include "world.h"

/**
 *
 */
pmap::pmap(world* theWorld, int mc) :
			x(theWorld->getWidth()), y(theWorld->getHeight()), z(theWorld->getDepth()),
			clevel(mc), vision(theWorld->getHeight()) {
	mop = std::vector<std::vector<std::vector<ncstring> > >(z);
	for(int k = 0; k < z; ++k) {
		mop[k] = std::vector<std::vector<ncstring> >(y);
		for (int j = 0; j < y; ++j) {
			mop[k][j] = std::vector<ncstring>(x);
		}
	}

//	vision = new bool*[y];
	for (int i = 0; i < y; ++i) {
		vision[i] = std::vector<bool>(x);
		for (int j = 0; j < x; ++j) {
			vision[i][j] = false;
		}
	}
}

/**
 *
 */
pmap::~pmap() {
	// TODO Auto-generated destructor stub
}

/**
 *
 */
void pmap::setCLevel(int mz) {
	if(mz < z) {
		clevel = mz;
	}
}

/**
 * This is called in vision::exploreLight when the tile becomes explored
 */
void pmap::insert(int mx, int my, ncstring str) {
	mop[clevel][my][mx] = str; // copied into mop
}

/**
 *
 */
void pmap::remove(int mx, int my) {
//	mop[clevel][my][mx] = NULL;
}

/**
 *
 */
ncstring pmap::toString(int mx, int my) const {
	return mop[clevel][my][mx];
}

void pmap::setVisible(int x, int y, bool vis) {
	vision[y][x] = vis;
}

bool pmap::isVisible(int x, int y) const {
	return vision[y][x];
}

int pmap::getX() const {return x;}
int pmap::getY() const {return y;}
int pmap::getZ() const {return z;}
