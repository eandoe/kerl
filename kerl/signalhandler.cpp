/*
 * signalhandler.cpp
 *
 *  Created on: Dec 5, 2012
 *      Author: Bloodnamed
 */

#include "signalhandler.h"
#include <signal.h>
#include <errno.h>

bool signalhandler::mbGotExitSignal = false;
bool signalhandler::mbGotWinchSignal = false;

/**
* Default Contructor.
*/
signalhandler::signalhandler()
{
}

/**
* Destructor.
*/
signalhandler::~signalhandler()
{
}

/**
* Returns the bool flag indicating whether we received an exit signal
* @return Flag indicating shutdown of program
*/
bool signalhandler::gotExitSignal()
{
    return mbGotExitSignal;
}

/**
* Sets the bool flag indicating whether we received an exit signal
*/
void signalhandler::setExitSignal(bool _bExitSignal)
{
    mbGotExitSignal = _bExitSignal;
}

/**
* Sets exit signal to true.
* @param[in] _ignored Not used but required by function prototype
*                     to match required handler.
*/
void signalhandler::exitsignalhandler(int _ignored)
{
    mbGotExitSignal = true;
}

/**
* Returns the bool flag indicating whether we received an exit signal
* @return Flag indicating shutdown of program
*/
bool signalhandler::gotWinchSignal()
{
    return mbGotWinchSignal;
}

/**
* Sets the bool flag indicating whether we received a winch signal
*/
void signalhandler::setWinchSignal(bool _bWinchSignal)
{
    mbGotWinchSignal = _bWinchSignal;
}

/**
* Sets exit signal to true.
* @param[in] _ignored Not used but required by function prototype
*                     to match required handler.
*/
void signalhandler::winchsignalhandler(int _ignored)
{
    mbGotWinchSignal = true;
}

/**
* Set up the signal handlers for CTRL-C.
*/
void signalhandler::setupsignalhandlers()
{
    if (signal((int) SIGINT, signalhandler::exitsignalhandler) == SIG_ERR)
    {
        throw SignalException("!!!!! Error setting up sigint !!!!!");
    }
}
