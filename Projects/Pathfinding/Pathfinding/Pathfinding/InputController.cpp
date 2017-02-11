#include "GameCore.h"
#include "InputController.h"

IMPLEMENT_DYNAMIC_CLASS(InputController)

void InputController::load(XMLElement* element)
{
	Component::load(element);
}

void InputController::update(float deltaTime)
{
	Component::update(deltaTime);

	// if server disable click input
	if (NetworkServer::Instance().isServer())
	{
		enabled = false;
	}

	if (enabled)
	{
		if (InputManager::Instance().mousePressed(sf::Mouse::Button::Left))
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition();
			writePacket(mousePos);
		}
	}
}

void InputController::writePacket(sf::Vector2i position)
{
	RakNet::BitStream bs;
	bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT);
	bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT_COMPONENT);
	bs.Write(gameObject->getUID());
	bs.Write(InputController::getClassHashCode());
	bs.Write((unsigned char)InputController::NetworkPackets::MOUSE_CLICK);
	bs.Write(position.x);
	bs.Write(position.y);

	NetworkClient::Instance().sendPacket(bs);
}

void InputController::processPacket(RakNet::BitStream& bitStream)
{
	unsigned char packetID;
	bitStream.Read(packetID);

	switch (packetID)
	{
	case InputController::NetworkPackets::MOUSE_CLICK:
	{
		int x, y;
		bitStream.Read(x);
		bitStream.Read(y);
		sf::Vector2i mousePos = sf::Vector2i(x, y);
		std::cout << "Mouse Click at: " << mousePos.x << ", " << mousePos.y << std::endl;
	}
	break;
	}
}