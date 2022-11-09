#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




//Logger MapData::log = Logger("MapData");


int MapData::MAP_GROUND_LAYER = 0;
int MapData::MAP_GROUND_DETAIL_LAYER = 1;
int MapData::MAP_SHADER_LAYER = 2;
int MapData::MAP_GROUND_SHADOW_LAYER = 3;
int MapData::MAP_OBJECT_LAYER = 4;
int MapData::MAP_OBJECT_DETAIL_LAYER = 5;
int MapData::MAP_OBJECT_SHADOW_LAYER = 6;
int MapData::MAP_ENTITY_LAYER = 12;
int MapData::MAP_DOOR_LAYER = 16;
int MapData::MAP_ABOVE_LAYER = 7;
int MapData::MAP_ABOVE_DETAIL_LAYER = 8;
int MapData::MAP_SPRITE_SHADOW_LAYER = 9;
int MapData::MAP_CAMERA_BOUNDS_LAYER = 10;
int MapData::MAP_HIT_LAYER = 11;
int MapData::MAP_LIGHT_LAYER = 13;
int MapData::MAP_AREA_LAYER = 14;
int MapData::MAP_LIGHT_MASK_LAYER = 15;
int MapData::MAP_ENTITY_LAYER_ABOVE = 100;
int MapData::layers = 17;

bool MapData::isTileLayer(int l)
{ //===============================================================================================
	if (l == MAP_DOOR_LAYER)
	{
		return false;
	}
	if (l == MAP_ENTITY_LAYER)
	{
		return false;
	}
	if (l == MAP_ENTITY_LAYER_ABOVE)
	{
		return false;
	}
	if (l == MAP_AREA_LAYER)
	{
		return false;
	}
	if (l == MAP_LIGHT_LAYER)
	{
		return false;
	}

	return true;
}

bool MapData::isTransparentLayer(int l)
{ //===============================================================================================
	if (l == MAP_AREA_LAYER)
	{
		return true;
	}
	if (l == MAP_LIGHT_LAYER)
	{
		return true;
	}
	if (l == MAP_HIT_LAYER)
	{
		return true;
	}
	if (l == MAP_SPRITE_SHADOW_LAYER)
	{
		return true;
	}
	if (l == MAP_GROUND_SHADOW_LAYER)
	{
		return true;
	}
	if (l == MAP_SHADER_LAYER)
	{
		return true;
	}
	if (l == MAP_OBJECT_SHADOW_LAYER)
	{
		return true;
	}
	if (l == MAP_CAMERA_BOUNDS_LAYER)
	{
		return true;
	}
	if (l == MAP_LIGHT_MASK_LAYER)
	{
		return true;
	}
	return false;
}

MapData::MapData()
{ //=========================================================================================================================
}

MapData::MapData(int id, const string& name, int widthTiles1X, int heightTiles1X)
{ //=========================================================================================================================


	AssetData(id, name);


	this->widthTiles1X = widthTiles1X;
	this->heightTiles1X = heightTiles1X;

	//		this.getWidthPixels1X = getWidthTiles1X*8; //1x pixel width
	//		this.getHeightPixels1X = getHeightTiles1X*8; //1x pixel width
	//		this.widthTilesHQ2X = getWidthTiles1X*2; //hq2x tile width
	//		this.heightTilesHQ2X = getHeightTiles1X*2; //hq2x tile width
	//		this.widthPixelsHQ2X = getWidthTiles1X*2*8; //hq2x pixel width
	//		this.heightPixelsHQ2X = getHeightTiles1X*2*8; //hq2x pixel width
}

//sp<MapData> MapData::fromBase64ZippedJSON(const string& b64)
//{ //===============================================================================================
//
//
//	string json = FileUtils::unzipBase64StringToString(b64);
//
//
//	//Gson gson = ms<Gson>();
//	//MapData data = gson.fromJson(json,MapData.class);
//
//	return fromJSON(json);
//}
//
//sp<MapData> MapData::fromJSON(const string& json)
//{ //===============================================================================================
//
//
//	//sp<Gson> gson = ms<Gson>();
//	sp<MapData> data = nullptr;// gson->fromJson(json, MapData::typeid);
//
//
//	return data;
//}




string& MapData::initFromString(string& t)
{
	t = super::initFromString(t);


	t = t.substr(t.find("mapNote:`") + 1);
	t = t.substr(t.find("`") + 1);
	mapNote = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("widthTiles1X:`") + 1);
	t = t.substr(t.find("`") + 1);
	widthTiles1X = stoi(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("heightTiles1X:`") + 1);
	t = t.substr(t.find("`") + 1);
	heightTiles1X = stoi(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("maxRandoms:`") + 1);
	t = t.substr(t.find("`") + 1);
	maxRandoms = stoi(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("isOutside:`") + 1);
	t = t.substr(t.find("`") + 1);
	isOutside = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("preload:`") + 1);
	t = t.substr(t.find("`") + 1);
	preload = OKBoolean::parseBoolean(t.substr(0, t.find("`")));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("groundLayerMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	groundLayerMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("groundObjectsMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	groundObjectsMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("groundShadowMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	groundShadowMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("objectsMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	objectsMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("objects2MD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	objects2MD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("objectShadowMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	objectShadowMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("aboveMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	aboveMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("above2MD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	above2MD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("spriteShadowMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	spriteShadowMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("groundShaderMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	groundShaderMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("cameraBoundsMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	cameraBoundsMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("hitBoundsMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	hitBoundsMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("lightMaskMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	lightMaskMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("paletteMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	paletteMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("tilesMD5:`") + 1);
	t = t.substr(t.find("`") + 1);
	tilesMD5 = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	t = t.substr(t.find("stateDataList:{") + 1);
	t = t.substr(t.find("{") + 1);
	while (OKString::startsWith(t,"}") == false)
	{
		sp<MapStateData>data = ms<MapStateData>();
		t = data->initFromString(t);
		stateDataList->add(data);
	}
	t = t.substr(t.find("}") + 1);
	t = t.substr(t.find(",") + 1);

	t = t.substr(t.find("eventDataList:{") + 1);
	t = t.substr(t.find("{") + 1);
	while (OKString::startsWith(t, "}") == false)
	{
		sp<EventData>data = ms<EventData>();
		t = data->initFromString(t);
		eventDataList->add(data);
	}
	t = t.substr(t.find("}") + 1);
	t = t.substr(t.find(",") + 1);

	t = t.substr(t.find("doorDataList:{") + 1);
	t = t.substr(t.find("{") + 1);
	while (OKString::startsWith(t,"}") == false)
	{
		sp<DoorData>data = ms<DoorData>();
		t = data->initFromString(t);
		doorDataList->add(data);
	}
	t = t.substr(t.find("}") + 1);
	t = t.substr(t.find(",") + 1);


	return t;


}





string MapData::getTYPEIDString()
{ //===============================================================================================
	return "MAP." + to_string(getID());
}

int MapData::getWidthTiles1X()
{
	return widthTiles1X;
}

int MapData::getHeightTiles1X()
{
	return heightTiles1X;
}

int MapData::getMaxRandoms()
{
	return maxRandoms;
}

bool MapData::getIsOutside()
{
	return isOutside;
}

bool MapData::getPreload()
{
	return preload;
}


string& MapData::getMapNote()
{
	return mapNote;
}


string& MapData::getGroundLayerMD5()
{
	return groundLayerMD5;
}


string& MapData::getGroundObjectsMD5()
{
	return groundObjectsMD5;
}


string& MapData::getGroundShadowMD5()
{
	return groundShadowMD5;
}


string& MapData::getObjectsMD5()
{
	return objectsMD5;
}


string& MapData::getObjects2MD5()
{
	return objects2MD5;
}


string& MapData::getObjectShadowMD5()
{
	return objectShadowMD5;
}


string& MapData::getAboveMD5()
{
	return aboveMD5;
}


string& MapData::getAbove2MD5()
{
	return above2MD5;
}


string& MapData::getSpriteShadowMD5()
{
	return spriteShadowMD5;
}


string& MapData::getGroundShaderMD5()
{
	return groundShaderMD5;
}


string& MapData::getCameraBoundsMD5()
{
	return cameraBoundsMD5;
}


string& MapData::getHitBoundsMD5()
{
	return hitBoundsMD5;
}


string& MapData::getLightMaskMD5()
{
	return lightMaskMD5;
}


string& MapData::getPaletteMD5()
{
	return paletteMD5;
}


string& MapData::getTilesMD5()
{
	return tilesMD5;
}

int MapData::getWidthPixelsHQ()
{
	return getWidthTiles1X() * 8 * 2;
}

int MapData::getHeightPixelsHQ()
{
	return getHeightTiles1X() * 8 * 2;
}

sp<vector<sp<MapStateData>>> MapData::getStateDataList()
{
	return stateDataList;
}

sp<vector<sp<EventData>>> MapData::getEventDataList()
{
	return eventDataList;
}

sp<vector<sp<DoorData>>> MapData::getDoorDataList()
{
	return doorDataList;
}

void MapData::setGroundLayerMD5(const string& s)
{
	groundLayerMD5 = s;
}

void MapData::setGroundObjectsMD5(const string& s)
{
	groundObjectsMD5 = s;
}

void MapData::setGroundShadowMD5(const string& s)
{
	groundShadowMD5 = s;
}

void MapData::setObjectsMD5(const string& s)
{
	objectsMD5 = s;
}

void MapData::setObjects2MD5(const string& s)
{
	objects2MD5 = s;
}

void MapData::setObjectShadowMD5(const string& s)
{
	objectShadowMD5 = s;
}

void MapData::setAboveMD5(const string& s)
{
	aboveMD5 = s;
}

void MapData::setAbove2MD5(const string& s)
{
	above2MD5 = s;
}

void MapData::setSpriteShadowMD5(const string& s)
{
	spriteShadowMD5 = s;
}

void MapData::setGroundShaderMD5(const string& s)
{
	groundShaderMD5 = s;
}

void MapData::setCameraBoundsMD5(const string& s)
{
	cameraBoundsMD5 = s;
}

void MapData::setHitBoundsMD5(const string& s)
{
	hitBoundsMD5 = s;
}

void MapData::setLightMaskMD5(const string& s)
{
	lightMaskMD5 = s;
}

void MapData::setPaletteMD5(const string& s)
{
	paletteMD5 = s;
}

void MapData::setTilesMD5(const string& s)
{
	tilesMD5 = s;
}

void MapData::setMapNote(const string& s)
{
	mapNote = s;
}

void MapData::setWidthTiles1X(int s)
{
	widthTiles1X = s;
}

void MapData::setHeightTiles1X(int s)
{
	heightTiles1X = s;
}

void MapData::setMaxRandoms(int s)
{
	maxRandoms = s;
}

void MapData::setIsOutside(bool s)
{
	isOutside = s;
}

void MapData::setPreload(bool s)
{
	preload = s;
}

