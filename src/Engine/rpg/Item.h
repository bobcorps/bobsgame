//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "oktypes.h"
class Logger;




class Item : public ServerObject
{
public:

	static Logger log;


	shared_ptr<Sprite> sprite = nullptr;

	string spriteAssetName = "";
	int spriteAssetID = -1;

private:
	bool haveItemValue_S = false;
	long long timeSet = -1;


public:
	Item(shared_ptr<Engine> g, const string& spriteAssetName);


	Item(shared_ptr<Engine> g, int spriteAssetID);


	Item(shared_ptr<Engine> g, shared_ptr<Sprite> sprite);


	void sendServerRequest();


	string& getName();
	string description();
	int getID();


	//The following method was originally marked 'synchronized':
	void getWithCaption_S();


	//The following method was originally marked 'synchronized':
	void setHaveItemValue_S(bool b);


	//The following method was originally marked 'synchronized':
	void initHaveItemValue_S(bool b, long long timeSet);


	//The following method was originally marked 'synchronized':
	bool getHaveItemValue_S();


	long long getTimeSet();
};

