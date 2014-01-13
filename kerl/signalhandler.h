/*
 * signalhandler.h
 *
 *  Created on: Dec 5, 2012
 *      Author: Bloodnamed
 */

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <stdexcept>

using std::runtime_error;

class SignalException : public runtime_error
{
public:
	SignalException(const std::string& _message)
: std::runtime_error(_message)
	{}
};

class signalhandler
{
protected:
	static bool mbGotExitSignal;
	static bool mbGotWinchSignal;

public:
	signalhandler();
	~signalhandler();

	static bool gotExitSignal();
	static void setExitSignal(bool _bExitSignal);

	static bool gotWinchSignal();
	static void setWinchSignal(bool _bExitSignal);

	static void setupsignalhandlers();
	static void exitsignalhandler(int _ignored);
	static void winchsignalhandler(int _ignored);

};
#endif /* SIGNALHANDLER_H_ */
