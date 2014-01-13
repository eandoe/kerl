/*
 * main.cpp
 *
 *  Created on: Nov 4, 2012
 *     Authors: Bloodnamed
 *      		Gorfiend
 *     Version: Pre-Alpha
 *
 * Welcome to KERL.  This is the start of a journey Gorfiend and I started together
 * in search of truth, edification, and all round hobbyism.  As per the GNU free
 * licensing agreement, this code is provided as is, without warranty or guarantee
 * and permission is granted to use or modify however the user desires excluding:
 * 	Plagiarism for any means.
 * 	Reproduction for the direct intention of turning a profit.
 *
 * Just a couple big examples.  If you think what you might be doing is in violation
 * of these rules, then please contact me at bloodnamednf@gmail.com to discuss
 * consent of use.  Note that much of the code herein is built upon public
 * libraries, and as such we are not the sole constituents in this matter.
 *
 */
#define _X_OPEN_SOURCE_EXTENDED
#include "gamemanager.h"

//bool widechars = false; // consider making these a static variable of some class, probably gamemanager
//bool allowresize = true; // consider making these a static variable of some class, probably gamemanager

int main(int argc, char **argv) {
	gamemanager gm;

	gm.run();

	return 0;
}
