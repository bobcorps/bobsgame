#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




Logger Dialogue::log = Logger("Dialogue");


Dialogue::Dialogue(shared_ptr<Engine> g, int id)
{ //=========================================================================================================================
	this->e = g;

	this->data = make_shared<DialogueData>(id, "", "", "", "");

	for (int i = 0; i < (int)getEventManager()->dialogueList.size(); i++)
	{
		if (getEventManager()->dialogueList.get(i)->getID() == data->getID())
		{
			log.error("Dialogue already exists:" + data->getName());
			return;
		}
	}
	getEventManager()->dialogueList.add(shared_from_this());
}

Dialogue::Dialogue(shared_ptr<Engine> g, shared_ptr<DialogueData> data)
{ //=========================================================================================================================
	this->e = g;

	this->data = data;
	setInitialized_S(true);

	for (int i = 0; i < (int)getEventManager()->dialogueList.size(); i++)
	{
		if (getEventManager()->dialogueList.get(i)->getID() == data->getID())
		{
			log.error("Dialogue already exists:" + data->getName());
			return;
		}
	}
	getEventManager()->dialogueList.add(shared_from_this());
}

//The following method was originally marked 'synchronized':
void Dialogue::setData_S(shared_ptr<DialogueData> data)
{ //=========================================================================================================================

	this->data = data;
	setInitialized_S(true);
}

shared_ptr<DialogueData> Dialogue::getData()
{
	return data;
}

int Dialogue::getID()
{
	return getData()->getID();
}


string& Dialogue::getName()
{
	return getData()->getName();
}


string& Dialogue::caption()
{
	return getData()->getCaption();
}


string& Dialogue::getComment()
{
	return getData()->getComment();
}


string& Dialogue::text()
{
	return getData()->getText();
}


string Dialogue::getTYPEIDString()
{
	return getData()->getTYPEIDString();
}

void Dialogue::setID(int id)
{
	getData()->setID(id);
}

void Dialogue::setName(const string& name)
{
	getData()->setName(name);
}

void Dialogue::setCaption(const string& caption)
{
	getData()->setCaption(caption);
}

void Dialogue::setComment(const string& comment)
{
	getData()->setComment(comment);
}

void Dialogue::setText(const string& text)
{
	getData()->setText(text);
}

//The following method was originally marked 'synchronized':
void Dialogue::setDialogueDoneValue_S(bool b)
{ //=========================================================================================================================

	//send a request to the server

	//Main.clientObject.sendDialogueDoneValueUpdate(id, b);

	timeSet = System::currentHighResTimer();

	if (b == true)
	{
		getServerConnection()->addQueuedGameSaveUpdateRequest_S("dialoguesDone:`" + to_string(getID()) + ":true:" + to_string(timeSet) + "`");
	}
	if (b == false)
	{
		getServerConnection()->addQueuedGameSaveUpdateRequest_S("dialoguesDone:`" + to_string(getID()) + ":false:" + to_string(timeSet) + "`");
	}


	doneValue_S = b;
}

//The following method was originally marked 'synchronized':
void Dialogue::initDialogueDoneValueFromGameSave_S(bool b, long long timeSet)
{ //=========================================================================================================================

	doneValue_S = b;
	this->timeSet = timeSet;
}

//The following method was originally marked 'synchronized':
bool Dialogue::getDialogueDoneValue_S()
{ //=========================================================================================================================
	return doneValue_S;
}

long long Dialogue::getTimeSet()
{ //=========================================================================================================================
	return timeSet;
}

