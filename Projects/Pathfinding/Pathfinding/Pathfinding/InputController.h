#pragma once

#include "Component.h"
#include "SFML\Window.hpp"

class InputController : public Component
{

	DECLARE_DYNAMIC_DERIVED_CLASS(InputController, Component);

	enum NetworkPackets
	{
		MOUSE_CLICK
	};

public:

	virtual void update(float deltaTime) override;
	virtual void load(XMLElement* element) override;
	virtual void writePacket(sf::Vector2i position);
	virtual void processPacket(RakNet::BitStream& bitstream);

private:
	bool enabled = true;
};