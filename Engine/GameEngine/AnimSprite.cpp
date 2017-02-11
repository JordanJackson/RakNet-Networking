#include "Core.h"
#include "AnimSprite.h"
#include "GameEngine.h"
#include "TextureAsset.h"
#include <SFML\Graphics.hpp>

IMPLEMENT_DYNAMIC_CLASS(AnimSprite)

void AnimSprite::initialize()
{
	Sprite::initialize();
	
	nextFrameLimit = 1000 / fps;

	TextureAsset* rawAsset = (TextureAsset*)textureAsset.get();
	xFrames = rawAsset->getTexture()->getSize().x / frameWidth;
	yFrames = rawAsset->getTexture()->getSize().y / frameHeight;
}

void AnimSprite::update(float deltaTime)
{
	// iterate through the animation in the designed frameRate
	ElapsedTime = Clock.getElapsedTime();
	timeCounter += ElapsedTime.asSeconds();

	if (timeCounter < nextFrameLimit)
	{
		return;
	}

	Clock.restart();
	timeCounter = 0;

	// kinda of a black math magic
	// gets the current frame and translate it to the X and Y coords
	// it was needed a whole bar of dark chocolate to do it 
	int frame = currentAnim->update();
	currentY = (floor(frame / xFrames)) * frameHeight;
	currentX = (frame - (currentY / frameHeight * xFrames)) * frameWidth;
}

void AnimSprite::render(sf::RenderWindow* window, std::shared_ptr<Transform> t)
{
	if (renderableSprite != NULL)
	{
		if (currentAnim == NULL)
		{
			renderableSprite->setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
		}
		else
		{
			renderableSprite->setTextureRect(sf::IntRect(currentX, currentY, frameWidth, frameHeight));
		}
		window->draw(*renderableSprite);
	}
}

void AnimSprite::load(XMLElement* node)
{
	if (node != NULL)
	{
		XMLElement* element = nullptr;

		element = node->FirstChildElement("NumOfFrames");
		THROW_RUNTIME_ERROR(element == nullptr, "Cannot load Number of Frames for AnimSprite");
		element->QueryIntText(&numOfFrames);

		element = node->FirstChildElement("FPS");
		THROW_RUNTIME_ERROR(element == nullptr, "Cannot load FPS for AnimSprite");
		element->QueryIntText(&fps);

		element = node->FirstChildElement("FrameWidth");
		THROW_RUNTIME_ERROR(element == nullptr, "Cannot load Frame Width for AnimSprite");
		element->QueryFloatText(&frameWidth);

		element = node->FirstChildElement("FrameHeight");
		THROW_RUNTIME_ERROR(element == nullptr, "Cannot load Frame Height for AnimSprite");
		element->QueryFloatText(&frameHeight);
	}
}

void AnimSprite::save(XMLElement* node)
{
	XMLElement* element = nullptr;

	element = node->GetDocument()->NewElement("NumOfFrames");
	element->SetText(numOfFrames);
	node->InsertEndChild(element);

	element = node->GetDocument()->NewElement("FPS");
	element->SetText(fps);
	node->InsertEndChild(element);

	element = node->GetDocument()->NewElement("FrameWidth");
	element->SetText(frameWidth);
	node->InsertEndChild(element);

	element = node->GetDocument()->NewElement("FrameHeight");
	element->SetText(frameHeight);
	node->InsertEndChild(element);
}

void AnimSprite::createAnim(const char* key, int startingFrame, int endingFrame)
{
	createAnim(getHashCode(key), startingFrame, endingFrame);
}

void AnimSprite::createAnim(STRCODE key, int startingFrame, int endingFrame)
{
	//add a new animation to the map
	animMap[key] = std::shared_ptr<Animation>(new Animation(key, startingFrame, endingFrame));
}

void AnimSprite::playAnim(std::string& key, bool loopAnim)
{
	playAnim(getHashCode(key.c_str()), loopAnim);
}

void AnimSprite::playAnim(STRCODE key, bool loopAnim)
{
	if (animMap[key] != NULL)
	{
		currentAnim = animMap[key];
		currentAnim->setLoop(loopAnim);
	}
}