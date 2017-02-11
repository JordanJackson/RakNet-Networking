#include "Core.h"
#include "NetworkServer.h"
#include "GameObjectManager.h"

void NetworkServer::initialize()
{
	ISystem::initialize();

	if (isserver == true)
	{
		loadSettings();
		rakInterface = RakNet::RakPeerInterface::GetInstance();
		state = NetworkServer::CREATE_SERVER;
	}
	else
	{
		state = NetworkServer::DISABLED;
	}
}

void NetworkServer::loadSettings()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Resources/NetworkSettings.xml");

	if (doc.ErrorID() == XML_SUCCESS)
	{
		XMLElement* portElement = doc.FirstChildElement("port");
		THROW_RUNTIME_ERROR(portElement == nullptr, "Unable to determine port");
		portElement->QueryIntText(&port);
	}
}

void NetworkServer::update(float deltaTime)
{
	switch (state)
	{
	case NetworkServer::DISABLED:
		break;

	case NetworkServer::CREATE_SERVER:
		setupServer();
		break;

	case NetworkServer::RUNNING:
		serverUpdate();
		break;

	default:
		break;
	}
}

void NetworkServer::sendPacket(RakNet::BitStream& bs)
{
	for (int i = 0; i < clientConnections.size(); i++)
	{
		rakInterface->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, clientConnections[i], false);
	}
}

void NetworkServer::setupServer()
{
	RakNet::SocketDescriptor sd(port, NULL);
	if (rakInterface->Startup(8, &sd, 1) != RakNet::RAKNET_STARTED)
	{
		std::cerr << "*** Failed to start server on port " << port << std::endl;
		exit(1);
	}

	// set maximum number of incoming connections
	rakInterface->SetMaximumIncomingConnections(8);

	std::cout << "Listening on port " << port << std::endl;
	std::cout << "IP addresses:" << std::endl;
	for (unsigned i = 0; i < rakInterface->GetNumberOfAddresses(); i++)
	{
		std::cout << rakInterface->GetLocalIP(i) << std::endl;
	}

	state = NetworkServer::RUNNING;
}

void NetworkServer::serverUpdate()
{
	// get a packet
	RakNet::Packet* packet = rakInterface->Receive();

	while (packet != NULL) 
	{
		RakNet::BitStream bs(packet->data, packet->length, false);

		unsigned char packetId;
		bs.Read(packetId);

		switch (packetId) {

		case ID_NEW_INCOMING_CONNECTION:
			// Somebody connected
			std::cout << "Got connection from " << packet->systemAddress.ToString(true) << std::endl;
			clientConnections.push_back(packet->guid);
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			std::cout << "Disconnected from " << packet->systemAddress.ToString(true) << std::endl;
			{
				std::vector<RakNet::RakNetGUID>::iterator position = std::find(clientConnections.begin(), clientConnections.end(), packet->guid);
				if (position != clientConnections.end())
				{
					clientConnections.erase(position);
				}
			}
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally terminated
			std::cout << "Connection lost to " << packet->systemAddress.ToString(true) << std::endl;
			break;

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			std::cerr << "Incomplatible protocol version from " << packet->systemAddress.ToString(true) << std::endl;
			break;

		case MSG_GAMEOBJECT:
			GameObjectManager::Instance().processPacket(bs);
			break;

		default:
			std::cout << "Oops, received an unhandled packet with id " << (unsigned)packetId << std::endl;
			break;
		}

		rakInterface->DeallocatePacket(packet);

		// get next packet
		packet = rakInterface->Receive();
	}
}