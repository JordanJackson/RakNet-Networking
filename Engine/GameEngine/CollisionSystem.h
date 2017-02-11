#ifndef _COLLISIONSYSTEM_H_
#define _COLLISIONSYSTEM_H_
#pragma once

#include "ISystem.h"
#include "ICollider.h"
#include "Component.h"

class CollisionSystem : public ISystem
{
public:
	std::list<ICollider*> colliderComponents;

	void AddCollide(ICollider* component);
	void RemoveCollide(ICollider* component);

public:
	inline static CollisionSystem& Instance()
	{
		static CollisionSystem instance;
		return instance;
	}

protected:
	void initialize();
	void update();

private:
	CollisionSystem() = default;
	~CollisionSystem() = default;
	CollisionSystem(CollisionSystem const&) = delete;

	friend class GameEngine;
};
#endif

