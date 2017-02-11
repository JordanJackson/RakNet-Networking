#include "Core.h"
#include "GameObjectManager.h"
#include "NetworkServer.h"

#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"
#include "Transform.h"

void GameObjectManager::initialize()
{
	ISystem::initialize();

	REGISTER_ABSTRACT_CLASS(Component);
	REGISTER_DYNAMIC_CLASS(Sprite);
	REGISTER_DYNAMIC_CLASS(Transform);
	REGISTER_DYNAMIC_CLASS(GameObject);
}

void GameObjectManager::AddRootGameObject(GameObject* gameObject)
{
	rootGameObjects.push_back(gameObject);
}

void GameObjectManager::AddChildGameobject(GameObject* parentObject, GameObject* gameObject)
{
	parentObject->getChildren().push_back(gameObject);
}

void GameObjectManager::RemoveGameObject(STRCODE id)
{
	std::list<GameObject*>::iterator iter = rootGameObjects.begin();
	while (iter != rootGameObjects.end())
	{
		if ((*iter)->getUID() == id)
		{
			ReleaseGameObject((*iter));
			delete (*iter);
			rootGameObjects.erase(iter);
			break;
		}
		else
		{
			RemoveGameObject(id, (*iter));
		}
		++iter;
	}
}

void GameObjectManager::RemoveGameObject(STRCODE id, GameObject* parent)
{
	std::list<GameObject*>::iterator iter = parent->getChildren().begin();
	while (iter != parent->getChildren().end())
	{
		if ((*iter)->getUID() == id)
		{
			std::cout << "Remove Game Object: " << id << std::endl;
			ReleaseGameObject((*iter));
			delete (*iter);
			parent->getChildren().erase(iter);
			break;
		}
		else
		{
			RemoveGameObject(id, (*iter));
		}
		++iter;
	}
}

void GameObjectManager::ReleaseGameObject(GameObject* go)
{
	std::list<GameObject*>::iterator iter = go->getChildren().begin();
	while (iter != go->getChildren().end())
	{
		ReleaseGameObject((*iter));
		delete (*iter);
		++iter;
	}

	delete go->parent;
}

GameObject* GameObjectManager::_FindGameObject(GameObject* parent, STRCODE id)
{
	std::list<GameObject*>::iterator iter = parent->getChildren().begin();
	while (iter != parent->getChildren().end())
	{
		if ((*iter)->getUID() == id)
		{
			return (*iter);
		}

		GameObject* wp = _FindGameObject((*iter), id);
		if (wp != nullptr)
		{
			return wp;
		}

		++iter;
	}
	return nullptr;
}

GameObject* GameObjectManager::FindGameObject(STRCODE id)
{
	std::list<GameObject*>::iterator iter = rootGameObjects.begin();
	while (iter != rootGameObjects.end())
	{
		if ((*iter)->getUID() == id)
		{
			return (*iter);
		}

		GameObject* wp = _FindGameObject((*iter), id);
		if (wp != nullptr)
		{
			return wp;
		}

		++iter;
	}
	
	return nullptr;
}

GameObject* GameObjectManager::CreateGameObject(tinyxml2::XMLDocument* doc)
{
	XMLElement* gameObjElement = doc->FirstChildElement("GameObject");
	THROW_RUNTIME_ERROR(gameObjElement == nullptr, "Unable to Load Game Object Prefab");

	GameObject* gObj = new GameObject();

	gObj->setFileID(NoName);
	gObj->load(gameObjElement);

	UUID uid;
	CreateUUID(&uid);
	gObj->setUID(GUIDToSTRCODE(uid));

	rootGameObjects.push_back(gObj);

	return gObj;
}

void GameObjectManager::load(XMLElement* node, STRCODE idOfFile)
{
	XMLElement* gameObjElement = node->FirstChildElement("GameObject");
	while (gameObjElement != NULL)
	{
		GameObject* gObj = new GameObject();
		gObj->setFileID(idOfFile);
		gObj->load(gameObjElement);

		rootGameObjects.push_back(gObj);

		gameObjElement = gameObjElement->NextSiblingElement("GameObject");
	}
}

void GameObjectManager::unload(STRCODE idOfFile)
{
}

void GameObjectManager::save(tinyxml2::XMLElement* node)
{
}

const std::list<GameObject*>& GameObjectManager::GetAllRootGameObjects()
{
	return rootGameObjects;
}

void GameObjectManager::DestroyGameObject(GameObject* go)
{
	destroyGameObjects.push_back(go);

	if (NetworkServer::Instance().isServer())
	{
		RakNet::BitStream bs;
		bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT);
		bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT_DESTROY);
		bs.Write(go->getUID());
		NetworkServer::Instance().sendPacket(bs);
	}
}

void GameObjectManager::_update(float deltaTime, GameObject* gameObject)
{
	gameObject->update(deltaTime);

	std::list<GameObject*>::iterator iter = gameObject->getChildren().begin();
	while (iter != gameObject->getChildren().end())
	{
		_update(deltaTime, (*iter));
		++iter;
	}
}

void GameObjectManager::update(float deltaTime)
{
	std::list<GameObject*>::iterator iter = rootGameObjects.begin();
	while (iter != rootGameObjects.end())
	{
		_update(deltaTime, (*iter));
		iter++;
	}

	std::list<GameObject*>::iterator destIt = destroyGameObjects.begin();
	while (destIt != destroyGameObjects.end())
	{
		RemoveGameObject((*destIt)->getUID());
		++destIt;
	}
	destroyGameObjects.clear();
}

// Networking
void GameObjectManager::processPacket(RakNet::BitStream& bitStream)
{
	unsigned char packetId;
	bitStream.Read(packetId);

	switch (packetId)
	{
		case NetworkPacketIds::MSG_GAMEOBJECT_DESTROY:
			{
				STRCODE goID;
				bitStream.Read(goID);
				GameObject* gameObject = FindGameObject(goID);
				if (gameObject != nullptr)
				{
					DestroyGameObject(gameObject);
					break;
				}
			}
			break;

		case NetworkPacketIds::MSG_GAMEOBJECT_COMPONENT:
			{
				STRCODE goID;
				bitStream.Read(goID);

				GameObject* gameObject = FindGameObject(goID);
				if (gameObject != nullptr)
				{
					STRCODE componentID;
					bitStream.Read(componentID);
					Component* component = gameObject->GetComponentByUUID(componentID);
					if (component != nullptr)
					{
						component->processPacket(bitStream);
					}
				}
			}
			break;
	}
}
