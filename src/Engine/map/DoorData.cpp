#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




//Logger DoorData::log = Logger("DoorData");


DoorData::DoorData()
{ //=========================================================================================================================
}

DoorData::DoorData(int id, const string& name, const string& spriteAssetName, int spawnXPixels1X, int spawnYPixels1X, const string& destinationTYPEID, int arrivalXPixels, int arrivalYPixels, bool randomNPCSpawnPoint, float randomSpawnChance, bool randomExitPoint, int randomSpawnDelay, bool randomSpawnKids, bool randomSpawnAdults, bool randomSpawnMales, bool randomSpawnFemales, shared_ptr<EventData> eventData, const string& comment)
{ //=========================================================================================================================


	initEntityData(id, name, spriteAssetName, spawnXPixels1X, spawnYPixels1X, 0, false, true, 255, 1.0f, 12, false, false, false, false, false, 0, 0, false, false, true, eventData, comment); //int eventID, - boolean getDisableShadow, - boolean getRandomFrames, - boolean getOnlyHereDuringEvent, - int ticksBetweenAnimation, - int getTicksBetweenFrames, - boolean randomTimeBetweenAnimation, - boolean animateThroughFrames, - boolean getAlwaysOnBottom, - boolean getAboveWhenEqual, - boolean getAboveTopLayer, - float getScale, - int alphaByte, - boolean getNonWalkable, - boolean getPushable, - int getInitialFrame, - int getSpawnYPixels1X, - int getSpawnXPixels1X, - String spriteAssetName, - String name, - int id,


	this->arrivalXPixels1X = arrivalXPixels;
	this->arrivalYPixels1X = arrivalYPixels;


	this->randomNPCSpawnPoint = randomNPCSpawnPoint;
	this->randomSpawnChance = randomSpawnChance;
	this->randomPointOfInterestOrExit = randomExitPoint; //point of interest
	this->randomSpawnDelay = randomSpawnDelay;
	this->randomSpawnKids = randomSpawnKids;
	this->randomSpawnAdults = randomSpawnAdults;
	this->randomSpawnMales = randomSpawnMales;
	this->randomSpawnFemales = randomSpawnFemales;

	this->destinationTYPEID = destinationTYPEID;
}

DoorData::DoorData(int id, const string& name)
{ //=========================================================================================================================
	EntityData(id, name, "", 0, 0, 0, false, true, 255, 1.0f, 12, false, false, false, false, false, 0, 0, false, false, true, nullptr, ""); //int eventID, - boolean getDisableShadow, - boolean getRandomFrames, - boolean getOnlyHereDuringEvent, - int ticksBetweenAnimation, - int getTicksBetweenFrames, - boolean randomTimeBetweenAnimation, - boolean animateThroughFrames, - boolean getAlwaysOnBottom, - boolean getAboveWhenEqual, - boolean getAboveTopLayer, - float getScale, - int alphaByte, - boolean getNonWalkable, - boolean getPushable, - int getInitialFrame, - int getSpawnYPixels1X, - int getSpawnXPixels1X, - String spriteAssetName, - String name, - int id,
}

//shared_ptr<DoorData> DoorData::fromBase64ZippedJSON(const string& b64)
//{ //===============================================================================================
//
//
//	string json = FileUtils::unzipBase64StringToString(b64);
//
//	//Gson gson = make_shared<Gson>();
//	//DoorData data = gson.fromJson(json,DoorData.class);
//
//
//	return fromJSON(json);
//}
//
//shared_ptr<DoorData> DoorData::fromJSON(const string& json)
//{ //===============================================================================================
//
//
//	//shared_ptr<Gson> gson = make_shared<Gson>();
//	shared_ptr<DoorData> data = nullptr;// gson->fromJson(json, DoorData::typeid);
//
//
//	return data;
//}



string& DoorData::initFromString(string& t)
{
	t = super::initFromString(t);


	t = t.substr(t.find("destinationTYPEID:`") + 1);
	t = t.substr(t.find("`") + 1);
	destinationTYPEID = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("arrivalXPixels1X:`") + 1);
	t = t.substr(t.find("`") + 1);
	arrivalXPixels1X = stoi(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("arrivalYPixels1X:`") + 1);
	t = t.substr(t.find("`") + 1);
	arrivalYPixels1X = stoi(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomNPCSpawnPoint:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomNPCSpawnPoint = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomSpawnChance:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomSpawnChance = stof(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomPointOfInterestOrExit:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomPointOfInterestOrExit = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomSpawnDelay:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomSpawnDelay = stoi(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomSpawnKids:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomSpawnKids = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomSpawnAdults:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomSpawnAdults = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomSpawnMales:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomSpawnMales = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("randomSpawnFemales:`") + 1);
	t = t.substr(t.find("`") + 1);
	randomSpawnFemales = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("destinationMapName:`") + 1);
	t = t.substr(t.find("`") + 1);
	destinationMapName = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("destinationDoorName:`") + 1);
	t = t.substr(t.find("`") + 1);
	destinationDoorName = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);


	return t;


}




string DoorData::getTYPEIDString()
{ //===============================================================================================
	return "DOOR." + to_string(getID());
}


string& DoorData::getDestinationTYPEIDString()
{
	return destinationTYPEID;
}


string& DoorData::getDestinationMapName()
{
	return destinationMapName;
}


string& DoorData::getDestinationDoorName()
{
	return destinationDoorName;
}

int DoorData::getArrivalXPixels1X()
{
	return arrivalXPixels1X;
}

int DoorData::getArrivalYPixels1X()
{
	return arrivalYPixels1X;
}

float DoorData::getArrivalXPixelsHQ()
{
	return (float)arrivalXPixels1X * 2;
}

float DoorData::getArrivalYPixelsHQ()
{
	return (float)arrivalYPixels1X * 2;
}

bool DoorData::getRandomNPCSpawnPoint()
{
	return randomNPCSpawnPoint;
}

float DoorData::getRandomSpawnChance()
{
	return randomSpawnChance;
}

bool DoorData::getRandomPointOfInterestOrExit()
{
	return randomPointOfInterestOrExit;
}

int DoorData::getRandomSpawnDelay()
{
	return randomSpawnDelay;
}

bool DoorData::getRandomSpawnKids()
{
	return randomSpawnKids;
}

bool DoorData::getRandomSpawnAdults()
{
	return randomSpawnAdults;
}

bool DoorData::getRandomSpawnMales()
{
	return randomSpawnMales;
}

bool DoorData::getRandomSpawnFemales()
{
	return randomSpawnFemales;
}

void DoorData::setDestinationTYPEIDString(const string& s)
{
	destinationTYPEID = s;
}

void DoorData::setDestinationMapName(const string& s)
{
	destinationMapName = s;
}

void DoorData::setDestinationDoorName(const string& s)
{
	destinationDoorName = s;
}

void DoorData::setRandomPointOfInterestOrExit(bool s)
{
	randomPointOfInterestOrExit = s;
}

void DoorData::setRandomNPCSpawnPoint(bool s)
{
	randomNPCSpawnPoint = s;
}

void DoorData::setRandomSpawnChance(float s)
{
	randomSpawnChance = s;
}

void DoorData::setRandomSpawnDelay(int s)
{
	randomSpawnDelay = s;
}

void DoorData::setRandomSpawnKids(bool s)
{
	randomSpawnKids = s;
}

void DoorData::setRandomSpawnAdults(bool s)
{
	randomSpawnAdults = s;
}

void DoorData::setRandomSpawnMales(bool s)
{
	randomSpawnMales = s;
}

void DoorData::setRandomSpawnFemales(bool s)
{
	randomSpawnFemales = s;
}

void DoorData::setArrivalXPixels1X(int s)
{
	arrivalXPixels1X = s;
}

void DoorData::setArrivalYPixels1X(int s)
{
	arrivalYPixels1X = s;
}

