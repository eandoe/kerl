/*
 * spiralfov.cpp
 *
 *  Created on: 2012/12/17
 *      Author: Bernhard Strauss
 */




/* The following is a complete implementation of Spiral Path FOV code in C.
     Spiral Path visits each square with light passing through it or into it
     exactly once, and does not visit any other squares -- so it's in the most
     efficient class of FOV algorithms.  I have not measured its performance
     against comparable implementations of other algorithms in the class; it
     may be a constant factor faster or slower.

     It calls out to three functions in a different module; These functions are
     Map_Distance to calculate distances,
     Map_Transparent to tell whether light can pass through a square, and
     Map_FovCallback to actually do the marking of squares in the map.

     It assumes that two of its own functions (and only two) may be called from
     outside the module; these functions are
     FOV_Init, which is called exactly once to initialize the data structures, and
     FOV_SpiralPath, which is called whenever we want to do radius or arc lighting,
     whenever we want to calculate a field of view, whenever we want to mark
     squares to be subject to a radius or cone effect that can be blocked, etc.

     Module written by Ray Dillinger in 2011.  This is an adaptation/rewrite of an
     original implementation, also by Ray Dillinger, from 2004 which was harder
     to understand.   Use and enjoy, subject to any Debian-Approved
     free-software license of your choice.
*/

#include "spiralfov.h"

#include <math.h>
#include <assert.h>

#include "level.h"
#include "pmap.h"

spiralfov::spiralfov(){
	//TODO: make dynamic/changeable
	FOV_SIGHTDIAMETER = 11;
	FOV_SIGHTRADIUS = 5;

	/* FOV_grid stores the angle from the center of the origin square or
	   (0.5,0.5) to the corner of the tile at the given coordinates (to
	   the true (0,0) corner in the case of the origin square. From these
	   we derive all corner coordinates we need for all tiles.  This has
	   the benefit of avoiding repeating potentially-slow trig operations,
	   but it's mainly for consistency.

	   Since we're using a lot of doubles, and we need particular angles
	   that we get to in several different ways, deriving all angles in an
	   absolutely consistent way from this grid ensures that equality
	   tests are meaningful and do what we want, despite possible roundoff
	   errors and peculiarities of implementation.

	   In principle this could use Integers instead of doubles, given a
	   monotonic mapping of integers to angles occurring in the grid.  */

	FOV_grid = new double*[FOV_SIGHTDIAMETER];
	for (int i = 0; i < FOV_SIGHTDIAMETER; i++){
		FOV_grid[i] = new double[FOV_SIGHTDIAMETER];
	}
//	FOV_grid = new double[FOV_SIGHTDIAMETER+1 ][FOV_SIGHTDIAMETER+1 ];

	/* This array stores the minimum and maximum lit angles of each square
	   within the sight radius.  Unlit squares are represented as 0,0.
	   each FOV operation must begin and end with minlit and maxlit angles
	   in FOV_lightgrid set equal to zero. These angles change during the
	   operation; if either (or both) are nonzero, then the corresponding
	   element is in the queue. Angles related to each (xy) square are
	   stored in FOV_grid(x-sightradius,y-sightradius) because C doesn't
	   allow non-zerobased arrays. */

	FOV_lightgrid = new FOVsquare*[FOV_SIGHTDIAMETER];
	for (int i = 0; i < FOV_SIGHTDIAMETER; i++){
		FOV_lightgrid[i] = new FOVsquare[FOV_SIGHTDIAMETER];
	}
//	FOV_lightgrid = new FOVsquare[FOV_SIGHTDIAMETER][FOV_SIGHTDIAMETER];
}

spiralfov::~spiralfov(){
	for(int i = 0; i < FOV_SIGHTDIAMETER; ++i){
		delete[] FOV_grid[i];
		delete[] FOV_lightgrid[i];
	}
	delete[] FOV_grid;
	delete[] FOV_lightgrid;
}


/* this function is a 'shim' that handles the map code referring to
   loctypes and the FOV code referring to integer coordinates.  I'm
   referring back to the map distance function in order to assure that
   the "distance" expressed by radius, etc, is the same distance as
   measured by the Map module's distance function. */
//double spiralfov::FOV_Distance(int x, int y, int x2, int y2){
//  loctype loc1;
//  loctype loc2;
//
//  loc1.x = x;
//  loc2.x = x2;
//  loc1.y = y;
//  loc2.y = y2;
//  /* For FOV purposes we're finding distances on the same z plane -
//     hence the z coordinate (as long as it's > 0) does not actually
//     matter. */
//  loc1.z = loc2.z = 1;
//  return(Map_Distance(loc1, loc2));
//}


/* normalizes angles to between zero and 2pi. */
double spiralfov::FOV_Anglenorm(double angle){
  while (angle < 0.0) angle += 2*PI;
  while (angle > 2*PI) angle -= 2*PI;
  return(angle);
}

/* Initializes the light grid.  Called from FOV_Init.  After that we
   are meticulous in re-zeroing these whenever we dequeue them so the
   grid will always be ready when one Spiralpath traversal is done to
   begin another. */
void spiralfov::FOV_InitLightGrid() {
  int x; int y;
  for (x = 0; x < FOV_SIGHTDIAMETER; x++)
    for (y = 0; y < FOV_SIGHTDIAMETER; y++){
      FOV_lightgrid[x][y].minlit = 0.0;
      FOV_lightgrid[x][y].maxlit = 0.0;
    }
}

/* Initializes the geometry grid.  Called from FOV_Init.
   The geometry grid is never altered after initialization. */
void spiralfov::FOV_InitGrid(){
  int x;
  int y;
  for (x = 0; x < FOV_SIGHTDIAMETER; x++)
    for (y = 0; y < FOV_SIGHTDIAMETER; y++){
      /* this is the angle from the center of the 0,0 square to the
	 closest-to-origin corner of each square.  All angles except
	 the angles to the squares in the zero row/column will be in
	 the range 0 to pi/2. */
      FOV_grid[x][y] = FOV_Anglenorm(atan2((y-0.5),(x-0.5)));
    }
}

/* Needs called once during game setup.  Handling it by calling
   it from Map_Init. */
void spiralfov::FOV_Init(){
  FOV_InitGrid();
  FOV_InitLightGrid();
}


/*returns the angle that "oughta" be in the geometry grid for given
  coordinates, if the grid went to those coordinates.*/
double spiralfov::FOV_Coordangle(int x, int y){

  if (x > FOV_SIGHTRADIUS || y > FOV_SIGHTRADIUS ||
      -x > FOV_SIGHTRADIUS || -y > FOV_SIGHTRADIUS )
    /* if it's outside of the precomputed zone and its rotations about
       the origin, then calculate it directly. */
    return(FOV_Anglenorm(atan2(y-0.5, x-0.5)));
  /* otherwise get it from the correct zone rotation & offset. */
  if (x >= 0 && y >= 0) return (FOV_grid[x][y]);
  if (x < 0 && y > 0) return (PI - FOV_grid[1-x][y]);
  if (x >= 0 && y < 0) return (2*PI - FOV_grid[x][1-y]);
  return(PI + FOV_grid[1-x][1-y]);
}


/* The minimum angle of the tile; that is, the angle of the
   smallest - angled corner.*/
double spiralfov::FOV_MinAngle(int x, int y){
  if (x == 0 && y == 0) return (0.0); /* origin special case */
  if (x >= 0 && y > 0) return (FOV_Coordangle(x+1,y)); /* first quadrant */
  if (x < 0 && y >= 0) return (FOV_Coordangle(x+1,y+1)); /* second quadrant*/
  if (x <= 0 && y < 0) return (FOV_Coordangle(x,y+1)); /* third quadrant */
  return(FOV_Coordangle(x,y)); /* fourth quadrant */
}


/* The maximum angle of the tile; that is, the angle of the
   largest-angled corner.*/
double spiralfov::FOV_MaxAngle(int x, int y){
  if (x == 0 && y == 0) return (2*PI);  /* origin special case */
  if (x > 0 && y >= 0) return (FOV_Coordangle(x,y+1)); /* first quadrant */
  if (x <= 0 && y > 0) return (FOV_Coordangle(x,y)); /* second quadrant */
  if (x < 0 && y <= 0) return (FOV_Coordangle(x+1,y));/* third quadrant */
  return(FOV_Coordangle(x+1,y+1)); /* fourth quadrant */
}

/* The angle of the outer corner of each tile: On the origin lines,
   the angle of the FIRST outer corner. */
double spiralfov::FOV_OuterAngle(int x, int y){
  if (x == 0 && y == 0) return (0.0); /* origin special case */
  if (x >= 0 && y > 0) return (FOV_Coordangle(x+1,y+1)); /* first quadrant with positive y axis*/
  if (x < 0 && y >= 0) return (FOV_Coordangle(x,y+1)); /* second quadrant with negative x axis*/
  if (x <= 0 && y < 0) return (FOV_Coordangle(x,y)); /* third quadrant with negative y axis */
  return(FOV_Coordangle(x+1,y)); /* fourth quadrant with positive x axis */
}

/* The squares on the axes (x or y == 0) have a second outer corner.
   This function identifies the angle from the center of the origin
   square to that corner.  */
double spiralfov::FOV_OuterAngle2(int x, int y){
  if (x != 0 && y != 0) return (0.0); /* meaningless on non-axis squares */
  if (x > 0) return(FOV_Coordangle(x+1,y+1));
  if (x < 0) return(FOV_Coordangle(x,y));
  if (y > 0) return(FOV_Coordangle(x, y+1));
  if (y < 0) return(FOV_Coordangle(x+1,y));
  return(0.0); /* meaningless on origin */
}

/* true iff a given square could pass any light inside a desired arc. */
int spiralfov::FOV_In_Arc(int x, int y, double arcstart, double arcend){
  if (arcstart > arcend)    /* arc includes anomaly line */
    return (FOV_MinAngle(x,y) < arcstart ||
	    FOV_MaxAngle(x,y) < arcstart ||
	    FOV_MinAngle(x,y) > arcend ||
	    FOV_MaxAngle(x,y) > arcend);
  else     /* arc does not include anomaly line */
    return (FOV_MaxAngle(x,y) > arcstart ||
	    FOV_MinAngle(x,y) < arcend);
}


/* find the first "child" tile to which light may be passed from x y.
   This is also the tile to be revealed as a corner touchup if x y is
   illuminated from its minimum-angle corner and is opaque. */
void spiralfov::FOV_FirstChild(int x, int y, int *childx, int *childy){
  if (x == 0 && y == 0) {*childx = x; *childy = y; return; } /* origin */
  if (x >= 0 && y > 0) {*childx = x+1; *childy = y; return; } /* quadrant 1 */
  if (x < 0 && y >= 0) {*childx = x; *childy = y+1; return; } /* quadrant 2 */
  if (x <= 0 && y < 0) {*childx = x-1; *childy = y; return; } /* quadrant 3 */
  *childx = x; *childy = y-1; return; /* quadrant 4 */
}

/* find the second "child" tile to which light may be passed from x y. */
void spiralfov::FOV_SecondChild(int x, int y, int *childx, int *childy){
  if (x == 0 && y == 0) {*childx = x; *childy = y; return; } /* origin */
  if (x >= 0 && y > 0) {*childx = x; *childy = y+1; return; } /* quadrant 1 */
  if (x < 0 && y >= 0) {*childx = x-1; *childy = y; return; } /* quadrant 2 */
  if (x <= 0 && y < 0) {*childx = x; *childy = y-1; return; } /* quadrant 3 */
  *childx = x+1; *childy = y; return; /* quadrant 4 */
}

/* find the third "child" tile to which light may be passed from x y.
   Meaningful only for axis tiles (x == 0 or y == 0) because they
   have a third child; non-axis tiles don't. */
void spiralfov::FOV_ThirdChild(int x, int y, int *childx, int *childy){
  if (x != 0 && y != 0) {*childx = *childy = 0; return;} /* non-axis*/
  if (x > 0) {*childx = x; *childy = y+1; return;}
  if (x < 0) {*childx = x; *childy = y-1; return;}
  if (y > 0) {*childx = x-1; *childy = y; return;}
  if (y < 0) {*childx = x+1; *childy = y; return;}
  *childx = 0;   *childy = 0;   return; /* origin */
}

void spiralfov::FOV_SetLitAngle(int x, int y, double minlit, double maxlit){
  /* boundary checking just to make sure.... I know of no way this
     code can ever violate these boundary checks, but the code that
     implements relevant preconditions is non-centralized and if
     they're violated we segfault, so leaving the checks in to catch
     any possible mistakes present or future. */

  assert(x+FOV_SIGHTRADIUS >= 0);
  assert(y+FOV_SIGHTRADIUS >= 0);
  assert(x+FOV_SIGHTRADIUS < FOV_SIGHTDIAMETER);
  assert(y+FOV_SIGHTRADIUS < FOV_SIGHTDIAMETER);

  FOV_lightgrid[x+FOV_SIGHTRADIUS][y+FOV_SIGHTRADIUS].minlit = minlit;
  FOV_lightgrid[x+FOV_SIGHTRADIUS][y+FOV_SIGHTRADIUS].maxlit = maxlit;
}

void spiralfov::FOV_GetLitAngle(int x, int y, double *minlit, double *maxlit){
  /* Again, boundary checking just to make sure.... */
  assert(x+FOV_SIGHTRADIUS >= 0);
  assert(y+FOV_SIGHTRADIUS >= 0);
  assert(x+FOV_SIGHTRADIUS < FOV_SIGHTDIAMETER);
  assert(y+FOV_SIGHTRADIUS < FOV_SIGHTDIAMETER);

  *minlit = FOV_lightgrid[x+FOV_SIGHTRADIUS][y+FOV_SIGHTRADIUS].minlit;
  *maxlit = FOV_lightgrid[x+FOV_SIGHTRADIUS][y+FOV_SIGHTRADIUS].maxlit;
}


/* we dequeue one item and set all the particulars.  Also, we set the
   squarelighting to zero for that tile so we know it's off the queue
   next time we come across it. */

void spiralfov::FOV_Dequeue(int *QX, int *QY, int *Qhead, int *curx, int *cury, int *child1X,
		 int *child1Y, int *child2X, int *child2Y, int *child3X, int *child3Y,
		 double *leastangle, double *outerangle, double *outerangle2,
		 double *greatestangle, double *leastlitangle, double *greatestlitangle){
  *curx = QX[*Qhead];
  *cury = QY[*Qhead];
  *Qhead = (*Qhead + 1) % (2 * FOV_SIGHTDIAMETER);

  FOV_FirstChild(*curx, *cury, child1X, child1Y);
  FOV_SecondChild(*curx, *cury, child2X, child2Y);
  FOV_ThirdChild(*curx, *cury, child3X, child3Y);
  *leastangle = FOV_MinAngle(*curx, *cury);
  *outerangle = FOV_OuterAngle(*curx, *cury);
  *outerangle2 = FOV_OuterAngle2(*curx, *cury);
  *greatestangle = FOV_MaxAngle(*curx, *cury);
  FOV_GetLitAngle(*curx, *cury, leastlitangle, greatestlitangle);

  /* and this is off the queue now so we zero it for next time. */
  FOV_SetLitAngle(*curx, *cury, 0.0, 0.0);
}

void spiralfov::FOV_enqueue(int *QX, int *QY, int *Qtail, int x, int y){
    QX[*Qtail] = x;
    QY[*Qtail] = y;
    *Qtail = (*Qtail + 1) % (2 * FOV_SIGHTDIAMETER);
}

/* This adds light to a tile. Also, if a tile is not in the queue,
   it enqueues it.*/
void spiralfov::FOV_Mark(int *QX, int *QY, int *Qtail, int x, int y, double min, double max){
  double minlit, maxlit;
  FOV_GetLitAngle(x,y, &minlit, &maxlit);
  if (minlit == maxlit && maxlit == 0.0){
    /* no light -- implies not in queue, so we add it to the queue.  */
    FOV_SetLitAngle(x,y, min, max);
    FOV_enqueue(QX, QY, Qtail, x,y);
  }
  else {
    if (min < minlit) minlit = min;
    if (max > maxlit) maxlit = max;
    FOV_SetLitAngle(x,y,minlit,maxlit);
  }
}




/* The total lit angle is represented by leastlitangle,
   greatestlitangle.  minangle and maxangle are the minimum and
   maximum that can be illuminated in this operation.  Our task is to
   test and see if we can add light to the tile.  If any light is
   added, we call FOV_Mark. */
void spiralfov::FOV_TestMark(int *QX, int *QY, int *Qtail, int x, int y, double leastlitangle, double greatestlitangle,
		  double minangle, double maxangle){
  if (leastlitangle > greatestlitangle)
    /* we're passing light along the anomaly axis. This takes
       advantage of the grid-geometric property that no
       less-than-total obstructions are possible. */
    FOV_Mark(QX, QY, Qtail, x, y, minangle, maxangle);
  else if (maxangle < leastlitangle || minangle > greatestlitangle)
    /* lightable area is outside the lighting. */
    return;
  else if (minangle <= leastlitangle && greatestlitangle <= maxangle)
    /* lightable area contains the lighting. */
    FOV_Mark(QX, QY, Qtail, x,y, leastlitangle, greatestlitangle);
  else if (minangle >= leastlitangle && greatestlitangle >= maxangle)
    /* lightable area contained by the lighting.  */
    FOV_Mark(QX, QY, Qtail, x,y, minangle, maxangle);
  else if (minangle >= leastlitangle && greatestlitangle <= maxangle)
    /* least of lightable area overlaps greatest of lighting. */
    FOV_Mark(QX, QY, Qtail, x,y, minangle, greatestlitangle);
  else if (minangle <= leastlitangle && greatestlitangle >= maxangle)
    /* greatest of lightable area overlaps least of lighting.  */
    FOV_Mark(QX, QY, Qtail, x,y, leastlitangle, maxangle);
  else /* This never happens. */
    assert(1 == 0); /* unhandled case, not on the anomaly line. */
}


/* Mapx, mapy, mapz, are the coordinates of the square where
   spiralpath starts. Radius is the range of the effect.  Radius must be less
   than FOV_SIGHTRADIUS.  Arcstart and arcend denote the minimum and
   maximum angles to mark.  They must be between 0.0 and 2*PI.  Action
   is a signal to Map_FovCallback what action to do to squares in the area.
   Its value doesn't matter to us, we just pass it back to the map
   module. */

void spiralfov::FOV_SpiralPath(level& lev, pmap& pm, int mapx, int mapy, int mapz, double radius, double arcstart,
		   double arcend, int action, int cornertouchup){


	removeVision(pm, lev);

  /* The next 4 variables are a Queue of X Y coordinates. */
  int QX[2 * FOV_SIGHTDIAMETER];
  int QY[2 * FOV_SIGHTDIAMETER];
  int Qhead = 0;
  int Qtail = 0;

  int curx, cury,  child1X, child1Y,  child2X, child2Y, child3X, child3Y;

  double leastangle, greatestangle, outerangle, outerangle2, leastlitangle, greatestlitangle;

  /* the point of origin is always marked by the traverse. */
//  Map_FovCallback(map, mapx, mapy, mapz, mapx, mapy, 0, 2*PI, 0, 2*PI, action);
	pm.setVisible(mapx, mapy, true);//add vision
	ncstring temp = lev.getTile(mapx, mapy).toString(true);
//	if(temp != NULL) {
		temp.setHLight();//set highlight for output
//	}
	pm.insert(mapx, mapy, temp);//add to the given map
  /* these 4 squares (in this order) are a valid "starting set" for Spiralpath traversal.
     A valid starting set is either a clockwise or counterclockwise traversal of all
     the points with manhattan distance 1 from the origin.   */
  FOV_TestMark(QX, QY, &Qtail, 1, 0, arcstart, arcend, FOV_MinAngle(1,0), FOV_MaxAngle(1,0));
  FOV_TestMark(QX, QY, &Qtail, 0, 1, arcstart, arcend, FOV_MinAngle(0,1), FOV_MaxAngle(0,1));
  FOV_TestMark(QX, QY, &Qtail, -1,0, arcstart, arcend, FOV_MinAngle(-1,0), FOV_MaxAngle(-1,0));
  FOV_TestMark(QX, QY, &Qtail, 0,-1, arcstart, arcend, FOV_MinAngle(0,-1), FOV_MaxAngle(0,-1));

  while(Qhead != Qtail){
    FOV_Dequeue(QX, QY, &Qhead, &curx, &cury, &child1X, &child1Y, &child2X, &child2Y, &child3X,
		&child3Y, &leastangle, &outerangle, &outerangle2, &greatestangle,
		&leastlitangle, &greatestlitangle);

    if (lev.distance(0,0,curx,cury) <= radius && FOV_In_Arc(curx, cury, arcstart, arcend)){
//      Map_FovCallback(map, mapx, mapy, mapz, mapx+curx, mapy+cury,
//		      leastangle, greatestangle, leastlitangle, greatestlitangle, action);
		pm.setVisible(mapx+curx, mapy+cury, true);//add vision
		ncstring temp = lev.getTile(mapx+curx, mapy+cury).toString(true);
//		if(temp != NULL) {
			temp.setHLight();//set highlight for output
//		}
		pm.insert(mapx+curx, mapy+cury, temp);//add to the given map
      if (lev.getOcclusion(mapx+curx,mapy+cury)){
	FOV_TestMark(QX, QY, &Qtail, child1X, child1Y, leastlitangle, greatestlitangle, leastangle, outerangle);
	if (outerangle2 != 0.0){
	  FOV_TestMark(QX, QY, &Qtail, child2X, child2Y, leastlitangle, greatestlitangle, outerangle, outerangle2);
	  FOV_TestMark(QX, QY, &Qtail, child3X, child3Y, leastlitangle, greatestlitangle, outerangle2, greatestangle);
	}
	else
	  FOV_TestMark(QX, QY, &Qtail, child2X, child2Y, leastlitangle, greatestlitangle, outerangle, greatestangle);
      }
      else /* cell is opaque.  We pass an infinitely-narrow ray of
	      light from its first corner to its first child if we
	      are doing corner touchup. */
	if (cornertouchup && (leastlitangle == leastangle))
	  FOV_Mark(QX, QY, &Qtail, child1X, child1Y, leastangle, leastangle);
    }
  }
}



void spiralfov::removeVision(pmap& m, level& l) {
	//Reset your vision
	for (int j = 0; j < m.getY(); ++j) {
		for (int i = 0; i < m.getX(); ++i) {
			if(m.isVisible(i,j)) {
				//get the tile on the level
				ncstring temp = l.getTile(i,j).toString(false);
//				if(temp != NULL) {
					//remove output highlighting
					temp.rmHLight();
					//add it to the map
					m.insert(i,j, temp);
//				}
			}
			m.setVisible(i,j, false);
		}
	}
}
