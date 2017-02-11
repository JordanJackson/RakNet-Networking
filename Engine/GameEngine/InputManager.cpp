#include "Core.h"
#include "InputManager.h"
#include "SFML\Graphics.hpp"
#include "RenderSystem.h"

using namespace sf;

void InputManager::initialize()
{
	maxEvents = 50; // default in case file load fails

	loadSettings();

	events.reserve(50);
	for (int i = 0; i < 50; i++)
	{
		events.push_back(Event());
	}
}

void InputManager::loadSettings()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Resources/InputManagerSettings.xml");

	if (doc.ErrorID() == XML_SUCCESS)
	{
		XMLElement* element = NULL;

		element = doc.FirstChildElement("EventCount");
		if (element != NULL)
		{
			element->QueryIntText(&maxEvents);
		}
	}
}

void InputManager::update(float deltaTime)
{
	numEvents = 0;
	while (RenderSystem::Instance().GetRenderWindow()->pollEvent(events[numEvents]))
	{
		numEvents++;
	}
}

//returns true if a key was pressed this frame
bool InputManager::keyPressed(Keyboard::Key key)
{
	for (int i = 0; i < numEvents; i++)
	{
		if ((events[i].type == Event::KeyPressed) && (events[i].key.code == key))
		{
			return true;
		}
	}

	return false;
}

//returns true if a key was released this frame
bool InputManager::keyReleased(Keyboard::Key key)
{
	//check for a release event with specified key
	for (int i = 0; i < numEvents; i++)
	{
		if ((events[i].type == Event::KeyReleased) && (events[i].key.code == key))
		{
			return true;
		}
	}

	return false;
}

//returns true if a mouse button was pressed this frame
bool InputManager::mousePressed(Mouse::Button button)
{
	for (int i = 0; i < numEvents; i++)
	{
		if ((events[i].type == Event::MouseButtonPressed) && (events[i].mouseButton.button == button))
		{
			return true;
		}
	}

	return false;
}

//returns true if a mouse button was released this frame
bool InputManager::mouseReleased(Mouse::Button button)
{
	for (int i = 0; i < numEvents; i++)
	{
		if ((events[i].type == Event::MouseButtonReleased) && (events[i].mouseButton.button == button))
		{
			return true;
		}
	}

	return false;
}

bool InputManager::mouseMoved()
{
	for (int i = 0; i < numEvents; i++)
	{
		if (events[i].type == Event::MouseMoved)
		{
			return true;
		}
	}

	return false;
}

float InputManager::getMouseWheelTicks()
{
	//check for a release event with specified key
	for (int i = 0; i < numEvents; i++)
	{
		if (events[i].type == Event::MouseWheelScrolled)
		{
			return events[i].mouseWheelScroll.delta;
		}
	}

	return false; //event not found
}

