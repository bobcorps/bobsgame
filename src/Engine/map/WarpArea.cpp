#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




Logger WarpArea::log = Logger("WarpArea");


WarpArea::WarpArea(sp<Engine> g, sp<AreaData> a, sp<Map> m)
{ //=========================================================================================================================

	this->e = g;

	this->data = a;

	this->mapX = a->getMapXPixelsHQ();
	this->mapY = a->getMapYPixelsHQ();

	this->map = m;


	if (getEventData() != nullptr)
	{
		this->event = ms<Event>(g, getEventData(), this);
		//sp<Event> event = EnginePart::getEventManager()->getEventByIDCreateIfNotExist(Area::getEventData()->getID());
		//event->area = this;
	}
}

void WarpArea::update()
{ //=========================================================================================================================
	Area::update();

	long long currentTime = System::currentHighResTimer();
	long long startTime = lastRequestedMapDataTime;
	int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
	if (ticksPassed > 5000)
	{
		lastRequestedMapDataTime = currentTime;

		if (getDestinationMapName().length() > 0)
		{
			getMapManager()->requestMapDataIfNotLoadedYet(getDestinationMapName());
		}
	}
}

void WarpArea::enter()
{ //=========================================================================================================================

	//go through mapMan().doorList and find door.mapAsset=destinationMapAsset and door.entityNameIdentifier=getDestinationDoorName, get arrival x and arrival y

	if (destinationTYPEIDString() == "" || destinationTYPEIDString().length() == 0 || destinationTYPEIDString() == "AREA." + to_string(getID()))
	{
		return;
	}


	sp<Map> map = getMapManager()->getMapByNameBlockUntilLoaded(getDestinationMapName());

	if (map != nullptr)
	{
		for (int i = 0; i < (int)map->warpAreaList->size(); i++)
		{
			sp<WarpArea> w = map->warpAreaList.get(i);


			//if(w.mapAsset==getMapManager()->getMapByName(getDestinationMapName()))//should always be true since we are checking the destination map above
			//{

			if (w->getName() == getDestinationWarpAreaName())
			{
				getMapManager()->warpEntered = this;
				getMapManager()->warpExited = w;


				float arrivalX = w->arrivalXPixelsHQ();
				float arrivalY = w->arrivalYPixelsHQ();


				//keep horizontal position in between warpareas and doors.

				//if this is wider than tall, keep x position and use arrivalY

				if (getWidth() > getHeight())
				{
					arrivalX = w->getX() + (getPlayer()->getX() - getX()) + getPlayer()->getWidth() / 2;
				}

				if (getHeight() > getWidth())
				{
					arrivalY = w->getY() + (getPlayer()->getY() - getY()) + getPlayer()->getHeight() / 2;
				}


				getMapManager()->changeMap(getDestinationMapName(), (int)(arrivalX) + 8, (int)(arrivalY) + 8);
				return;
			}
			//}
		}
	}
}

void WarpArea::renderDebugInfo()
{ //=========================================================================================================================


	float x = screenLeft();
	float y = screenTop();
	GLUtils::drawOutlinedString(getName(), x, y - 36, OKColor::white);


	GLUtils::drawOutlinedString("getDestinationTYPEIDString: " + destinationTYPEIDString(), x, y - 27, ms<OKColor>(200, 0, 255));

	if (destinationTYPEIDString() == "AREA." + to_string(getID()) || destinationTYPEIDString() == "" || destinationTYPEIDString() == "none" || destinationTYPEIDString() == "self") //if it doesn't have a destination set, mark it as problematic
	{
		GLUtils::drawOutlinedString("WarpArea: Has no destination!", x, y - 18, OKColor::red);
	}
	//else
	GLUtils::drawOutlinedString("WarpArea: Goes to Map.Name: " + getDestinationMapName() + "." + getDestinationWarpAreaName(), x, y - 9, ms<OKColor>(200, 0, 255, 255));


	Area::renderDebugInfo();
}

void WarpArea::setDestinationTYPEIDString(const string& typeID)
{
	getData()->setDestinationTYPEIDString(typeID);
}


string& WarpArea::getDestinationMapName()
{
	return getData()->getDestinationMapName();
}


string& WarpArea::getDestinationWarpAreaName()
{
	return getData()->getDestinationWarpAreaName();
}

