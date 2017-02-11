#ifndef _ANIMSPRITE_H_
#define _ANIMSPRITE_H_
#pragma once

#include "Sprite.h"
#include "SFML/System/Clock.hpp"
#include <map>

class Animation
{
private:
	// anim attributes
	STRCODE ID;
	int _startFrame;
	int _endFrame;
	bool _loop;
	int _currentFrame;

public:
	// animation receives an ID , a start and end frame
	Animation(STRCODE ID, int _startFrame, int _endFrame) :
		ID(ID), 
		_startFrame(_startFrame), 
		_endFrame(_endFrame),
		_currentFrame(_startFrame)
	{ }

	~Animation()
	{ }

	void setLoop(bool loop) { _loop = loop; }

	int update()
	{
		_currentFrame++;
		if (_currentFrame > _endFrame)
		{
			if (_loop)
			{
				_currentFrame = _startFrame;
			}
			else
			{
				_currentFrame--;
			}
		}

		return _currentFrame;
	}
};


class AnimSprite final : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(AnimSprite, Sprite)

public:

	//AnimSprite(int numOfFrames, int fps, float _frameWidth, float _frameHeight);

	//creates an Animation, setting an ID to be identified on the map, a start frame, and ending frame and if it's looping or not
	// the frames will be executed in sequence, so it cant render the sprites in location that are not sequential
	void createAnim(const char* key, int startingFrame, int endingFrame);
	void createAnim(STRCODE key, int startingFrame, int endingFrame);

	// change the animation through a registered ID
	void playAnim(std::string& key, bool loopAnim);
	void playAnim(STRCODE key, bool loopAnim);

	void initialize() override;
	void update(float deltaTime) override;

	void load(XMLElement* node);
	void save(XMLElement* node);

	void render(sf::RenderWindow* window, std::shared_ptr<Transform>& t) override;

	//~AnimSprite()
	//{
	//	std::map<std::string, Animation*>::iterator animMapIt;
	//	for (animMapIt = animMap.begin(); animMapIt != animMap.end(); ++animMapIt)
	//	{
	//		delete animMapIt->second;
	//	}

	//	animMap.clear();
	//}

private :

	sf::Clock Clock;
	sf::Time ElapsedTime;
	float timeCounter = 0;
	int numOfFrames = 0;
	int fps = 0;

	float nextFrameLimit = 0;
	float frameWidth = 0;
	float frameHeight = 0;
	std::shared_ptr<Animation> currentAnim = NULL;
	bool loop = false;
	int currentX = 0;
	int currentY = 0;
	int xFrames = 0;
	int yFrames = 0;

	std::map<STRCODE, std::shared_ptr<Animation>> animMap;
};


#endif

