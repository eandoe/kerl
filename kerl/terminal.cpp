/*
 * terminal.cpp
 *
 *  Created on: Jan 15, 2013
 *      Author: peterryszkiewicz
 *
 *      The Terminal is responsible for holding onto the terminal textual content
 *      and making sure it prints properly. The display holds an object of Terminal.
 */

#include "terminal.h"
#include "display.h"
#include "ncursesw/curses.h"


// terminal type constructor
terminal_type::terminal_type() : top(0), bottom(0),
							terminal_view_size(0),
							terminal_view_X(0),
							terminal_view_Y(0)
{
	content.reserve(1000); // preallocate memory for content
}

// terminal type constructor
terminal_type::terminal_type(const int& terminal_view_size_,
							 const int& terminal_view_X_,
							 const int& terminal_view_Y_) :
							top(0), bottom(0),
							terminal_view_size(terminal_view_size_),
							terminal_view_X(terminal_view_X_),
							terminal_view_Y(terminal_view_Y_)
{
	content.reserve(1000); // preallocate memory for content
}

// terminal_type destructor
terminal_type::~terminal_type(){
	// nothing to do
}

// add string to the terminal
bool terminal_type::string_push_back(const std::string& input) {
	if(input.length() != 0) {
		content.push_back(input); // copies the input into the content
		++bottom;
		// if there is more content than the text box can display,
		// top is bottom - terminal length, else content (first element)
		content_index terminal_size = terminal_view_size-1;
		top = content.size() > terminal_size ? bottom - terminal_size: 0;
		//add the string to the terminal
//		return print_terminal(); // shouldn't be here
	}
	return false;

}

bool terminal_type::print_terminal() const{
	content_index lines = bottom-top;
	for(content_index i = 0; i < lines; ++i) {
		mvaddstr(i+terminal_view_Y, 1+terminal_view_X, content.at(i+top).c_str());
	}
	display::refreshWin();
	return true;
}

// must call when display changes
void terminal_type::adjust_terminal_position(const int& terminal_view_size_,
											const int& terminal_view_X_,
											const int& terminal_view_Y_){
	if(terminal_view_size_ > 0 && terminal_view_X_ >= 0 && terminal_view_Y_ >= 0){
		terminal_view_size = terminal_view_size_;
		terminal_view_X = terminal_view_X_;
		terminal_view_Y = terminal_view_Y_;
	}

}

bool terminal_type::adjust_terminal_view(const int& offset){
	if(offset != 0) {
		if(offset < 0 && top != 0) {
			//Move term back
			for(int i = 0; top != 0 && i < (-offset); ++i) {
				--top;
				--bottom;
			}
		} else if(offset > 0 && bottom != content.size()){
			//Move term forward
			for(int i = 0; bottom != content.size() && i < offset; ++i) {
				++top;
				++bottom;
			}
		}
	}
	return print_terminal();
}
