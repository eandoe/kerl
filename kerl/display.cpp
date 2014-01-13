/*
 * display.cpp
 *
 *	Handles all graphical or user interface functions for the program.
 *
 *  Created on: 2012/11/09
 *      Author: Bloodnamed
 *      		Bernhard Strauss
 */

#include "display.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cstring>
#include <ctime>
#include <math.h>
#include <cmath>
#include <locale.h>

#include "player.h"
#include "ncstring.h"
#include "ncursesw/curses.h"
#include "gamemanager.h"
#include "pmap.h"
#include "vision.h"

using namespace std;

//ASCII box characters
const char toptri[] = "-";
const char bottri[] = "-";
const char lefttri[] = "|";
const char righttri[] = "|";
const char tlcorner[] = "+";
const char trcorner[] = "+";
const char blcorner[] = "+";
const char brcorner[] = "+";
const char hrline[] = "-";
const char veline[] = "|";

//Unicode box characters
const char utoptri[] = "\u252c";
const char ubottri[] = "\u2534";
const char ulefttri[] = "\u251c";
const char urighttri[] = "\u2524";
const char utlcorner[] = "\u250c";
const char utrcorner[] = "\u2510";
const char ublcorner[] = "\u2514";
const char ubrcorner[] = "\u2518";
const char uhrline[] = "\u2500";
const char uveline[] = "\u2502";


display::display() {
// Set a wide character locale to support later switches unicode/utf output
	setlocale(LC_ALL,"en_US.UTF-8");
// Start curses mode
	initscr();
// Enables the reading of function keys like F1, F2, arrow keys etc.
	keypad(stdscr, TRUE);
// Resize the screen to fit the terminal and initialize control variables
	resize(getmaxx(stdscr),	getmaxy(stdscr));
// Initialize colors
	if(!has_colors()) {

	}
	else {
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_BLACK, COLOR_WHITE);
	}
// Don't echo() while we do getch
	noecho();
// Line buffering disabled
	raw();
// Set cursor to invisible
	curs_set(0);
	refresh();
}

/**
 * Destructor for display.
 */
display::~display() {
	echo();						/* Turn echo() back on		*/
	endwin();					/* End curses mode			*/
}

/**
 * Detects whether a change has occurred in the dimensions of the terminal.
 */
bool display::changed() {
	return (scrX != getmaxx(stdscr) || scrY != getmaxy(stdscr)) ? true : false;
}

/**
 * Dynamic resize method.
 */
void display::resize() {
	if(gamemanager::getAllowResize()) {
		scrX = getmaxx(stdscr);
		scrY = getmaxy(stdscr);
		mapX = 1;
		mapX_L = 3*scrX/5;
		mapY = 1;
		mapY_L = 3*scrY/5;
		termX_L = scrX-2;
		termX = 1;
		termY_L = scrY-mapY_L-3;
		termY = mapY_L+2;
		statX_L = scrX-mapX_L-3;
		statX = mapX_L+2;
		statY_L = mapY_L;
		statY = 1;
		terminal.adjust_terminal_position(termY_L, termX, termY);
	}
}

/**
 * Static resize method.
 */
void display::resize(int x, int y) {
	scrX = x;
	scrY = y;
	mapX = 1;
	mapX_L = 3*scrX/5;
	mapY = 1;
	mapY_L = 3*scrY/5;
	termX_L = scrX-2;
	termX = 1;
	termY_L = scrY-mapY_L-3;
	termY = mapY_L+2;
	statX_L = scrX-mapX_L-3;
	statX = mapX_L+2;
	statY_L = mapY_L;
	statY = 1;
	terminal.adjust_terminal_position(termY_L, termX, termY);
}

/**
 * Interfacing function from display for getting a character.
 *
 * @return The character input by the user.
 */
int display::getChar() {
	return getch();
}

/**
 * Interfacing function from display for getting a string.
 * TODO Add check for whether input size is valid.
 *
 * @return The string input by the user.
 */
std::string display::getString(int size) const {
	echo();
	char *input = new char[size+1]; // +1 for null terminating char
	getnstr(input, size);
	noecho();
	std::string input_str(input);
	delete[] input;
	return input_str;
}

/**
 * Gets a number input from the console and returns the integer value
 * of the string entered. Number must be greater than or equal to 0, base 10.
 *
 * @parameter prompt - A pointer to a null terminated string text prompt.
 * @parameter size - The size in characters of the maximum integer input.
 *
 * @return The strtol() conversion of the user input.
 */
int display::getNum(const char* prompt, int size) {
	//set up control variables
	std::string in;
	bool fin = false;
	int out;
	//get the characters
	while(!fin) {
		// clear text at bottom
		for(int i = termX; i < scrX-1; ++i) {
			mvaddstr(scrY-2,i, " ");
		}
		mvaddstr(scrY-2,termX+1, prompt); //print display prompt
		in = getString(size); // get input from user
		char *end;
		out = (int)strtol(in.c_str(), &end, 10); // get number in base 10
		// successful if end now points to the end of the c_str
		fin = end == in.c_str()+in.size()*sizeof(char) && out >= 0 ? true : false;
	} // end of while
	for(int i = termX; i < scrX-1; ++i) { // clear prompt
		mvaddstr(scrY-2,i, " ");
	}
	return out;
}

/**
 * Moves the ncurses cursor to the position indicated by the parameters
 *
 * @param x - length coordinate
 * @param y - line number coordinate
 */
void display::moveCursor(int y, int x) {
	move(y,x);
}

/**
 * Prints the given string wherever the current cursor is.
 *
 * @param str - c string output
 */
void display::printsw(const char* str) {
	addstr(str);
}

/**
 * Prints a null terminated string to the ncurses window
 * at the 0 indexed position (x,y)
 *
 * @parameter x - The horizontal offset in the line.
 * @parameter y - The vertical offset in the window.
 */
void display::prints(int y, int x, const char* str) {
	mvaddstr(y,x, str);
}

/**
 * Complimentary function to prints, flushes output to screen
 */
void display::refreshWin() {
	refresh();
}

/**
 * Puts a string to the terminal content. Does not print it.
 */
bool display::appendString(const std::string& str) {
	return terminal.string_push_back(str);
}

/**
 * Prints a string to the terminal message screen.
 */
void display::printTerminal() const {
	terminal.print_terminal();
}

/**
 * Appends the string inside of the linebuffer into the term and updates the screen
 * To be removed.
 */
//void display::updateTerm(char* str) {
//}

/**
 * Calls the curses functions to print the terminal
 * To be removed.
 */
//void display::printTerm() {
//}

/**
 * Moves the terminal head
 */
void display::moveTerm(int i) {
	terminal.adjust_terminal_view(i);
}

/**
 * Prints the edge boundaries to the curses window.
 */
void display::printFrame() const {
	if(gamemanager::getWidechars()) {
		for(int i = 0; i < scrX; ++i) {//top+bot border
			if(i == 0) {//column 0
				mvaddstr(0,i, utlcorner);
				mvaddstr(scrY-1,i, ublcorner);
			}
			else if(i == statX-1) {// map/stat window border
				mvaddstr(0,i, utoptri);
				mvaddstr(scrY-1,i, uhrline);
			}
			else if(i == scrX-1) {//column 119
				mvaddstr(0,i, utrcorner);
				mvaddstr(scrY-1,i, ubrcorner);
			}
			else {//all other cells on row 0
				mvaddstr(0,i, uhrline);
				if(i < statX-1) {
					mvaddstr(termY-1,i, uhrline);
				}
				else {
					mvaddstr(termY-1,i, uhrline);
				}
				mvaddstr(scrY-1,i, uhrline);
			}
		}
		for(int j = 1; j < scrY-1; ++j) {//column dividers
			if(j != termY-1) {
				mvaddstr(j,0, uveline);
				if(j < termY-1) {
					mvaddstr(j,statX-1, uveline);
				}
				mvaddstr(j,scrX-1, uveline);
			}
			else if(j == termY-1) {
				mvaddstr(j,0, ulefttri);
				mvaddstr(j,statX-1, ubottri);
				mvaddstr(j,scrX-1, urighttri);
			}
		}
	}
	else {
		for(int i = 0; i < scrX; ++i) {//top+bot border
			if(i == 0) {//column 0
				mvaddstr(0,i, tlcorner);
				mvaddstr(scrY-1,i, blcorner);
			}
			else if(i == statX-1) {//column 61
				mvaddstr(0,i, toptri);
				mvaddstr(scrY-1,i, hrline);
			}
			else if(i == scrX-1) {//column 119
				mvaddstr(0,i, trcorner);
				mvaddstr(scrY-1,i, brcorner);
			}
			else {//all other cells on row 0
				mvaddstr(0,i, hrline);
				if(i < statX-1) {
					mvaddstr(termY-1,i, hrline);
				}
				else {
					mvaddstr(termY-1,i, hrline);
				}
				mvaddstr(scrY-1,i, hrline);
			}
		}
		for(int j = 1; j < scrY-1; ++j) {//column dividers
			if(j != termY-1) {
				mvaddstr(j,0, veline);
				if(j < termY-1) {
					mvaddstr(j,statX-1, veline);
				}
				mvaddstr(j,scrX-1, veline);
			}
			else if(j == termY-1) {
				mvaddstr(j,0, lefttri);
				mvaddstr(j,statX-1, bottri);
				mvaddstr(j,scrX-1, righttri);
			}
		}
	}
}

/**
 * Prints the string representation of a given area, with respect to the
 * player's location, to the ncurses window.
 *
 * @parameter a - The area to be printed.
 *
 * @return TRUE on success, FALSE on error.
 */
bool display::printMap(const player* p) const {
	const pmap& m = p->getMap(); // pmap alias
	int off_X = (mapX_L)/2; //Map x offset from player location
	int mod_X = mapX_L%2;   //Odd width fix
	int off_Y = (mapY_L)/2; //Map y offset from player location
	int mod_Y = mapY_L%2;   //Odd height fix

	int x = 0;              //ncurses x position
	int y = 0;              //ncurses y position
	//for each row
	for (int j = p->getY()-off_Y; j < (p->getY()+off_Y+mod_Y); ++j) {
		x=0;//ncurses x position reset for new row
		for(int i = p->getX()-off_X; i < (p->getX()+off_X+mod_X); ++i) {
			//if outside the boundaries print space
			if(j < 0 || i < 0 || j >= m.getY() || i >= m.getX()) {
				printNonTile(x, y);
			} else {
				//grab the ncstring from the map and print it
				printNCString(x+mapX, y+mapY, m.toString(i,j));
			}
			++x;
		}
		++y;
	}
	refresh();
	return true;
}

/**
 * Takes ncstring format and converts it to ncurses format output
 *
 * @param x - Column to be printed on.
 * @param y - Row to be printed on.
 * @param str - The ncstring to be printed.
 */
void display::printNCString(int x, int y, const ncstring& str) const {
	if(str.isHLight()) {
		attron(A_BOLD);
	}
	if(gamemanager::getWidechars()){
		mvaddstr(y,x, str.toString().c_str());
	} else {
		mvaddch(y,x, str.toChar());
	}
	if(str.isHLight()) {
		attroff(A_BOLD);
	}
}

/**
 * Prints out a character to represent a tile outside of the playable boundary
 *
 * @param x - Column to be printed on.
 * @param y - Row to be printed on.
 */
void display::printNonTile(int x, int y) const {
	//set outside boundary location
	attron(A_DIM);
	attron(COLOR_PAIR(2));
	if(!gamemanager::getWidechars()) {
		//ASCII out is a space
		mvaddstr(y+mapY, x+mapX, " ");
	}
	else {
		//Unicode out is a special character
		mvaddstr(y+mapY, x+mapX, "\u2591");
	}
	//unset outside boundary location
	attroff(A_DIM);
	attroff(COLOR_PAIR(2));
}

/**
 * Prints out player information to the player area
 *
 * @parem p - The player info to print.
 *
 * @return TRUE on success, FALSE on failure.
 */
bool display::printPlayer(const player* p) const {
	std::stringstream ret;
	int statTop = termY - 9;
	ret << p->enumToString() << p->getName() << ":" << "\0";
	mvaddstr(statTop++, 0+statX, ret.str().c_str());

	ret.str(std::string());
	ret << "Health: " << p->getHP() << "/" << p->getHPMax() << "\0";
	mvaddstr(statTop++, 1+statX, ret.str().c_str());

	ret.str(std::string());
	ret << "Magic:  " << p->getMP() << "/" << p->getMPMax() << "\0";
	mvaddstr(statTop++, 1+statX, ret.str().c_str());

	ret.str(std::string());
	ret << "Level:  " << p->getLevel() << "\0";
	mvaddstr(statTop++, 1+statX, ret.str().c_str());

	ret.str(std::string());
	ret << "XP:     " << p->getXP() << "\0";
	mvaddstr(statTop++, 1+statX, ret.str().c_str());

	ret.str(std::string());
	ret << "Hunger: ";
	ret << p->getHungerStatus();
//	ret << "\0";
	mvaddstr(statTop++, 1+statX, ret.str().c_str());

	ret.str(std::string());
	ret << "Px (" << p->getX() << ", " << p->getY() << ")      \0";
	mvaddstr(statTop++, 1+statX, ret.str().c_str());

	ret.str(std::string());
	ret << "Gold: $" << p->getGold() << "      \0";
	mvaddstr(statTop++, 1+statX, ret.str().c_str());

	refresh();
	return true;
}

/**
 * Prints the given text file, with maximum length 80,
 * to the main ncurses window.
 *
 * @parameter file - A string reference to the file.
 *
 * @return TRUE on success, FALSE on error
 */
bool display::printTextFile(const char* file) {
	int offx = (scrX-80)/2;
	int offy = (scrY-40)/2;
	string line;
	ifstream menufile (file);
	if (menufile.is_open())
	{
		for (int i = 0; i < 50 && menufile.good(); ++i)
		{
			getline (menufile,line);
			mvaddstr(i+offy, offx, line.c_str());
		}
		menufile.close();
	}
	else {
		mvaddstr(0, 0, "Unable to open file: " + *file);
		return false;
	}
	refresh();

	return true;
}

/**
 * Clears the game window
 */
void display::clear() {
	erase();
}

/**
 * Prints debug information for recreating bugs to debug file.
 */
bool display::printDebugFile(int seed, int x, int y, int z) {
	time_t rawtime = seed;
	struct tm * timeinfo;
	char time[40];
	std::stringstream out;

	timeinfo = localtime (&rawtime);

	strftime(time, 40, "%c", timeinfo);
	out << time << " : " << seed <<
			" : (" << x << ", " << y << ", " << z << ")" << "\n";
	ofstream file ("debug.txt", ios::app);
	if (file.is_open())
	{
		file << out.str().c_str();
	}
	else {
		mvaddstr(0, 0, "Unable to open file: debug.txt");
		return false;
	}

	return true;
}

/**
 * Prints debug information about the calc line method to the debug file.
 */
bool display::printLine(int x, int y, std::vector<vision::coord> line) {
	std::stringstream out;
	ofstream file ("raster.txt", ios::app);

	if (file.is_open())
	{
		out << ": " << "(" << x << "," << y << ") : ";
		for(unsigned int i = 0; i < line.size(); i++) {
			out << "(" << line[i].x << "," << line[i].y << ") ";
		}
		out << "\n";
		file << out.str().c_str();
	}
	else {
		mvaddstr(0, 0, "Unable to open file: debug.txt");
		return false;
	}
	return true;
}

/**
 * Prints debug information about the rastercircle to the debug file.
 */
bool display::printRaster(vision::rastercircle rc) const {
	std::stringstream out;
	ofstream file ("raster.txt", ios::trunc);

	std::vector<vision::coord> arr = rc.getMask();
	for(unsigned int i = 0; i < arr.size(); i++) {
		out.str("");
		out << i << ": " <<
				"(" << arr[i].x << "," << arr[i].y << ")" << "\n";
		if (file.is_open())
		{
			file << out.str().c_str();
		}
		else {
			mvaddstr(0, 0, "Unable to open file: debug.txt");
			return false;
		}
	}

	return true;
}
