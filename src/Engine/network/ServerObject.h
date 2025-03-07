//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

#pragma once
#include "oktypes.h"
class Logger;

#include "../EnginePart.h"

class Logger;

class ServerObject : public EnginePart//, public std::enable_shared_from_this<ServerObject>
{
public:

	static Logger log;
	ServerObject();
	ServerObject(sp<Engine> g);

protected:
	bool _initialized = false;
	long long lastTimeRequestedFromServer = 0;

public:
	//The following method was originally marked 'synchronized':
	bool getInitialized_S();

public:
	//The following method was originally marked 'synchronized':
	void setInitialized_S(bool i);

private:
	virtual void sendServerRequest();

protected:
	bool loadedInfoDataFromServer = false; //non-threaded check
	long long lastCheckedInitializedTime = 0;

	bool getLoadedFromServerSendRequestIfFalse();

public:
	virtual void update();
};

