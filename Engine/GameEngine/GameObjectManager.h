#ifndef _GAMEOBJECTMANAGER_H_
#define _GAMEOBJECTMANAGER_H_
#pragma once

#include "ISystem.h"

class GameObject;
class Component;

class GameObjectManager : public ISystem
{
private:
	// list of Parent Game Objects
	std::list<GameObject*> rootGameObjects;

	std::list<GameObject*> destroyGameObjects;

public:

	inline static GameObjectManager& Instance()
	{
		static GameObjectManager instance;
		return instance;
	}


	void AddRootGameObject(GameObject* gameObject);
	void AddChildGameobject(GameObject* parentObject, GameObject* gameObject);


	GameObject* FindGameObject(STRCODE id);

	const std::list<GameObject*>& GetAllRootGameObjects();

	GameObject* CreateGameObject(tinyxml2::XMLDocument* doc);
	void DestroyGameObject(GameObject* go);

	void save(tinyxml2::XMLDocument* node);
	void save(tinyxml2::XMLElement* node);
	void load(XMLElement* node, STRCODE);
	void unload(STRCODE idOfFile);

	// Networking
	void processPacket(RakNet::BitStream& bitStream);

protected:
	void initialize();
	void update(float deltaTime);

private:
	void RemoveGameObject(STRCODE id);
	void RemoveGameObject(STRCODE id, GameObject* parent);
	void ReleaseGameObject(GameObject* go);

	void _update(float deltaTime, GameObject* gameObject);
	GameObject* _FindGameObject(GameObject* parent, STRCODE id);

private:
	GameObjectManager() = default;
	~GameObjectManager() = default;
	GameObjectManager(GameObjectManager const&) = delete;

	friend class GameEngine;
};

#endif

