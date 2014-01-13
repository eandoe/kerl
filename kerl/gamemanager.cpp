/*
 * gamemanager.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: Bloodnamed
 */

#include "gamemanager.h"

#include <signal.h>
#include <string>
#include <ncursesw/ncurses.h>
#include <ncursesw/curses.h> // remove eclipse errors for KEY_UP ... macros
#include <sstream>

#include "player.h"
#include "pfunctions.h"
#include "level.h"
#include "signalhandler.h"
#include "pmap.h"
#include "vision.h"
#include "spiralfov.h"
#include "creature.h"
#include "pet.h"
#include "world.h"

/**
 * The fun begins.  Gamemanager's duty is to control all linking of game
 * elements such as actors, the world, display, ancillary components such as
 * random number generators and the like.  In order to maintain polymorphism,
 * this class should only access methods of interfaces.
 */
gamemanager::gamemanager() :
	worldXFactor(2),	worldYFactor(2), worldZFactor(1),
	termX(80), termY(40),
	cid(0),
	theworld(NULL), turn(1)
	{
	//SignalHandler::setupSignalHandlers();
	// TODO Auto-generated constructor stub
//	spf = new spiralfov();
//	spf->FOV_Init();
}

/**
 * Default destructor of gamemanager.  Should destroy all high level objects
 * which then initiates a chain of object deletion.
 */
gamemanager::~gamemanager() {
	// TODO Auto-generated destructor stub
	if(theworld != NULL)
		delete theworld;
	for(std::list<actor*>::iterator iter = activeActors.begin(); iter != activeActors.end(); ++iter ){
		delete *iter;
	}
//	delete spf;
}

/**
 * run is essentially the entry point of the program.  The below method will
 * distinguish the major phases of the game.  Currently these are:
 *  Pre-game menu
 *  Gameplay
 *
 * Future work might include during gameplay option settings, post game.
 */
bool gamemanager::run() {
	//Pre-game menu
	bool exit = menu();

	//Gameplay loop
	while(!exit && !signalhandler::gotExitSignal()){
		if(disp.changed()) {
			disp.clear();
			disp.resize();
		}
		for(std::list<actor*>::iterator iter = activeActors.begin();
//				i < cid && i < int(activeActors.size()) && !exit; ++i) {
				iter != activeActors.end() && !exit; ++iter) {
//			if(activeActors[i]->getNTurn() <= turn)
			exit = actorTurn(*iter);
		}
//		turn++; // let the player handle the turn
	}

	//Return
	return exit;
}

/**
 * Disambiguation method.  Determines how to handle the actor's turn. Most
 * commonly the determination between AI and Human actors.
 *
 * @param a - The actor whose turn it is.
 * @return A boolean stating whether to exit game.
 */
bool gamemanager::actorTurn(actor* a) {
//	if(a->isPlayer()) { // shouldn't need this kind of check
//		return playerTurn((player*)a);
//	}
//	else {
//		return creatureTurn((actor*)a);
//	}
	return a->takeTurn();
}

/**
 * Performs the actions necessary for an AI controlled actor's turn.
 *
 * @param a - The actor whose turn it is.
 * @return A boolean whether to quit the game.
 */
//bool gamemanager::creatureTurn(actor* a) {
//	bool moved = false;
//	int x = 0;
//	int n_x = 0;
//	int y = 0;
//	int n_y = 0;
//	int tries = 0;
//	while(!moved && tries < 5) {
//		x = rng.uniform(-1, 1); //Random X movement
//		y = rng.uniform(-1, 1); //Random Y movement
//		n_x = a->getX()+x;//new x
//		n_y = a->getY()+y;//new y
//		//Make sure movement isn't out of bounds.
//		if(n_x >= 0 && n_x <= worldX && n_y >= 0 && n_y <= worldY) {
//			level& clevel = theworld->getArea(a->getZ());
//			//Check to see the tile is empty and enterable.
//			if( (clevel.getTile(n_x,n_y).isEmpty() &&
//					 a->canMove(clevel.getTile(n_x,n_y).getType()))
//					|| (a == clevel.getTileRef(n_x,n_y).getActor())) // check if moving to the same tile
//			{ //If the spot is empty and the creature can move there
////				(lev.getTile(x,y).isEmpty())
////										// for if the player "skips" and the tile has a pointer to the player
////										|| (p == dynamic_cast<player*>(lev.getTileRef(x,y).getActor()))
//				moveActor( a, n_x, n_y, a->getZ(), false );
//				moved = true;
//			}//Otherwise get a new direction.
//			else ++tries;
//		} else ++tries;
//	} // end of while
//	return false;
//}

/**
 * Performs the actions necessary for a human controlled actor's turn.
 *
 * @param p - The player whose turn it is.
 */
//bool gamemanager::playerTurn(player* p) {
//	// append turn number to terminal
//	std::stringstream tur;
//	spf->FOV_SpiralPath(theworld->getArea(p->getZ()),  p->getMapRef(), p->getX(), p->getY(), p->getZ(), 4, 0, 3.14159265*2, 0, 1);
//	tur << "Your turn: " << turn << "           "; // need space for overwriting text
//	disp.appendString(tur.str().c_str());
//	printHUD(p); //Reprints player info to the screen.
//
//	if(p->isWaiting()){
//		doAction(p, pfunctions::SKIP); // to increment hunger while waiting
//		p->decrementWait();
//	} else {
//		pfunctions::playeraction act = pfunctions::NONE;
//		bool valid = false;
//		while(act == pfunctions::NONE || !valid) {
//			act = getAction(p);
//			if(act == pfunctions::QUIT) {
//				return true;
//			}
//			valid = doAction(p, act);
//		}
//	}
//	return false;
//}

//pfunctions::playeraction gamemanager::getAction(player* p) {
//	int inputc;
//	int altinput;
//	int qinput;
//	bool fin = false;
//
//	while (!fin) {
//		inputc = disp.getChar();
//
//		switch (inputc) {
//		case '1':
//			return pfunctions::MOVEDOWNLEFT;
//		case '2':
//			return pfunctions::MOVEDOWN;
//		case '3':
//			return pfunctions::MOVEDOWNRIGHT;
//		case '4':
//			return pfunctions::MOVELEFT;
//		case '5':
//			return pfunctions::SKIP;
//		case '6':
//			return pfunctions::MOVERIGHT;
//		case '7':
//			return pfunctions::MOVEUPLEFT;
//		case '8':
//			return pfunctions::MOVEUP;
//		case '9':
//			return pfunctions::MOVEUPRIGHT;
//		case KEY_DOWN:
//			return arrowKeysEnabled ? pfunctions::MOVEDOWN : pfunctions::NONE;
//		case KEY_LEFT:
//			return arrowKeysEnabled ? pfunctions::MOVELEFT : pfunctions::NONE;
//		case KEY_RIGHT:
//			return arrowKeysEnabled ? pfunctions::MOVERIGHT : pfunctions::NONE;
//		case KEY_UP:
//			return arrowKeysEnabled ? pfunctions::MOVEUP : pfunctions::NONE;
//		case 's':
//			return pfunctions::WAIT;
//		case '[':
//			return pfunctions::TERMUP;
//		case ']':
//			return pfunctions::TERMDOWN;
//		case 'd':
//			return pfunctions::REFRESH;
//		case 27:
//			altinput = disp.getChar();
//			if(altinput == 'Q' || altinput == 'q') {
//				bool efin = false;
//				disp.prints(0,1, "Are you sure you wish to quit? y/n");
//				disp.refreshWin();
//				while (!efin) {
//					qinput = disp.getChar();
//					switch(qinput) {
//					case 'Y':
//					case 'y':
//						return pfunctions::QUIT;
//					case 'N':
//					case 'n':
//						for(int i = 0; i < 37; ++i) {
//							disp.moveCursor(0,i+1);
//							if(!widechars) {
//								disp.printsw("-");
//							}
//							else {
//								disp.printsw("\u2500");
//							}
//						}
//						efin = true;
//					default:
//						break;
//					}
//				}
//			}
//			break;
//#ifdef __APPLE__ // temp fix for quitting on Apple computers
//		case 'q':
//			return pfunctions::QUIT;
//#endif
//		default:
//			break;
//		}
//	}
//
//	return pfunctions::NONE;
//}

/**
 *
 */
//bool gamemanager::doAction(player* p, pfunctions::playeraction a) {
//	if(pfunctions::isMove(a)) {
//		level& lev = theworld->getArea(p->getZ());
//		int x = p->getX();
//		int y = p->getY();
//		switch(a) {
//		case pfunctions::MOVEDOWNLEFT:
//			x += -1;
//			y += 1;
//			break;
//		case pfunctions::MOVEDOWN:
//			y += 1;
//			break;
//		case pfunctions::MOVEDOWNRIGHT:
//			x += 1;
//			y += 1;
//			break;
//		case pfunctions::MOVELEFT:
//			x += -1;
//			break;
//		case pfunctions::MOVERIGHT:
//			x += 1;
//			break;
//		case pfunctions::MOVEUPLEFT:
//			x += -1;
//			y += -1;
//			break;
//		case pfunctions::MOVEUP:
//			y += -1;
//			break;
//		case pfunctions::MOVEUPRIGHT:
//			x += 1;
//			y += -1;
//			break;
//		default:
//			break;
//		}
//		if (x>=0 && x<= worldX && y>=0 && y<=worldY) {	//staying in this area
//			if( p->canMove(lev.getTile(x,y).getType()) ) {
//				if ((lev.getTile(x,y).isEmpty())
//						// for if the player "skips" and the tile has a pointer to the player
//						|| (p == dynamic_cast<player*>(lev.getTileRef(x,y).getActor()))) {
//					moveActor(p, x, y, p->getZ(), true);
//					return true;
//				}
//				else {
//					return false;
//					//COMBAT!
//				}
//			}
//			else {
//				return false;
//			}
//		}
//		else {
//			return false;
//		}
//	}
//	else {
//		switch(a) {
//		case pfunctions::TERMUP:
//			disp.moveTerm(-5);
//			break;
//		case pfunctions::TERMDOWN:
//			disp.moveTerm(5);
//			break;
//		case pfunctions::REFRESH:{
//			disp.clear();
//			disp.resize();
//			std::stringstream msg;
//			msg << "Screen Refreshed                             "; // need space for overwriting text
//			disp.appendString(msg.str().c_str());
//			printHUD(p); //Reprints player info to the screen.
//			break;
//		}
//		default:
//			break;
//		}
//	}
//	if(a == pfunctions::SKIP)
//		return true;
//	if(a == pfunctions::WAIT) {
//		// FIXME Sometimes, instead of waiting n turns, game waits 1 turn then skips n turns on next turn.
////		p->setNTurn(turn + disp.getNum("Wait for how long? ", 4));
//		int waitFor = disp.getNum("Wait for how long? ", 4);
//		while(waitFor < 1){
//			waitFor = disp.getNum("Wait for how long? ", 4);
//		}
//		p->increaseWait(waitFor - 1); // include current turn
//		return true;
//	}
//	return false;
//}

/**
 *
 */
//void gamemanager::printHUD(const player* p) const {
//	//print the turn counter to the terminal
//	disp.printTerminal();
//	//print the player's information
//	disp.printPlayer(p);
//	//print the map the player is currently on
//	disp.printMap(p, p->getMap());
//	//reprint the frame in case of overprints
//	disp.printFrame();//to be removed
//}

//void gamemanager::moveActor(actor* a, int x, int y, int z, bool isplayer) {
//	if(isplayer) {
////		theworld->getArea(a->getZ())->removeVis(a->getX(), a->getY(), a->getSRadius());
//	}
//	//TODO: not very efficient assigning of default tile
//	tile& oldt = theworld->getArea(a->getZ()).getTileRef(a->getX(), a->getY());
//	tile& dest = theworld->getArea(z).getTileRef(x, y);
//	oldt.removeActor();
//	if(oldt.isDefault()){
//		tile t;
//		t.setType(oldt.getType());
//		//if(turn != 1) delete oldt;
//	//TODO: first turn the player is on a default tile, shouldn't be like this
//		theworld->getArea(a->getZ()).setTile(a->getX(), a->getY(),t);
//	}
//	if(dest.isDefault()){
//		tile t;
//		t.setType(dest.getType());
//		t.setActor(a);
//		theworld->getArea(z).setTile(x, y, t);
//	}
//	else theworld->getArea(z).getTileRef(x,y).setActor(a);
//	if(isplayer) {
//		//Perform exploration record keeping.
////		vision::exploreLight(x, y, a->getSRadius(), a->getMapRef(),
////				theworld->getArea(z));
//		// moved call to beginning of playerTurn, so creature positions are in sync with vision
////		spf->FOV_SpiralPath(theworld->getArea(z),  a->getMapRef(), x, y, z, 4, 0, 3.14159265*2, 0, 1);
//	}
//
//	a->move(x, y, z);
//}

bool gamemanager::menu() {
	bool menu = disp.printTextFile("MENU.txt");
	char c;
	bool fin = false;
	while (!fin && menu) {
		c = disp.getChar();

		switch (c) {
			case 'N':
			case 'n':
				if(newGame())
					fin = true;
				else
					disp.prints(0,0, "New Game Error!");
			break;
			case 'L':
			case 'l':
				if(loadGame())
					fin = true;
				else
					disp.prints(0,0, "Load Game Error!");
			break;
			case 'O':
			case 'o':
				if(!options())
					disp.prints(0,0, "Options Error!");
			break;
			case 'Q':
			case 'q':
				return true;
			default:
			break;
		}
	}

	return false;
}

bool gamemanager::newGame() {
	//Add debug string for reproducing bugs.
	disp.printDebugFile(rngke::getSeed(), worldXFactor, worldYFactor, worldZFactor);

	//Make the player and the world.
	theworld = new world(worldXFactor, worldYFactor, worldZFactor);

	player* p = makePlayer();
	activeActors.push_back(p);

	// give player a pet
	activeActors.push_back(makePet(p));

	return true;
}

player* gamemanager::makePlayer() {
	char c;
	player::playerclass pc = player::none;
	bool fin = false;
	std::string name;
	disp.clear();
	while(!fin) {
		disp.prints(1,1, "What is your name?                                                                                                      ");
		disp.moveCursor(1, 20);
		name = disp.getString(20);
		if(name.length() > 15) {
			disp.prints(2,2, "Your name is too long, no one will remember that..."); // **snicker**
		}
		else {
			fin = true;
		}
	}
	disp.prints(2,2, "                                                   ");
	disp.prints(3,1, "Pick your class: ");
	disp.prints(4,2, "F) Fighter");
	disp.prints(5,2, "A) Archer");
	disp.prints(6,2, "M) Mage");
	fin = false;
	while (!fin) {
		c = disp.getChar();

		switch (c) {
			case 'F':
			case 'f':
				pc = player::fighter;
				fin = true;
			break;
			case 'A':
			case 'a':
				pc = player::archer;
				fin = true;
			break;
			case 'M':
			case 'm':
				pc = player::mage;
				fin = true;
			break;
			default:
			break;
		}
	}
	player* p = new player(name, pc, cid++, theworld, actortype::HUMAN); // gamemanager::theworld must be defined before calling makePlayer()
	disp.clear();

	return p;
}

// NOTE: This function used to place a monster in a tile next to the player, but the time has come to replace it.
// Caller must delete.
creature* gamemanager::makeCreature() {
	creature* ret = new creature(2, 2, 0, cid++, theworld, actortype::TEST); // gamemanager::theworld must be defined before calling makeCreature()
	return ret;
}

pet* gamemanager::makePet(player* p) {
	pet* ret = new pet(2, 2, 0, cid++, theworld, actortype::TEST, p); // gamemanager::theworld must be defined before calling makePet()
//	ret->moveActor(2, 2, 0); // in actor constructor
	p->givePet(ret);
	return ret;
}

bool gamemanager::loadGame() {
	disp.prints(0,0, "Load Game!");
	return true;
}

bool gamemanager::options() {
	disp.printTextFile("MENUoptions.txt");

	char c;
	bool fin = false;
	while (!fin) {
		c = disp.getChar();

		switch (c) {
			case 'W':
			case 'w':
				worldOptions();
			break;
			case 'D':
			case 'd':
				displayOptions();
			break;
			case 'C':
			case 'c':
				controlsOptions();
			break;
			case 'R':
			case 'r':
				fin = disp.printTextFile("MENU.txt");
			break;
			default:
			break;
		}
	}
	return true;
}

void gamemanager::worldOptions() {
	disp.printTextFile("MENUworld.txt");

	char c;
	bool fin = false;
	while (!fin) {
		c = disp.getChar();

		switch (c) {
			case 'L':
			case 'l':
				worldXFactor = 4;
				worldYFactor = 4;
			break;
			case 'M':
			case 'm':
				worldXFactor = 2;
				worldYFactor = 2;
			break;
			case 'S':
			case 's':
				worldXFactor = 1;
				worldYFactor = 1;
			break;
			case 'R':
			case 'r':
				disp.clear();
				fin = disp.printTextFile("MENUoptions.txt");
			break;
			default:
			break;
		}
	}
}

void gamemanager::displayOptions() {
	disp.printTextFile("MENUdisplay.txt");

	int in = 0;
	char c;
	bool fin = false;
	while (!fin) {
		c = disp.getChar();

		switch (c) {
			case 'X':
			case 'x':
				in = 0;
				while(in < 80) {
					in = disp.getNum("Enter new terminal width: ", 3);
					if(in >= 80) {
						termX = in;
						disp.resize(in, termY);
					}
				}
				disp.clear();
				disp.printTextFile("MENUdisplay.txt");
			break;
			case 'Y':
			case 'y':
				in = 0;
				while(in < 40) {
					in = disp.getNum("Enter new terminal height: ", 3);
					if(in >= 40) {
						termY = in;
						disp.resize(termX, in);
					}
				}
				disp.clear();
				disp.printTextFile("MENUdisplay.txt");
			break;
			case 'C':
			case 'c':
				widechars = !widechars;
				if(widechars) {
					disp.prints(0,0, "Unicode Enabled    ");
				}
				else {
					disp.prints(0,0, "Unicode Disabled   ");
				}
			break;
			case 'R':
			case 'r':
				disp.clear();
				fin = disp.printTextFile("MENUoptions.txt");
			break;
			default:
			break;
		}
	}
}

// menu for changing control style
void gamemanager::controlsOptions() {
	disp.clear(); // remove junk text
	disp.printTextFile("MENUcontrols.txt");

	char c;
	bool fin = false;
	while (!fin) {
		c = disp.getChar();

		switch (c) {
			case 'A':
			case 'a':
				// toggle arrow keys controls
				arrowKeysEnabled = !arrowKeysEnabled;
				disp.prints(0,0, arrowKeysEnabled ? "Arrow Keys Enabled    " : "Arrow Keys Disabled   ");
			break;
			case 'R':
			case 'r':
				disp.clear();
				fin = disp.printTextFile("MENUoptions.txt");
			break;
			default:
			break;
		}
	}
}

#ifdef __APPLE__ // allow arrow keys enabled for Macs by default; otherwise, not
	bool gamemanager::arrowKeysEnabled = true;
#else
	bool gamemanager::arrowKeysEnabled = false;
#endif

display gamemanager::disp; // default construction
bool gamemanager::widechars = false;
bool gamemanager::allowresize = true;

