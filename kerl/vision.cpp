/*
 * vision.cpp
 *
 *  Created on: Dec 9, 2012
 *      Author: Bloodnamed
 */

#include "vision.h"

#include <map>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "level.h"
#include "pmap.h"
#include "display.h"
#include "gamemanager.h"

std::map<int, vision::rastercircle> radiusmask =
		*new std::map<int, vision::rastercircle>();

/**
 * Returns the coordinate vector for the rastercircle.
 */
std::vector<vision::coord> vision::rastercircle::getMask() {
	return circle;
}

/**
 * Returns the line vector for the rastercircle.
 */
std::vector<std::vector<vision::coord> > vision::rastercircle::getLines() {
	return lines;
}

/**
 *
 */
void vision::rastercircle::setMask(std::vector<vision::coord> t) {
	circle = t;
}

/**
 *
 */
void vision::rastercircle::setLines(std::vector<std::vector<vision::coord> > t)
{
	lines = t;
}

/**
 * This is a simple exploring method which clears the old vision mask and adds
 * a new one which is unhindered by terrain.
 *
 * @param x,y - Level coordinates.
 * @param sradius - The sight radius to be used.
 * @param m - The map object to add tile memory to.
 * @param l - The level being explored.
 */
void vision::exploreXRay(int x, int y, int sradius, pmap& m, level& l) {
	//Remove previous vision.
	removeVision(x, y, m, l);
	//rudimentary sight radius LoS
	for(int j = y-sradius; (j <= y+sradius); ++j) {
		for(int i = x-sradius; (i <= x+sradius); ++i) {
			if((i>-1)&&(i<m.getX())&&(j>-1)&&(j<m.getY())) {
				if(pow (sradius,2) > (pow (x-abs(i),2) + pow (y-abs(j),2))) {
					m.setVisible(i,j, true);//add vision
					ncstring temp = l.getTile(i,j).toString(true);
					temp.setHLight();//set highlight for output
					m.insert(i,j, temp);//add to the given map
				}
			}
		}
	}
}

/**
 * This is a complex exploring method which clears the old vision mask and adds
 * a new one.  This is very computation heavy.
 *
 * @param x,y - Level coordinates.
 * @param sradius - The sight radius to be used.
 * @param m - The map object to add tile memory to.
 * @param l - The level being explored.
 */
void vision::exploreHeavy(int x, int y, int sradius, pmap& m, level& l) {
	//Remove previous vision.
	removeVision(x, y, m, l);

	bool last_tile_blocked; // set but unused warning

	//Grab the already present rastercircle or make a new one
	vision::rastercircle temp = (radiusmask.find(sradius) != radiusmask.end())
			? radiusmask.find(sradius)->second : initRMask(sradius);
	std::vector<vision::coord> mask = temp.getMask();
	std::vector<std::vector<vision::coord> > lines = temp.getLines();
	int size = lines.size();

	for (int i = 0; i < size; ++i) {
		double _x = mask[i].x;
		double _y = mask[i].y;
		std::vector<vision::coord> line = lines[i];

		int j = -1;
		last_tile_blocked = false;
		do {
			j++;
			int dx = line[j].x;
			int dy = line[j].y;
			if( (y+dy>-1) && (y+dy<l.getHeight())
					&& (x+dx>-1) && (x+dx<l.getWidth()) )
			{
				last_tile_blocked = addVision(x+dx, y+dy, m, l);
				// Up down left and right optimizations
				if(_x == 0.0) {
					addVision(x+dx-1, y+dy, m, l);
					addVision(x+dx+1, y+dy, m, l);
				}
				else if (_y == 0.0) {
					addVision(x+dx, y+dy-1, m, l);
					addVision(x+dx, y+dy+1, m, l);
				}
				else {
					last_tile_blocked = !l.getTile(x+dx, y+dy).isFloor();
				}
			}
		} while(!(line[j].x == _x && line[j].y == _y) && !last_tile_blocked);

	}
}

/**
 * Adds vision to the specified tile in the actor's map
 *
 * @param x - X position of the tile.
 * @param y - Y position of the tile.
 * @param m - The actor's map.
 * @param l - The level the tile is on.
 */
bool vision::addVision(int x, int y, pmap& m, level& l) {
	m.setVisible(x, y, true);
	ncstring t = l.getTile(x, y).toString(true);
	t.setHLight();//set highlight for output
	m.insert(x, y, t);//add to the given map
	return !l.getTile(x, y).isFloor();
}

/**
 * Recursively removes all vision inherently related to the actor.
 *
 * @param x - X position of the actor.
 * @param y - Y position of the actor.
 * @param m - The actor's map.
 * @param l - The level the actor is on.
 */
void vision::removeVision(int x, int y, pmap& m, level& l) {
	//Reset your vision
	if(m.isVisible(x,y)) {
		//get the tile on the level
		ncstring temp = l.getTile(x,y).toString(false);
		//remove output highlighting
		temp.rmHLight();
		//add it to the map
		m.insert(x,y, temp);
		//clear the vision
		m.setVisible(x,y, false);

		//Recursively add surrounding tiles to the queue.
		if(x-1 > 0)
			removeVision(x-1, y, m, l);
		if(x+1 < l.getWidth())
			removeVision(x+1, y, m, l);
		if(y-1 > 0)
			removeVision(x, y-1, m, l);
		if(y+1 < l.getHeight())
			removeVision(x, y+1, m, l);
	}
}

/**
 * This is some of the big money.  This method will dynamically allocate masks
 * for circles rendered in grid.
 *
 * @param radius - The radius of the raster circle to be generated and added.
 */
vision::rastercircle vision::initRMask(int radius) {
	vision::rastercircle t;
	std::vector<coord> temp;
	int size = 4;
//BEGIN Midpoint circle algorithm.
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;
	//calculated the number of points recursively
	while(x < y) {
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if(x != y) {
			size += 8;
		}
		else {
			size += 4;
		}
	}
//END Midpoint circle algorithm.

	//Create the array of coordinates
	for (int i = 0; i < size+4; ++i) {
		vision::coord tempc;
		tempc.x = 0.0;
		tempc.y = 0.0;
		tempc.z = 0.0;
		temp.push_back(tempc);
	}
//BEGIN Midpoint circle algorithm.
	f = 1 - radius;
	ddF_x = 1;
	ddF_y = -2 * radius;
	x = 0;
	y = radius;

	temp[0].x 			= radius; 	temp[0].y 			= 0; 		// 0
	temp[size/4].x 		= 0; 		temp[size/4].y 		= -radius; 	// Pi/2
	temp[size/2].x 		= -radius; 	temp[size/2].y 		= 0; 		// Pi
	temp[3*size/4].x 	= 0;		temp[3*size/4].y 	= radius; 	// 3Pi/2

	int it1 = 1;
	int it2 = size/4-1;
	int it3 = size/4+1;
	int it4 = size/2-1;
	int it5 = size/2+1;
	int it6 = 3*size/4-1;
	int it7 = 3*size/4+1;
	int it8 = size-1;
	while(x < y)
	{
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		// 0 to 45
		temp[it1].x 	= y; 	temp[it1].y 	= -x;
		// 90 to 45
		temp[it2].x 	= x; 	temp[it2].y 	= -y;
		// 90 to 135
		temp[it3].x 	= -x; 	temp[it3].y 	= -y;
		// 180 to 135
		temp[it4].x 	= -y; 	temp[it4].y 	= -x;
		// 180 to 225
		temp[it5].x 	= -y;	temp[it5].y 	= x;
		// 270 to 225
		temp[it6].x 	= -x; 	temp[it6].y 	= y;
		// 270 to 315
		temp[it7].x 	= x; 	temp[it7].y 	= y;
		// 360 to 315
		temp[it8].x 	= y;	temp[it8].y 	= x;

		it1++; it3++; it5++; it7++;
		it2--; it4--; it6--; it8--;
	}
//END Midpoint circle algorithm.
	int max = ceil(sqrt(pow((double)radius, 2.0)/2.0));
	for(int i = 0; i < 4; i++) {
		temp[size+i].x = (i == 1 || i == 2) ? -max : max;
		temp[size+i].y = (i == 0 || i == 1) ? -max : max;
	}
	t.setMask(temp);

	display& disp = gamemanager::getDisplay();
	disp.printRaster(t);

	std::vector<std::vector<vision::coord> > lines;
	for(int i = 0; i < size; i++) {
		lines.push_back(computeLine(temp[i].x, temp[i].y));
		disp.printLine(temp[i].x, temp[i].y, lines[i]);
	}
	for(int i = 1; i <= 4; i++) {
		lines.push_back(getDiag(i, radius));
		disp.printLine(temp[size+i-1].x, temp[size+i-1].y, lines[size+i-1]);
	}

	t.setLines(lines);
	radiusmask.insert(std::make_pair(radius, t));
	return t;
}

/**
 * Computes a line between (0,0) and (dx,-dy).
 *
 * @param dx - The x offset from the initial point
 * @param dy - The y offset from the initial point NOTE: dy inverted
 */
std::vector<vision::coord>
	vision::computeLine(double dx, double dy)
{
	std::vector<vision::coord> ret;
	double maxY = fabs(dy);
	double maxX = fabs(dx);

	/**
	 * The first part of the algorithm handles the 4 cardinal directions.
	 */
	if(dx == 0.0 || dy == 0.0) {
		for(int i = 0; (dx == 0.0) ? i <= fabs(dy) : i <= fabs(dx); i++) {
			vision::coord tempc;
			tempc.z = 0.0;
			// tempc.x = 0, j or -j depending on dx = 0 or +-dx
			tempc.x = (dx == 0.0) ? 0.0 : (dx > 0.0) ? (double)i : (double)-i;
			// tempc.y = 0, j or -j depending on dy = 0 or +-dy
			tempc.y = (dy == 0.0) ? 0.0 : (dy > 0.0) ? (double)i : (double)-i;
			ret.push_back(tempc);
		}
	}
	/**
	 * The following algorithm seeks to separate a line into segments based
	 * on x and y traversal.  The k loop handles vertical movement while
	 * the j loop handles horizontal movement.
	 */
	else {
		double angle = atan2(-dy, dx);
		for (int k = 0; k <= maxY; ++k) {//Ancient
			//Start at the x value the line reach height k
			int jset = abs(k/tan(angle));
			//Loop until the x value the line reaches height k+1
			double jcheck1 = fabs(((double)k+1.0)/tan(angle));
			for (int j = jset; j <= jcheck1 && j <= maxX; ++j) {//Greek
				vision::coord tempc;
				// tempc.x = +-j depending on +-dx
				tempc.x = (dx > 0) ? (double)j : (double)-j;
				// tempc.y = +-k depending on +-dy
				tempc.y = (dy > 0) ? (double)k : (double)-k;
				tempc.z = 0.0;
				ret.push_back(tempc);
			}
		}
	}
	return ret;
}

/**
 * Makes a diagonal into quadrant q with r = radius
 *
 * @param q - The quadrant to extend the diagonal into.
 * @param radius - The radius of the rastercircle.
 *
 * @return - The y coordinate is inverted because up is a negative offset
 */
std::vector<vision::coord> vision::getDiag(int q, int radius) {
	int max = ceil(sqrt(pow((double)radius, 2.0)/2.0));
	// Quadrant 2 and 3 have negative x increments
	int incX = (q == 2 || q == 3) ? -1 : 1;
	// Quadrant 1 and 2 have negative y increments. see @return
	int incY = (q == 1 || q == 2) ? -1 : 1;
	std::vector<vision::coord> ret;
	for(int i = 0; i <= max; i++) {
		vision::coord tempc;
		// Step out from 0,0 diagonally
		tempc.x = (double)(i*incX);
		tempc.y = (double)(i*incY);
		tempc.z = 0.0;
		ret.push_back(tempc);
	}
	return ret;
}
