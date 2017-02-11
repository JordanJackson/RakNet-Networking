#ifndef _CIRCLECOLLIDER_H_
#define _CIRCLECOLLIDER_H_
#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
	DECLARE_DYNAMIC_DERIVED_CLASS(CircleCollider, Collider)

public:
	void load(XMLElement* node);
	void save(XMLElement* node);

	float getRadius() { return radius; }

private:
	float radius;
};

#endif

