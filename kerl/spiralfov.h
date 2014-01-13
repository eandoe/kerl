/*
 * spiralfov.h
 *
 *  Created on: 2012/12/17
 *      Author: Bernhard Strauss
 */

#ifndef SPIRALFOV_H_
#define SPIRALFOV_H_

class level;
class pmap;

#define PI 3.14159265

class spiralfov {
public:
	spiralfov();
	~spiralfov();
	void FOV_Init();
	void FOV_SpiralPath(level& lev, pmap& pm, int mapx, int mapy, int mapz, double radius, double arcstart,
			   double arcend, int action, int cornertouchup);

private:
	int FOV_Transparent(level* lev, int x, int y);
	double FOV_Distance(int x, int y, int x2, int y2);
	double FOV_Anglenorm(double angle);
	void FOV_InitLightGrid();
	void FOV_InitGrid();
	double FOV_Coordangle(int x, int y);
	double FOV_MinAngle(int x, int y);
	double FOV_MaxAngle(int x, int y);
	double FOV_OuterAngle(int x, int y);
	double FOV_OuterAngle2(int x, int y);
	int FOV_In_Arc(int x, int y, double arcstart, double arcend);
	void FOV_FirstChild(int x, int y, int *childx, int *childy);
	void FOV_SecondChild(int x, int y, int *childx, int *childy);
	void FOV_ThirdChild(int x, int y, int *childx, int *childy);
	void FOV_SetLitAngle(int x, int y, double minlit, double maxlit);
	void FOV_GetLitAngle(int x, int y, double *minlit, double *maxlit);
	void FOV_Dequeue(int *QX, int *QY, int *Qhead, int *curx, int *cury, int *child1X,
			 int *child1Y, int *child2X, int *child2Y, int *child3X, int *child3Y,
			 double *leastangle, double *outerangle, double *outerangle2,
			 double *greatestangle, double *leastlitangle, double *greatestlitangle);
	void FOV_enqueue(int *QX, int *QY, int *Qtail, int x, int y);
	void FOV_Mark(int *QX, int *QY, int *Qtail, int x, int y, double min, double max);
	void FOV_TestMark(int *QX, int *QY, int *Qtail, int x, int y, double leastlitangle, double greatestlitangle,
			  double minangle, double maxangle);

	void removeVision(pmap& m, level& l);


	int FOV_SIGHTDIAMETER;
	int FOV_SIGHTRADIUS;



	/* a data type that stores the minimum and maximum lit angles of a
	   given tile. */
	typedef struct{
	  double minlit;
	  double maxlit;
	} FOVsquare;



	double** FOV_grid;
	FOVsquare** FOV_lightgrid;

};


#endif /* SPIRALFOV_H_ */
