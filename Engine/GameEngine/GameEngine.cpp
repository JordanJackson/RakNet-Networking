// Notes:
//		Will need a load method possibly?

#include "Core.h"
#include "GameEngine.h"

// The systems we support
#include "AssetManager.h"
#include "FileSystem.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "RenderSystem.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

void GameEngine::initialize()
{
	// Determine what we are here (easier for our implementation)
	int serverClientChoice = -1;
	std::cout << "Server [0] or Client [1]: ";
	std::cin >> serverClientChoice;
	NetworkClient::Instance().isclient = (serverClientChoice == 0 ? false : true);
	NetworkServer::Instance().isserver = (serverClientChoice == 0 ? true : false);

	NetworkClient::Instance().initialize();
	NetworkServer::Instance().initialize();

	// Init the Random?
	AssetManager::Instance().initialize();
	GameObjectManager::Instance().initialize();
	RenderSystem::Instance().initialize();
	InputManager::Instance().initialize();

	// Needs to be at the end because we will load a default file
	FileSystem::Instance().initialize();
}

void GameEngine::GameLoop()
{
	float gameTime = 0.0f;
	clock_t _time;
	float deltaTime = 0.0f;

	while (true)
	{
		_time = clock();

		NetworkClient::Instance().update(deltaTime);
		NetworkServer::Instance().update(deltaTime);

		InputManager::Instance().update(deltaTime);

		FileSystem::Instance().update(deltaTime);

		AssetManager::Instance().update(deltaTime);

		GameObjectManager::Instance().update(deltaTime);

		if (gameUpdate != nullptr)
		{
			gameUpdate(deltaTime);
		}

		RenderSystem::Instance().update(deltaTime);

		_time = clock() - _time;
		deltaTime = ((float)_time) / ((clock_t)1000);
		gameTime += deltaTime;

		LOG("Current Game Time: " << gameTime << " Delta Time: " << deltaTime);
	}
}

