#include "Core.h"
#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"

IMPLEMENT_DYNAMIC_CLASS(GameObject)

GameObject::~GameObject()
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		delete (*it).second;
	}
}

void GameObject::AddComponent(Component* component)
{
	STRCODE _componentHashCode = component->getDerivedClassHashCode();
	std::multimap<STRCODE, Component*>::iterator it = components.find(_componentHashCode);
	if (it != components.end())
	{
		delete (*it).second;
	}

	components.insert(std::pair<STRCODE, Component*>(component->getDerivedClassHashCode(), component));
}

void GameObject::RemoveComponent(STRCODE component)
{
	std::multimap<STRCODE, Component*>::iterator it = components.find(component);
	if (it != components.end())
	{
		delete (*it).second;
		components.erase(component);
	}
}

Component* GameObject::GetComponentByUUID(STRCODE uuid)
{
	std::multimap<STRCODE, Component*>::iterator it = components.find(uuid);

	if (it != components.end())
	{
		return (*it).second;
	}

	return nullptr;
}

Component* GameObject::GetComponent(std::string& comp)
{
	// Returns the first component found of given type
	for (std::multimap<STRCODE, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if (it->second->getClassName() == comp)
		{
			return (*it).second;
		}
	}
	return nullptr;
}

Component* GameObject::GetComponentByType(std::string className)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		if (it->second->getDerivedClassName() == className)
		{
			return it->second;
		}
	}

	return nullptr;
}

void GameObject::update(float deltaTime)
{
	for (auto component : components)
	{
		for (auto component : components)
		{
			component.second->update(deltaTime);
		}
	}
}

void GameObject::load(XMLElement* node)
{
	if (node != NULL)
	{
		UID = getHashCode(node->Attribute("uuid"));
		name = node->Attribute("name");

		XMLElement* compsElement = node->FirstChildElement("Components");
		if (compsElement != NULL)
		{
			XMLElement* cElement = compsElement->FirstChildElement("Component");
			while (cElement != NULL)
			{
				const char* className = cElement->Attribute("class");
				THROW_RUNTIME_ERROR(className == nullptr, "Component class name missing");

				Component* component((Component*)CreateObject(className));
				component->setGameObject(this);
				component->load(cElement);
				component->initialize();
				AddComponent(component);
			
				cElement = cElement->NextSiblingElement("Component");
			}
		}
	}
}

void GameObject::save(XMLElement* node)
{
	tinyxml2::XMLElement* compsElement = node->GetDocument()->NewElement("Components");
	node->GetDocument()->InsertFirstChild(compsElement);;
	for (std::multimap<STRCODE, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		tinyxml2::XMLElement* cElement = node->GetDocument()->NewElement("Component");
		cElement->SetAttribute("class", (*it).second->getDerivedClassName().c_str());
		(*it).second->save(cElement);
		compsElement->InsertEndChild(cElement);
	}
	tinyxml2::XMLElement* childElement = node->GetDocument()->NewElement("Children");
	node->GetDocument()->InsertFirstChild(compsElement);
	for (std::list<GameObject*>::iterator iter = children.begin(); iter != children.end();)
	{
		tinyxml2::XMLElement* childGObjElement = node->GetDocument()->NewElement("GameObject");
		childGObjElement->SetAttribute("class", (*iter)->getDerivedClassName().c_str());
		(*iter)->save(childGObjElement);
		childElement->InsertEndChild(childGObjElement);
	}
}

bool GameObject::isSaveAble()
{
	return saveAble;
}

void GameObject::AddChild(GameObject* gameObject)
{
	children.push_back(gameObject);
}

void GameObject::RemoveChild(GameObject* gameObject)
{
	for (std::list<GameObject*>::iterator iter = children.begin(); iter != children.end();)
	{
		if ((*iter) == gameObject)
		{
			children.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
}

GameObject* GameObject::getChildByName(std::string name)
{
	for (std::list<GameObject*>::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		if ((*iter)->getName() == name)
		{
			return (*iter);
		}
	}
	// if one is not found, return an empty game object...
	return nullptr;
}

void GameObject::OnCollisionEnter(GameObject* collision)
{
}

