#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_
#pragma once

#include "ISystem.h"
#include "NetworkCommon.h"

class NetworkClient : public ISystem
{
public:
	enum NetworkClientState
	{
		DISABLED,
		WAITING_TO_CONNECT,
		WAITING_FOR_FIRST_PACKET,
		CONNECTED,
		NETWORK_ERROR
	};

	inline static NetworkClient& Instance()
	{
		static NetworkClient instance;
		return instance;
	}

	bool isClient() { return isclient; }

	void sendPacket(RakNet::BitStream& bs);

protected: 
	void initialize();
	void update(float deltaTime);

private:
	bool isclient;
	int port;
	std::string ipAddress;
	NetworkClientState state;

	RakNet::RakPeerInterface* rakInterface;
	RakNet::RakNetGUID serverGUID;

private:
	void loadSettings();
	void waitingForFirstPacket();
	void clientUpdate();

private:
	NetworkClient() = default;
	~NetworkClient() = default;
	NetworkClient(NetworkClient const&) = delete;
	friend class GameEngine;
};


#endif

