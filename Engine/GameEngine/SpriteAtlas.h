#include "GameEngine.h"
#include "Sprite.h"
#include <map>
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class SpriteRect
{
private:
	int startX;
	int startY;
	int width;
	int height;

public:
	SpriteRect(int _startX, int _startY, int _width, int _height)
		: startX(_startX), startY(_startY), width(_width), height(_height)
	{
		//s->setTextureRect(sf::IntRect(startX, startY, width, height));
	}
	
	int getX() { return startX; }
	int getY() { return startY; }
	int getWidth() { return width; }
	int getHeight() { return height; }

	void setX(int X) { startX = X; }
	void setY(int Y) { startY = Y; }

	void changeAtlasPosition(int _startX, int _startY, int _width, int _height)
	{
		startX = _startX;
		startY = _startY;
		width = _width;
		height = _height;
	};

	void changePosition(int _startX, int _startY)
	{
		startX = _startX;
		startY = _startY;
	};
};

class SpriteAtlas : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(SpriteAtlas, Sprite)
public:

	void initialize();

	void load(XMLElement* node);
	void save(XMLElement* node);

	void createNewSpriteFromAtlas(const char* ID, int startX, int startY, int width, int height);
	void createNewSpriteFromAtlas(STRCODE ID, int startX, int startY, int width, int height);

	void changeAtlasPos(const char* ID, int newStartX, int newStartY, int newEndX, int newEndY);
	void changeAtlasPos(STRCODE ID, int newStartX, int newStartY, int newEndX, int newEndY);

	void changesPos(const char* ID, int X, int Y);
	void changesPos(STRCODE ID, int X, int Y);

	void removeSprite(const char* ID);
	void removeSprite(STRCODE ID);

	void render(sf::RenderWindow* window, std::shared_ptr<Transform>& t) override;

private:
	std::map<STRCODE, std::shared_ptr<SpriteRect>> atlasMap;
};

