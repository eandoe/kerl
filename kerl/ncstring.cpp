/*
 * ncstring.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: Bloodnamed
 */

#include "ncstring.h"

#include <stdlib.h>

/* outString definitions */
ncstring::ncstring() {
	ch = ' ';
	col = NONE;
	h_light = false;
}

ncstring::ncstring(char _ch, const std::string& _str, bool _h_light, color _color)
		: ch(_ch), str(_str), h_light(_h_light), col(_color) { // FIXME Color does nothing
}

ncstring::ncstring(const ncstring& st) {
	ch = st.toChar();
	str = st.toString(); // copy constructs string
	col = st.getColor();
	h_light = st.isHLight();
}

ncstring::~ncstring() {
}

char ncstring::toChar() const {
	return ch;
}

std::string ncstring::toString() const {
	return str;
}

void ncstring::update(actortype::a_type a) {
	ch = actortype::toChar(a)[0];
	str = actortype::toString(a); // copy constructs string
}

void ncstring::update(tiletype::t_type t) {
	ch = tiletype::toChar(t)[0];
	str = tiletype::toString(t); // copy constructs string
}

void ncstring::setHLight() {
	h_light = true;
}

void ncstring::rmHLight() {
	h_light = false;
}

void ncstring::setColor(color c) {
	col = c;
}

bool ncstring::isHLight() const {return h_light;}
ncstring::color ncstring::getColor() const {return col;}
