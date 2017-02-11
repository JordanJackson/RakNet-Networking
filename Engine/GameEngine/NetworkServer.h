#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_
#pragma once

#include "ISystem.h"
#include "NetworkCommon.h"

class NetworkServer : public ISystem
{
public:
	enum NetworkServerState
	{
		DISABLED,
		CREATE_SERVER,
		RUNNING
	};

	inline static NetworkServer& Instance()
	{
		static NetworkServer instance;
		return instance;
	}
	
	bool isServer() { return isserver; }

	void sendPacket(RakNet::BitStream& bs);

protected: 
	void initialize();
	void update(float deltaTime);

private:
	int port;
	bool isserver;
	NetworkServerState state;

	RakNet::RakPeerInterface* rakInterface;
	std::vector<RakNet::RakNetGUID> clientConnections;

private:
	void loadSettings();
	void setupServer();
	void serverUpdate();

private:
	NetworkServer() = default;
	~NetworkServer() = default;
	NetworkServer(NetworkServer const&) = delete;
	friend class GameEngine;
};


#endif

