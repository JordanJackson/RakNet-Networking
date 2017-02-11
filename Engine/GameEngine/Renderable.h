#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_
#pragma once

#include "Component.h"

class Renderable : public Component
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Renderable, Component)

protected:
	virtual void render(sf::RenderWindow* window, Transform* t) override {};

	friend class RenderSystem;
};

#endif

