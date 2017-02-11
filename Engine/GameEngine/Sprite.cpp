#include "Core.h"
#include "Sprite.h"
#include "TextureAsset.h"
#include "AssetManager.h"
#include "Transform.h"
#include <SFML/Graphics.hpp>

IMPLEMENT_DYNAMIC_CLASS(Sprite);

Sprite::~Sprite()
{
	delete renderableSprite;
}

void Sprite::initialize()
{
	// creates the SFML sprite
	renderableSprite = new sf::Sprite();

	if (textureAsset != nullptr)
	{
		TextureAsset* rawAsset = (TextureAsset*)textureAsset;
		renderableSprite->setTexture(*rawAsset->getTexture());
		renderableSprite->setOrigin(rawAsset->getTexture()->getSize().x * 0.5f, rawAsset->getTexture()->getSize().y * 0.5f);
	}
}

void Sprite::render(sf::RenderWindow* window, Transform* t)
{
	if (renderableSprite != NULL)
	{
		sf::RenderStates states;
		states.transform = t->getTransform();
		window->draw(*renderableSprite, states);
	}
}

void Sprite::load(XMLElement* node)
{
	XMLElement* textureElement = node->FirstChildElement("TextureUID");
	if (textureElement != NULL)
	{
		textureUID = textureElement->Attribute("uuid");
		textureAsset = AssetManager::Instance().getAsset(getHashCode(textureUID.c_str()), TextureAsset::getClassHashCode());
	}
}

void Sprite::save(XMLElement* node)
{
	XMLElement* textureElement = node->GetDocument()->NewElement("TextureUID");
	textureElement->SetAttribute("uuid", textureUID.c_str());
	node->InsertEndChild(textureElement);
}

