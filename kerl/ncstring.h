/*
 * ncstring.h
 *
 *  Created on: Nov 26, 2012
 *      Author: Bloodnamed
 */

#ifndef NCSTRING_H_
#define NCSTRING_H_

#include <string>

#include "actortype.h"
#include "tiletype.h"

class ncstring {
public:
	enum color {
		RED, BLUE, YELLOW,
		PURPLE, GREEN, ORANGE,
		NONE,
	};
	ncstring();
	ncstring(char, const std::string&, bool h_light, color);
	ncstring(const ncstring& st);
	~ncstring();
	char toChar() const;
	std::string toString() const;
	void setHLight();
	void rmHLight();
	void setColor(color c);
	void update(actortype::a_type t);
	void update(tiletype::t_type t);
	bool isHLight() const;
	ncstring::color getColor() const;
private:
	char ch;
	std::string str;
	bool h_light;
	color col;
};

#endif /* NCSTRING_H_ */
