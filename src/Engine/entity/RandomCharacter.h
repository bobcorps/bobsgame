//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "oktypes.h"
class Logger;




class RandomCharacter : public Character
{
public:
	static Logger log;


	int targetX = -1;
	int targetY = -1;

	string cameFrom = "";
	bool gotThere = false;


	//public Texture shadowTexture = null; // just use the normal hq2x shadow texture in the spriteasset since it doesn't matter.


	bool kid = false;
	bool adult = false;
	bool male = false;
	bool female = false;
	bool car = false;


	RandomCharacter(shared_ptr<Engine> g, shared_ptr<Map> map, int spawnXPixels1X, int spawnYPixels1X, bool kid, bool adult, bool male, bool female, bool car);


	shared_ptr<Map> getCurrentMap() override;


	//The following method was originally marked 'synchronized':
	shared_ptr<ByteArray> getByteBuffer_S();


	//The following method was originally marked 'synchronized':
	void setByteBuffer_S(shared_ptr<ByteArray> bb);


	shared_ptr<ByteArray> textureByteBuffer_S = nullptr;

	int eyeSet = -1;
	int skinSet = -1;
	int hairSet = -1;
	int shirtSet = -1;
	int pantsSet = -1;
	int shoeSet = -1;
	int carSet = -1;


	int selectRandomSet(shared_ptr<Sprite> s);


	bool createdThread = false;


	void createRandomTexture();


	virtual void update() override;


	string getRandomAreaWarpOrDoorTYPEID();


	string getRandomConnectionTYPEIDFromCurrentPoint();
};

