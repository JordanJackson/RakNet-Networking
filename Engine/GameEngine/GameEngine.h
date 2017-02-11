#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_
#pragma once

class ISystem;

class GameEngine
{
public:
	inline static GameEngine& Instance()
	{
		static GameEngine instance;
		return instance;
	}

	void initialize();
	void GameLoop();

	void setGameUpdate(std::function<void(float)> func) { gameUpdate = func; }

private:
	std::function<void(float)> gameUpdate;

	GameEngine() = default;
	~GameEngine() = default;
	GameEngine(GameEngine const&) = delete;
};

#endif

