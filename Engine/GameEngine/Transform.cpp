#include "Core.h"
#include "Transform.h"
#include "GameObject.h"
#include "NetworkServer.h"

IMPLEMENT_DYNAMIC_CLASS(Transform);

void Transform::updateChildPositions(const sf::Vector2f& offset)
{
	if (gameObject != NULL)
	{
		for (std::list<GameObject*>::iterator iter = gameObject->children.begin(); iter != gameObject->children.end(); ++iter)
		{
			Transform* child = (*iter)->getTransform();
			child->move(offset);
		}
	}
}


void Transform::updateChildRotations(const sf::Vector2f& point, float angle)
{
	if (gameObject != NULL)
	{
		for (std::list<GameObject*>::iterator iter = gameObject->children.begin(); iter != gameObject->children.end(); ++iter)
		{
			Transform* child = (*iter)->getTransform();
			child->rotateAround(point, angle);
		}
	}
}


void Transform::updateChildScales(const sf::Vector2f& point, const sf::Vector2f& factor)
{
	if (gameObject != NULL)
	{
		for (std::list<GameObject*>::iterator iter = gameObject->children.begin(); iter != gameObject->children.end(); ++iter)
		{
			Transform* child = (*iter)->getTransform();
			child->scaleFrom(point, factor);
		}
	}
}


void Transform::move(const sf::Vector2f& offset)
{
	_transform.move(offset);
	updateChildPositions(offset);
}


void Transform::move(float offsetX, float offsetY)
{
	_transform.move(offsetX, offsetY);
	updateChildPositions(sf::Vector2f(offsetX, offsetY));
}


void Transform::rotate(float angle)
{
	_transform.rotate(angle);
	updateChildRotations(_transform.getPosition(), angle);
}


void Transform::rotateAround(const sf::Vector2f& point, float angle)
{
	sf::Vector2f local = sf::Vector2f(_transform.getPosition().x - point.x, _transform.getPosition().y - point.y);
	local = rotateVector(local, angle);

	setPosition(local.x, local.y);
	rotate(angle);
}


void Transform::scale(const sf::Vector2f& factor)
{
	_transform.scale(factor);
	updateChildScales(_transform.getPosition(), factor);
}


void Transform::scale(float factorX, float factorY)
{
	_transform.scale(factorX, factorY);
	updateChildScales(_transform.getPosition(), sf::Vector2f(factorX, factorY));
}


void Transform::scaleFrom(const sf::Vector2f& point, const sf::Vector2f& factor)
{
	sf::Vector2f local = sf::Vector2f(_transform.getPosition().x - point.x, _transform.getPosition().y - point.y);
	sf::Vector2f world;

	world.x = (local.x * factor.x) + point.x;
	world.y = (local.y * factor.y) + point.y;

	setPosition(world);
	scale(factor);
}


void Transform::setOrigin(const sf::Vector2f& origin)
{
	_transform.setOrigin(origin);
}


void Transform::setOrigin(float x, float y)
{
	_transform.setOrigin(x, y);
}


void Transform::setPosition(const sf::Vector2f& position)
{
	sf::Vector2f offset = sf::Vector2f(position.x - _transform.getPosition().x, position.y - _transform.getPosition().y);
	updateChildPositions(offset);
	_transform.setPosition(position);
}


void Transform::setPosition(float x, float y)
{
	sf::Vector2f offset = sf::Vector2f(x - _transform.getPosition().x, y - _transform.getPosition().y);
	_transform.setPosition(x, y);
	updateChildPositions(offset);
}


void Transform::setRotation(float angle)
{
	_transform.setRotation(angle);
	updateChildRotations(_transform.getPosition(), angle);
}


void Transform::setScale(const sf::Vector2f& scale)
{
	_transform.setScale(scale);
	updateChildScales(_transform.getPosition(), scale);
}


void Transform::setScale(float x, float y)
{
	_transform.setScale(x, y);
	updateChildScales(_transform.getPosition(), sf::Vector2f(x, y));
}


sf::Vector2f Transform::getUp()
{
	sf::Vector2f up = sf::Vector2f(0, 1);
	up = normalizeVector(rotateVector(up, _transform.getRotation()));
	return up;
}


sf::Vector2f Transform::getRight()
{
	sf::Vector2f right = sf::Vector2f(1, 0);
	right = normalizeVector(rotateVector(right, _transform.getRotation()));
	return right;
}

 sf::Vector2f Transform::getPosition()
{
	return _transform.getPosition();
}

sf::Vector2f Transform::getLocalPosition()
{
	if ((gameObject != nullptr) && (gameObject->parent != nullptr))
	{
		sf::Vector2f parentLocal = gameObject->parent->getTransform()->getTransformAble().getPosition();
		return sf::Vector2f(_transform.getPosition().x - parentLocal.x, _transform.getPosition().y - parentLocal.y);
	}
	else
	{
		return _transform.getPosition();
	}
}


float Transform::getLocalRotation()
{
	if ((gameObject != NULL) && (gameObject->parent != nullptr))
	{
		float parentLocal = gameObject->parent->getTransform()->getTransformAble().getRotation();
		return _transform.getRotation() - parentLocal;
	}
	else
	{
		return _transform.getRotation();
	}
}


sf::Vector2f Transform::getLocalScale()
{
	if ((gameObject != nullptr) && (gameObject->parent != nullptr))
	{
		sf::Vector2f parentLocal = gameObject->parent->getTransform()->getTransformAble().getScale();
		return sf::Vector2f(_transform.getScale().x / parentLocal.x, _transform.getScale().y / parentLocal.y);
	}
	else
	{
		return _transform.getScale();
	}
}


void Transform::save(XMLElement* node)
{
	XMLElement* posElement = node->GetDocument()->NewElement("Position");
	sf::Vector2f pos = _transform.getPosition();
	posElement->SetAttribute("x", pos.x);
	posElement->SetAttribute("y", pos.y);

	XMLElement* rotElement = node->GetDocument()->NewElement("Rotation");
	rotElement->SetAttribute("a", _transform.getRotation());

	XMLElement* scalElement = node->GetDocument()->NewElement("Scale");
	sf::Vector2f sca = _transform.getScale();
	scalElement->SetAttribute("x", sca.x);
	scalElement->SetAttribute("y", sca.y);

	node->GetDocument()->InsertEndChild(scalElement);
}

void Transform::load(XMLElement* node)
{
	if (node != NULL)
	{
		gameObject->setTransform(this);

		XMLElement* posElement = node->FirstChildElement("Position");
		if (posElement != NULL)
		{
			_transform.setPosition(sf::Vector2f(posElement->FloatAttribute("x"), posElement->FloatAttribute("y")));
		}

		XMLElement* rotElement = node->FirstChildElement("Rotation");
		if (rotElement != NULL)
		{
			_transform.setRotation(posElement->FloatAttribute("a"));
		}

		XMLElement* scalElement = node->FirstChildElement("Scale");
		if (scalElement != NULL)
		{
			_transform.setScale(sf::Vector2f(scalElement->FloatAttribute("x"), scalElement->FloatAttribute("y")));
		}
	}
}

sf::Vector2f rotateVector(sf::Vector2f vector, float angle)
{
	float theta = DEG_TO_RAD(angle);

	float cs = cos(theta);
	float sn = sin(theta);

	float px = (vector.x * cs) - (vector.y * sn);
	float py = (vector.x * sn) + (vector.y * cs);

	return sf::Vector2f(px, py);
}


sf::Vector2f normalizeVector(sf::Vector2f vector)
{
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	if (length != 0)
	{
		return sf::Vector2f(vector.x / length, vector.y / length);
	}
	else
	{
		return vector;
	}
}

// Networking

void Transform::sendPositionPacket()
{
	RakNet::BitStream bs;
	bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT);
	bs.Write((unsigned char)NetworkPacketIds::MSG_GAMEOBJECT_COMPONENT);
	bs.Write(gameObject->getUID());
	bs.Write(Transform::getClassHashCode());
	bs.Write((unsigned char)Transform::NetworkPackets::TRANSFORM_POSITION);
	bs.Write(getPosition().x);
	bs.Write(getPosition().y);

	NetworkServer::Instance().sendPacket(bs);
}

void Transform::processPacket(RakNet::BitStream& bitStream)
{
	unsigned char packetID;
	bitStream.Read(packetID);

	switch (packetID)
	{
	case Transform::NetworkPackets::TRANSFORM_POSITION:
		{
			float x, y;
			bitStream.Read(x);
			bitStream.Read(y);
			setPosition(x, y);
		}
		break;
	}
}
