#pragma once

#include "Sprite.h"
#include <SFML\System.hpp>

class RainDrop : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(RainDrop, Sprite)

public:
	void initialize() override;
	virtual void update(float deltaTime) override;
	virtual void load(XMLElement* element) override;

private:
	sf::Vector2f speed;
	sf::Vector2f minSpeed;
	sf::Vector2f maxSpeed;
};

