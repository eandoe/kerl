/*
 * level.cpp
 *
 *  Created on: 2012/12/01
 *      Author: Bernhard Strauss
 */

#include "level.h"

#include <math.h>

#include "world.h"
#include "tile.h"
#include "pfunctions.h"

/**
 * Default constructor.
 */
//level::level() :height(30), width(60) {
	// TODO Auto-generated constructor stub
//	tiles = NULL;
//	tiledefaults = NULL;
//}


level::level(int x, int y) :height(y), width(x), tiles(height) {
	for (int i = 0; i < height; ++i) {
		tiles[i] = std::vector<tile>(width);
	}
}


/**
 * Default destructor for area.
 */
level::~level() {
}

//add a set of tiles
void level::setTiles(int x, int y, const std::vector<std::vector<tile> >& newTiles){
	for(int i = 0; i < world::baseAreaHeight; i++){
		for(int j = 0; j < world::baseAreaWidth; j++){
			tiles[i + y * world::baseAreaHeight][j + x * world::baseAreaWidth] = newTiles[i][j];
			if(tiles[i + y * world::baseAreaHeight][j + x * world::baseAreaWidth].isDefault()){
				tiletype::t_type ty = tiles[i + y * world::baseAreaHeight][j + x * world::baseAreaWidth].getType();
				tiles[i + y * world::baseAreaHeight][j + x * world::baseAreaWidth] = getDefault(ty);
			}
		}
	}
}

//add a single tile
void level::setTile(int x, int y, const tile& newTile){
	tiles[y][x] = newTile;
	if(tiles[y][x].isDefault()){
		tiles[y][x] = getDefault(tiles[y][x].getType());
	}
}

tile level::getDefault(tiletype::t_type ty){ //TODO: Probably want to move to world
	return(tiledefaults[ty]);
}

tile& level::getTileRef(int x, int y) {
	return tiles[y][x];
}

/**
 * Returns the tile at the given x,y coordinate
 *
 * @return - The tile located at line y, column x
 */
const tile& level::getTile(int x, int y) const {
	return tiles[y][x];
}

int level::getHeight() const {return height;}
int level::getWidth() const {return width;}


// construct level::tiledefaults with 12 default tiles
std::vector<tile> level::init_tiledefaults(){
	std::vector<tile> temp;
	temp.push_back(tile().setType(tiletype::STAIRSUP));
	temp.push_back(tile().setType(tiletype::STAIRSDOWN));
	temp.push_back(tile().setType(tiletype::ROOMFLOOR));
	temp.push_back(tile().setType(tiletype::HROOMWALL));
	temp.push_back(tile().setType(tiletype::VROOMWALL));
	temp.push_back(tile().setType(tiletype::TLROOMCORNER));
	temp.push_back(tile().setType(tiletype::TRROOMCORNER));
	temp.push_back(tile().setType(tiletype::BLROOMCORNER));
	temp.push_back(tile().setType(tiletype::BRROOMCORNER));
	temp.push_back(tile().setType(tiletype::SOLIDWALL));
	temp.push_back(tile().setType(tiletype::RUBBLE));
	temp.push_back(tile().setType(tiletype::NONE));

	return temp;
}
// initialize tiledefaults
std::vector<tile> level::tiledefaults(level::init_tiledefaults());

bool level::getOcclusion(int x, int y){
	if(x == 0 || y == 0) return false;
	if(tiles[y][x].isStair() || tiles[y][x].isFloor()) return true;
	return false;
}

double level::distance(int x, int y, int x2, int y2){
	return sqrt(pow(x2 - x, 2) + pow(y2 - y, 2));
}
