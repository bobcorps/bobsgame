//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

#pragma once
#include "bobtypes.h"

#include "BobsGame.h"
#include "GameType.h"

#include <iostream>
#include <sstream>
#include <src/main.h>

//=========================================================================================================================
class GameSequence
{//=========================================================================================================================
public:

	GameSequence()
	{
		boost::uuids::random_generator generator;
		uuid = to_string(generator());
	}

	string uuid = "";
	string name = "My New Game Sequence";
	string description = "This is an empty game sequence.";

	//this is saved to XML, we don't want to save the entire currentGameType object
	ArrayList<string> importExport_gameUUIDs;

	//this is populated from loadedGameTypes when sequence is loaded, if a name from gameUUIDs cannot be found it will not be in here and an error will be logged
	ArrayList<shared_ptr<GameType>>gameTypes;

	bool randomizeSequence = true;

	//this should only be exported when sending to other players
	string currentDifficultyName = "Beginner";

	//non exported
	//bool builtInType = false;
	bool downloaded = false;

	//filled in from the server
	long long creatorUserID = 0;
	string creatorUserName = "";
	long long dateCreated = 0;
	long long lastModified = 0;
	long long howManyTimesUpdated = 0;
	long long upVotes = 0;
	long long downVotes = 0;
	string yourVote = "none";

public:

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(uuid);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(description);
		if (version < 3)
		{
			ArrayList<string> gameUUIDs;
			ar & BOOST_SERIALIZATION_NVP(gameUUIDs);
			importExport_gameUUIDs = gameUUIDs;
		}
		else
		{
			ar & BOOST_SERIALIZATION_NVP(importExport_gameUUIDs);
		}

		ar & BOOST_SERIALIZATION_NVP(randomizeSequence);
		if (version < 5)
		{
			bool endlessMode = false;
			ar & BOOST_SERIALIZATION_NVP(endlessMode);
		}

		if (version < 3)
		{
			ar & BOOST_SERIALIZATION_NVP(currentDifficultyName);
		}

		if (version < 4)
		{
			bool builtInType = false;
			ar & BOOST_SERIALIZATION_NVP(builtInType);
		}

		if (version == 0)
		{
			string loadedFilename = "";
			ar & BOOST_SERIALIZATION_NVP(loadedFilename);
		}
		if(version>0)
		{
			ar & BOOST_SERIALIZATION_NVP(creatorUserID);
			ar & BOOST_SERIALIZATION_NVP(creatorUserName);
			ar & BOOST_SERIALIZATION_NVP(dateCreated);
			ar & BOOST_SERIALIZATION_NVP(lastModified);
			ar & BOOST_SERIALIZATION_NVP(howManyTimesUpdated);
			ar & BOOST_SERIALIZATION_NVP(upVotes);
			ar & BOOST_SERIALIZATION_NVP(downVotes);
			
		}
		if(version==1)
		{
			bool haveYouVoted = false;
			ar & BOOST_SERIALIZATION_NVP(haveYouVoted);
		}
		if(version>1)
		{
			ar & BOOST_SERIALIZATION_NVP(yourVote);
		}
	}

};
BOOST_CLASS_VERSION(GameSequence, 5)
BOOST_CLASS_TRACKING(GameSequence, boost::serialization::track_never)
//=========================================================================================================================
class NetworkGameSequence : public GameSequence
{//=========================================================================================================================
public:

	NetworkGameSequence()
	{

	}

	// when we receive a network game sequence, let's save both the sequence and the game types contained within in our local cache folder
	NetworkGameSequence(GameSequence g)
	{
		this->uuid = g.uuid;
		this->name = g.name;
		this->description = g.description;
		this->importExport_gameUUIDs = g.importExport_gameUUIDs;
		this->gameTypes = g.gameTypes;
		this->randomizeSequence = g.randomizeSequence;
		this->currentDifficultyName = g.currentDifficultyName;
		//this->builtInType = g.builtInType;
		this->creatorUserID = g.creatorUserID;
		this->creatorUserName = g.creatorUserName;
		this->dateCreated = g.dateCreated;
		this->lastModified = g.lastModified;
		this->howManyTimesUpdated = g.howManyTimesUpdated;
		this->upVotes = g.upVotes;
		this->downVotes = g.downVotes;

	}

	ArrayList<GameType>importExport_games; //we do want the entire currentGameType objects for the games when we are playing multiplayer and need to send this to other players

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version);

	//=========================================================================================================================
	string toBase64GZippedXML()
	{//=========================================================================================================================
		shared_ptr<NetworkGameSequence >s = this;
		NetworkGameSequence gs;
		gs = *s;

		std::stringstream ss;
		boost::archive::xml_oarchive oarchive(ss);
		oarchive << BOOST_SERIALIZATION_NVP(gs);
		//Main::log.debug(ss.str());
		string zip = FileUtils::zipStringToBase64String(ss.str());
		return zip;
	}

	//=========================================================================================================================
	static shared_ptr<NetworkGameSequence >fromBase64GZippedXML(string b64GZipXML)
	{//=========================================================================================================================
		string xml = FileUtils::unzipBase64StringToString(b64GZipXML);

		if (xml == "" || xml.length() == 0)
		{
			return nullptr;
		}

		stringstream ss;
		ss << xml;
		boost::archive::xml_iarchive ia(ss);

		try
		{
			NetworkGameSequence gs;
			ia >> BOOST_SERIALIZATION_NVP(gs);
			shared_ptr<NetworkGameSequence >s = make_shared<NetworkGameSequence>();
			*s = gs;

			return s;
		}
		catch(exception)
		{
			Main::log.error("Could not unserialize NetworkGameSequence");
		}
		return nullptr;
	}
};
BOOST_CLASS_VERSION(NetworkGameSequence, 2)
BOOST_CLASS_TRACKING(NetworkGameSequence, boost::serialization::track_never)