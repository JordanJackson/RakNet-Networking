#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

class Transform : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Transform, Component)

private:
	/**
	* Moves the Transforms of all children by the given offset.
	*/
	void updateChildPositions(const sf::Vector2f& offset);

	/**
	* Rotates the Transforms of all children by the given angle.
	* This will affect their position in world space.
	*/
	void updateChildRotations(const sf::Vector2f& point, float angle);

	/**
	* Scales the Transforms of all children by the given factor.
	* This will affect their position in world space.
	*/
	void updateChildScales(const sf::Vector2f& point, const sf::Vector2f& factor);

	sf::Transformable _transform;

public:
	// Networking
	enum NetworkPackets
	{
		TRANSFORM, // not used yet
		TRANSFORM_POSITION,
	};

	void sendPositionPacket();
	void processPacket(RakNet::BitStream& bitStream);

	sf::Transformable& getTransformAble() { return _transform; }
	const sf::Transform& getTransform() { return _transform.getTransform(); }

	/**
	* Moves this Transform by the given offset from its current position.
	* Children will be approiately and immediately affected by this operation.
	*/
	void move(const sf::Vector2f& offset);

	/**
	* Moves this Transform by the given offset from its current position.
	* Children will be approiately and immediately affected by this operation.
	*/
	void move(float offsetX, float offsetY);


	/**
	* Rotates this Transform by the given amount around its origin.
	* Children will be approiately and immediately affected by this operation.
	*/
	void rotate(float angle);

	/**
	* Rotates this Transform around the given point in world space.
	* Will also rotate the Transform around its origin by the given angle.
	* Children will be approiately and immediately affected by this operation.
	*/
	void rotateAround(const sf::Vector2f& point, float angle);


	/**
	* Scales this Transform by the given factor on top of its current scale.
	* Children will be approiately and immediately affected by this operation.
	*/
	void scale(const sf::Vector2f& factor);

	/**
	* Scales this Transform by the given factor on top of its current scale.
	* Children will be approiately and immediately affected by this operation.
	*/
	void scale(float factorX, float factorY);

	/**
	* Scales this Transform by the given factor in relation to the given point.
	* Children will be approiately and immediately affected by this operation.
	*/
	void scaleFrom(const sf::Vector2f& point, const sf::Vector2f& factor);


	/**
	* Sets the origin of this Transform. The point according to which all other
	* transformations are in reference to.
	*/
	void setOrigin(const sf::Vector2f& origin);

	/**
	* Sets the origin of this Transform. The point according to which all other
	* transformations are in reference to.
	*/
	void setOrigin(float x, float y);


	/**
	* Sets the position of this Transform in world space.
	* Children will be approiately and immediately affected by this operation.
	*/
	void setPosition(const sf::Vector2f& position);

	/**
	* Sets the position of this Transform in world space.
	* Children will be approiately and immediately affected by this operation.
	*/
	void setPosition(float x, float y);


	/**
	* Sets the rotation of this Transform in world space.
	* Children will be approiately and immediately affected by this operation.
	*/
	void setRotation(float angle);


	/**
	* Sets the scale of this Transform in world space.
	* Children will be approiately and immediately affected by this operation.
	*/
	void setScale(const sf::Vector2f& scale);


	/**
	* Sets the position of this Transform in world space.
	* Children will be approiately and immediately affected by this operation.
	*/
	void setScale(float x, float y);


	/**
	* Returns the local upward facing vector for this Transform.
	*/
	sf::Vector2f getUp();

	/**
	* Returns the local right facing vector for this Transform.
	*/
	sf::Vector2f getRight();

	/**
	* Returns world position
	*/
	sf::Vector2f getPosition();

	/**
	* Returns local position of this Transform.
	*/
	sf::Vector2f getLocalPosition();

	/**
	* Returns local rotation of this Transform.
	*/
	float getLocalRotation();


	/**
	* Returns local scale of this Transform.
	*/
	sf::Vector2f getLocalScale();

	void save(XMLElement* node);
	void load(XMLElement* node);
};


/**
* Returns a vector rotated by the given angle from the given vector.
*/
sf::Vector2f rotateVector(sf::Vector2f vector, float angle);

/**
* Returns a normalized version of the given vector.
*/
sf::Vector2f normalizeVector(sf::Vector2f vector);

#endif

