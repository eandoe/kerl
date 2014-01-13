/*
 * areagen.h
 *
 *  Created on: Nov 13, 2012
 *      Author: Bernhard Straus
 */

#ifndef AREAGEN_H_
#define AREAGEN_H_

//#include <sstream>
#include <vector>

#include "rngke.h"
#include "tile.h"
#include "areatype.h"

class areagen {
public:
//	std::stringstream stairs;
	areagen(int x, int y);
	virtual ~areagen();
	typedef const std::vector<int>& vecint;
	typedef const std::vector<std::vector<int> >& vecvecint;
	typedef std::vector<std::vector<tile> > tilestruct;
	tilestruct generate(areatype::a_type type, vecint topCons, vecint botCons, vecint leftCons, vecint rightCons,
			vecint numUDCons, vecvecint upCons, vecvecint downCons);
private:
	int height;
	int width;
//	actor* p;
	tilestruct tiles;
//	rngke rng;
	std::vector<std::vector<int> > areastate;
	void generateEmptyRoom();
	void emptyRoomOuterWall();
	void emptyRoomSetStairs();
	void generateCave();
	void generateRoomsandHalls();
	bool makeRectRoom(int sx, int sy, int h, int w);
	bool makeCorridor(const std::vector<std::vector<int> >& roomdat, int n, int con, bool roomStart);

	/* Connectivity info
	 * edgeCons area an array of length of that edge (height/width); 0 = no connection, 1 = connection,
	 * (-n = make n connections, probably wont use here)
	 * numStairsUD is number of upstairs, number of downstairs
	 * stairslocs is a list of locations of those stairs, of length determined by numStairsUD
	 */

	std::vector<int> topCons;
	std::vector<int> botCons;
	std::vector<int> leftCons;
	std::vector<int> rightCons;
	std::vector<int> numUDCons;
	std::vector<std::vector<int> > upCons;
	std::vector<std::vector<int> > downCons;
};
#endif /* AREAGEN_H_ */
