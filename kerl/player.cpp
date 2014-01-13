/*
 * player.cpp
 *
 *  Created on: 2012/11/09
 *      Author: Bernhard Strauss
 */

#include "player.h"

#include <sstream>
#include <stdexcept>
#include "ncursesw/curses.h"

#include "tiletype.h"
#include "vision.h"
#include "spiralfov.h"
#include "display.h"
#include "gamemanager.h"
#include "world.h"
#include "pet.h"

using namespace std;


player::player(const std::string& n, playerclass c, int _id, world* _world, actortype::a_type _type) :
		// default position(x,y,z): (10, 10, 0)
		// default sight radius 4; sight radius 5 makes assertion fail in spiralfov, for now
		actor(10, 10, 0, _id, 4, _world, _type, "You"),
		name(n), waiting(0),
		myPet(NULL)
		{
//	type = actortype::HUMAN; // type initialized in actor constructor
	pClass = c;

	switch(pClass) {
		case fighter:
			hp = 20;
			hpmax = 20;
			mp = 5;
			mpmax = 5;
			strength = 16;
			dexterity = 14;
			constitution = 16;
			intelligence = 10;
			wisdom = 10;
			charisma = 10;
			xp = 0;
			hunger = 1;
			level = 0;
			break;
		case archer:
			hp = 15;
			hpmax = 15;
			mp = 10;
			mpmax = 10;
			strength = 12;
			dexterity = 16;
			constitution = 12;
			intelligence = 14;
			wisdom = 12;
			charisma = 10;
			xp = 0;
			hunger = 1;
			level = 0;
			break;
		case mage:
			hp = 10;
			hpmax = 10;
			mp = 20;
			mpmax = 20;
			strength = 10;
			dexterity = 10;
			constitution = 10;
			intelligence = 18;
			wisdom = 18;
			charisma = 10;
			xp = 0;
			hunger = 1;
			level = 0;
			break;
		default:
			hp = 1;
			hpmax = 1;
			mp = 1;
			mpmax = 1;
			strength = 1;
			dexterity = 1;
			constitution = 1;
			intelligence = 1;
			wisdom = 1;
			charisma = 1;
			xp = 1;
			hunger = 1;
			level = 1;
			break;
	}

//	posX = 10; // put in initializer list
//	posY = 10;

	spf = new spiralfov();
	spf->FOV_Init();

}

player::~player() {
	delete spf;
}

/*
 * Performs the actions necessary for a human controlled actor's turn.
 *
 * @return A boolean whether to exit game
 */
bool player::takeTurn(){
//	spf->FOV_SpiralPath(theWorld->getArea(posZ),  getMapRef(), getX(), getY(), getZ(), sradius, 0, 3.14159265*2, 0, 1);
	vision::exploreHeavy(getX(), getY(), sradius, getMapRef(), theWorld->getArea(posZ));
	// append turn number to terminal
	std::stringstream tur;
	tur << "Your turn: " << nturn << "           "; // need space for overwriting text
	display& disp = gamemanager::getDisplay(); // display alias
	disp.appendString(tur.str().c_str());
	printHUD(); //Reprints player info to the screen.

	if(isWaiting()){
		doAction(pfunctions::SKIP); // to increment hunger while waiting
		decrementWait();
	} else {
		pfunctions::playeraction act = pfunctions::NONE;
		bool nextTurn = false;
		while(act == pfunctions::NONE || !nextTurn) {
			act = getAction();
			if(act == pfunctions::QUIT) {
				return true;
			}
			nextTurn = doAction(act);
		}
	}
	++nturn; // end of current turn
	return false;
}

//bool player::canMove(const int& type) const {
//	return (type == tiletype::ROOMFLOOR || type == tiletype::STAIRSUP || type == tiletype::STAIRSDOWN);
//}

bool player::act(){
	return false;
}

void player::move(int x, int y, int z) {
	posX = x;
	posY = y;
	posZ = z;
	++hunger;
	updateHungerStatus();
}

/*
 * Retrieve the desired action from the player.
 *
 * @return The playeraction value of the requested action.
 */
pfunctions::playeraction player::getAction() {
	int inputc;
	int altinput;
	int qinput;
	bool fin = false;
	display& disp = gamemanager::getDisplay(); // display alias

	while (!fin) {
		inputc = disp.getChar();

		switch (inputc) {
		case '1':
			return pfunctions::MOVEDOWNLEFT;
		case '2':
			return pfunctions::MOVEDOWN;
		case '3':
			return pfunctions::MOVEDOWNRIGHT;
		case '4':
			return pfunctions::MOVELEFT;
		case '5':
			return pfunctions::SKIP;
		case '6':
			return pfunctions::MOVERIGHT;
		case '7':
			return pfunctions::MOVEUPLEFT;
		case '8':
			return pfunctions::MOVEUP;
		case '9':
			return pfunctions::MOVEUPRIGHT;
		case KEY_DOWN:
			return gamemanager::getArrowKeysEnabled() ? pfunctions::MOVEDOWN : pfunctions::NONE;
		case KEY_LEFT:
			return gamemanager::getArrowKeysEnabled() ? pfunctions::MOVELEFT : pfunctions::NONE;
		case KEY_RIGHT:
			return gamemanager::getArrowKeysEnabled() ? pfunctions::MOVERIGHT : pfunctions::NONE;
		case KEY_UP:
			return gamemanager::getArrowKeysEnabled() ? pfunctions::MOVEUP : pfunctions::NONE;
		case 's':
			return pfunctions::WAIT;
		case '[':
			return pfunctions::TERMUP;
		case ']':
			return pfunctions::TERMDOWN;
		case 'd':
			return pfunctions::REFRESH;
		case 27:
			altinput = disp.getChar();
			if(altinput == 'Q' || altinput == 'q') {
				bool efin = false;
				disp.prints(0,1, "Are you sure you wish to quit? y/n");
				disp.refreshWin();
				while (!efin) {
					qinput = disp.getChar();
					switch(qinput) {
					case 'Y':
					case 'y':
						return pfunctions::QUIT;
					case 'N':
					case 'n':
						for(int i = 0; i < 37; ++i) {
							disp.moveCursor(0,i+1);
							if(!gamemanager::getWidechars()) {
								disp.printsw("-");
							}
							else {
								disp.printsw("\u2500");
							}
						}
						efin = true;
					default:
						break;
					}
				}
			}
			break;
#ifdef __APPLE__ // temp fix for quitting on Apple computers
		case 'q':
			return pfunctions::QUIT;
#endif
		default:
			break;
		}
	}

	return pfunctions::NONE;
}

/*
 * Performs requested player action.
 *
 * @return A boolean signifying a valid move, and therefore the end of the player's turn.
 */
bool player::doAction(pfunctions::playeraction a) {
	display& disp = gamemanager::getDisplay(); // display alias
	if(pfunctions::isMove(a)) {
		::level& lev = theWorld->getArea(getZ());
		int x = getX();
		int y = getY();
		int z = getZ();
		switch(a) {
		case pfunctions::MOVEDOWNLEFT:
			x += -1;
			y += 1;
			break;
		case pfunctions::MOVEDOWN:
			y += 1;
			break;
		case pfunctions::MOVEDOWNRIGHT:
			x += 1;
			y += 1;
			break;
		case pfunctions::MOVELEFT:
			x += -1;
			break;
		case pfunctions::MOVERIGHT:
			x += 1;
			break;
		case pfunctions::MOVEUPLEFT:
			x += -1;
			y += -1;
			break;
		case pfunctions::MOVEUP:
			y += -1;
			break;
		case pfunctions::MOVEUPRIGHT:
			x += 1;
			y += -1;
			break;
		default:
			break;
		}
		tile& tileProspect = lev.getTileRef(x,y); // tile to move to
		if (x>=0 && x<= theWorld->getWidth() && y>=0 && y<= theWorld->getHeight()) { //staying in this area
			if( canMove(tileProspect.getType()) ) { // is the tile a floor, stairs, etc.
				if ((tileProspect.isEmpty())
						// for if the player "skips" and the tile has a pointer to the player itself
						|| (this == dynamic_cast<player*>(tileProspect.getActor())) ) {
					moveActor(x, y, z);
					return true;
				} else if(myPet == dynamic_cast<pet*>(tileProspect.getActor())){ // if player runs into its pet
					// then perform displacePet()
					displacePet();
					return true;
				} else {
					return false;
					//COMBAT!
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		switch(a) {
		case pfunctions::TERMUP:
			disp.moveTerm(-5);
			break;
		case pfunctions::TERMDOWN:
			disp.moveTerm(5);
			break;
		case pfunctions::REFRESH:{
			disp.clear();
			disp.resize();
			std::stringstream msg;
			msg << "Screen Refreshed                             "; // need space for overwriting text
			disp.appendString(msg.str().c_str());
			printHUD(); //Reprints player info to the screen.
			break;
		}
		default:
			break;
		}
	}
	if(a == pfunctions::SKIP)
		return true;
	if(a == pfunctions::WAIT) {
		// FIXME Sometimes, instead of waiting n turns, game waits 1 turn then skips n turns on next turn.
//		p->setNTurn(turn + disp.getNum("Wait for how long? ", 4));
		int waitFor = disp.getNum("Wait for how long? ", 4);
		while(waitFor < 1){
			waitFor = disp.getNum("Wait for how long? ", 4);
		}
		increaseWait(waitFor - 1); // include current turn
		return true;
	}
	return false;
}


//void player::setNTurn(int i){
//	if(i > nturn){  // i should always be higher...
//		hunger += i - nturn; // increment hunger
//		nturn = i;
//	}
//}

char player::toChar() const {
	return '@';
}

std::string player::enumToString() const{
	switch(pClass) {
	case fighter:
		return "Fighter ";
		break;
	case archer:
		return "Archer ";
		break;
	case mage:
		return "Mage ";
		break;
	default:
		break;
	}
	return " ";
}

//void player::giveMap(const pmap& mop) {
//	playermap = mop;
//}

void player::decrementWait(){
	if(waiting){
		--waiting;
	}
}

void player::increaseWait(int incr){
	if(incr > 0){
		waiting += incr;
	} // else error
}

void player::printHUD() const {
	display& disp = gamemanager::getDisplay(); // display alias
	//print the turn counter to the terminal
	disp.printTerminal();
	//print the player's information
	disp.printPlayer(this);
	//print the map the player is currently on
	disp.printMap(this);
	//reprint the frame in case of overprints
	disp.printFrame();//to be removed
}

void player::givePet(pet* newPet){
	if(myPet == NULL && newPet != NULL){
		myPet = newPet;
	} else {
		throw std::invalid_argument("In player::receivePet: invalid argument.");
	}
}

/*
 * Implements swapping the player and pet positions.
 */
void player::displacePet(){
	tile& playerTile = theWorld->getArea(getZ()).getTileRef(getX(), getY());
	tile& petTile = theWorld->getArea(myPet->getZ()).getTileRef(myPet->getX(), myPet->getY());
	playerTile.removeActor();
	petTile.removeActor();

	tile newPetTile;
	newPetTile.setType(playerTile.getType());
	newPetTile.setActor(myPet);
	theWorld->getArea(getZ()).setTile(getX(), getY(), newPetTile); // put new pet tile where player was

	tile newPlayerTile;
	newPlayerTile.setType(petTile.getType());
	newPlayerTile.setActor(this);
	theWorld->getArea(myPet->getZ()).setTile(myPet->getX(), myPet->getY(), newPlayerTile); // put new player tile where pet was

	int oldPlayerX = getX(), oldPlayerY = getY(), oldPlayerZ = getZ(); // temps for player position
	move(myPet->getX(), myPet->getY(), myPet->getZ()); // move player where pet was
	myPet->move(oldPlayerX, oldPlayerY, oldPlayerZ); // move pet where player was

	// print to terminal that the pet was displaced
	std::stringstream tur;
	tur << "You displaced your pet!           "; // need space for overwriting text
	display& disp = gamemanager::getDisplay(); // display alias
	disp.appendString(tur.str().c_str());
}

void player::updateHungerStatus() {
	if(hunger < 0) {
		hungerStatus = "N/A      ";
	}
	else if(hunger < 100) {
		hungerStatus = "Full    ";
	}
	else if(hunger < 200) {
		hungerStatus = "Satiated";
	}
	else if(hunger < 300) {
		hungerStatus = "Hungry  ";
	}
	else if(hunger < 400) {
		hungerStatus = "Starving";
	}
	else {
		hungerStatus = "Dying   ";
	}
}

std::string player::getHungerStatus() const {
	return hungerStatus;
}

std::string player::getName() const {return name;}
int player::getStrength() const {return strength;}
int player::getDexterity() const {return dexterity;}
int player::getConstitution() const {return constitution;}
int player::getIntelligence() const {return intelligence;}
int player::getWisdom() const {return wisdom;}
int player::getCharisma() const {return charisma;}
int player::getXP() const {return xp;}
int player::getHunger() const {return hunger;}
int player::getLevel() const {return level;}
bool player::isWaiting() const {return waiting;} // if waiting is 0, returns false; true otherwise
