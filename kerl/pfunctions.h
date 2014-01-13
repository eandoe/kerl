/*
 * pfunctions.h
 *
 *  Created on: Dec 8, 2012
 *      Author: Bloodnamed
 */

#ifndef PFUNCTIONS_H_
#define PFUNCTIONS_H_

class pfunctions {
public:
	enum playeraction {
		MOVEUPLEFT=0, MOVEUP=1, MOVEUPRIGHT=2,
		MOVELEFT=3, MOVERIGHT=4,
		MOVEDOWNLEFT=5, MOVEDOWN=6, MOVEDOWNRIGHT=7,
		SKIP=8, WAIT,
		TERMUP, TERMDOWN,
		REFRESH,
		QUIT,
		NONE,
	};
	pfunctions();
	virtual ~pfunctions();
	static bool isMove(playeraction a);
};

#endif /* PFUNCTIONS_H_ */
