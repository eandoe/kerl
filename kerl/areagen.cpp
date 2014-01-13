/*
 * areagen.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: Bernhard Straus
 */

#include "areagen.h"

#include <stdlib.h>

using namespace std;

/**
 *	No default constructor to keep class invariant
 */

/**
 *	Non-default constructor
 *
 * @param x -
 * @param y -
 * @param pp -
 * @param rng -
 */
areagen::areagen(int x, int y) :height(y), width(x),
					areastate(height) {
// Bugy areagen seed: Sat, Dec 15, 2012  5:43:17 PM : 1355614997 : (2, 2, 1)

	for (int i = 0; i < height; ++i) {
		areastate[i] = std::vector<int>(width);
	}

	topCons = std::vector<int>(width);
	botCons = std::vector<int>(width);
	leftCons = std::vector<int>(height);
	rightCons = std::vector<int>(height);
	numUDCons = std::vector<int>(2);
	upCons = std::vector<std::vector<int> >(2);	//temp initialization
	downCons = std::vector<std::vector<int> >(2);	//temp initialization
}

/**
 *	Destructor.
 */
areagen::~areagen() {
}

/**
 *
 *
 * @param type -
 * @param topCon -
 * @param botCon -
 * @param leftCon -
 * @param rightCon -
 * @param numUDCon -
 * @param upCon -
 * @param downCon -
 */
areagen::tilestruct areagen::generate(areatype::a_type type, vecint topCon, vecint botCon,
		vecint leftCon, vecint rightCon, vecint numUDCon,
		vecvecint upCon, vecvecint downCon)
{
	//assign connections
	topCons = topCon;
	botCons = botCon;
	leftCons = leftCon;
	rightCons = rightCon;
	numUDCons = numUDCon;
	upCons = upCon;
	downCons = downCon;

	//Create tile arrays

	tiles = std::vector<std::vector<tile> >(height);
	for (int i = 0; i < height; ++i) {
		tiles[i]= std::vector<tile>(width); // tile automatically default constructed
	}

	switch(type){
		case areatype::EMPTYROOM:
			generateEmptyRoom();
			break;
		case areatype::CAVE:
			generateCave();
			break;
		case areatype::ROOMSANDHALLS:
			generateRoomsandHalls();
		default:
			break;
	}
	return tiles;
}

/**
 * Creates a simple empty room with encircling wall
 */
void areagen::generateEmptyRoom(){
	emptyRoomOuterWall();
	emptyRoomSetStairs();
}

/**
 * Initializes the walls of one big area.  No connections.
 */
void areagen::emptyRoomOuterWall() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if(j == 0) {
				tiles[i][j].setType(tiletype::VROOMWALL);
				if(leftCons[i] == 1){
					tiles[i][j].setType(tiletype::ROOMFLOOR);
				}
			}
			else if(j == width-1){
				tiles[i][j].setType(tiletype::VROOMWALL);
				if(rightCons[i] == 1){
					tiles[i][j].setType(tiletype::ROOMFLOOR);
				}
			}
			else if(i == 0) {
				if(j == 0) {
					tiles[i][j].setType(tiletype::TLROOMCORNER);
				}
				else if(j == width-1) {
					tiles[i][j].setType(tiletype::TRROOMCORNER);
				}
				else {
					tiles[i][j].setType(tiletype::HROOMWALL);
				}
				if(topCons[j] == 1){
					tiles[i][j].setType(tiletype::ROOMFLOOR);
				}
			}
			else if(i == height-1) {
				if(j == 0) {
					tiles[i][j].setType(tiletype::BLROOMCORNER);
				}
				else if(j == width-1) {
					tiles[i][j].setType(tiletype::BRROOMCORNER);
				}
				else {
					tiles[i][j].setType(tiletype::HROOMWALL);
				}
				if(botCons[j] == 1){
					tiles[i][j].setType(tiletype::ROOMFLOOR);
				}
			}
			else {
				tiles[i][j].setType(tiletype::ROOMFLOOR);
			}
		}
	}
}

/**
 *
 */
void areagen::emptyRoomSetStairs() {
	//Place stairs according to the given connectivity
	for(int k = 0; k < numUDCons[0]; k++){
		tiles[upCons[1][k]][upCons[0][k]].setType(tiletype::STAIRSUP);
	}

	for(int k = 0; k < numUDCons[1]; k++){
		tiles[downCons[1][k]][downCons[0][k]].setType(tiletype::STAIRSDOWN);
	}

}

/**
 * Creates a cave-like area with large, irregular caverns and connecting
 * tunnels.
 */
void areagen::generateCave(){
	//Fill map with rock
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			tiles[i][j].setType(tiletype::SOLIDWALL);
		}
	}
	bool done = false;
	while(!done){

	}
}

/**
 * Creates rooms with connecting hallways, similar to Nethack's dungeons
 */
void areagen::generateRoomsandHalls(){

	int tmpa;
	int tmpb;
	int tmpc;
	int tmpd;
	int minRoomX = 5;
	int minRoomY = 4;
	int maxRoomX = 10;
	int maxRoomY = 12;
	int minNumRooms = height * width / 250;
	int maxNumRooms = 2 * minNumRooms;

	std::vector<std::vector<int> > roomdat(maxNumRooms); // contains position and size of each room, plus # of
	// bug here? should be  i < maxNumRooms;
//	for (int i = 0; i < maxNumRooms + 1; ++i) { // connecting corridors and connectivity to other rooms
	for (int i = 0; i < maxNumRooms; ++i) { // connecting corridors and connectivity to other rooms
		roomdat[i]= std::vector<int>(5);
	}

	//Fill map with rock
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			tiles[i][j].setType(tiletype::SOLIDWALL);
			areastate[i][j] = 0;
		}
	}

	//start with making a bunch of rooms
	bool done = false;
	int fails = 0;
	int rooms = 0;

	//First set the stairs
	for(int k = 0; k < numUDCons[0]; k++){
		tiles[upCons[1][k]][upCons[0][k]].setType(tiletype::STAIRSUP);
	}

	for(int k = 0; k < numUDCons[1]; k++){
		tiles[downCons[1][k]][downCons[0][k]].setType(tiletype::STAIRSDOWN);
	}

	//Then make rooms on the stairs
	for(int k = 0; (k < numUDCons[0] + numUDCons[1]) && (size_t(rooms)<roomdat.size()); k++){
		done = false;
		if(k < numUDCons[0]){
			if(tiles[upCons[1][k] + 1][upCons[0][k]].isFloor()
					|| tiles[upCons[1][k] - 1][upCons[0][k]].isFloor()
					|| tiles[upCons[1][k]][upCons[0][k] + 1].isFloor()
					|| tiles[upCons[1][k]][upCons[0][k] - 1].isFloor()) done = true;
		}
		else{
			int j = k - numUDCons[0];
			if(tiles[downCons[1][j] + 1][downCons[0][j]].isFloor()
					|| tiles[downCons[1][j] - 1][downCons[0][j]].isFloor()
					|| tiles[downCons[1][j]][downCons[0][j] + 1].isFloor()
					|| tiles[downCons[1][j]][downCons[0][j] - 1].isFloor()) done = true;
		}
		while(!done){
			tmpc = rngke::uniform(minRoomX, maxRoomX);
			tmpd = rngke::uniform(minRoomY, maxRoomY);
			if(k < numUDCons[0]){
				tmpa = rngke::uniform(upCons[0][k] - tmpc + 2,upCons[0][k] - 1);
				tmpb = rngke::uniform(upCons[1][k] - tmpd + 2,upCons[1][k] - 1);
			}
			else{
				int j = k - numUDCons[0];
				tmpa = rngke::uniform(downCons[0][j] - tmpc + 2,downCons[0][j] - 1);
				tmpb = rngke::uniform(downCons[1][j] - tmpd + 2,downCons[1][j] - 1);
			}
			if(makeRectRoom(tmpa, tmpb, tmpc, tmpd)){
				roomdat[rooms][0] = tmpa;
				roomdat[rooms][1] = tmpb;
				roomdat[rooms][2] = tmpc;
				roomdat[rooms][3] = tmpd;
				roomdat[rooms][4] = 0;
				rooms++;
				for(int i = tmpa + 1; i < tmpa + tmpc - 1; ++i){
					for(int j = tmpb + 1; j < tmpb + tmpd - 1; ++j){
						areastate[j][i] = rooms;
					}
				}
				done = true;
			}
		}
	}
	// rooms is <= roomdat.size()
	// so roomdat[rooms] may segfault

	done = false;
	//Then more random rooms
	while(!done && (size_t(rooms)<roomdat.size())){
		tmpa = rngke::uniform(2,width-2-minRoomX);
		tmpb = rngke::uniform(2,height-2-minRoomY);
		tmpc = rngke::uniform(minRoomX, maxRoomX);
		tmpd = rngke::uniform(minRoomY, maxRoomY);
		if(makeRectRoom(tmpa, tmpb, tmpc, tmpd)){
			fails = 0;
			roomdat[rooms][0] = tmpa;
			roomdat[rooms][1] = tmpb;
			roomdat[rooms][2] = tmpc;
			roomdat[rooms][3] = tmpd;
			roomdat[rooms][4] = 0;
			rooms++;
			for(int i = tmpa + 1; i < tmpa + tmpc - 1; ++i){
				for(int j = tmpb + 1; j < tmpb + tmpd - 1; ++j){
					areastate[j][i] = rooms;
				}
			}
		}
		else {
			fails++;
		}
		if(fails > minNumRooms * 2 && rooms > minNumRooms) done = true; // enough failed attempts, and acceptable # of rooms
		if(size_t(rooms) >= roomdat.size()-1) done = true;
	}
	// rooms is <= roomdat.size()
	// so roomdat[rooms] may segfault

	//now make at least one corridor from each room

	for(int n = 0; n < rooms; n++){
		//Have at most 4 corridors entering a small room, 6 for bigger ones
//		if(roomdat[n][4] < 4 || (roomdat[n][4] < 6 && (roomdat[n][2] + roomdat[n][3] > 11))){
			makeCorridor(roomdat, n, n + 1, true);
			//fill it in
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					if(areastate[i][j] > 0){
						if(tiles[i][j].isSolid()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);
						}
						if(tiles[i][j].isWall()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);//change to set a door
						}
					}
				}
			}
//		}
	}

	//and one corridor from each connecting point


	int big = width - height > 0 ? width : height;
	int small = width - height > 0 ? height : width;
	int con = rooms + 1;
	// rooms is <= roomdat.size()
	// so roomdat[rooms] may segfault
	for(int i = 0; (i < big) && (size_t(rooms)+1 < roomdat.size()); i++){ // temp fix: (size_t(rooms)+1 < roomdat.size())
		if(topCons[i] == 1){
			roomdat[rooms + 1][0] = i;
			roomdat[rooms + 1][1] = 0;
			roomdat[rooms + 1][2] = 0;
			roomdat[rooms + 1][3] = 0;
			roomdat[rooms + 1][4] = 0;
			makeCorridor(roomdat, rooms + 1, con++, false);
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					if(areastate[i][j] > 0){
						if(tiles[i][j].isSolid()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);
						}
						if(tiles[i][j].isWall()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);//change to set a door
						}
					}
				}
			}
		}
		if(botCons[i] == 1){
			roomdat[rooms + 1][0] = i;
			roomdat[rooms + 1][1] = height-1;
			roomdat[rooms + 1][2] = 0;
			roomdat[rooms + 1][3] = 0;
			roomdat[rooms + 1][4] = 0;
			makeCorridor(roomdat, rooms + 1, con++, false);
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					if(areastate[i][j] > 0){
						if(tiles[i][j].isSolid()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);
						}
						if(tiles[i][j].isWall()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);//change to set a door
						}
					}
				}
			}
		}
		if(i < small){
			if(leftCons[i] == 1){
				roomdat[rooms + 1][0] = 0; // FIXME has segfaulted with seed 1358308330
				roomdat[rooms + 1][1] = i;
				roomdat[rooms + 1][2] = 0;
				roomdat[rooms + 1][3] = 0;
				roomdat[rooms + 1][4] = 0;
				makeCorridor(roomdat, rooms + 1, con++, false);
				for (int i = 0; i < height; ++i) {
					for (int j = 0; j < width; ++j) {
						if(areastate[i][j] > 0){
							if(tiles[i][j].isSolid()){
								tiles[i][j].setType(tiletype::ROOMFLOOR);
							}
							if(tiles[i][j].isWall()){
								tiles[i][j].setType(tiletype::ROOMFLOOR);//change to set a door
							}
						}
					}
				}
			}
			if(rightCons[i] == 1){
				roomdat[rooms + 1][0] = width - 1; // FIXME has segfaulted with seed 1358308550
				roomdat[rooms + 1][1] = i;
				roomdat[rooms + 1][2] = 0;
				roomdat[rooms + 1][3] = 0;
				roomdat[rooms + 1][4] = 0;
				makeCorridor(roomdat, rooms + 1, con++, false);
				for (int k = 0; k < height; ++k) {
					for (int j = 0; j < width; ++j) {
						if(areastate[k][j] > 0){
							if(tiles[k][j].isSolid()){
								tiles[k][j].setType(tiletype::ROOMFLOOR);
							}
							if(tiles[k][j].isWall()){
								tiles[k][j].setType(tiletype::ROOMFLOOR);//change to set a door
							}
						}
					}
				}
			}
		}
	}
	// rooms is <= roomdat.size()
	// so roomdat[rooms] may segfault

	//randomly make more until all are connected
	int max = rooms;
	big = 0;
	small = 0;
	int r;
	bool changed;
	fails = 0;
	std::vector<std::vector<int> > areastateold = std::vector<std::vector<int> >(height);
	for (int i = 0; i < height; ++i) {
		areastateold[i]= std::vector<int>(width);
	}

	while(1){
		//update connectivity
		changed = false;
		for (int i = 0; i < height - 1; ++i) {
			for (int j = 0; j < width - 1; ++j) {
				if(areastate[i][j + 1] != 0 && areastate[i][j] != 0 && areastate[i][j] != areastate[i][j + 1]){
					small = areastate[i][j + 1];
					big = areastate[i][j];
					//Ensures it will be 1 when all connected
					if(big < small){
						int tmp = small;
						small = big;
						big = tmp; // swapping this way doesn't work
					}
					for (int k = 0; k < height; ++k) {
						for (int l = 0; l < width; ++l) {
							if(areastate[k][l] == big){
								areastate[k][l] = small;
							}
						}
					}
					changed = true;
				}
				if(areastate[i + 1][j] != 0 && areastate[i][j] != 0 && areastate[i][j] != areastate[i + 1][j]){
					small = areastate[i + 1][j];
					big = areastate[i][j];
					if(big < small){
						int tmp = small;
						small = big;
						big = tmp; // swapping this way doesn't work
					}
					for (int k = 0; k < height; ++k) {
						for (int l = 0; l < width; ++l) {
							if(areastate[k][l] == big){
								areastate[k][l] = small;
							}
						}
					}
					changed = true;
				}
			}
		}


		if(!changed){
			if(rngke::uniform(0,fails * 2) == 0) changed = true;
		}

		//fill in the new corridor
		if(changed){
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					if(areastate[i][j] > 0){
						if(tiles[i][j].isSolid()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);
						}
						if(tiles[i][j].isWall()){
							tiles[i][j].setType(tiletype::ROOMFLOOR);//change to set a door
						}
					}
				}
			}
		}
		else fails++;

		max = 0;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if(areastate[i][j] > max){
					max = areastate[i][j];
				}
				//Create/update backup to revert to if we don't want the next corridor
				if(changed){
					areastateold[i][j] = areastate[i][j];
				}
				//Revert to backup to not use corridor
				else{
					areastate[i][j] = areastateold[i][j];
				}
			}
		}
		if(max == 1) break; //only 1 connectivity group, so done

		//Have at most 4 corridors entering a small room, 6 for bigger ones
//		if(roomdat[n][4] < 4 || (roomdat[n][4] < 6 && (roomdat[n][2] + roomdat[n][3] > 11))){
		while(1){
			r = rngke::uniform(0,rooms-1);
//			if(areastate[roomdat[r][0] + 1][roomdat[r][1] + 1]){
				makeCorridor(roomdat, r, r + 1, true);
				break;
//			}
		}
//		}
	}

	return;
}

/**
 * Checks an area, and if unused, creates a rectangular room
 *
 * @param sx -
 * @param sy -
 * @param w -
 * @param h -
 */
bool areagen::makeRectRoom(int sx, int sy, int w, int h){

	if(sx + w + 1 >= width || sx < 2) return false;
	if(sy + h + 1 >= height || sy < 2) return false;

	for(int i = sx - 2; i < sx + w + 1; ++i){
		for(int j = sy - 2; j < sy + h + 1; ++j){
			//make sure the area is not already built in or next to (1 tile each way)
			if(i != sx - 2 && i != sx + w + 1 && j != sy - 2 && j != sy + h + 1)
					if(!tiles[j][i].isSolid()) return false;

			//and stairs 2 tiles away or on wall are bad
			if(i <= sx || i >= sx + w - 1 || j <= sy || j >= sy + h - 1)
				if(tiles[j][i].isStair()) return false;
		}
	}

	for(int i = sx; i < sx + w; ++i){
		for(int j = sy; j < sy + h; ++j){
			if(i == sx && j == sy) {
				tiles[j][i].setType(tiletype::TLROOMCORNER);
			}
			else if(i == sx && j == sy + h - 1) {
				tiles[j][i].setType(tiletype::BLROOMCORNER);
			}
			else if(i == sx + w - 1 && j == sy) {
				tiles[j][i].setType(tiletype::TRROOMCORNER);
			}
			else if(i == sx + w - 1 && j == sy + h - 1) {
				tiles[j][i].setType(tiletype::BRROOMCORNER);
			}
			else if(i == sx || i == sx + w - 1) {
				tiles[j][i].setType(tiletype::VROOMWALL);
			}
			else if(j == sy || j == sy + h - 1) {
				tiles[j][i].setType(tiletype::HROOMWALL);
			}
			else {
				if(!tiles[j][i].isStair()){
				tiles[j][i].setType(tiletype::ROOMFLOOR);
				}
			}
		}
	}


	return true;
}

/**
 * Make a corridor from a given room to any other part of the dungeon
 *
 * @param roomdat -
 * @param n -
 * @param con -
 * @param roomStart -
 */
bool areagen::makeCorridor(const std::vector<std::vector<int> >& roomdat, int n, int con, bool roomStart){
	//TODO: make this check areastate > 0 instead of isFloor
	int sx = roomdat[n][0];
	int sy = roomdat[n][1];
	int w = roomdat[n][2];
	int h = roomdat[n][3];
	int dir = rngke::uniform(0,3);
	int r = 0;
	int disx = 0;
	int disy = 0;
	int fails = 0; // keep track of # of failed attempts at continuing the corridor to prevent infinite loops
	while(1){
		if(fails > 15) return true;
		fails++; //reset when progress made
		switch (dir) {
		case 0: //Up
			//starting here, so pick a point in the correct wall
			if(disx == 0 && disy == 0){
				disy = 0;
				if(roomStart){
					disx = rngke::uniform(1,w - 2);
					if(tiles[sy + disy][sx + disx + 1].isWall()//need to be wall type
							&& tiles[sy + disy][sx + disx - 1].isWall()){
						//Don't want to be adjacent to another door/corridor
						areastate[sy + disy - 1][sx + disx] = con;
						areastate[sy + disy][sx + disx] = con;
						if(tiles[sy + disy - 1][sx + disx + 1].isFloor() == tiletype::ROOMFLOOR
							|| tiles[sy + disy - 1][sx + disx - 1].isFloor()
							|| tiles[sy + disy - 2][sx + disx].isFloor()){// Found a corridor
							return true;
						}
						disy--;
						dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
						fails = 0;
						break;
					}
					else {
						dir = rngke::uniform(0,3); // Try again
						disx = 0;
						disy = 0;
						break;
					}
				}
				else if(sy != height - 1){
					dir = 1; // Try again
					disx = 0;
					disy = 0;
					break;
				}
			}

			for(int i = 1; i < sy + disy; i++){ //try to find an existing room or corridor in the current direction
				if(tiles[sy + disy - i][sx + disx].isFloor()
						|| tiles[sy + disy - i][sx + disx + 1].isFloor()
						|| tiles[sy + disy - i][sx + disx - 1].isFloor()){ // Found a corridor
					for(int j = 0; j <= i; j++){
						areastate[sy + disy - j][sx + disx] = con;
					}
					return true;
				}
				else if(tiles[sy + disy - i][sx + disx].isWall()){ // Found a Room
					if(tiles[sy + disy - i][sx + disx + 1].isWall()
							&& tiles[sy + disy - i][sx + disx - 1].isWall()){ //Room side
						for(int j = 0; j <= i; j++){
							areastate[sy + disy - j][sx + disx] = con;
						}
						return true;
					}
					else{// room corner
						for(int j = 0; j < i; j++){
							areastate[sy + disy - j][sx + disx] = con;
						}
						if(tiles[sy + disy - i - 1][sx + disx + 1].isFloor()){
							areastate[sy + disy - i + 1][sx + disx + 1] = con;
							if(!tiles[sy + disy - i + 1][sx + disx + 2].isFloor())
								areastate[sy + disy - i][sx + disx + 1] = con;
						}
						else{
							areastate[sy + disy - i + 1][sx + disx - 1] = con;
							if(!tiles[sy + disy - i + 1][sx + disx - 2].isFloor())
								areastate[sy + disy - i][sx + disx - 1] = con;
						}
						return true;
					}
				}
			}

			//nothing in the given direction, so go a little bit then take a turn (maybe)
			r = rngke::uniform(3,9);
			if(sy + disy - r > 0){ //stay within the bounds
				for(int i = 0; i < r; i++){

					areastate[sy + disy - i][sx + disx] = con;
				}
				disy -= r;
				fails = 0;
			}
			dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
			break;







		case 1: //Right
			//starting here, so pick a point in the correct wall
			if(disx == 0 && disy == 0){
				if(roomStart){
					disy = rngke::uniform(1, h - 2);
					disx = w - 1;
					if(tiles[sy + disy + 1][sx + disx].isWall()
							&& tiles[sy+ disy - 1][sx + disx].isWall()){
						//Don't want to be adjacent to another door/corridor
						areastate[sy + disy][sx + disx + 1] = con;
						areastate[sy + disy][sx + disx] = con;
						if(tiles[sy + disy + 1][sx + disx + 1].isFloor()
							|| tiles[sy + disy - 1][sx + disx + 1].isFloor()
							|| tiles[sy + disy][sx + disx + 2].isFloor()){// Found a corridor
							return true;
						}
						disx++;
						dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
						fails = 0;
						break;
					}
					else {
						dir = rngke::uniform(0,3); // Try again
						disx = 0;
						disy = 0;
						break;
					}
				}
				else if(sx != 0){
					dir = 2; // Try again
					disx = 0;
					disy = 0;
					break;
				}

			}

			for(int i = 1; i < width - 1 - sx - disx; i++){ //try to find an existing room or corridor in the current direction
				if(tiles[sy + disy][sx + disx + i].isFloor()
						|| tiles[sy + disy - 1][sx + disx + i].isFloor()
						|| tiles[sy + disy + 1][sx + disx + i].isFloor()){ // Found a corridor
					for(int j = 0; j <= i; j++){
						areastate[sy + disy][sx + disx + j] = con;
					}
					return true;
				}
				else if(tiles[sy + disy][sx + disx + i].isWall()){ // Found a Room
					if(tiles[sy + disy + 1][sx + disx + i].isWall()
							&& tiles[sy + disy - 1][sx + disx + i].isWall()){ //Room side
						for(int j = 0; j <= i; j++){
							areastate[sy + disy][sx + disx + j] = con;
						}
						return true;
					}
					else{// room corner
						for(int j = 0; j < i; j++){
							areastate[sy + disy][sx + disx + j] = con;
						}
						if(tiles[sy + disy + 1][sx + disx + i + 1].isFloor()){
							areastate[sy + disy + 1][sx + disx + i - 1] = con;
							if(!tiles[sy + disy + 2][sx + disx + i - 1].isFloor())
								areastate[sy + disy + 1][sx + disx + i] = con;
						}
						else{
							areastate[sy + disy - 1][sx + disx + i - 1] = con;
							if(!tiles[sy + disy - 2][sx + disx + i - 1].isFloor())
								areastate[sy + disy - 1][sx + disx + i] = con;
						}
						return true;
					}
				}
			}

			//nothing in the given direction, so go a little bit then take a turn (maybe)
			r = rngke::uniform(3,12);
			if(sx + disx + r < width - 1){ //stay within the bounds
				for(int i = 0; i < r; i++){
					areastate[sy + disy][sx + disx + i] = con;
				}
				disx += r;
				fails = 0;
			}
			dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
			break;








		case 2: //Down
			//starting here, so pick a point in the correct wall
			if(disx == 0 && disy == 0){
				if(roomStart){
					disy = h - 1;
					disx = rngke::uniform(1,w - 2);
					if(tiles[sy + disy][sx + disx + 1].isWall()
							&& tiles[sy + disy][sx + disx - 1].isWall()){
						//Don't want to be adjacent to another door/corridor
						areastate[sy + disy + 1][sx + disx] = con;
						areastate[sy + disy][sx + disx] = con;
						if(tiles[sy + disy + 1][sx + disx + 1].isFloor()
							|| tiles[sy + disy + 1][sx + disx - 1].isFloor()
							|| tiles[sy + disy + 2][sx + disx].isFloor()){// Found a corridor
							return true;
						}
						disy++;
						dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
						fails = 0;
						break;
					}
					else {
						dir = rngke::uniform(0,3); // Try again
						disx = 0;
						disy = 0;
						break;
					}
				}
				else if(sy != 0){
					dir = 3; // Try again
					disx = 0;
					disy = 0;
					break;
				}
			}

			for(int i = 1; i < height - 1 - sy - disy; i++){ //try to find an existing room or corridor in the current direction
				if(tiles[sy + disy + i][sx + disx].isFloor()
						|| tiles[sy + disy + i][sx + disx + 1].isFloor()
						|| tiles[sy + disy + i][sx + disx - 1].isFloor()){ // Found a corridor
					for(int j = 0; j <= i; j++){
						areastate[sy + disy + j][sx + disx] = con;
					}
					return true;
				}
				else if(tiles[sy + disy + i][sx + disx].isWall()){ // Found a Room
					if(tiles[sy + disy + i][sx + disx - 1].isWall()
							&& tiles[sy + disy + i][sx + disx + 1].isWall()){ //Room side
						for(int j = 0; j <= i; j++){
							areastate[sy + disy + j][sx + disx] = con;
						}
						return true;
					}
					else{// room corner
						for(int j = 0; j < i; j++){
							areastate[sy + disy + j][sx + disx] = con;
						}
						if(tiles[sy + disy + i + 1][sx + disx + 1].isFloor()){
							areastate[sy + disy + i - 1][sx + disx + 1] = con;
							if(!tiles[sy + disy + i - 1][sx + disx + 2].isFloor())
								areastate[sy + disy + i][sx + disx + 1] = con;
						}
						else{
							areastate[sy + disy + i - 1][sx + disx - 1] = con;
							if(!tiles[sy + disy + i - 1][sx + disx - 2].isFloor())
								areastate[sy + disy + i][sx + disx - 1] = con;
						}
						return true;
					}
				}
			}

			//nothing in the given direction, so go a little bit then take a turn (maybe)
			r = rngke::uniform(3,9);
			if(sy + disy + r < height - 1){ //stay within the bounds
				for(int i = 0; i < r; i++){
					areastate[sy + disy + i][sx + disx] = con;
				}
				disy += r;
				fails = 0;
			}
			dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
			break;







		case 3: //Left
			//starting here, so pick a point in the correct wall
			if(disx == 0 && disy == 0){
				if(roomStart){
					disy = rngke::uniform(1, h - 2);
					if(tiles[sy + disy + 1][sx + disx].isWall()
							&& tiles[sy+ disy - 1][sx + disx].isWall()){
						//Don't want to be adjacent to another door/corridor
						areastate[sy + disy][sx + disx - 1] = con;
						areastate[sy + disy][sx + disx] = con;
						if(tiles[sy + disy + 1][sx + disx - 1].isFloor()
							|| tiles[sy + disy - 1][sx + disx - 1].isFloor()
							|| tiles[sy + disy][sx + disx - 2].isFloor()){// Found a corridor
							return true;
						}
						disx--;
						dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
						fails = 0;
						break;
					}
					else {
						dir = rngke::uniform(0,3); // Try again
						disx = 0;
						disy = 0;
						break;
					}
				}
				else if(sx != width - 1){
					dir = 0; // Try again
					disx = 0;
					disy = 0;
					break;
				}
			}

			for(int i = 1; i < sx + disx; i++){ //try to find an existing room or corridor in the current direction
				if(tiles[sy + disy][sx + disx - i].isFloor()
						|| tiles[sy + disy - 1][sx + disx - i].isFloor()
						|| tiles[sy + disy + 1][sx + disx - i].isFloor()){ // Found a corridor
					for(int j = 0; j <= i; j++){
						areastate[sy + disy][sx + disx - j] = con;
					}
					return true;
				}
				else if(tiles[sy + disy][sx + disx - i].isWall()){ // Found a Room
					if(tiles[sy + disy - 1][sx + disx - i].isWall()
							&& tiles[sy + disy + 1][sx + disx - i].isWall()){ //Room side
						for(int j = 0; j <= i; j++){
							areastate[sy + disy][sx + disx - j] = con;
						}
						return true;
					}
					else{// room corner
						for(int j = 0; j < i; j++){
							areastate[sy + disy][sx + disx - j] = con;
						}
						if(tiles[sy + disy + 1][sx + disx - i - 1].isFloor()){
							areastate[sy + disy + 1][sx + disx - i + 1] = con;
							if(!tiles[sy + disy + 2][sx + disx - i + 1].isFloor())
								areastate[sy + disy + 1][sx + disx - i] = con;
						}
						else{
							areastate[sy + disy - 1][sx + disx - i + 1] = con;
							if(!tiles[sy + disy - 2][sx + disx - i + 1].isFloor())
								areastate[sy + disy - 1][sx + disx - i] = con;
						}
						return true;
					}
				}
			}

			//nothing in the given direction, so go a little bit then take a turn (maybe)
			r = rngke::uniform(3,12);
			if(sx + disx - r > 0){ //stay within the bounds
				for(int i = 0; i < r; i++){
					areastate[sy + disy][sx + disx - i] = con;
				}
				disx -= r;
				fails = 0;
			}
			dir = (dir + rngke::uniform(-1,1) + 4)%4; // Pick a new direction (anything but reverse direction)
			break;

		default:
			return false;
		}
	}



	return false;
}

