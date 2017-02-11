#include "GameCore.h"
#include "RainDrop.h"
#include "PrefabAsset.h"
#include "Transform.h"

IMPLEMENT_DYNAMIC_CLASS(RainDrop)

void RainDrop::initialize()
{
	Sprite::initialize();

	speed.x = (maxSpeed.x - minSpeed.x) * Random.Random();
	speed.y = (maxSpeed.y - minSpeed.y) * Random.Random();

	sf::Vector2f pos;
	pos.x = RenderSystem::Instance().GetRenderWindow()->getSize().x * Random.Random();
	pos.y = (RenderSystem::Instance().GetRenderWindow()->getSize().y * -1.0f) - 64.0f;
	gameObject->getTransform()->setPosition(pos);
}

void RainDrop::load(XMLElement* element)
{
	Sprite::load(element);

	XMLElement* minElement = element->FirstChildElement("MinSpeed");
	THROW_RUNTIME_ERROR(minElement == nullptr, "No Min Element");
	minSpeed = sf::Vector2f(minElement->FloatAttribute("x"), minElement->FloatAttribute("y"));

	XMLElement* maxElement = element->FirstChildElement("MaxSpeed");
	THROW_RUNTIME_ERROR(maxElement == nullptr, "No Max Element");
	maxSpeed = sf::Vector2f(maxElement->FloatAttribute("x"), maxElement->FloatAttribute("y"));
}

void RainDrop::update(float deltaTime)
{
	Component::update(deltaTime);

	if (NetworkClient::Instance().isClient())
	{
		return;
	}

	if (gameObject->getTransform()->getPosition().y < 50)
	{
		gameObject->getTransform()->move(speed.x * deltaTime, speed.y * deltaTime);
		gameObject->getTransform()->sendPositionPacket();
	}
	else
	{
		GameObjectManager::Instance().DestroyGameObject(gameObject);

	}
}

