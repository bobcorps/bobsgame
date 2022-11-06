//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "oktypes.h"
class Logger;



class Main;

class TitleScreenState : public Engine
{
public:

	static Logger log;


	TitleScreenState();


	int frame = 0;
	int ticks = 0;

	int count = 0;


	virtual void update() override;


	virtual void render() override;
};

