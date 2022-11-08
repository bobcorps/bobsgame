#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




Logger Skill::log = Logger("Skill");


Skill::Skill(shared_ptr<Engine> g, int id)
{ //=========================================================================================================================
	this->e = g;

	this->data = make_shared<SkillData>(id, "");

	for (int i = 0; i < (int)getEventManager()->skillList.size(); i++)
	{
		if (getEventManager()->skillList.at(i)->getID() == data->getID())
		{
			log.error("Skill already exists:" + data->getName());
			return;
		}
	}
	getEventManager()->skillList.push_back(shared_from_this());
}

Skill::Skill(shared_ptr<Engine> g, shared_ptr<SkillData> data)
{ //=========================================================================================================================
	this->e = g;

	this->data = data;
	setInitialized_S(true);

	for (int i = 0; i < (int)getEventManager()->skillList.size(); i++)
	{
		if (getEventManager()->skillList.at(i)->getID() == data->getID())
		{
			log.error("Skill already exists:" + data->getName());
			return;
		}
	}
	getEventManager()->skillList.push_back(shared_from_this());
}

shared_ptr<SkillData> Skill::getData()
{
	return data;
}

int Skill::getID()
{
	return getData()->getID();
}


string& Skill::getName()
{
	return getData()->getName();
}


string Skill::getTYPEIDString()
{
	return getData()->getTYPEIDString();
}

void Skill::setID(int id)
{
	getData()->setID(id);
}

void Skill::setName(const string& name)
{
	getData()->setName(name);
}

//The following method was originally marked 'synchronized':
void Skill::setData_S(shared_ptr<SkillData> data)
{ //=========================================================================================================================
	this->data = data;
	setInitialized_S(true);
}

//The following method was originally marked 'synchronized':
void Skill::setValue_S(float f)
{ //=========================================================================================================================
	timeSet = System::currentHighResTimer();
	value = f;
	getServerConnection()->addQueuedGameSaveUpdateRequest_S("skillValues:`" + to_string(data->getID()) + ":" + to_string(f) + ":" + to_string(timeSet) + "`");
}

void Skill::setValue_S(int i)
{ //=========================================================================================================================
	timeSet = System::currentHighResTimer();
	value = (float)i;
	getServerConnection()->addQueuedGameSaveUpdateRequest_S("skillValues:`" + to_string(data->getID()) + ":" + to_string(i) + ":" + to_string(timeSet) + "`");
}

//The following method was originally marked 'synchronized':
void Skill::initValueFromGameSave_S(float f, long long timeSet)
{ //=========================================================================================================================
	value = f;
	this->timeSet = timeSet;
}

//The following method was originally marked 'synchronized':
float Skill::getValue_S()
{ //=========================================================================================================================
	return value;
}

long long Skill::getTimeSet()
{ //=========================================================================================================================
	return timeSet;
}

