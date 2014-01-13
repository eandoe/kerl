/*
 * level.h
 *
 *  Created on: 2012/12/01
 *      Author: Bernhard Strauss
 *      		Bloodnamed
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "tile.h"
#include "tiletype.h"

class level {
public:
//	level();
	level(int x, int y);
	virtual ~level();
	typedef std::vector<std::vector<tile> > tilestruct;
	void setTiles(int x, int y, const tilestruct& newTiles);
	void setTile(int x, int y, const tile& newTile);
	tile getDefault(tiletype::t_type);
	tile& getTileRef(int x, int y);
	const tile& getTile(int x, int y) const;
	int getHeight() const;
	int getWidth() const;
	static std::vector<tile> init_tiledefaults();
	bool getOcclusion(int x, int y);
	double distance(int x, int y, int x2, int y2);
private:
	int height;
	int width;
	tilestruct tiles;
	static std::vector<tile> tiledefaults;
};

#endif /* LEVEL_H_ */
