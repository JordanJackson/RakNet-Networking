#include "Core.h"
#include "Object.h"

IMPLEMENT_ABSTRACT_CLASS(Object);

void Object::initialize()
{
	initialized = true;
}

void Object::load(XMLElement* element)
{
}

void Object::save(XMLElement* element)
{
}

