/*
 * world.cpp
 *
 *  Created on: Nov 7, 2012
 *      Author: Bloodnamed
 */

#include "world.h"

#include <iostream>
#include <stdexcept>

#include "Item.h"

world::world(int xFactor, int yFactor, int zFactor)
				:areaYFactor(yFactor), areaXFactor(xFactor), levelz(zFactor),
				 areaWidth(xFactor*baseAreaWidth), areaHeight(yFactor*baseAreaHeight),
				agen(baseAreaWidth, baseAreaHeight), worldareas(areaXFactor)
{

	for (int i = 0; i < areaXFactor; ++i) {
		worldareas[i] = std::vector<std::vector<area> >(areaYFactor);
		for (int j = 0; j < areaYFactor; ++j) {
			worldareas[i][j] = std::vector<area>();
		}
	}

	//generate all the area types
	for (int i = 0; i < areaXFactor; ++i) {
		for (int j = 0; j < areaYFactor; ++j) {
			for (int k = 0; k < levelz; ++k) {
//				if(j == i + k) worldareas[i][j].push_back(area(areaWidth, areaHeight, areatype::EMPTYROOM, agen));
//				else 		   worldareas[i][j].push_back(area(areaWidth, areaHeight, areatype::ROOMSANDHALLS, agen));
				//randomly assign areatypes everywhere, with empty room to spawn in
				if(i == 0 && j == 0) worldareas[i][j].push_back(area(baseAreaWidth, baseAreaHeight, areatype::EMPTYROOM, agen));
				else {
					switch(rngke::uniform(0,1)){
					case 0:
						worldareas[i][j].push_back(area(baseAreaWidth, baseAreaHeight, areatype::ROOMSANDHALLS, agen));
						break;
					case 1:
						worldareas[i][j].push_back(area(baseAreaWidth, baseAreaHeight, areatype::EMPTYROOM, agen));
						break;
					default:
						worldareas[i][j].push_back(area(baseAreaWidth, baseAreaHeight, areatype::EMPTYROOM, agen));
						break;
					}

				}
			}
		}
	}

	//set the connectivity for everything
//	bool done; // unused
//	int count; // unused
	std::vector<int> consemh(baseAreaHeight);

	for(int i = 0; i < baseAreaHeight; i++){
		consemh[i] = 0;
	}
	std::vector<int> consemw(baseAreaWidth);
	for(int i = 0; i < baseAreaWidth; i++){
		consemw[i] = 0;
	}
	for (int i = 0; i < areaXFactor; ++i) {
		for (int j = 0; j < areaYFactor; ++j) {
			for (int k = 0; k < levelz; ++k) {
				if(i == 0) worldareas[i][j][k].setLeftCons(consemh);

				if(i == areaXFactor - 1) worldareas[i][j][k].setRightCons(consemh);
				else{
//					std::vector<int> cons(areaHeight);
//					for(int m = 0; m < areaHeight; m++){
//						cons[m] = 0;
//					}
//					done = false;
//					count = 0;
//					while(!done){
//						cons[rng.uniform(1,areaHeight - 2)] = 1;
//						rng.uniform(0,2) == 0 ? done = true : done = false; //random num of connections
//						if(count++ == 3) done = true; //at most 3 connections
//					}
//					 worldareas[i][j][k].setRightCons(cons);
//					 worldareas[i + 1][j][k].setLeftCons(cons);
					std::vector<int> cons = generateCons(worldareas[i][j][k].getType(), worldareas[i + 1][j][k].getType(), baseAreaHeight);
					worldareas[i][j][k].setRightCons(cons);
					worldareas[i + 1][j][k].setLeftCons(cons);
				}


				if(j == 0) worldareas[i][j][k].setTopCons(consemw);


				if(j == areaYFactor - 1) worldareas[i][j][k].setBotCons(consemw);
				else{
//					std::vector<int> cons(areaWidth);
//					for(int m = 0; m < areaWidth; m++){
//						cons[m] = 0;
//					}
//					done = false;
//					count = 0;
//					while(!done){
//						cons[rng.uniform(1,areaWidth - 2)] = 1;
//						rng.uniform(0,4) == 0 ? done = true : done = false; //random num of connections
//						if(count++ == 6) done = true; //at most 5 connections
//					}
//					 worldareas[i][j][k].setBotCons(cons);
//					 worldareas[i][j + 1][k].setTopCons(cons);
					std::vector<int> cons = generateCons(worldareas[i][j][k].getType(), worldareas[i][j + 1][k].getType(), baseAreaWidth);
					worldareas[i][j][k].setBotCons(cons);
					worldareas[i][j + 1][k].setTopCons(cons);
				}


				if(k == 0) worldareas[i][j][k].setUpCons(std::vector<std::vector<int> >(), 0);


				if(k == levelz - 1) worldareas[i][j][k].setDownCons(std::vector<std::vector<int> >(), 0);


			}
		}
	}


//	area::tilestruct gen;

//	levels = new level*[z];
	for(int k = 0; k < levelz; k++){
		levels.push_back(level(baseAreaWidth * areaXFactor, baseAreaHeight * areaYFactor));
//		levels[k] = new level(areaWidth * areaNumX, areaHeight * areaNumY);
		for (int i = 0; i < areaXFactor; ++i) {
			for (int j = 0; j < areaYFactor; ++j) {
				levels[k].setTiles(i, j, worldareas[i][j][k].generate());
			}
		}
	}

	populateItems();
}

world::~world() {
}

std::vector<int> world::generateCons(areatype::a_type a1, areatype::a_type a2, int length){
	//TODO: make depend on the areatypes
//	int* cons = new int[length];
	std::vector<int> cons(length);
	for(int m = 0; m < length; m++){
		cons[m] = 0;
	}

	bool done = false;
	int count = 0;
	while(!done){
		cons[rngke::uniform(1,baseAreaHeight - 2)] = 1;
		rngke::uniform(0,2) == 0 ? done = true : done = false; //random num of connections
		if(count++ == 3) done = true; //at most 3 connections
	}
	return cons;
}

int world::populate(world* w) {

	return 0;
}

void world::populateItems(){
	// find and record roomfloor tiles for the purpose of randomly placing items on them
	// maybe there's a more elegant way... possibly push_back while the tiles are made instead of traversing all tiles again
	for(size_t k = 0; k < levels.size(); ++k){
		for(int j = 0; j < levels[k].getHeight(); ++j){
			for(int i = 0; i < levels[k].getWidth(); ++i){
				if(levels[k].getTile(i, j).isFloor()){
					roomFloorPoints.push_back(Point(i, j, k));
				}
			}
		}
	}

	// place some number of gold into the world
	int numberOfGold = rngke::uniform(50, 100); // from 50 to 100 gold pieces
	numberOfGold = roomFloorPoints.size(); // gold everywhere! muahaha; delete later, haha
	for(int a = 0; a < numberOfGold; ++a){
		int goldAmount = rngke::uniform(5, 50); // random gold value from 5 to 50
		// put gold onto a random floor tile
		size_t roomFloorPointsSize = roomFloorPoints.size();
		if(roomFloorPointsSize == 0)
//			throw std::domain_error("Error: ran out of room floor points to place items");
			break; // no more spots to place items
		size_t randomTileIndex = rngke::uniform(0, roomFloorPointsSize-1);
		Point randomPoint = roomFloorPoints[randomTileIndex];
		try{
			levels[randomPoint.z].getTileRef(randomPoint.x, randomPoint.y).setItem(new Gold(goldAmount)); // the tile owns the gold pointer, must be handled properly
			// remove the point from possible random item insertions after successful item insertion
			roomFloorPoints.erase(roomFloorPoints.begin() + randomTileIndex);
		} catch (const std::invalid_argument& e){
			// ignore item or actor collision
		}
	}
}

//area* world::generateLevel(int type) {
//	return agen->generate(type, 0,0,0,0,0,0);//TODO: remove?
//}

//const char*** world::toString() {
//	return levels[0]->toString();
//}

level& world::getCurrentArea() {
	return levels[0];
}

level& world::getArea(int z) {
	if(z < levelz) return levels[z];
	std::cerr << "in world::getArea(int z): z=" << z << " out of range" << std::endl; // consider throwing exception
	return levels[0];
}

int world::getBaseHeight() {return baseAreaHeight;}
int world::getBaseWidth() {return baseAreaWidth;}
int world::getHeight() const {return areaHeight;}
int world::getWidth() const {return areaWidth;}
int world::getDepth() const {return levelz;}
