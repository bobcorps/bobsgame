
#include "stdafx.h"
#include <iostream>

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

Logger TCPServerConnection::log = Logger("TCPServerConnection");
shared_ptr<Logger> TCPServerConnection::_threadLog = make_shared<Logger>("TCPServerConnection");

#include "src/Puzzle/Room.h"


TCPServerConnection::TCPServerConnection()
{//===============================================================================================

}

TCPServerConnection::~TCPServerConnection()
{//===============================================================================================

	cleanup();

}

void TCPServerConnection::cleanup()
{//===============================================================================================

	if (threadStarted)
	{
		setStopThread_S(true);
		t.join();
		threadStarted = false;
	}
}

//===============================================================================================
void TCPServerConnection::update()
{//===============================================================================================

	if (threadStarted == false)
	{
		log.debug("Created server thread");
		t = thread(&TCPServerConnection::updateThreadLoop, this);
		threadStarted = true;
	}

	//handle incoming messages queue which is populated from the thread
	while (incomingMessageQueueSize_S() > 0)
	{
		string s = incomingMessageQueueFront_S();
		incomingMessageQueuePop_S();

		//log.info("LZ4: " + s.substr(0, min(s.length(),160)));

		if (String::startsWith(s, "PARTIAL:"))
		{
			s = s.substr(s.find(":") + 1);


			partialPacketString += s;
		}
		else
		if (String::startsWith(s, "FINAL:"))
		{
			s = s.substr(s.find(":") + 1);

			partialPacketString += s;

			string packet = FileUtils::unlz4Base64StringToString(partialPacketString);

			partialPacketString = "";

			//strip off endline
			packet = packet.substr(0, packet.find(BobNet::endline));

#ifdef _DEBUG
			if(packet.find("Login")!=string::npos || packet.find("Reconnect") != string::npos || packet.find("Create_Account") != string::npos)log.info("FROM SERVER: " + packet.substr(0, packet.find(":")+1));
			else log.info("FROM SERVER: " + packet.substr(0, 230));
#endif
			messageReceived(packet);

		}
		else
		{
			if (partialPacketString.length()>0)
			{
				log.warn("Partial packet from server was not completed before got another packet");
			}


			string packet = FileUtils::unlz4Base64StringToString(s);

			//strip off endline
			packet = packet.substr(0, packet.find(BobNet::endline));

#ifdef _DEBUG
			if (packet.find("Login") != string::npos || packet.find("Reconnect") != string::npos || packet.find("Create_Account") != string::npos)log.info("FROM SERVER: " + packet.substr(0, packet.find(":") + 1));
			else log.info("FROM SERVER: " + packet.substr(0, 230));
#endif

			messageReceived(packet);

		}
	}

}

//===============================================================================================
void TCPServerConnection::updateThreadLoop(shared_ptr<TCPServerConnection >u)
{//===============================================================================================

	long long _queuedSaveGameUpdateDelayTime = 0;
	long long _sendOnlineFriendListRequestTime = 0;
	long long _sendOutgoingAuthorizedMessageQueueTime = 0;
	long long _lastCheckedSessionToken = -20000;

	while (u->getStopThread_S() == false)
	{

		this_thread::sleep_for(chrono::milliseconds(10));
		//SDL_Delay(10);

		if (u->ensureConnectedToServerThreadBlock_S())
		{
			long long currentTime = System::currentHighResTimer();

			u->_checkForIncomingTraffic();
			u->_sendKeepAlivePing();
			u->_updateServerStats();
			u->_getClientLocation();
			u->_checkForTimeout();

			if (u->getAuthorizedOnServer_S() == false)
			{
				if ((int)(System::getTicksBetweenTimes(_lastCheckedSessionToken, currentTime)) > 1000)
				{
					_lastCheckedSessionToken = currentTime;

					string userName = u->getLastUserNameOrEmail_S();
					string pass = u->getLastPassword_S();

					if (userName != "" && pass != "")u->_doLoginNoCaptions(userName, pass, false);
					else u->checkForSessionTokenAndLogInIfExists();

				}
			}

			if (u->getAuthorizedOnServer_S() == true)
			{
				u->_getInitialGameSave();

				if ((int)(System::getTicksBetweenTimes(_queuedSaveGameUpdateDelayTime, currentTime)) > 2000)
				{
					_queuedSaveGameUpdateDelayTime = currentTime;
					u->sendQueuedGameSaveUpdates();
				}

				if ((int)(System::getTicksBetweenTimes(_sendOutgoingAuthorizedMessageQueueTime, currentTime)) > 500)
				{
					_sendOutgoingAuthorizedMessageQueueTime = currentTime;
					while (u->outgoingAuthorizedMessageQueueSize_S() > 0)
					{
						string s = u->outgoingAuthorizedMessageQueueFront_S();
						u->outgoingAuthorizedMessageQueuePop_S();
						u->write_S(s);
					}
				}

				if ((int)(System::getTicksBetweenTimes(_sendOnlineFriendListRequestTime, currentTime)) > 10000)
				{
					_sendOnlineFriendListRequestTime = currentTime;
					u->sendOnlineFriendListRequest_S();
				}
			}

		}
	}

}

//===============================================================================================
void TCPServerConnection::_updateServerStats()
{//===============================================================================================

	if (ensureConnectedToServerThreadBlock_S())
	{
		//send keepalive
		//keep last got friend keepalive ping/pong
		long long currentTime = System::currentHighResTimer();
		long long startTime = _lastSentGetServerStatsTime;
		int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
		if (ticksPassed > 10000) //10 seconds
		{

			_lastSentGetServerStatsTime = currentTime;
			write_S(BobNet::Server_Stats_Request + BobNet::endline);

		}

	}
}
//===============================================================================================
void TCPServerConnection::_getClientLocation()
{//===============================================================================================

	if (ensureConnectedToServerThreadBlock_S() && _requestedClientLocation == false)
	{
		_requestedClientLocation = true;
		write_S(BobNet::Client_Location_Request + BobNet::endline);
	}
}
	
//===============================================================================================
void TCPServerConnection::_sendKeepAlivePing()
{//===============================================================================================

	if (ensureConnectedToServerThreadBlock_S())
	{
		//send keepalive
		//keep last got friend keepalive ping/pong
		long long currentTime = System::currentHighResTimer();
		long long startTime = getLastReceivedDataTime_S();
		int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
		if (ticksPassed > 10000) //10 seconds
		{
			//send ping
			startTime = _lastSentPingTime;
			int pingTicksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
			if (pingTicksPassed > 10000)
			{
				_lastSentPingTime = currentTime;
				write_S("ping" + BobNet::endline);
			}
		}
	}
}

//===============================================================================================
void TCPServerConnection::_checkForTimeout()
{//===============================================================================================

	long long currentTime = System::currentHighResTimer();
	long long startTime = getLastReceivedDataTime_S();
	int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
	if (ticksPassed > 30000)
	{

		//close if no keepalive
		//connection to server lost
		setDisconnectedFromServer_S("Timed out.");
		return;
	}
}

//===============================================================================================
void TCPServerConnection::_getInitialGameSave()
{//===============================================================================================

	if (_initialGameSaveReceived_nonThreaded == false)
	{
		long long currentTime = System::currentHighResTimer();
		//hammering the synchronized methods was causing lock issues, put a small delay on network checks
		long long startTime = _checkInitialGameSaveReceivedDelayTime;
		int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
		if (ticksPassed > 5000)
		{
			if (getInitialGameSaveReceived_S() == false)
			{
				//don't send initial request until we have authorized.
				//otherwise we will create an authorization thread that is sitting there waiting for us to have a userID anyway.

				_checkInitialGameSaveReceivedDelayTime = currentTime;
				sendInitialGameSaveRequest();

			}
			else
			{
				_initialGameSaveReceived_nonThreaded = true; //non threaded, a bit faster.
				Main::console->add("Authorized on server: " + getGameSave_S().userName, 5000, BobColor::green);
			}
		}
		return;
	}
}

//===============================================================================================
void TCPServerConnection::setDisconnectedFromServer_S(string reason)
{//===============================================================================================
	setConnectedToServer_S(false);
	setServerIPAddressString_S("");
	setNotAuthorizedOnServer_S();
	//initialGameSaveReceived_nonThreaded = false;

	threadLogWarn_S(string("Disconnected from server: "+reason));
	Main::console->add("Disconnected from Server: "+ reason, 5000, BobColor::red);

	SDLNet_TCP_Close(getSocket_S());

	if (getSocketAddedToSet_S())
	{
		if (SDLNet_TCP_DelSocket(getSocketSet_S(), getSocket_S()) < 0)
		{
			threadLogWarn_S("SDLNet_TCP_DelSocket: " + string(SDLNet_GetError()) + string(SDL_GetError()));
			SDL_ClearError();
		}

		setSocketAddedToSet_S(false);
	}

}

//===============================================================================================
void TCPServerConnection::_checkForIncomingTraffic()
{//===============================================================================================

	if (getSocketAddedToSet_S())
	{
		int numReady = SDLNet_CheckSockets(getSocketSet_S(), 0);
		if (numReady < 0)
		{
			threadLogWarn_S("SDLNet_CheckSockets: " + string(SDLNet_GetError()) + string(SDL_GetError()));
			SDL_ClearError();
		}



		queue<string> packetsToProcess;

		int bytesReceived = 1;

		while (numReady > 0 && getStopThread_S() == false)
		{


			int rd = SDLNet_SocketReady(getSocket_S());

			if (rd < 0)
			{
				threadLogWarn_S("SDLNet_TCP_Recv Error: " + string(SDLNet_GetError()) + string(SDL_GetError()));
				SDL_ClearError();
			}

			while (rd > 0)
			{
				rd--;

				//while (bytesReceived > 0)//dont do this for tcp, doesnt work
				{
					const int size = 65535;
					char* buf = new char[size];

					bytesReceived = SDLNet_TCP_Recv(getSocket_S(), buf, size);

					if (bytesReceived > 0)
					{
						string *s = new string(buf, bytesReceived);

						packetsToProcess.push(s);
						delete[] buf;

					}
					else if (bytesReceived < 0)
					{
						delete[] buf;
						//connection to server lost
						threadLogWarn_S("SDLNet_TCP_Recv:" + to_string(bytesReceived));
						setDisconnectedFromServer_S("Error receiving data.");
						
					}
					else
						delete[] buf;
				}
			}

			numReady = SDLNet_CheckSockets(getSocketSet_S(), 0);
			if (numReady < 0)
			{
				threadLogWarn_S("SDLNet_CheckSockets: " + string(SDLNet_GetError()) + string(SDL_GetError()));
				SDL_ClearError();
			}


		}

		while (packetsToProcess.size() > 0)
		{

			string *sp = packetsToProcess.front();
			packetsToProcess.pop();
			string s = *sp;

			if (_truncatedPacketString != "")
			{
				string *temp = new string(_truncatedPacketString + *sp);
				delete sp;
				sp = temp;
				s = *sp;
				_truncatedPacketString = "";
			}

			if (s.find(BobNet::endline) == string::npos)
			{
				//threadLogWarn_S("Packet doesn't contain endline, waiting for next packet to append to.");
				_truncatedPacketString += s;
			}
			else
			{
				//handled below
//				if (s.substr(s.length() - BobNet::endline.length()) != BobNet::endline)
//				{
//					threadLogWarn_S("Packet doesn't end in endline, ends in: " + s.substr(s.length() - BobNet::endline.length()) + " | Full packet: "+s);
//				}

				while (s.find(BobNet::endline) != string::npos)
				{

					//strip off endline
					string packet = s.substr(0, s.find(BobNet::endline));
					s = s.substr(s.find(BobNet::endline) + BobNet::endline.length());

					setLastReceivedDataTime_S(System::currentHighResTimer());


//#ifdef _DEBUG
//					threadLogInfo_S("RAW FROM SERVER: " + packet.substr(0, min(packet.length(), 160)));
//#endif

					if (String::startsWith(packet, "ping"))
					{
						//log.debug("SERVER: ping");
						write_S("pong" + BobNet::endline);
						delete sp;
						return;
					}

					if (String::startsWith(packet, "pong"))
					{
						delete sp;
						return;
					}


					incomingMessageQueuePush_S(packet);

				}

				if (s.length() > 0)
				{
					//threadLogWarn_S("Storing remainder of packet to append to next packet:"+s);
					_truncatedPacketString += s;
				}
			}

			delete sp;
		}

	}

}


//when this goes bad, cant reconnect, get a new one from the load balancer.

//when i connect to a server, it checks its hashtable and determines whether i am registered there right now.

//if not, it grabs my last encryption key from the database and proceeds as usual.

//as a client, if i have a sessionToken set, i am good to go.
//otherwise, do login to get a sessionToken.

//just send the sessionToken to the new server i connect to.
//if i need an initialGameState, get it.

//if i already have a gameState, just proceed normally, auth with the sessionToken to register with the server

//yeah, should pause the game on every channel.write, put transparency over screen and say waiting to reconnect to server
//this should be very infrequent and it will allow for reconnecting to a different server.

//nD should be on a different thread, so minigames can keep going

//===============================================================================================
bool TCPServerConnection::ensureConnectedToServerThreadBlock_S()
{//===============================================================================================

	if (getConnectedToServer_S() == false)
	{
		//log.debug("Not connected to server. Trying...");

		//if we dont have a server ip address, connect to the load balancer
		if (getServerIPAddressString_S() == "")
		{

			if (_couldNotResolveLoadBalancer)return false;
			if (_couldNotOpenConnectionToLoadBalancerCount>5)return false;

			if (getSocketAddedToSet_S() == false)
			{
				//resolve load balancer
				if(_loadBalancerAddress == nullptr)
				{
					Main::console->add("Connecting to server...", 5000, BobColor::green);
					//Main::whilefix();
					threadLogDebug_S("Resolving host to load balancer...");

					_loadBalancerAddress = make_shared<IPaddress>();
					if (SDLNet_ResolveHost(_loadBalancerAddress, Main::serverAddressString.c_str(), Main::serverTCPPort) < 0 )
					{
						threadLogWarn_S("Could not resolve load balancer IP: " + string(SDLNet_GetError()) + string(SDL_GetError()));
						SDL_ClearError();
						_couldNotResolveLoadBalancer = true;
						threadLogWarn_S("Networking is disabled");
						Main::console->add("Could not connect to server: Networking is disabled.", 5000, BobColor::red);
						return false;
					}

				}

				//open connection to load balancer
				{
					long long currentTime = System::currentHighResTimer();
					long long startTime = _lastLoadBalancerConnectTime;
					int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
					if (ticksPassed > _couldNotOpenConnectionToLoadBalancerCount * 1000)
					{
						_lastLoadBalancerConnectTime = currentTime;

						threadLogDebug_S("Connecting to load balancer...");

						setSocket_S(SDLNet_TCP_Open(_loadBalancerAddress));//TODO: if it can't connect to the server the thread stalls here
						if (!getSocket_S())
						{
							//SDLNet_FreeSocketSet(set);
							threadLogWarn_S("Could not open connection to load balancer: " + string(SDLNet_GetError()) + string(SDL_GetError()));
							SDL_ClearError();
							_couldNotOpenConnectionToLoadBalancerCount++;
							return false;
						}

						setSocketIsOpen_S(true);

						if (getSocketAddedToSet_S() == false)
						{
							if (SDLNet_TCP_AddSocket(getSocketSet_S(), getSocket_S()) < 0)
							{
								threadLogWarn_S("SDLNet_TCP_AddSocket: " + string(SDLNet_GetError()) + string(SDL_GetError()));
								SDL_ClearError();
							}
							else
								setSocketAddedToSet_S(true);
						}
					}
					else
					{
						return false;
					}
				}

			}

			//wait for load balancer to respond (we are connected to a server)

			//when connected to load balancer, send getServerIPCommand to get the servers real IP behind the load balancer
			long long currentTime = System::currentHighResTimer();
			long long startTime = _lastSentServerIPRequestTime;
			int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
			if (ticksPassed > 1000)
			{
				threadLogDebug_S("Requesting server IP from load balancer...");//Don't have server IP

				_lastSentServerIPRequestTime = currentTime;

				write_S(BobNet::Server_IP_Address_Request + BobNet::endline);
			}
			//wait for server to return IP in message response (handled elsewhere)
			//message response will set server IP which will break out of the loop or try again with a new server
			//for (int i = 0; i < 25; i++)
			//{

				this_thread::sleep_for(chrono::milliseconds(100));
				//Main::delay(100);
				_checkForIncomingTraffic();

				this_thread::sleep_for(chrono::milliseconds(100));

				if (getServerIPAddressString_S() != "")
				{
					threadLogDebug_S("Got server IP address from load balancer, connecting to server.");
					//i = 25;
					//break;
				}
				else return false;
			//}
			//return false;

			//if we have server IP here we will break out of the while loop and continue.
			//otherwise we try again by connecting to the LB again and get a new server.
			//if (getServerIPAddress_S() == nullptr)return false;
		}

		//disconnecting from the LB will set the address to null again, so we store it.


		if(getSocketIsOpen_S())
		{
			//close the connection to the load balancer
			SDLNet_TCP_Close(getSocket_S());
			setSocketIsOpen_S(false);
		}

		if (_serverAddress == nullptr)
		{

			//for running server locally
			if (Main::serverAddressString == "localhost")setServerIPAddressString_S("localhost");

			_serverAddress = make_shared<IPaddress>();
			if (SDLNet_ResolveHost(_serverAddress, getServerIPAddressString_S().c_str(), Main::serverTCPPort) < 0)
			{
				threadLogError_S("Could not resolve server address: " + string(SDL_GetError()));
				setDisconnectedFromServer_S("Could not resolve server address.");
				return false;
			}
		}

		if (getSocketAddedToSet_S())
		{
			if (SDLNet_TCP_DelSocket(getSocketSet_S(), getSocket_S()) < 0)
			{
				threadLogDebug_S("SDLNet_TCP_DelSocket: " + string(SDLNet_GetError()) + string(SDL_GetError()));
				SDL_ClearError();
			}

			setSocketAddedToSet_S(false);
		}

		if (_couldNotOpenConnectionToServerCount > 10)
		{
			setDisconnectedFromServer_S("Could not open connection to server.");
			_couldNotOpenConnectionToServerCount = 0;
			return false;
		}

		long long currentTime = System::currentHighResTimer();
		long long startTime = _lastServerConnectTime;
		int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
		if (ticksPassed > _couldNotOpenConnectionToServerCount * 1000)
		{
			_lastServerConnectTime = currentTime;

			//connect to the server
			setSocket_S(SDLNet_TCP_Open(_serverAddress));
			if (!getSocket_S())
			{
				threadLogWarn_S("Could not open connection to server: " + string(SDLNet_GetError()) + string(SDL_GetError()));
				SDL_ClearError();
				_couldNotOpenConnectionToServerCount++;
				return false;
			}

			if (SDLNet_TCP_AddSocket(getSocketSet_S(), getSocket_S()) < 0)
			{
				threadLogWarn_S("SDLNet_TCP_AddSocket: " + string(SDLNet_GetError()) + string(SDL_GetError()));
				SDL_ClearError();
			}
			else
				setSocketAddedToSet_S(true);

			threadLogDebug_S("Connected to server.");
			Main::console->add("Connected to server.", 5000, BobColor::green);

			//wait for the server to open the channel

			setConnectedToServer_S(true);
			setLastReceivedDataTime_S(currentTime);
		}
	}
	return true;

}

////===============================================================================================
//bool TCPServerConnection::ensureAuthorizedOnServerThreadBlock_S()
//{//===============================================================================================
//
//	if (getAuthorizedOnServer_S() == false)
//	{
//		if (getUserID_S() != -1) //we have a userID set, we must have dropped the connection. reconnect.
//		{
//			//send reconnect request
//
//			//wait for server to authorize our credentials
//
//			//set got reconnect response
//
//			//set session authorized
//
//			//write immediately in this thread, don't create another thread, because the queue is already blocking on this one!
//			write_S(BobNet::Reconnect_Request + "`" + to_string(getUserID_S()) + "`,`" + getSessionToken_S() + "`" + BobNet::endline);
//		}
//		else
//		{
//			//we haven't successfully logged in yet.
//			threadLogWarn_S("Thread is waiting to authorize on Server before we have logged in.");
//		}
//
//		//wait for server to return auth in message response
//		for (int i = 0; i < 5; i++)
//		{
//			//Main::delay(500);
//			this_thread::sleep_for(chrono::milliseconds(500));
//
//			_checkForIncomingTraffic();
//
//			if (getAuthorizedOnServer_S() == true)
//			{
//				threadLogDebug_S("Authenticated to server.");
//
//				return true;
//			}
//		}
//
//		return false;
//	}
//	return true;
//}

bool TCPServerConnection::messageReceived(string &s)// shared_ptr<ChannelHandlerContext> ctx, shared_ptr<MessageEvent> e)
{ //===============================================================================================





	if (String::startsWith(s, BobNet::Server_IP_Address_Response))
	{
		incomingServerIPAddressResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Server_Stats_Response))
	{
		incomingServerStatsResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Client_Location_Response))
	{
		incomingClientLocationResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Login_Response))
	{
		incomingLoginResponse(s);
		return true;
	}

//	if (String::startsWith(s, BobNet::Facebook_Login_Response))
//	{
//		incomingFacebookCreateAccountOrLoginResponse(s);
//		return true;
//	}

	if (String::startsWith(s, BobNet::Reconnect_Response))
	{
		incomingReconnectResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Tell_Client_Their_Session_Was_Logged_On_Somewhere_Else))
	{
		incomingSessionWasLoggedOnSomewhereElse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Tell_Client_Servers_Are_Shutting_Down))
	{
		incomingServersAreShuttingDown(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Tell_Client_Servers_Have_Shut_Down))
	{
		incomingServersHaveShutDown(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Password_Recovery_Response))
	{
		incomingPasswordRecoveryResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Create_Account_Response))
	{
		incomingCreateAccountResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Initial_GameSave_Response))
	{
		incomingInitialGameSaveResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Encrypted_GameSave_Update_Response))
	{
		incomingGameSaveUpdateResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Update_Facebook_Account_In_DB_Response))
	{
		incomingUpdateFacebookAccountInDBResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Online_Friends_List_Response))
	{
		incomingOnlineFriendsListResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Friend_Is_Online_Notification))
	{
		incomingFriendOnlineNotification(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Add_Friend_By_UserName_Response))
	{
		incomingAddFriendByUserNameResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Bobs_Game_GameTypesAndSequences_Download_Response))
	{
		incomingBobsGameGameTypesAndSequencesDownloadResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Bobs_Game_GameTypesAndSequences_Upload_Response))
	{
		incomingBobsGameGameTypesAndSequencesUploadResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Bobs_Game_GameTypesAndSequences_Vote_Response))
	{
		incomingBobsGameGameTypesAndSequencesVoteResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Bobs_Game_RoomList_Response))
	{
		incomingBobsGameRoomListResponse(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Bobs_Game_NewRoomCreatedUpdate))
	{
		incomingBobsGameNewRoomCreatedUpdate(s);
		return true;
	}

	if (String::startsWith(s, BobNet::Chat_Message))
	{
		incomingChatMessage(s);
		return true;
	}



	if (String::startsWith(s, BobNet::Bobs_Game_UserStatsLeaderBoardsAndHighScoresBatched))
	{
		//cut off first command
		s = s.substr(s.find(":") + 1);

		while (s.length() > 0)
		{
			string stats = "";
			if (s.find(BobNet::batch) != string::npos)
			{
				stats = s.substr(0, s.find(BobNet::batch));
				s = s.substr(s.find(BobNet::batch) + BobNet::batch.length());
			}
			else
			{
				stats = s;
				s = "";
			}

			if (String::startsWith(stats, BobNet::Bobs_Game_UserStatsForSpecificGameAndDifficulty))
			{
				incomingBobsGameUserStatsForSpecificGameAndDifficulty(stats);
			}
			else
			if (String::startsWith(stats, BobNet::Bobs_Game_LeaderBoardsByTotalTimePlayed))
			{
				incomingBobsGameLeaderBoardByTotalTimePlayed(stats);

			}
			else
			if (String::startsWith(stats, BobNet::Bobs_Game_LeaderBoardsByTotalBlocksCleared))
			{
				incomingBobsGameLeaderBoardByTotalBlocksCleared(stats);

			}
			else
			if (String::startsWith(stats, BobNet::Bobs_Game_LeaderBoardsByPlaneswalkerPoints))
			{
				incomingBobsGameLeaderBoardByPlaneswalkerPoints(stats);

			}
			else
			if (String::startsWith(stats, BobNet::Bobs_Game_LeaderBoardsByEloScore))
			{
				incomingBobsGameLeaderBoardByEloScore(stats);

			}
			else
			if (String::startsWith(stats, BobNet::Bobs_Game_HighScoreBoardsByTimeLasted))
			{
				incomingBobsGameHighScoreBoardsByTimeLasted(stats);

			}
			else
			if (String::startsWith(stats, BobNet::Bobs_Game_HighScoreBoardsByBlocksCleared))
			{
				incomingBobsGameHighScoreBoardsByBlocksCleared(stats);

			}		

		}
		return true;

	}

	
	if (String::startsWith(s, BobNet::Bobs_Game_GameStats_Response))
	{
		incomingBobsGameGameStatsResponse_S(s);
		return true;
	}	

	if (String::startsWith(s, BobNet::Bobs_Game_ActivityStream_Response))
	{
		incomingBobsGameActivityStreamResponse_S(s);
		return true;
	}
	if (String::startsWith(s, BobNet::Bobs_Game_ActivityStream_Update))
	{
		incomingBobsGameActivityStreamUpdate_S(s);
		return true;
	}

	bool processed = false;
	for (int i = 0; i < BobNet::engines.size(); i++)
	{
		if (BobNet::engines.get(i)->serverMessageReceived(s))processed = true;
	}
	if (processed)return true;

	log.error("Did not handle TCP packet:"+s);

	return false;
}

bool TCPServerConnection::write_S(string s)
{ //===============================================================================================

	if (s.find(BobNet::endline) == string::npos)
	{
		threadLogError_S("Message doesn't end with endline");
		s = s + BobNet::endline;
	}

#ifdef _DEBUG
	//if (String::startsWith(s, "ping")==false && String::startsWith(s, "pong") == false)
	{
		if (s.find("Login") != string::npos || s.find("Reconnect") != string::npos || s.find("Create_Account") != string::npos)threadLogDebug_S("SEND SERVER: " + s.substr(0, s.find(":") + 1));
		else threadLogDebug_S("SEND SERVER: " + s.substr(0, s.length() - BobNet::endline.length()));
	}
#endif

	const char* buf = s.c_str();

	int bytesSent = 0;
	//while(bytesSent<(int)s.length())
		bytesSent += SDLNet_TCP_Send(getSocket_S(), (void *)buf, (int)s.length());

		if(bytesSent<(int)s.length())
		{
			threadLogWarn_S("Could not sent full string, sent: " + s.substr(0, bytesSent));
			return false;
		}
	//delete buf;
		return true;

}



void TCPServerConnection::incomingServerIPAddressResponse(string s)
{ //=========================================================================================================================

  //ServerIP:ip
	s = s.substr(s.find(":") + 1); //ip
	setServerIPAddressString_S(s);
}



void TCPServerConnection::incomingServerStatsResponse(string s)
{ //=========================================================================================================================



  //Server_Stats_Response:stats object
	s = s.substr(s.find(":") + 1);

	shared_ptr<ServerStats >stats = make_shared<ServerStats>();
	stats->initFromString(s);

	serverStats = stats;

	log.debug("Server uptime:" + to_string(stats->serverUptime) + " Users online:" + to_string(stats->usersOnline));
}



void TCPServerConnection::incomingClientLocationResponse(string s)
{ //=========================================================================================================================

  //Client_Location_Response:State, Country
	s = s.substr(s.find(":") + 1); //ip


	clientLocation = s;

	if (clientLocation != "")log.info("Got client IP location from server:" + clientLocation);

}


bool TCPServerConnection::connectAndWriteToChannelBeforeAuthorization_S(string s)
{ //=========================================================================================================================

  //   serverCommandExecutorService->execute([&] ()
  //      {
  //         try
  //         {
  //            Thread::currentThread().setName("ClientTCP_connectAndWriteToChannelBeforeAuthorization");
  //         }
  //         catch (SecurityException e)
  //         {
  //            e->printStackTrace();
  //         }
  //log.debug("connectAndWriteToChannelBeforeAuthorization() Start");
	if (ensureConnectedToServerThreadBlock_S())
	{
		//log.debug("connectAndWriteToChannelBeforeAuthorization() Success");
		write_S(s);
		return true;
	}
	return false;
	//}
	//);
}

bool TCPServerConnection::connectAndAuthorizeAndQueueWriteToChannel_S(string s)
{ //=========================================================================================================================

	if (Main::previewClientInEditor || Main::introMode)
	{
		threadLogDebug_S("Blocked writing to network: " + s);
		return false;
	}

	//queue in outgoingAuthorizedMessageQueue

	outgoingAuthorizedMessageQueuePush_S(s);

//	thread *t = new thread
//	(
//			[](shared_ptr<TCPServerConnection>u,string e)
//			{
//				while (u->ensureConnectedToServerThreadBlock_S() == false)
//				{
//					this_thread::sleep_for(chrono::milliseconds(1000));
//				}
//
//				while(u->ensureAuthorizedOnServerThreadBlock_S() == false)
//				{
//					this_thread::sleep_for(chrono::milliseconds(1000));
//				}
//
//				u->write_S(e);
//
//			},this,s
//	);

	return true;
}


void TCPServerConnection::sendLoginRequest(string email, string password, bool stats)
{ //=========================================================================================================================

	statsAllowed = stats;

	string message = "";

	if (stats == false)
	{
		message = BobNet::Login_Request + "`" + email + "`,`" + password + "`" + BobNet::endline;
	}
	else
	{
		//send session info
		string clientInfoString = "";// Main::mainObject->clientInfo->encode();
		message = BobNet::Login_Request + "`" + email + "`,`" + password + "`," + clientInfoString + BobNet::endline;
	}

	connectAndWriteToChannelBeforeAuthorization_S(message);
}

void TCPServerConnection::sendReconnectRequest(long long userID, string sessionToken, bool stats)
{ //=========================================================================================================================

	statsAllowed = stats; //just used if we need to reconnect

	string message = "";

	if (stats == false)
	{
		message = BobNet::Reconnect_Request + "`" + to_string(userID) + "`,`" + sessionToken + "`" + BobNet::endline;
	}
	else
	{
		//send session info
		string clientInfoString = "";// Main::mainObject->clientInfo->encode();

		message = BobNet::Reconnect_Request + "`" + to_string(userID) + "`,`" + sessionToken + "`," + clientInfoString + BobNet::endline;
	}

	connectAndWriteToChannelBeforeAuthorization_S(message);
}

void TCPServerConnection::incomingLoginResponse(string s)
{ //=========================================================================================================================

  //LoginResponse:Failed
  //LoginResponse:Success,userID,`sessionToken`
	s = s.substr(s.find(":") + 1); //Success,userID,`sessionToken`

	if (String::startsWith(s, "Success") == false)
	{
		setLoginResponse_S(true, false);
	}
	else
	{
		setLoginResponse_S(true, true);

		//server sends back userID and sessionToken, need to store these
		s = s.substr(s.find(",") + 1); //userID,`sessionToken`
		long long userID = -1;

		try
		{
			userID = stoll(s.substr(0, s.find(",")));
		}
		catch(exception)
		{
			log.error("Could not parse userID in login response");
		}

		s = s.substr(s.find("`") + 1); //sessionToken`
		string sessionToken = s.substr(0, s.find("`"));

		setUserID_S(userID);
		setSessionToken_S(sessionToken);
	}
}

void TCPServerConnection::incomingReconnectResponse(string s)
{ //=========================================================================================================================

  //ReconnectResponse:Failed
  //ReconnectResponse:Success,userID,`sessionToken`
	s = s.substr(s.find(":") + 1); //Success,userID,`sessionToken`

	if (String::startsWith(s, "Success") == false)
	{
		setReconnectResponse_S(true, false);
	}
	else
	{
		setReconnectResponse_S(true, true);

		//server sends back userID and sessionToken, need to store these
		s = s.substr(s.find(",") + 1); //userID,`sessionToken`
		long long userID = -1;
		try
		{
			userID = stoll(s.substr(0, s.find(",")));
		}
		catch (exception)
		{
			log.error("Could not parse userID in reconnect response");
		}

		s = s.substr(s.find("`") + 1); //sessionToken`
		string sessionToken = s.substr(0, s.find("`"));

		setUserID_S(userID);
		setSessionToken_S(sessionToken);
	}
}



void TCPServerConnection::incomingSessionWasLoggedOnSomewhereElse(string s)
{
  //TellClientTheirSessionWasLoggedOnSomewhereElse

  //setUserID_S(-1);
  //setSessionToken_S("");

	Main::getMain()->stateManager->pushState(Main::getMain()->loggedOutState);
}

void TCPServerConnection::incomingServersAreShuttingDown(string s)
{

	Main::getMain()->serversAreShuttingDown = true;
}

void TCPServerConnection::incomingServersHaveShutDown(string s)
{

	Main::getMain()->stateManager->pushState(Main::getMain()->serversHaveShutDownState);
}




void TCPServerConnection::sendInitialGameSaveRequest()
{ //=========================================================================================================================
  //the game should wait until this is received.
  //it should also get an encrypted game save.

  //needs to resend this request every 3 seconds

//	long long startTime = lastInitialGameSaveRequestTime;
//	long long currentTime = System::currentHighResTimer();
//	int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
//	if (ticksPassed > 3000)
//	{
		//lastInitialGameSaveRequestTime = currentTime;
		connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Initial_GameSave_Request + BobNet::endline);
	//}
}

void TCPServerConnection::incomingInitialGameSaveResponse(string s)
{ //=========================================================================================================================
  //parse off all values, initialize engine
  //parse off flagsSet,dialoguesDone,skillValues

  //parse gamesave

  //InitialGameSave:userID:`1`,thing:`thing`,,etc.
	string gameSaveString = s.substr(s.find(":") + 1);

	GameSave g = getGameSave_S();
	g.decodeGameSave(gameSaveString);
	setGameSave_S(g);

	//setGameSave_S(g);
	setInitialGameSaveReceived_S(true);

}




void TCPServerConnection::sendQueuedGameSaveUpdates()
{ //=========================================================================================================================

  //keep resending the same game update request every few seconds until we have a definite reply.
	GameSaveUpdateRequest g = getQueuedGameSaveUpdateRequest_S(0);

	if (g.requestString!="")
	{
		if (g.sent == true)
		{
			long long startTime = g.timeLastSent;
			long long currentTime = System::currentHighResTimer();
			int ticksPassed = (int)(System::getTicksBetweenTimes(startTime, currentTime));
			if (ticksPassed > 3000)
			{
				g.timeLastSent = currentTime;
				connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Encrypted_GameSave_Update_Request + to_string(g.requestID) + "," + g.requestString + ",gameSave:" + getEncryptedGameSave_S() + BobNet::endline);

				threadLogInfo_S("Sent Game Save Update Request:" + to_string(g.requestID));
			}
		}
		else
		{
			//GameSaveUpdateRequest:14,flagsSet:`3`,gameSave
			connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Encrypted_GameSave_Update_Request + to_string(g.requestID) + "," + g.requestString + ",gameSave:" + getEncryptedGameSave_S() + BobNet::endline);
			g.sent = true;

			threadLogInfo_S("Sent Game Save Update Request:" + to_string(g.requestID));
		}
	}
}

void TCPServerConnection::incomingGameSaveUpdateResponse(string s)
{ //=========================================================================================================================

  //EncryptedGameSave:id,blob
	s = s.substr(s.find(":") + 1); //id,blob
	int gameSaveID = -1;

	try
	{
		gameSaveID = stoi(s.substr(0, s.find(",")));
	}
	catch (exception)
	{
		log.error("Could not parse gameSaveID in gameSave response");
	}

	s = s.substr(s.find(",") + 1); //blob
	string encryptedGameSave = s;

	log.info("Received Encrypted Game Save:" + to_string(gameSaveID));

	//store encrypted gameSave
	setEncryptedGameSave_S(encryptedGameSave);

	//parse off ID, remove from queue
	removeQueuedGameSaveUpdateRequestByID_S(gameSaveID);
}


//void TCPServerConnection::sendFacebookLoginCreateAccountIfNotExist(string facebookID, string accessToken, bool stats)
//{ //=========================================================================================================================
//	statsAllowed = stats;
//
//	string message = "";
//
//	if (stats == false)
//	{
//		message = BobNet::Facebook_Login_Request + "`" + facebookID + "`,`" + accessToken + "`" + BobNet::endline;
//	}
//	else
//	{
//		//send session info
//		string clientInfoString = Main::mainObject->clientInfo->encode();
//		message = BobNet::Facebook_Login_Request + "`" + facebookID + "`,`" + accessToken + "`," + clientInfoString + BobNet::endline;
//	}
//
//	connectAndWriteToChannelBeforeAuthorization_S(message);
//}
//
//void TCPServerConnection::incomingFacebookCreateAccountOrLoginResponse(string s)
//{ //=========================================================================================================================
//
//  //FacebookLoginResponse:Failed
//  //FacebookLoginResponse:Success,userID,`sessionToken`
//	s = s.substr(s.find(":") + 1); //Success,userID,`sessionToken`
//
//	if (String::startsWith(s, "Success") == false)
//	{
//		setFacebookLoginResponse_S(true, false);
//	}
//	else
//	{
//		setFacebookLoginResponse_S(true, true);
//
//		//server sends back userID and sessionToken, need to store these
//		s = s.substr(s.find(",") + 1); //userID,`sessionToken`
//		int userID = -1;
//
//		try
//		{
//			userID = stoi(s.substr(0, s.find(",")));
//		}
//		catch (exception)
//		{
//			log.error("Could not parse userID in facebook create account or login response");
//		}
//
//		s = s.substr(s.find("`") + 1); //sessionToken`
//		string sessionToken = s.substr(0, s.find("`"));
//
//		setUserID_S(userID);
//		setSessionToken_S(sessionToken);
//	}
//}



void TCPServerConnection::sendCreateAccountRequest(string userName, string email, string password)
{ //=========================================================================================================================

	connectAndWriteToChannelBeforeAuthorization_S(BobNet::Create_Account_Request + "`" + userName + "`,`" + email + "`,`" + password + "`" + BobNet::endline);
}

void TCPServerConnection::incomingCreateAccountResponse(string s)
{ //=========================================================================================================================

  s = s.substr(s.find(":")+1);

  //it doesn't matter what the response was, we should not provide any information otherwise they can determine whether the email is signed up.
  //so we just say "ok we tried to make an account, check your email"

	setGotCreateAccountResponse_S(s);
}



void TCPServerConnection::sendPasswordRecoveryRequest(string email)
{
	connectAndWriteToChannelBeforeAuthorization_S(BobNet::Password_Recovery_Request + "`" + email + "`" + BobNet::endline);
}

void TCPServerConnection::incomingPasswordRecoveryResponse(string s)
{
  //s = s.substring(s.indexOf(":")+1);

	setGotPasswordRecoveryResponse_S(true);
}



void TCPServerConnection::sendUpdateFacebookAccountInDBRequest_S()
{
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Update_Facebook_Account_In_DB_Request + BobNet::endline);
}

void TCPServerConnection::incomingUpdateFacebookAccountInDBResponse(string s)
{ //=========================================================================================================================
  //UpdateFacebookAccountInDBResponse:Failed
  //UpdateFacebookAccountInDBResponse:Success

	s = s.substr(s.find(":") + 1); //Success

	if (String::startsWith(s, "Success") == true)
	{
		setFacebookAccountUpdateResponseState_S(true, true);

		//	BobNet.UpdateFacebookAccountInDBResponse+"Success:`"+
		//	facebookID+"`,`"+
		//	facebookAccessToken+"`,`"+
		//	facebookEmail+"`,`"+
		//	facebookBirthday+"`,`"+
		//	facebookFirstName+"`,`"+
		//	facebookLastName+"`,`"+
		//	facebookGender+"`,`"+
		//	facebookLocale+"`,`"+
		//	facebookTimeZone+"`,`"+
		//	facebookUsername+"`,`"+
		//	facebookWebsite+"`"+

		GameSave g = getGameSave_S();

		s = s.substr(s.find("`") + 1);
		g.facebookID = s.substr(0, s.find("`"));
		s = s.substr(s.find("`") + 3);
		g.facebookAccessToken = s.substr(0, s.find("`"));
		s = s.substr(s.find("`") + 3);
		g.facebookEmail = s.substr(0, s.find("`"));
		s = s.substr(s.find("`") + 3);
//		getGameSave_S()->facebookBirthday = s.substr(0, s.find("`"));
//		s = s.substr(s.find("`") + 3);
		g.facebookFirstName = s.substr(0, s.find("`"));
		s = s.substr(s.find("`") + 3);
		g.facebookLastName = s.substr(0, s.find("`"));
		s = s.substr(s.find("`") + 3);
		g.facebookGender = s.substr(0, s.find("`"));
//		s = s.substr(s.find("`") + 3);
//		getGameSave_S()->facebookLocale = s.substr(0, s.find("`"));
//		s = s.substr(s.find("`") + 3);
//
//		getGameSave_S()->facebookTimeZone = stof(s.substr(0, s.find("`")));
//
//		s = s.substr(s.find("`") + 3);
//		getGameSave_S()->facebookUsername = s.substr(0, s.find("`"));
//		s = s.substr(s.find("`") + 3);
//		getGameSave_S()->facebookWebsite = s.substr(0, s.find("`"));

		setGameSave_S(g);
	}
	else
	{
		setFacebookAccountUpdateResponseState_S(true, false);
	}
}

//===============================================================================================
void TCPServerConnection::sendOnlineFriendListRequest_S()
{//===============================================================================================

	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Online_Friends_List_Request + BobNet::endline);

}

//===============================================================================================
void TCPServerConnection::sendBobsGameGameTypesAndSequencesDownloadRequest_S()
{//===============================================================================================

	connectAndWriteToChannelBeforeAuthorization_S(BobNet::Bobs_Game_GameTypesAndSequences_Download_Request + BobNet::endline);

}
//===============================================================================================
void TCPServerConnection::sendBobsGameGetHighScoresAndLeaderboardsRequest_S()
{//===============================================================================================

	connectAndWriteToChannelBeforeAuthorization_S(BobNet::Bobs_Game_GetHighScoresAndLeaderboardsRequest + BobNet::endline);

}

//===============================================================================================
void TCPServerConnection::incomingBobsGameGameTypesAndSequencesDownloadResponse(string &s)
{ //=========================================================================================================================
	//BobsGameGamesResponse:GameType:MD5:XML:userid:username:name:uuid:datecreated:lastmodified:howmanytimesupdated:upvotes:downvotes:haveyouvoted
	//					GameSequence:MD5
	s = s.substr(s.find(":") + 1);

	setGotBobsGameGameTypesAndSequencesDownloadResponse_S(true);

	//threadsafe store gametypes

	BobsGame::parseIncomingGameTypesAndSequencesFromServer_S(s);

}

//===============================================================================================
void TCPServerConnection::incomingBobsGameGameTypesAndSequencesUploadResponse(string &s)
{ //=========================================================================================================================
  //BobsGameGamesResponse:Success:
  //BobsGameGamesResponse:Failed:
	s = s.substr(s.find(":") + 1);

	setGotBobsGameGameTypesAndSequencesUploadResponse_S(s);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameGameTypesAndSequencesVoteResponse(string &s)
{ //=========================================================================================================================
  //BobsGameGamesResponse:Success:
  //BobsGameGamesResponse:Failed:
	s = s.substr(s.find(":") + 1);

	setGotBobsGameGameTypesAndSequencesVoteResponse_S(s);
}

//===============================================================================================
void TCPServerConnection::sendBobsGameRoomListRequest_S()
{//===============================================================================================

	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_RoomList_Request + BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameRoomListResponse(string &s)
{ //=========================================================================================================================
	//Bobs_Game_RoomList_Response:
	s = s.substr(s.find(":") + 1);

	setBobsGameRoomListResponse_S(s);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameNewRoomCreatedUpdate(string &s)
{ //=========================================================================================================================
 
	s = s.substr(s.find(":") + 1);
	string userName = FileUtils::removeSwearWords(s.substr(0, s.find(":")));
	s = s.substr(s.find(":") + 1);
	//shared_ptr<Room >r = Room::decodeRoomData(s, false);

	if (Main::globalSettings->hideNotifications == false)
	{
		Main::rightConsole->add("" + userName + " is hosting a multiplayer room!", 5000, BobColor::green);
	}
}


//===============================================================================================
void TCPServerConnection::tellBobsGameRoomHostMyUserID_S(const string& roomUUID)
{//===============================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_TellRoomHostToAddMyUserID+roomUUID +":"+ BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::tellServerBobsGameHostingPublicGameUpdate_S(const string& roomDescription)
{//===============================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_HostingPublicRoomUpdate + roomDescription + ":" + BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::tellServerBobsGameIHaveCanceledTheGame_S(const string& roomUUID)
{//===============================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_HostingPublicRoomCanceled + roomUUID + ":" + BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::tellServerBobsGameIHaveStartedTheGame_S(const string& roomUUID)
{//===============================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_HostingPublicRoomStarted + roomUUID + ":" + BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::tellServerBobsGameTheGameHasEnded_S(const string& roomUUID, const string& results)
{//===============================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_HostingPublicRoomEnded + roomUUID + ":" + results + ":" + BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::sendBobsGameGameStats_S(const string& statsString)
{//===============================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_GameStats + statsString + ":" + BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameGameStatsResponse_S(string s)
{ //=========================================================================================================================
  //Bobs_Game_GameStats_Response:
	s = s.substr(s.find(":") + 1);

	ArrayList<string> responseStrings;
	while(s.find("`")!=string::npos)
	{
		s = s.substr(s.find("`") + 1);
		responseStrings.add(s.substr(0, s.find("`")));
		s = s.substr(s.find("`") + 1);
		s = s.substr(s.find(",") + 1);
	}

	setBobsGameGameStatsResponse_S(responseStrings);
	setGotBobsGameGameStatsResponse_S(true);
}

//===============================================================================================
void TCPServerConnection::sendBobsGameActivityStreamRequest_S()
{//===============================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Bobs_Game_ActivityStream_Request + BobNet::endline);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameActivityStreamResponse_S(string s)
{ //=========================================================================================================================
  //Bobs_Game_ActivityStream_Response:
	s = s.substr(s.find(":") + 1);

	
	while(s.find("`")!=string::npos)
	{
		s = s.substr(s.find("`") + 1);
		BobsGame::activityStream.add(FileUtils::removeSwearWords(s.substr(0, s.find("`"))));
		s = s.substr(s.find("`") + 1);
		s = s.substr(s.find(",") + 1);
	}

	//setGotBobsGameActivityStreamResponse_S(true);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameActivityStreamUpdate_S(string s)
{ //=========================================================================================================================
  //Bobs_Game_ActivityStream_Update:
	s = s.substr(s.find(":") + 1);

	ArrayList<string> strings;
	while (s.find("`") != string::npos)
	{
		s = s.substr(s.find("`") + 1);
		strings.add(s.substr(0, s.find("`")));
		s = s.substr(s.find("`") + 1);
		s = s.substr(s.find(",") + 1);
	}
	for(int i=strings.size()-1;i<=0;i--)
	{
		string a = FileUtils::removeSwearWords(strings.get(i));
		BobsGame::activityStream.insert(0,a);

		if (Main::globalSettings->hideNotifications == false)
		{
			Main::rightConsole->add(a, 5000, BobColor::magenta);
		}
	}

}

//===============================================================================================
void TCPServerConnection::incomingBobsGameUserStatsForSpecificGameAndDifficulty(string &s)
{//===============================================================================================
	s = s.substr(s.find(":") + 1);

	shared_ptr<BobsGameUserStatsForSpecificGameAndDifficulty >gameStats = make_shared<BobsGameUserStatsForSpecificGameAndDifficulty>(s);
	for(int i=0;i<BobsGame::userStatsPerGameAndDifficulty.size();i++)
	{
		shared_ptr<BobsGameUserStatsForSpecificGameAndDifficulty >temp = BobsGame::userStatsPerGameAndDifficulty.get(i);
		if(
			temp->isGameTypeOrSequence == gameStats->isGameTypeOrSequence &&
			temp->gameTypeUUID == gameStats->gameTypeUUID &&
			temp->gameSequenceUUID == gameStats->gameSequenceUUID &&
			temp->difficultyName == gameStats->difficultyName &&
			temp->objectiveString == gameStats->objectiveString
			)
		{
			BobsGame::userStatsPerGameAndDifficulty.removeAt(i);
			BobsGame::userStatsPerGameAndDifficulty.insert(i, gameStats);
			delete temp;
			return;
		}
	}
	BobsGame::userStatsPerGameAndDifficulty.add(gameStats);
}
//===============================================================================================
void TCPServerConnection::addToLeaderboard(ArrayList<shared_ptr<BobsGameLeaderBoardAndHighScoreBoard>> &boardArray, shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >leaderBoard)
{//===============================================================================================

	for (int i = 0; i<boardArray.size(); i++)
	{
		shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >temp = boardArray.get(i);
		if (
			temp->isGameTypeOrSequence == leaderBoard->isGameTypeOrSequence &&
			temp->gameTypeUUID == leaderBoard->gameTypeUUID &&
			temp->gameSequenceUUID == leaderBoard->gameSequenceUUID &&
			temp->difficultyName == leaderBoard->difficultyName &&
			temp->objectiveString == leaderBoard->objectiveString
			)
		{
			boardArray.removeAt(i);
			boardArray.insert(i, leaderBoard);
			delete temp;
			return;
		}
	}
	boardArray.add(leaderBoard);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameLeaderBoardByTotalTimePlayed(string &s)
{//===============================================================================================
	s = s.substr(s.find(":") + 1);

	shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >leaderBoard = make_shared<BobsGameLeaderBoardAndHighScoreBoard>(s);

	addToLeaderboard(BobsGame::topPlayersByTotalTimePlayed, leaderBoard);

}

//===============================================================================================
void TCPServerConnection::incomingBobsGameLeaderBoardByTotalBlocksCleared(string &s)
{//===============================================================================================
	s = s.substr(s.find(":") + 1);

	shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >leaderBoard = make_shared<BobsGameLeaderBoardAndHighScoreBoard>(s);

	addToLeaderboard(BobsGame::topPlayersByTotalBlocksCleared, leaderBoard);

}

//===============================================================================================
void TCPServerConnection::incomingBobsGameLeaderBoardByPlaneswalkerPoints(string &s)
{//===============================================================================================
	s = s.substr(s.find(":") + 1);

	shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >leaderBoard = make_shared<BobsGameLeaderBoardAndHighScoreBoard>(s);

	addToLeaderboard(BobsGame::topPlayersByPlaneswalkerPoints, leaderBoard);
}

//===============================================================================================
void TCPServerConnection::incomingBobsGameLeaderBoardByEloScore(string &s)
{//===============================================================================================
	s = s.substr(s.find(":") + 1);

	shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >leaderBoard = make_shared<BobsGameLeaderBoardAndHighScoreBoard>(s);

	addToLeaderboard(BobsGame::topPlayersByEloScore, leaderBoard);

}

//===============================================================================================
void TCPServerConnection::incomingBobsGameHighScoreBoardsByTimeLasted(string &s)
{//===============================================================================================
	s = s.substr(s.find(":") + 1);

	shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >leaderBoard = make_shared<BobsGameLeaderBoardAndHighScoreBoard>(s);

	addToLeaderboard(BobsGame::topGamesByTimeLasted, leaderBoard);


}

//===============================================================================================
void TCPServerConnection::incomingBobsGameHighScoreBoardsByBlocksCleared(string &s)
{//===============================================================================================
	s = s.substr(s.find(":") + 1);


	shared_ptr<BobsGameLeaderBoardAndHighScoreBoard >leaderBoard = make_shared<BobsGameLeaderBoardAndHighScoreBoard>(s);

	addToLeaderboard(BobsGame::topGamesByBlocksCleared, leaderBoard);




}



//===============================================================================================
void TCPServerConnection::sendChatMessage(string s)
{ //==============================================================================================

	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Chat_Message +s+ BobNet::endline);

}
//===============================================================================================
void TCPServerConnection::incomingChatMessage(string s)
{ //==============================================================================================

	s = s.substr(s.find(":") + 1);

	s = s.substr(0, s.find(":END:"));

	if (Main::globalSettings->hideChat==false)
	{
		Main::rightConsole->add(FileUtils::removeSwearWords(s), BobColor::white);

		Main::rightConsole->pruneChats(10);
	}
}

//===============================================================================================
void TCPServerConnection::incomingOnlineFriendsListResponse(string s)
{ //==============================================================================================
  //OnlineFriendsListResponse:type:id,type:id
  //type can be fb, g+, t, zip
	s = s.substr(s.find(":") + 1);

	//parse each friend, do incomingFriendOnlineNotification for each
	while (s.length() > 0)
	{
		int type = -1;
		string typeString = s.substr(0, s.find(":"));
		s = s.substr(s.find(":") + 1);
		if (typeString == "facebook")
		{
			type = UDPPeerConnection::FACEBOOK_TYPE;
		}
		else
		if (typeString == "userName")
		{
			type = UDPPeerConnection::USERNAME_TYPE;
		}
		else
		{
			type = UDPPeerConnection::ANON_TYPE;
		}

		int friendUserID = -1;
		try
		{
			friendUserID = stoi(s.substr(0, s.find(",")));
		}
		catch (exception)
		{
			log.error("Could not parse friendUserID in incomingOnlineFriendsListResponse");
		}

		if (friendUserID == -1)
		{
			log.error("Could not parse friendUserID in incomingFriendOnlineNotification");
			return;
		}

		s = s.substr(s.find(",") + 1);

		BobNet::addFriendID(friendUserID, type);
	}
}

void TCPServerConnection::incomingFriendOnlineNotification(string s)
{ //=========================================================================================================================

  //FriendOnlineNotification:type:id

  //make a new friendConnection and add it to the friend manager
  //check existing friends to see if userID already exists

	s = s.substr(s.find(":") + 1);
	int type = -1;
	string typeString = s.substr(0, s.find(":"));
	s = s.substr(s.find(":") + 1);
	if (typeString == "facebook")
	{
		type = UDPPeerConnection::FACEBOOK_TYPE;
	}
	else
	if (typeString == "userName")
	{
		type = UDPPeerConnection::USERNAME_TYPE;
	}
	else
	{
		type = UDPPeerConnection::ANON_TYPE;
	}

	int friendUserID = -1;
	try
	{
		//friendUserID = stoi(s.substr(0, s.find(",")));
		friendUserID = stoi(s);
	}
	catch (exception)
	{
		log.error("Could not parse friendUserID in incomingFriendOnlineNotification");
	}

	if(friendUserID == -1)
	{
		log.error("Could not parse friendUserID in incomingFriendOnlineNotification");
		return;
	}

	BobNet::addFriendID(friendUserID, type);
}



void TCPServerConnection::setAddFriendByUserNameResponse(string b)
{
	addFriendByUserNameResponse = b;
}


string& TCPServerConnection::getAddFriendByUserNameResponse()
{
	return addFriendByUserNameResponse;
}

void TCPServerConnection::sendAddFriendByUserNameRequest_S(string friendUserName)
{ //=========================================================================================================================
	connectAndAuthorizeAndQueueWriteToChannel_S(BobNet::Add_Friend_By_UserName_Request + "`" + friendUserName + "`" + BobNet::endline);
}

void TCPServerConnection::incomingAddFriendByUserNameResponse(string s)
{ //=========================================================================================================================

	s = s.substr(s.find(":") + 1); //Success

	if (String::startsWith(s, "Success") == true)
	{
		setAddFriendByUserNameResponse("Success");
	}
	else
	{
		setAddFriendByUserNameResponse("Failed");
	}
}



bool TCPServerConnection::_doLoginNoCaptions(string &userNameOrEmail, string &password, bool stayLoggedIn)
{ //=========================================================================================================================


//
//	if (statusLabel != nullptr) statusLabel->setText(" ");
//	if (errorLabel != nullptr)errorLabel->setText(" ");
//
//	if (userNameOrEmail.find("`") != string::npos)
//	{
//		if (errorLabel != nullptr)errorLabel->setText("Username/Email must not contain `");
//		return false;
//	}
//	if (userNameOrEmail.find(",") != string::npos)
//	{
//		if (errorLabel != nullptr)errorLabel->setText("Username/Email must not contain ,");
//		return false;
//	}
//	if (userNameOrEmail.length() == 0)
//	{
//		if (errorLabel != nullptr)errorLabel->setText("Enter your username or email address.");
//		return false;
//	}
//	if (password.find("`") != string::npos)
//	{
//		if (errorLabel != nullptr)errorLabel->setText("Password must not contain `");
//		return false;
//	}
//
//	if (password.length() == 0)
//	{
//		if (errorLabel != nullptr)errorLabel->setText("Please enter a password.");
//		return false;
//	}

	setLastUserNameOrEmail_S(userNameOrEmail);
	setLastPassword_S(password);

	//if email address blank, say "please type email address"
	//if password is blank, say "must type password"
	//check if email address is valid
	//may not contain `

	//say "trying to connect to server"

	//if (statusLabel != nullptr)statusLabel->setText("Connecting to server...");
	//if (errorLabel != nullptr)errorLabel->setText(" ");
	//Main::delay(20);

	bool connected = getConnectedToServer_S();

	if (connected == false)
	{
		ensureConnectedToServerThreadBlock_S();

		//-------------------------------
		//check to see if connected every 1 second
		//when connected, proceed.
		//-------------------------------
		int tries = 0;
		while (connected == false)
		{
			connected = getConnectedToServer_S();

			if (connected == false)
			{
				tries++;

				//make dots cycle
//				string dots = "";
//				for (int i = 0; i < tries % 4; i++)
//				{
//					dots += ".";
//				}
				//if (statusLabel != nullptr)statusLabel->setText("Connecting to server" + dots);
				//if (errorLabel != nullptr)errorLabel->setText(" ");

				if (tries > 10)
				{
					tries = 0;
					//if (statusLabel != nullptr)statusLabel->setText(" ");
					//if (errorLabel != nullptr)errorLabel->setText("Could not connect to server.");
					return false;
				}
				this_thread::sleep_for(chrono::milliseconds(500));
				//Main::delay(500);

			}
		}
	}

	if (connected == true)
	{
		//if (statusLabel != nullptr) statusLabel->setText("Connected! Checking login...");
		//if (errorLabel != nullptr)errorLabel->setText(" ");
		//Main::delay(20);
		this_thread::sleep_for(chrono::milliseconds(20));
	}

	sendLoginRequest(userNameOrEmail, password, false);// sendStatsToggleButton->isActive());

	//-------------------------------
	//check to see if password response every 1 second
	//-------------------------------

	int passwordTries = 0;
	bool gotResponse = false;
	while (gotResponse == false)
	{
		//_checkForIncomingTraffic();
		gotResponse = getGotLoginResponse_S();

		if (gotResponse == false)
		{
			passwordTries++;
			if (passwordTries > 10)
			{
				passwordTries = 0;
				//if (statusLabel != nullptr) statusLabel->setText(" ");
				//if (errorLabel != nullptr)errorLabel->setText("Timed out validating password. Please try again.");
				return false;
			}

			this_thread::sleep_for(chrono::milliseconds(100));
			//Main::delay(1000);

		}
	}

	//we have the response, now lets see if it was a valid login (two steps)
	bool passValid = getWasLoginResponseValid_S();

	//reset the response state in case we need to try again
	setGotLoginResponse_S(false);

	if (passValid)
	{
		//if (statusLabel != nullptr) statusLabel->setText("Login successful!");
		//if (errorLabel != nullptr)errorLabel->setText(" ");
		//Main::delay(1000);

		//-------------------------------------------------------

		//if (stayLoggedIn)
		{

			//FileUtils::writeSessionTokenToCache(getUserID_S(), getSessionToken_S(), stayLoggedIn);
			//log.debug("Wrote session token");
			//String temp = Main.cacheManager.readSessionTokenFromCache();
			//log.debug("Read session: "+temp);
			//Main.cacheManager.deleteSessionTokenFromCache();
			//temp = Main.cacheManager.readSessionTokenFromCache();
			//log.debug("Deleted session: "+temp);

		}

		//-------------------------------------------------------

		//setEnabled(false);
		//setActivated(false);

		return true;
	}
	else
	{

		//delete session cookie if there is one
		//FileUtils::deleteSessionTokenFromCache();
		//log.debug("Deleted session token if existed");

		//say "password wrong or account doesn't exist"
		//if (statusLabel != nullptr) statusLabel->setText("");// If you just signed up, please check your email and click the verification link.");
		//if (errorLabel != nullptr) errorLabel->setText("Password incorrect, account doesn't exist, or account isn't verified yet.");

		return false;
	}

}

bool TCPServerConnection::doLogin(shared_ptr<Caption >statusLabel, shared_ptr<Caption >errorLabel, string &userNameOrEmail, string &password, bool stayLoggedIn)
{ //=========================================================================================================================



	if (statusLabel != nullptr)statusLabel->setText(" ");
	if (errorLabel != nullptr)errorLabel->setText(" ");

	if (userNameOrEmail.find("`") != string::npos)
	{
		if (errorLabel != nullptr)errorLabel->setText("Username/Email must not contain `");
		return false;
	}

	if (userNameOrEmail.find(",") != string::npos)
	{
		if (errorLabel != nullptr)errorLabel->setText("Username/Email must not contain ,");
		return false;
	}

	if (userNameOrEmail.length() == 0)
	{
		if (errorLabel != nullptr)errorLabel->setText("Enter your username or email address.");
		return false;
	}

	if (password.find("`") != string::npos)
	{
		if (errorLabel != nullptr)errorLabel->setText("Password must not contain `");
		return false;
	}

	if (password.length() == 0)
	{
		if (errorLabel != nullptr)errorLabel->setText("Please enter a password.");
		return false;
	}

	setLastUserNameOrEmail_S(userNameOrEmail);
	setLastPassword_S(password);

	//if email address blank, say "please type email address"
	//if password is blank, say "must type password"
	//check if email address is valid
	//may not contain `

	//say "trying to connect to server"

	if (statusLabel != nullptr)statusLabel->setText("Connecting to server...");
	if (errorLabel != nullptr)errorLabel->setText(" ");
	Main::delay(20);

	bool connected = getConnectedToServer_S();

	if (connected == false)
	{
		ensureConnectedToServerThreadBlock_S();

		//-------------------------------
		//check to see if connected every 1 second
		//when connected, proceed.
		//-------------------------------
		int tries = 0;
		while (connected == false)
		{
			connected = getConnectedToServer_S();

			if (connected == false)
			{
				tries++;

				//make dots cycle
				string dots = "";
				for (int i = 0; i < tries % 4; i++)
				{
					dots += ".";
				}
				if (statusLabel != nullptr)statusLabel->setText("Connecting to server" + dots);
				if (errorLabel != nullptr)errorLabel->setText(" ");

				if (tries > 10)
				{
					tries = 0;
					if (statusLabel != nullptr)statusLabel->setText(" ");
					if (errorLabel != nullptr)errorLabel->setText("Could not connect to server.");
					return false;
				}

				Main::delay(500);

			}
		}
	}

	if (connected == true)
	{
		if (statusLabel != nullptr) statusLabel->setText("Connected! Checking login...");
		if (errorLabel != nullptr)errorLabel->setText(" ");
		Main::delay(20);
	}

	sendLoginRequest(userNameOrEmail, password, false);// sendStatsToggleButton->isActive());

																			  //-------------------------------
																			  //check to see if password response every 1 second
																			  //-------------------------------
	int passwordTries = 0;
	bool gotResponse = false;
	while (gotResponse == false)
	{
		//_checkForIncomingTraffic();
		gotResponse = getGotLoginResponse_S();

		if (gotResponse == false)
		{
			passwordTries++;
			if (passwordTries > 10)
			{
				passwordTries = 0;
				if (statusLabel != nullptr) statusLabel->setText(" ");
				if (errorLabel != nullptr)errorLabel->setText("Timed out validating password. Please try again.");
				return false;
			}

			Main::delay(1000);

		}
	}

	//we have the response, now lets see if it was a valid login (two steps)
	bool passValid = getWasLoginResponseValid_S();

	//reset the response state in case we need to try again
	setGotLoginResponse_S(false);

	if (passValid)
	{
		if (statusLabel != nullptr) statusLabel->setText("Login successful!");
		if (errorLabel != nullptr)errorLabel->setText(" ");
		Main::delay(1000);

		//-------------------------------------------------------

		if (stayLoggedIn)
		{

			FileUtils::writeSessionTokenToCache(getUserID_S(), getSessionToken_S(), stayLoggedIn);
			log.debug("Wrote session token");
			//String temp = Main.cacheManager.readSessionTokenFromCache();
			//log.debug("Read session: "+temp);
			//Main.cacheManager.deleteSessionTokenFromCache();
			//temp = Main.cacheManager.readSessionTokenFromCache();
			//log.debug("Deleted session: "+temp);

		}

		//-------------------------------------------------------

		//setEnabled(false);
		//setActivated(false);

		return true;
	}
	else
	{

		//delete session cookie if there is one
		//FileUtils::deleteSessionTokenFromCache();
		//log.debug("Deleted session token if existed");

		//say "password wrong or account doesn't exist"
		if (statusLabel != nullptr) statusLabel->setText("");// If you just signed up, please check your email and click the verification link.");
		if (errorLabel != nullptr) errorLabel->setText("Password incorrect, account doesn't exist, or account isn't verified yet.");

		return false;
	}

}

bool TCPServerConnection::doCreateAccount(shared_ptr<Caption >statusLabel, shared_ptr<Caption >errorLabel, string &userName, string &email, string &password, string &confirmPassword)
{ //=========================================================================================================================

	statusLabel->setText(" ");
	errorLabel->setText(" ");

	if (userName.find("`") != string::npos)
	{
		errorLabel->setText("Username must not contain `");
		return false;
	}

	if (userName.find("@") != string::npos)
	{
		errorLabel->setText("Username must not contain @");
		return false;
	}

	if (userName.find(",") != string::npos)
	{
		errorLabel->setText("Username must not contain ,");
		return false;
	}

	if (email.find("`") != string::npos)
	{
		errorLabel->setText("Email must not contain `");
		return false;
	}
	if (email.find(",") != string::npos)
	{
		errorLabel->setText("Email must not contain ,");
		return false;
	}
	if (userName.length() == 0)
	{
		errorLabel->setText("Enter a username.");
		return false;
	}

	if (email.length() == 0)
	{
		errorLabel->setText("Enter your email address.");
		return false;
	}

	if (email.find("@") == string::npos)
	{
		errorLabel->setText("Email address must contain @");
		return false;
	}

	if (password.find("`") != string::npos)
	{
		errorLabel->setText("Password must not contain `");
		return false;
	}

	if (password.length() == 0)
	{
		errorLabel->setText("Please enter a password.");
		return false;
	}
//	if (password != confirmPassword)
//	{
//		errorLabel->setText("Passwords don't match.  Please confirm password.");
//		return false;
//	}

	statusLabel->setText("Connecting to server...");
	errorLabel->setText(" ");
	Main::delay(20);

	ensureConnectedToServerThreadBlock_S();

	//-------------------------------
	//check to see if connected every 1 second
	//when connected, proceed.
	//-------------------------------
	int tries = 0;
	bool connected = false;
	while (connected == false)
	{
		connected = getConnectedToServer_S();

		if (connected == false)
		{
			tries++;

			//make dots cycle
			string dots = "";
			for (int i = 0; i < tries % 4; i++)
			{
				dots += ".";
			}
			statusLabel->setText("Connecting to server" + dots);
			errorLabel->setText(" ");

			if (tries > 10)
			{
				tries = 0;
				statusLabel->setText(" ");
				errorLabel->setText("Error: Could not connect to server.");

				return false;
			}

			Main::delay(1000);

		}
	}

	if (connected == true)
	{
		statusLabel->setText("Connected! Creating Account...");
		errorLabel->setText(" ");
		Main::delay(20);
	}

	sendCreateAccountRequest(userName, email, password);

	//-------------------------------
	//check to see if response every 1 second
	//-------------------------------
	int createAccountTries = 0;
	bool gotResponse = false;
	while (gotResponse == false)
	{
		//_checkForIncomingTraffic();
		string response = getGotCreateAccountResponse_S();

		if (response == "Success")
		{
			gotResponse = true;
			statusLabel->setText("Your account was created!");
			errorLabel->setText(" ");
			setGotCreateAccountResponse_S("");
			Main::delay(2000);
			return true;
		}
		if (response == "UserNameTaken")
		{
			gotResponse = true;
			statusLabel->setText(" ");
			errorLabel->setText("Username is taken, please try another username.");
			setGotCreateAccountResponse_S("");
			return false;
		}

		if (gotResponse == false)
		{
			createAccountTries++;
			if (createAccountTries > 10)
			{
				createAccountTries = 0;
				statusLabel->setText(" ");
				errorLabel->setText("Timed out creating account. The servers may be overloaded, please wait a few minutes and try again.");
				return false;
			}

			Main::delay(1000);

		}
	}
	return false;
}



bool TCPServerConnection::checkForSessionTokenAndLogInIfExists()
{ //=========================================================================================================================


  //check cache for session token
  //if session token cache exists, try to log in with that.
  //set browser cookie, refresh iframe

	//log.debug("Checking for Session Token");

	const string token = FileUtils::readSessionTokenFromCache();

	if (token != "")
	{
		threadLogDebug_S("Session Token Found");
	}
	else
	{
		//log.debug("Session Token not found.");
	}

	if (token != "")
	{

		//token = userId,`sessionToken`,statsAllowed
		string s = token;

		long long userID = -1;
		try
		{
			userID = stoll(s.substr(0, s.find(",")));
		}
		catch (exception)
		{
			threadLogError_S("Could not parse userID in checkForSessionTokenAndLogInIfExists");
		}

		s = s.substr(s.find("`") + 1); //sessionToken`,statsAllowed
		string sessionToken = s.substr(0, s.find("`"));
		s = s.substr(s.find(",") + 1); //statsAllowed
		bool statsAllowed = StringConverterHelper::fromString<bool>(s);

		//errorLabel->replaceText(" ");
		//statusLabel->replaceText("Existing session found! Connecting to server...");

		bool connected = getConnectedToServer_S();

		if (connected == false)
		{
			ensureConnectedToServerThreadBlock_S();

			//-------------------------------
			//check to see if connected every 1 second
			//when connected, proceed.
			//-------------------------------
			int tries = 0;
			while (connected == false)
			{
				connected = getConnectedToServer_S();

				if (connected == false)
				{
					tries++;

					//make dots cycle
					string dots = "";
					for (int i = 0; i < tries % 4; i++)
					{
						dots += ".";
					}

					//errorLabel->replaceText(" ");
					//statusLabel->replaceText("Existing session found! Connecting to server" + dots);

					if (tries > 20)
					{

						tries = 0;
						//statusLabel->replaceText(" ");
						//errorLabel->replaceText("Could not connect to server.");

						return false;
					}

					//Main::delay(1000);

				}
			}
		}

		if (connected == true)
		{
			//errorLabel->replaceText(" ");
			//statusLabel->replaceText("Connected! Checking session...");
		}

		sendReconnectRequest(userID, sessionToken, statsAllowed);

		//-------------------------------
		//check to see if password response every 1 second
		//-------------------------------
		int passwordTries = 0;
		bool gotResponse = false;
		while (gotResponse == false)
		{
			//_checkForIncomingTraffic();
			gotResponse = getGotReconnectResponse_S();

			if (gotResponse == false)
			{

				passwordTries++;

				if (passwordTries > 20)
				{
					passwordTries = 0;
					//statusLabel->replaceText(" ");
					//errorLabel->replaceText("Timed out validating session. Please try again.");
					return false;
				}

				//Main::delay(1000);

			}
		}

		//we have the response, now lets see if it was a valid login (two steps)
		bool passValid = getWasReconnectResponseValid_S();

		//reset the response state in case we need to try again
		setGotReconnectResponse_S(false);

		if (passValid)
		{
			//errorLabel->replaceText(" ");
			//statusLabel->replaceText("Login successful!");
			threadLogDebug_S("Session Token valid, logged in.");
			return true;

		}
		else
		{

			//delete session cookie if there is one
			FileUtils::deleteSessionTokenFromCache();

			//statusLabel->replaceText(" ");
			//errorLabel->replaceText("Session was not valid. Please log in.");
			threadLogDebug_S("Session Token invalid, deleted.");
			return false;
		}

	}
	return false;
}

bool TCPServerConnection::doForgotPassword(shared_ptr<Caption >statusLabel, shared_ptr<Caption >errorLabel, string &userNameOrEmail)
{ //=========================================================================================================================

  //send forgot password request to server, wait for response

	statusLabel->setText(" ");
	errorLabel->setText(" ");

	if (userNameOrEmail.find("`") != string::npos)
	{
		errorLabel->setText("Username or Email must not contain `");
		return false;
	}
	if (userNameOrEmail.find(",") != string::npos)
	{
		errorLabel->setText("Username or Email must not contain ,");
		return false;
	}
	if (userNameOrEmail.length() == 0)
	{
		errorLabel->setText("Enter your username or email address.");
		return false;
	}

	//say "trying to connect to server"
	statusLabel->setText("Connecting to server...");
	errorLabel->setText(" ");
	Main::delay(20);

	bool connected = getConnectedToServer_S();

	if (connected == false)
	{
		ensureConnectedToServerThreadBlock_S();

		//-------------------------------
		//check to see if connected every 1 second
		//when connected, proceed.
		//-------------------------------
		int tries = 0;
		while (connected == false)
		{
			connected = getConnectedToServer_S();

			if (connected == false)
			{
				tries++;

				//make dots cycle
				string dots = "";
				for (int i = 0; i < tries % 4; i++)
				{
					dots += ".";
				}
				statusLabel->setText("Connecting to server" + dots);
				errorLabel->setText(" ");

				if (tries > 10)
				{
					tries = 0;
					statusLabel->setText("Could not connect to server. Is your internet working?");
					errorLabel->setText(" ");

					return false;
				}

				Main::delay(1000);
			}
		}
	}

	if (connected == true)
	{
		statusLabel->setText("Connected! Sending account recovery request...");
		errorLabel->setText(" ");
		Main::delay(20);
	}

	sendPasswordRecoveryRequest(userNameOrEmail);

	//-------------------------------
	//check to see if password response every 1 second
	//-------------------------------
	int passwordTries = 0;
	bool gotResponse = false;
	while (gotResponse == false)
	{
		//_checkForIncomingTraffic();
		gotResponse = getGotPasswordRecoveryResponse_S();

		if (gotResponse == false)
		{
			passwordTries++;
			if (passwordTries > 10)
			{
				passwordTries = 0;
				statusLabel->setText(" ");
				errorLabel->setText("Timed out sending request. Please try again.");
				return false;
			}

			Main::delay(1000);
		}
	}

	//reset the response state in case we need to try again
	setGotPasswordRecoveryResponse_S(false);

	//say "an email was sent if the account was registered"
	statusLabel->setText("If the account exists, a recovery email was sent. Please check your email.");
	errorLabel->setText(" ");

	return true;

}


bool TCPServerConnection::linkFacebookAccount(shared_ptr<Caption >statusLabel, shared_ptr<Caption >errorLabel)
{ //=========================================================================================================================

	errorLabel->setText(" ");
	statusLabel->setText(" ");

	//do we have a sessionToken already?

	string facebookAccessToken = getGameSave_S().facebookAccessToken;
	//bool haveAccessToken = false;
	//bool accessTokenIsValid = true;

	if (facebookAccessToken.length() > 0)
	{
		//              haveAccessToken = true;
		//
		//
		//              //check if our session token is valid
		//              //restFB stuff here
		//              shared_ptr<FacebookClient> facebookClient = nullptr;
		//
		//              //------------------------
		//              //log into facebook to test token
		//              //------------------------
		//              try
		//              {
		//                 facebookClient = make_shared<DefaultFacebookClient>(facebookAccessToken);
		//                 shared_ptr<User> user = facebookClient->fetchObject("me", User::typeid);
		//
		//                 string facebookID = user->getId();
		//                 log.debug("Facebook ID: " + facebookID);
		//              }
		//              catch (exception& ex)
		//              {
		//                 accessTokenIsValid = false;
		//              }
	}

	//if we have facebook accessToken already, let's not bother the user



	//if (haveAccessToken == false || accessTokenIsValid == false)
	{
		statusLabel->setText("Please authorize Facebook in the browser window, then press Start or the Space key.");

		//open browser window, we can't get it with JS as a desktop client so we need to redirect to PHP or something which stores it in SQL
		string url = "https://bobsgame.com/facebook.php?u=" + to_string(getUserID_S());

		Main::openURL(url);

	}

	while (Main::getMain()->getControlsManager()->miniGame_CONFIRM_Pressed() == false)
	{
		Main::whilefix();
	}

	//if we are a desktop app, we should just tell the server to update from the database, since the authorization happened there.
	//we could keep asking the server if we have the token yet

	statusLabel->setText("Checking to see if account was authorized. Please wait for 10 seconds...");

	//DONE: send update facebook account command
	//UpdateFacebookAccountInDB

	//TODO: should do this on login if the session is still valid
	//do this on login on the server side and then send online friend list, don't even request from client.

	//refresh the session token and send to server
	sendUpdateFacebookAccountInDBRequest_S();

	//-------------------------------
	//check to see if response every 1 second
	//-------------------------------
	int responseTries = 0;
	bool gotResponse = false;
	while (gotResponse == false)
	{
		gotResponse = getFacebookAccountUpdateResponseReceived_S();

		if (gotResponse == false)
		{
			responseTries++;
			if (responseTries > 10)
			{
				responseTries = 0;
				statusLabel->setText(" ");
				errorLabel->setText("Did not receive a response from the server.");

				return false;
			}

			Main::delay(1000);
		}
	}

	bool wasValid = getFacebookAccountUpdateResponseWasValid_S();

	//reset the state
	setFacebookAccountUpdateResponseState_S(false, false);

	if (wasValid == false)
	{
		statusLabel->setText(" ");
		errorLabel->setText("Facebook session could not be validated. Please try again.");
		return false;
	}
	else
	{
		sendOnlineFriendListRequest_S();

		statusLabel->setText("Facebook connected!");
		errorLabel->setText(" ");
		return true;
	}
	return false;

}

//=========================================================================================================================
bool TCPServerConnection::doAddFriendByUsername(shared_ptr<Caption >statusLabel, shared_ptr<Caption >errorLabel, const string& friendUserName)
{//=========================================================================================================================

	statusLabel->setText(" ");
	errorLabel->setText(" ");

	if (friendUserName.find("`") != string::npos)
	{
		errorLabel->setText("Username must not contain `");
		return false;
	}

	if (friendUserName.length() == 0)
	{
		errorLabel->setText("Enter a username.");
		return false;
	}

	if (friendUserName.find("@") != string::npos)
	{
		errorLabel->setText("Username must not contain @");
		return false;
	}
	if (friendUserName.find(",") != string::npos)
	{
		errorLabel->setText("Username must not contain ,");
		return false;
	}
	//say "trying to connect to server"
	statusLabel->setText("Connecting to server...");
	errorLabel->setText(" ");
	Main::delay(20);

	ensureConnectedToServerThreadBlock_S();

	//-------------------------------
	//check to see if connected every 1 second
	//when connected, proceed.
	//-------------------------------
	int tries = 0;
	bool connected = false;
	while (connected == false)
	{
		connected = getConnectedToServer_S();

		if (connected == false)
		{
			tries++;

			//make dots cycle
			string dots = "";
			for (int i = 0; i < tries % 4; i++)
			{
				dots += ".";
			}

			errorLabel->setText(" ");
			statusLabel->setText("Connecting to server" + dots);

			if (tries > 10)
			{
				tries = 0;
				statusLabel->setText(" ");
				errorLabel->setText("Error: Could not connect to server.");

				return false;
			}

			Main::delay(1000);
		}
	}

	if (connected == true)
	{
		statusLabel->setText("Connected! Searching for friend...");
		errorLabel->setText(" ");

	}

	sendAddFriendByUserNameRequest_S(friendUserName);

	//-------------------------------
	//check to see if response every 1 second
	//-------------------------------
	int createAccountTries = 0;
	bool gotResponse = false;
	while (gotResponse == false)
	{
		//_checkForIncomingTraffic();
		string response = getAddFriendByUserNameResponse();

		if (response == "Success")
		{
			setAddFriendByUserNameResponse("");
			gotResponse = true;

			statusLabel->setText("Friend was added!");
			errorLabel->setText(" ");

			return true;
		}
		if (response == "Failed")
		{
			setAddFriendByUserNameResponse("");
			gotResponse = true;
			statusLabel->setText(" ");
			errorLabel->setText("Could not add friend. Username could not be found.");

			return false;
		}

		if (gotResponse == false)
		{
			createAccountTries++;
			if (createAccountTries > 10)
			{
				createAccountTries = 0;
				statusLabel->setText(" ");
				errorLabel->setText("Timed out adding friend. The servers may be overloaded, please try again.");

				return false;
			}

			Main::delay(1000);
		}
	}
	return false;

}

