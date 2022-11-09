//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "oktypes.h"
class Logger;




class RamioBlock : public Entity
{
public:

	static Logger log;


	RamioBlock();
	RamioBlock(sp<Engine> g);


	
	bool hit = false;
	int hitOffsetY = 0;
	bool hitUpDownToggle = false;


	virtual void update() override;


	virtual void getHit();


	void checkHit(float x, float y);
};

