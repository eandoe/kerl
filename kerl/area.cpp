/*
 * area.cpp
 *
 *  Created on: Nov 7, 2012
 *      Author: Bernhard Strauss
 *      		Bloodnamed
 */

#include "area.h"

#include <cmath>
#include <cstdlib>


/**
 * Default constructor.  Should really never be called. So it shouldn't be callable to protect class invariant.
 */

/**
 * Main area constructor.  Sets the type.
 */
area::area(int x, int y, areatype::a_type ty, areagen& ag) :
	height(y), width(x), type(ty), agen(ag), topCons(width), botCons(width), leftCons(height),
	rightCons(height), numUDCons(2)
{


	for (int i = 0; i < 2; ++i) {
		upCons.push_back(std::vector<int>(1)); // push back vector of int of size 1
		downCons.push_back(std::vector<int>(1)); // push back vector of int of size 1
	}
	//stairLocs[x][n], stairLocs[y][n]


	//test default connections init
	topCons[width/2] = 1;
	botCons[width/2] = 1;
	leftCons[height/2] = 1;
	rightCons[height/2] = 1;
	numUDCons[0] = 1;
	numUDCons[1] = 1;
	upCons[0][0] = 5;
	upCons[1][0] = 10;
	downCons[0][0] = 15;
	downCons[1][0] = 20;
}

/**
 * Default destructor for area.
 */
area::~area() {
}

void area::setTopCons(const std::vector<int>& con){
	topCons = con;
}

void area::setBotCons(const std::vector<int>& con){
	botCons = con;
}

void area::setLeftCons(const std::vector<int>& con){
	leftCons = con;
}

void area::setRightCons(const std::vector<int>& con){
	rightCons = con;
}

void area::setUpCons(const std::vector<std::vector<int> >& con, int num){
	numUDCons[0] = num;
	for (int i = 0; i < 2; ++i) {
//		upCons[i]= new int[num];
		upCons[i]= std::vector<int>(num); // destructs old vector
	}
	upCons = con;
}

void area::setDownCons(const std::vector<std::vector<int> >& con, int num){
	numUDCons[1] = num;
	for (int i = 0; i < 2; ++i) {
		downCons[i]= std::vector<int>(num); // destructs old vector
	}
	downCons = con;
}

/**
 * Generates the area based on its properties
 */
area::tilestruct area::generate(){
	tiles = agen.generate(type, topCons, botCons, leftCons, rightCons, numUDCons, upCons, downCons);
	return tiles;
}

/**
 * Returns the tile at the given x,y coordinate
 *
 * @return - The tile located at line y, column x
 */
tile& area::getTile(int x, int y){
	return tiles[y][x];
}

int area::getHeight() const {return height;}
int area::getWidth() const {return width;}
areatype::a_type area::getType() {return type;}


