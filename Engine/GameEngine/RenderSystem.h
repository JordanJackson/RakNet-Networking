#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_
#pragma once

#include "ISystem.h"
#include <SFML\Graphics.hpp>

class Renderable;
class GameObject;

class RenderSystem : public ISystem
{
private:
	// File properties
	bool loadedSettings;
	std::string name;
	int width;
	int height;
	bool fullscreen;
	std::string views;
	int voriginX;
	int voriginY;
	sf::View view;

	/// Render components

	std::list<Renderable*> renderComponents;

	/// The Window
	sf::RenderWindow* window;

public:
	inline static RenderSystem& Instance()
	{
		static RenderSystem instance;
		return instance;
	}

	sf::View& getView() { return view; }
	sf::RenderWindow* GetRenderWindow() { return window; }

	virtual void update(float deltaTime);

protected:
	virtual void initialize();

private:
	void renderObject(GameObject* gObj);
	void loadSettings();

private:

	RenderSystem() = default;
	~RenderSystem() = default;
	RenderSystem(RenderSystem const&) = delete;

	friend class GameEngine;
};

#endif

