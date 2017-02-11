#include "Core.h"
#include "SpriteAtlas.h"
#include <SFML\Graphics.hpp>


void SpriteAtlas::initialize()
{
	Sprite::initialize();
}

void SpriteAtlas::load(XMLElement* node)
{
	Sprite::load(node);
}

void SpriteAtlas::save(XMLElement* node)
{
	Sprite::save(node);
}

void SpriteAtlas::render(sf::RenderWindow* window, std::shared_ptr<Transform>& t)
{
	std::map<STRCODE, std::shared_ptr<SpriteRect>>::iterator it;

	for (it = atlasMap.begin(); it != atlasMap.end(); ++it)
	{
		SpriteRect* rect = (*it).second.get();
		renderableSprite->setTextureRect(sf::IntRect(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight()));
		renderableSprite->setOrigin(rect->getX() + rect->getWidth() * 0.5f, rect->getY() + rect->getHeight() * 0.5f);
		Sprite::render(window, t);
	}
}

void SpriteAtlas::createNewSpriteFromAtlas(const char* ID, int startX, int startY, int width, int height)
{
	createNewSpriteFromAtlas(getHashCode(ID), startX, startY, width, height);
}

void SpriteAtlas::createNewSpriteFromAtlas(STRCODE ID, int startX, int startY, int width, int height)
{
	atlasMap[ID] = std::shared_ptr<SpriteRect>(new SpriteRect(startX, startY, width, height));
}

void SpriteAtlas::changeAtlasPos(const char* ID, int newStartX, int newStartY, int newEndX, int newEndY)
{
	changeAtlasPos(getHashCode(ID), newStartX, newStartY, newEndX, newEndY);
}

void SpriteAtlas::changeAtlasPos(STRCODE ID, int newStartX, int newStartY, int newEndX, int newEndY)
{
	atlasMap[ID]->changeAtlasPosition(newStartX, newStartY, newEndX, newEndY);
}

void SpriteAtlas::changesPos(const char* ID, int X, int Y)
{
	changesPos(getHashCode(ID), X, Y);
}

void SpriteAtlas::changesPos(STRCODE ID, int X, int Y)
{
	atlasMap[ID]->changePosition(X, Y);
}

void SpriteAtlas::removeSprite(const char* ID)
{
	removeSprite(getHashCode(ID));
}

void SpriteAtlas::removeSprite(STRCODE ID)
{
	if (atlasMap[ID] != nullptr)
	{
		atlasMap[ID].reset();
		atlasMap.erase(ID);
	}
}