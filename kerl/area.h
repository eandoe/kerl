/*
 * area.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Bernhard Strauss
 *      		Bloodnamed
 */

#ifndef AREA_H_
#define AREA_H_

#include <vector>

#include "tile.h"
#include "areagen.h"
#include "areatype.h"

class area {
public:
	area(int x, int y, areatype::a_type ty, areagen& ag);
	virtual ~area();
	const char*** toString();
	int getActors();
	tile& getTile(int x, int y); // tile modifiable
	int getHeight() const;
	int getWidth() const;
	typedef std::vector<std::vector<tile> > tilestruct;
	tilestruct generate();
	typedef const std::vector<int>& vecint;
	typedef const std::vector<std::vector<int> >& vecvecint;
	void setTopCons(vecint topCon);
	void setBotCons(vecint botCon);
	void setLeftCons(vecint leftCon);
	void setRightCons(vecint rightCon);
	void setUpCons(vecvecint upLoc, int num);
	void setDownCons(vecvecint downLoc, int num);
	areatype::a_type getType();
private:
	int height;
	int width;
	areatype::a_type type;
	tilestruct tiles;
	areagen agen;
	std::vector<int> topCons;
	std::vector<int> botCons;
	std::vector<int> leftCons;
	std::vector<int> rightCons;
	std::vector<int> numUDCons;
	std::vector<std::vector<int> > upCons;
	std::vector<std::vector<int> > downCons;
};
#endif /* AREA_H_ */
