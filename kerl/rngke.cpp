/*
 * rngke.cpp
 *
 *  Created on: 2012/11/19
 *      Author: Bernhard Strauss
 */
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <stdexcept>

#include "rngke.h"

rngke::rngke()  {
//	srand ( seed );
}

rngke::~rngke() {
}

void rngke::setSeed(int _seed=time(NULL)){ // seed parameter optional: seed is time(NULL) by default
	seed = _seed;
	srand(seed);
}

/*
 * Generate a random int, uniformly distributed from [min, max].
 * min should be [INT_MIN, INT_MAX]
 * max should be [min, INT_MAX]
 * Handles negative distributions.
 * Warning: The modulus operation for obtaining a random number may produce a non-uniform distribution
 * for ranges close to RAND_MAX.
 */
int rngke::uniform(int min, int max) {
	if(min > max){ // check so no division by zero
		std::stringstream error_msg("In rngke::uniform: min must be <= max: found min=");
		error_msg << min << ", max=" << max;
		throw std::invalid_argument(error_msg.str());
	}
	return rand()%(max - min + 1) + min;
}

/*
 * Generate a random double, uniformly distributed from [min, max].
 * min should be [INT_MIN, INT_MAX]
 * max should be [min, INT_MAX]
 * Handles negative distributions.
 */
double rngke::uniformFloat(double min, double max) {
	if(min > max){ // check for valid input
		std::stringstream error_msg("In rngke::uniformFloat: min must be <= max: found min=");
		error_msg << error_msg << min << ", max=" << max;
		throw std::invalid_argument(error_msg.str());
	}
	return ((double) rand() / (RAND_MAX)) * (max - min) + min;
}

/*
 * Generate a random int, normally distributed from [avg-maxdev, avg+maxdev].
 */
int rngke::normal(int avg, int stddev, int maxdev) {
	if(stddev < 0){ // check for valid input
		std::stringstream error_msg("In rngke::normal: stddev must be >= 0: found stddev=");
		error_msg << stddev;
		throw std::invalid_argument(error_msg.str());
	}
	double n;
	double sum = 0;
	// sum 16 uniforms
	for(int i = 0; i < 16; i++){
		n = uniformFloat(-stddev, stddev) + avg;
		sum += n;
	}
	sum /= 16;
	// sum is in [avg-stddev, avg+stddev]
	if(sum > avg + maxdev) sum = avg + maxdev;
	if(sum < avg - maxdev) sum = avg - maxdev;
	return sum;
}

int rngke::xdn(int x, int n) {
	if(n == 0){ // check for valid input
		std::stringstream error_msg("In rngke::xdn: invalid argument: n = 0");
		throw std::invalid_argument(error_msg.str());
	}
	int sum = 0;
	while(x > 0){
		sum += rand()%n + 1;
		x--;
	}
	return sum;
}

int rngke::getSeed() {return seed;}

int rngke::initSeed(){
	int seed = time(NULL);
	srand(seed);
	return seed;
}
int rngke::seed = initSeed(); // default seed

