/*
 * rngke.h
 *
 *  Created on: 2012/11/19
 *      Author: Bernhard Strauss
 *
 * Static random number generator class.
 * Features uniform distributed int, uniform distributed float, normal distributed int...
 */

#ifndef RNGKE_H_
#define RNGKE_H_

class rngke {
	rngke(); // rngke object unneeded
public:
	~rngke();
	static void setSeed(int seed);
	static int uniform(int min, int max);
	static double uniformFloat(double min, double max);
	static int normal(int avg, int stddev, int maxdev);
	static int xdn(int x, int n);
	static int getSeed();
private:
	static int initSeed();
	static int seed;
};


#endif /* RNGKE_H_ */
