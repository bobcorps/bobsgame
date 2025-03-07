#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




Logger EventParameter::log = Logger("EventParameter");


EventParameter::EventParameter(sp<Engine> g, const string& parameterString)
{ //===============================================================================================

	this->e = g;
	this->parameterString = parameterString;
}

void EventParameter::parsePrimitive(const string& typeString, const string& primitiveValueString)
{ //===============================================================================================

	if (OKString::startsWith(typeString, "BOOL"))
	{
		//type=TYPE_BOOL;
		try
		{
			this->b = OKBoolean::parseBoolean(primitiveValueString);
		}
		catch (exception)
		{
			log.error("Could not parse bool in primitiveValueString");
		}
		
	}
	else
	{
		if (OKString::startsWith(typeString, "INT"))
		{
			//type=TYPE_INT;
			try
			{
				this->i = stoi(primitiveValueString);
			}
			catch (exception)
			{
				log.error("Could not parse int in primitiveValueString");
			}
		}
		else
		{
			if (OKString::startsWith(typeString, "FLOAT"))
			{
				//type=TYPE_FLOAT;
				try
				{
					this->f = stof(primitiveValueString);
				}
				catch (exception)
				{
					log.error("Could not parse float in primitiveValueString");
				}
			}
		}
	}
}

void EventParameter::updateParameterVariablesFromString(sp<Event> event)
{ //===============================================================================================

	//parameterName is always OBJECT.id


	if (OKString::startsWith(parameterString, "BOOL."))
	{
		parsePrimitive("BOOL", parameterString.substr(parameterString.find(".") + 1));
	}
	else
	{
		if (OKString::startsWith(parameterString, "INT."))
		{
			parsePrimitive("INT", parameterString.substr(parameterString.find(".") + 1));
		}
		else
		{
			if (OKString::startsWith(parameterString, "FLOAT."))
			{
				parsePrimitive("FLOAT", parameterString.substr(parameterString.find(".") + 1));
			}
			else
			{
				if (OKString::startsWith(parameterString, "STRING."))
				{
					parsePrimitive("STRING", parameterString.substr(parameterString.find(".") + 1));
				}
				else
				{
					if (parameterString == "PLAYER")
					{
						this->entityObject = (sp<Entity>)getPlayer().get();
					}
					else
					{
						if (parameterString == "THIS")
						{
							sp<Entity> o = nullptr;

							if (event->door != nullptr)
							{
								o = event->door;
							}
							if (event->entity != nullptr)
							{
								o = event->entity;
							}

							this->entityObject = o;
						}
						else
						{
							//if we made it here, it's a map object.
							sp<Map> o = ms<Map>(getEngine()->getGameObjectByTYPEIDName(parameterString));

							if (o == nullptr)
							{
								log.error("Could not find GameObject: " + parameterString + " when parsing Event Parameter.");
							}
							else
							{
								//String newTypeString = parameterString.substring(parameterString.indexOf("."));

								this->mapObject = o;
							}
						}
					}
				}
			}
		}
	}
}


string EventParameter::toString()
{ //===============================================================================================


	log.error("Should never call toString in EventParameter");


	return "";
}

