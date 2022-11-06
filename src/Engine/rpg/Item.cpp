#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




Logger Item::log = Logger("Item");


Item::Item(shared_ptr<Engine> g, const string& spriteAssetName)
{ //=========================================================================================================================
	this->e = g;

	this->spriteAssetName = spriteAssetName;

	getEventManager()->itemList.add(this);
}

Item::Item(shared_ptr<Engine> g, int spriteAssetID)
{ //=========================================================================================================================
	this->e = g;

	this->spriteAssetID = spriteAssetID;

	getEventManager()->itemList.add(this);
}

Item::Item(shared_ptr<Engine> g, shared_ptr<Sprite> sprite)
{ //=========================================================================================================================
	this->e = g;

	this->sprite = sprite;
	this->spriteAssetName = sprite->getName();
	this->spriteAssetID = sprite->getID();

	setInitialized_S(true);

	getEventManager()->itemList.add(this);
}

void Item::sendServerRequest()
{ //=========================================================================================================================
	if (getInitialized_S() == false)
	{
		shared_ptr<Sprite> sprite = nullptr;

		if (spriteAssetName != "")
		{
			sprite = getSpriteManager()->getSpriteByNameOrRequestFromServerIfNotExist(spriteAssetName);
		}
		if (spriteAssetID != -1)
		{
			sprite = getSpriteManager()->getSpriteAssetByIDOrRequestFromServerIfNotExist(spriteAssetID);
		}

		if (sprite != nullptr)
		{
			this->sprite = sprite;
			setInitialized_S(true);
		}
	}
}


string& Item::getName()
{
	return spriteAssetName;
}


string Item::description()
{
	if (_initialized)
	{
		return sprite->getItemGameDescription();
	}
	else
	{
		return "";
	}
}

int Item::getID()
{
	return spriteAssetID;
}

//The following method was originally marked 'synchronized':
void Item::getWithCaption_S()
{ //=========================================================================================================================

	setHaveItemValue_S(true);

	string name = "Got " + this->getName() + "!";

	if (getPlayer() != nullptr)
	{
		getCaptionManager()->newManagedCaption(Caption::Position::CENTERED_OVER_ENTITY, 0, 0, 5000, name, BobFont::font_normal_11_outlined, BobColor::green);
	}
	else
	{
		getCaptionManager()->newManagedCaption(Caption::Position::CENTERED_SCREEN, 0, 0, 5000, name, BobFont::font_normal_11_outlined, BobColor::green);
	}
	getAudioManager()->playSound("gotitem", 0.25f, 1.0f, 1);
}

//The following method was originally marked 'synchronized':
void Item::setHaveItemValue_S(bool b)
{ //=========================================================================================================================
	timeSet = System::currentHighResTimer();

	//   if (b == true)
	//   {
	//      getServerConnection()->addQueuedGameSaveUpdateRequest_S("itemsHeld:`" + getID() + ":true:" + timeSet + "`");
	//   }
	//   if (b == false)
	//   {
	//      getServerConnection()->addQueuedGameSaveUpdateRequest_S("itemsHeld:`" + getID() + ":false:" + timeSet + "`");
	//   }

	haveItemValue_S = b;
}

//The following method was originally marked 'synchronized':
void Item::initHaveItemValue_S(bool b, long long timeSet)
{ //=========================================================================================================================
	haveItemValue_S = b;
	this->timeSet = timeSet;
}

//The following method was originally marked 'synchronized':
bool Item::getHaveItemValue_S()
{ //=========================================================================================================================
	return haveItemValue_S;
}

long long Item::getTimeSet()
{ //=========================================================================================================================
	return timeSet;
}

