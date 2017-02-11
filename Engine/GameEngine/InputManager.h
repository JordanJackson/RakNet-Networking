#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_
#pragma once

#include "ISystem.h"
#include "SFML/Window.hpp"

class InputManager : public ISystem
{
public:
	inline static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}

	//KEYBOARD EVENT GETTERS:
	bool keyPressed(sf::Keyboard::Key);
	bool keyReleased(sf::Keyboard::Key);

	//MOUSE EVENT GETTERS:
	bool mousePressed(sf::Mouse::Button);
	bool mouseReleased(sf::Mouse::Button);
	bool mouseMoved();
	float getMouseWheelTicks();

protected: 
	void initialize();
	void update(float deltaTime);

private:
	std::vector<sf::Event> events;
	int numEvents;
	int maxEvents;

private:
	void loadSettings();

private:
	InputManager() = default;
	~InputManager() = default;
	InputManager(InputManager const&) = delete;
	friend class GameEngine;
};


#endif

