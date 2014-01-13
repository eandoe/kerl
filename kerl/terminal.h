/*
 * terminal.h
 *
 *  Created on: Jan 15, 2013
 *      Author: peterryszkiewicz
 *
 *      Terminal interface. No implementation details.
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <string>
#include <vector>

/* terminal_interface describes the interface the use has for the terminal.
 * The pure virtual functions must be implemented.
 */
class terminal_interface {
public:
	virtual ~terminal_interface() {}
	virtual bool string_push_back(const std::string&)=0;
	virtual bool print_terminal() const =0;
	virtual bool adjust_terminal_view(const int&)=0;
	virtual void adjust_terminal_position(const int&, const int&, const int&)=0;

};

// terminal_type implements terminal_interface
class terminal_type : public terminal_interface {
public:
	terminal_type(); // default constructor
	terminal_type(const int& size, const int& x, const int& y); // constructor taking terminal position and size
	~terminal_type(); // destructor
	bool string_push_back(const std::string&);
	bool print_terminal() const;
	bool adjust_terminal_view(const int& offset);
	void adjust_terminal_position(const int& size, const int& x, const int& y); // must call when display changes

private:
	typedef std::vector<std::string> content_t;
	typedef std::vector<std::string>::size_type content_index;
	content_t content;
	content_index top; // index of highest element to be shown in terminal
	content_index bottom; // index of lowest element + 1 to be shown in terminal
	int terminal_view_size;
	int terminal_view_X;
	int terminal_view_Y;
};

#endif /* TERMINAL_H_ */
