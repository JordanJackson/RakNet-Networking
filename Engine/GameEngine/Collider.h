#ifndef _ICOLLIDER_H_
#define _ICOLLIDER_H_
#pragma once

#include "Component.h"

class Collider : public Component
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Collider, Component)

public:
	friend class CollisionSystem;
};

#endif

