#include "Core.h"
#include "NetworkClient.h"
#include "GameObjectManager.h"

void NetworkClient::initialize()
{
	ISystem::initialize();

	if (isclient == true)
	{
		loadSettings();
		state = NetworkClient::WAITING_TO_CONNECT;
		rakInterface = RakNet::RakPeerInterface::GetInstance();
		RakNet::SocketDescriptor sd(0, NULL);
		rakInterface->Startup(1, &sd, 1);
	}
	else
	{
		state = NetworkClient::DISABLED;
	}
}

void NetworkClient::loadSettings()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Resources/NetworkSettings.xml");

	if (doc.ErrorID() == XML_SUCCESS)
	{
		XMLElement* element = NULL;

		XMLElement* ipElement = doc.FirstChildElement("ipaddress");
		THROW_RUNTIME_ERROR(ipElement == nullptr, "Unable to determine ip address");
		ipAddress = ipElement->GetText();

		XMLElement* portElement = doc.FirstChildElement("port");
		THROW_RUNTIME_ERROR(portElement == nullptr, "Unable to determine port");
		portElement->QueryIntText(&port);
	}
}

void NetworkClient::update(float deltaTime)
{
	switch (state)
	{
	case NetworkClient::DISABLED:
		break;

	case NetworkClient::WAITING_TO_CONNECT:
		if (rakInterface->Connect(ipAddress.c_str(), port, NULL, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
		{
			std::cerr << "*** Failed to connect to server. Going to try later....." << std::endl;
		}
		else
		{
			state = NetworkClient::WAITING_FOR_FIRST_PACKET;
		}
		break;

	case NetworkClient::WAITING_FOR_FIRST_PACKET:
		waitingForFirstPacket();
		break;

	case NetworkClient::CONNECTED:
		clientUpdate();
		break;

	default:
		break;
	}
}

void NetworkClient::sendPacket(RakNet::BitStream& bs)
{
	rakInterface->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, serverGUID, false);
}

void NetworkClient::waitingForFirstPacket()
{
	// wait for the first packet to arrive
	RakNet::Packet* packet = rakInterface->Receive();
	if (packet == nullptr)
		return;

	// get the packet type identifier
	unsigned char packetId = packet->data[0];

	switch (packetId) {
	case ID_CONNECTION_REQUEST_ACCEPTED:
		std::cout << "\nConnected to " << packet->systemAddress.ToString(true) << std::endl;
		state = NetworkClient::CONNECTED;
		break;

	case ID_CONNECTION_ATTEMPT_FAILED:
		std::cerr << "*** Connection attempt failed" << std::endl;
		state = NetworkClient::NETWORK_ERROR;
		isclient = false;
		break;

	case ID_NO_FREE_INCOMING_CONNECTIONS:
		std::cerr << "*** Server is full" << std::endl;
		state = NetworkClient::NETWORK_ERROR;
		isclient = false;
		break;

	case ID_INCOMPATIBLE_PROTOCOL_VERSION:
		std::cerr << "*** Incompatible protocol version" << std::endl;
		state = NetworkClient::NETWORK_ERROR;
		isclient = false;
		break;

	default:
		std::cerr << "*** Unknown connection response" << std::endl;
		state = NetworkClient::NETWORK_ERROR;
		isclient = false;
		break;
	}

	serverGUID = packet->guid;

	rakInterface->DeallocatePacket(packet);
}

void NetworkClient::clientUpdate()
{
	unsigned char packetId;

	RakNet::Packet* packet = rakInterface->Receive();
	
	while(packet != nullptr)
	{
		RakNet::BitStream bs(packet->data, packet->length, false);
		bs.Read(packetId);

		switch (packetId) 
		{
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
				state = NetworkClient::NETWORK_ERROR;
				std::cout << "Disconnected from server" << std::endl;
				break;

			case MSG_GAMEOBJECT:
				GameObjectManager::Instance().processPacket(bs);
				break;
		}

		rakInterface->DeallocatePacket(packet);
		packet = rakInterface->Receive();
	}
}
