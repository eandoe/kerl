/*
 * pmap.h
 *
 *  Created on: Dec 5, 2012
 *      Author: Bloodnamed
 */

#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include "ncstring.h"

class world;

class pmap {
public:
	pmap(world*, int c);
	virtual ~pmap();
	void setVisible(int x, int y, bool vis);
	bool isVisible(int x, int y) const;
	void setCLevel(int mz);
	void insert(int mx, int my, ncstring str);
	void remove(int mx, int my);
	ncstring toString(int mx, int my) const;
	int getX() const;
	int getY() const;
	int getZ() const;
private:
	int x,y,z;
	int clevel;
	std::vector< std::vector<std::vector<ncstring> > > mop;
	std::vector<std::vector<bool> > vision;
};

#endif /* MAP_H_ */
