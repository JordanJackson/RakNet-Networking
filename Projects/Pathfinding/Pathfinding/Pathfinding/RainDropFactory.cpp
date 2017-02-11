#include "GameCore.h"
#include "RainDropFactory.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "PrefabAsset.h"

IMPLEMENT_DYNAMIC_CLASS(RainDropFactory)

void RainDropFactory::load(XMLElement* element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	prefabID = getHashCode(id);

	tinyxml2::XMLElement* spawnDelayElement = element->FirstChildElement("SpawnDelay");
	THROW_RUNTIME_ERROR(spawnDelayElement == nullptr, "No SpawnDelay element");
	spawnDelay = spawnDelayElement->FloatAttribute("value");
}

void RainDropFactory::update(float deltaTime)
{
	Component::update(deltaTime);

	// Disable the Component when we are a client
	if (NetworkClient::Instance().isClient())
	{
		enabled = false;
	}

	if (InputManager::Instance().keyReleased(sf::Keyboard::Return))
	{
		enabled = !enabled;
		currentSpawnDelay = 0.0f;
	}

	if (enabled)
	{
		currentSpawnDelay += deltaTime;
		if (currentSpawnDelay > spawnDelay)
		{
			currentSpawnDelay = 0.0f;
			Asset* asset = AssetManager::Instance().getAsset(prefabID);
			if (asset != nullptr)
			{
				PrefabAsset* prefab = (PrefabAsset*)asset;
				GameObject* go = prefab->CreatePrefab();

				// Call write packet
				writePacket(go);
			}
		}
	}
}

// Implement a writePacket function that will tell clients to spawn a raindrop....
// Remember we need to make sure we setup the packetIDS properly
void RainDropFactory::writePacket(GameObject* go)
{
	RakNet::BitStream bs;
	bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT);
	bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT_COMPONENT);
	bs.Write(gameObject->getUID());
	bs.Write(RainDropFactory::getClassHashCode());
	bs.Write((unsigned char)RainDropFactory::NetworkPackets::SPAWN);
	bs.Write(go->getUID());

	NetworkServer::Instance().sendPacket(bs);
}

// Implement the process packet for what you wrote from the server
// Override the processPacket function
void RainDropFactory::processPacket(RakNet::BitStream& bitStream)
{
	unsigned char packetID;
	bitStream.Read(packetID);

	switch (packetID)
	{
	case RainDropFactory::NetworkPackets::SPAWN:
	{
		Asset* asset = AssetManager::Instance().getAsset(prefabID);
		if (asset != nullptr)
		{
			STRCODE uuid;
			PrefabAsset* prefab = (PrefabAsset*)asset;
			GameObject* go = prefab->CreatePrefab();

			// read packet
			bitStream.Read(uuid);
			go->setUID(uuid);
		}
	}
	break;
	}
}