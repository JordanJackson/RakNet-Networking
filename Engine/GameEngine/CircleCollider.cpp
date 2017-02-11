#include "Core.h"
#include "CircleCollider.h"

void CircleCollider::load(XMLElement* node)
{
	if (node != NULL)
	{
		XMLElement* radiusElement = node->FirstChildElement("Radius");
		THROW_RUNTIME_ERROR(radiusElement == nullptr, "Missing Radius");
		radius = radiusElement->FloatAttribute("value");
	}
}

void CircleCollider::save(XMLElement* node)
{
	XMLElement* radiusElement = node->GetDocument()->NewElement("Radius");
	radiusElement->SetAttribute("value", radius);
	node->GetDocument()->InsertEndChild(radiusElement);
}

