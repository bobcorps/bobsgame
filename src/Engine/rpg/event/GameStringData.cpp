#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




//Logger GameStringData::log = Logger("GameStringData");


GameStringData::GameStringData()
{ //=========================================================================================================================
}

GameStringData::GameStringData(int id, const string& name, const string& text)
{ //=========================================================================================================================

	//AssetData(id, name);
	this->id = id;
	this->name = name;

	this->text = text;
}

//sp<GameStringData> GameStringData::fromBase64ZippedJSON(const string& b64)
//{ //===============================================================================================
//
//
//
//	string json = FileUtils::unzipBase64StringToString(b64);
//
//	//Gson gson = ms<Gson>();
//	//GameStringData data = gson.fromJson(json,GameStringData.class);
//
//	return fromJSON(json);
//}
//
//sp<GameStringData> GameStringData::fromJSON(const string& json)
//{ //===============================================================================================
//
//	//sp<Gson> gson = ms<Gson>();
//	sp<GameStringData> data = nullptr;// gson->fromJson(json, GameStringData::typeid);
//
//
//	return data;
//}


string& GameStringData::initFromString(string& t)
{
	t = super::initFromString(t);


	t = t.substr(t.find("text:`") + 1);
	t = t.substr(t.find("`") + 1);
	text = t.substr(0, t.find("`"));
	t = t.substr(t.find("`,") + 2);

	return t;
}




string GameStringData::getTYPEIDString()
{ //===============================================================================================
	return "GAMESTRING." + to_string(getID());
}


string& GameStringData::getText()
{
	return text;
}

void GameStringData::setText(const string& s)
{
	text = s;
}

