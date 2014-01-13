/*
 * vision.h
 *
 *  Created on: Dec 9, 2012
 *      Author: Bloodnamed
 */

#ifndef VISION_H_
#define VISION_H_

#include <vector>

#define PI 3.14159265

class level;
class pmap;

class vision {
public:
	struct coord {
		double x;
		double y;
		double z;
	};
	struct rastercircle {
		void setMask(std::vector<coord> t);
		void setLines(std::vector<std::vector<coord> > l);
		std::vector<coord> getMask();
		std::vector<std::vector<coord> > getLines();
	private:
		std::vector<coord> circle;
		std::vector<std::vector<coord> > lines;
		int size;
	};
	vision();
	virtual ~vision();
	static void exploreXRay(int x, int y, int sradius, pmap& m, level& l);
	static void exploreHeavy(int x, int y, int sradius, pmap& m, level& l);
private:
	static std::vector<coord> computeLine(double dx, double dy);
	static std::vector<coord> getDiag(int q, int radius);
	static bool addVision(int x, int y, pmap& m, level& l);
	static void removeVision(int x, int y, pmap& m, level& l);
	static rastercircle initRMask(int radius);
};

#endif /* VISION_H_ */
