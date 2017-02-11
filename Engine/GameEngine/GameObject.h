#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#pragma once

#include "Object.h"

class Component;
class Transform;
class GameObjectManager;

class GameObject final : public Object
{
	friend class GameObjectManager;

	DECLARE_DYNAMIC_DERIVED_CLASS(GameObject, Object)

private:
	STRCODE fileID;
	STRCODE UID;
	std::string name;
	bool saveAble;

	std::multimap<STRCODE, Component*> components;

	// We didn't make it and its safe in the component array
	Transform* transform;

public:
	GameObject* parent;
	std::list<GameObject*> children;

public:
	~GameObject();
	
	STRCODE getFileID() { return fileID; }
	void setFileID(STRCODE _fileID) { fileID = _fileID; }

	void setUID(STRCODE uid) { UID = uid; }
	STRCODE getUID() { return UID; }

	const std::string& getName() { return name; }

	void setTransform(Transform* trans) { transform = trans; }
	Transform* getTransform() { return transform; }

	void AddComponent(Component* component);
	void RemoveComponent(STRCODE component);

	//collisions:
	void OnCollisionEnter(GameObject* collision);

	//Name of this has to be changed to "GetComponentByTypeName", being used in RenderSystem at the moment, must speak with a member
	Component* GameObject::GetComponentByType(std::string className);
	
	std::multimap<STRCODE, Component*>& getComponents() { return components; }
	std::list<GameObject*>& getChildren() { return children; }

	Component* GetComponentByUUID(STRCODE uuid);
	Component* GetComponent(std::string& comp);

	virtual void update(float deltaTime);

	void load(XMLElement* node);
	void save(XMLElement* node);

	bool isSaveAble();
	void AddChild(GameObject*);
	void RemoveChild(GameObject*);
	GameObject* getChildByName(std::string name);

};

#endif

