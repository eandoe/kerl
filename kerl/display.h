/*
 * display.h
 *
 *  Created on: 2012/11/09
 *      Author: Bloodnamed
 *      		Bernhard Strauss
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <string>
#include <vector>

#include "ncstring.h"
#include "terminal.h"
#include "vision.h"

class pmap; // forward declarations
class player;
class vision;
struct coord;

class display {
public:
	display();
	virtual ~display();
	bool changed();
	void resize();
	void resize(int x, int y);
	int getChar();
	std::string getString(int size) const;
	int getNum(const char* prompt, int size);
	void moveCursor(int x, int y);
	void prints(int x, int y, const char* str);
	void printsw(const char* str);
	static void refreshWin();
	bool appendString(const std::string& str);
	void moveTerm(int i);
	void printTerminal() const;
	void printFrame() const;
	bool printMap(const player* p) const;
	void printNCString(int x, int y, const ncstring& str) const;
	void printNonTile(int x, int y) const;
	bool printPlayer(const player* p) const;
	bool printLine(int x, int y, std::vector<vision::coord> line);
	bool printRaster(vision::rastercircle rc) const;
	bool printTextFile(const char* file);
	bool printDebugFile(int seed, int x, int y, int z);
	void clear();
private:
	int scrX, scrY;
	int mapX, mapX_L;
	int mapY, mapY_L;
	int termX, termX_L;
	int termY, termY_L;
	int statX, statX_L;
	int statY, statY_L;
	terminal_type terminal;
};
#endif /* DISPLAY_H_ */
