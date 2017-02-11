#include "Core.h"
#include "CollisionSystem.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include "Transform.h"

#include <cmath>

void CollisionSystem::initialize()
{
}

void CollisionSystem::update()
{
	//loop through colliders checking for collisions
	for (std::list<ICollider*>::iterator itr = colliderComponents.begin(); itr != colliderComponents.end(); ++itr)
	{
		if (typeid(*itr).name() == "CircleCollider")
		{
			ICollider* collider = *itr;
			CircleCollider* circleC = dynamic_cast<CircleCollider*>(collider);
			float radius1 = circleC->radius;
			sf::Vector2f pos1 = circleC->gameObject->getTransform()->getPosition();

			for (std::list<ICollider*>::iterator citr = colliderComponents.begin(); citr != colliderComponents.end(); ++itr)
			{
				if (typeid(*citr).name() == "CircleCollider")
				{
					ICollider* collider2 = *citr;
					CircleCollider* circleC2 = dynamic_cast<CircleCollider*>(collider2);

					float radius2 = circleC2->radius;
					sf::Vector2f pos2 = circleC2->gameObject->getTransform()->getPosition();

					pos1 = pos2 - pos1; //pos1 is now the displacement vector between pos1/pos2

					float distance = (float)std::sqrt((double)(std::pow(pos1.x, 2.0) + std::pow(pos1.y, 2.0))); //distance between colliders is magnitude of disp vector
					if (circleC != circleC2 && distance <= (radius1 + radius2))
					{
						circleC->gameObject->OnCollisionEnter(circleC2->gameObject);
					}
				}
			}
		}
	}
}

void CollisionSystem::AddCollide(ICollider* component)
{
	colliderComponents.push_back(component);
}

void CollisionSystem::RemoveCollide(ICollider* component)
{
	colliderComponents.remove(component);
}

