/*
 * world.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Bloodnamed
 */

#ifndef WORLD_H_
#define WORLD_H_

#include "level.h"
#include "area.h"

class world {
public:
//	world(); // not recommended to keep class invariant
	world(int xfactor, int yfactor, int zfactor);
	virtual ~world();
	const char*** toString();
	level& getArea(int z);
	level& getCurrentArea();
//	area* generateLevel(int type);
	static int getBaseHeight();
	static int getBaseWidth();
	int getHeight() const;
	int getWidth() const;
	int getDepth() const;
	static const int baseAreaHeight = 31;
	static const int baseAreaWidth = 61;
private:
	std::vector<int> generateCons(areatype::a_type a1, areatype::a_type a2, int length);
	//world dimensions
	int areaYFactor;
	int areaXFactor;
	int levelz;
	int areaWidth;
	int areaHeight;
	std::vector<level> levels;
	areagen agen;
	typedef std::vector<std::vector<std::vector<area> > > areastruct;
	areastruct worldareas;
	int populate(world* w);
	void populateItems();
	struct Point {
		int x, y, z;
		Point(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {} // constructor
	};
	typedef std::vector<Point> Points;
	Points roomFloorPoints;
};

#endif /* WORLD_H_ */
