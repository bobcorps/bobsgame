//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "oktypes.h"
class Logger;




class PingBall : public EnginePart
{
public:

	static Logger log;


	static int h;
	static int w;

	//Texture texture;

	float x = 0;
	float y = 0;

	//float speed;


	PingBall(shared_ptr<Engine> g);


	void update();


	void render();

	float left();

	float right();

	float top();

	float bottom();

	float middleX();

	float middleY();
};

