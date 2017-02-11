#ifndef _SPRITE_H_
#define _SPRITE_H_
#pragma once

#include "Renderable.h"

class Asset;
namespace sf { class Sprite; }

class Sprite : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Sprite, Renderable)

public :
	~Sprite();

	virtual void initialize();

	void load(XMLElement* node);
	void save(XMLElement* node);

	virtual void render(sf::RenderWindow* window, Transform* t) override;

protected:
	sf::Sprite* renderableSprite;
	Asset* textureAsset;
	std::string textureUID;
};

#endif

