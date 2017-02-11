#include "Core.h"
#include "FileSystem.h"
#include "AssetManager.h"
#include "GameObjectManager.h"
#include "LuaState.h"

int FileSystem_LoadFile(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc == 1)
	{
		const char* value = lua_tostring(L, 1);
		std::string fileName = value;
		FileSystem::Instance().LoadFile(fileName);
	}
	return 1;
};
REGISTER_LUA_FUNCTION(FileSystem, LoadFile)

int FileSystem_UnloadFile(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc == 1)
	{
		const char* value = lua_tostring(L, 1);
		std::string fileName = value;
		FileSystem::Instance().UnLoadFile(fileName);
	}
	return 1;
};
REGISTER_LUA_FUNCTION(FileSystem, UnloadFile)

int FileSystem_SaveFile(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc == 1)
	{
		const char* value = lua_tostring(L, 1);
		//FileSystem::Instance().save(value);
	}
	return 1;
};
REGISTER_LUA_FUNCTION(FileSystem, SaveFile)

void FileSystem::initialize()
{
	load(defaultFileName);
}

void FileSystem::update(float deltaTime)
{
	for (int i = 0; i < unloadFiles.size(); i++)
	{
		unload(loadFiles[i]);
	}

	for (int i = 0; i < loadFiles.size(); i++)
	{
		load(loadFiles[i]);
	}

	if (saveFile == true)
	{
		save(saveFileName);
		saveFile = false;
	}

	unloadFiles.clear();
	loadFiles.clear();
}

void FileSystem::load(std::string& fileName)
{
	try
	{
		std::vector<std::string> additionalFilesToLoad;

		LOG("Loading File: " << fileName.c_str());

		levelId = getHashCode(fileName.c_str());

		tinyxml2::XMLDocument doc;
		doc.LoadFile(fileName.c_str());
		THROW_RUNTIME_ERROR(doc.ErrorID() != XML_SUCCESS, "Unable to Load file: " + fileName);

		XMLElement* fileElement = doc.FirstChildElement("Files");
		if (fileElement != nullptr)
		{
			XMLElement* fileNode = fileElement->FirstChildElement("File");
			while (fileNode != nullptr)
			{
				additionalFilesToLoad.push_back(fileNode->GetText());
				fileNode = fileNode->NextSiblingElement("File");
			}
		}

		XMLElement* assetsElement = doc.FirstChildElement("Assets");
		if (assetsElement != NULL)
		{
			AssetManager::Instance().load(assetsElement, levelId);
		}

		XMLElement* gameObjectsElement = doc.FirstChildElement("GameObjects");
		if (gameObjectsElement != NULL)
		{
			GameObjectManager::Instance().load(gameObjectsElement, levelId);
		}

		for (int i = 0; i < additionalFilesToLoad.size(); i++)
		{
			load(additionalFilesToLoad[i]);
		}

	}
	catch (std::runtime_error& error)
	{
		std::cout << "Runtime Error loading file:" << std::endl;
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error loading file" << std::endl;
	}
}

void FileSystem::unload(std::string& fileName)
{
	STRCODE levelId = getHashCode(fileName.c_str());
	AssetManager::Instance().unload(levelId);
	GameObjectManager::Instance().unload(levelId);
}

void FileSystem::save(std::string& fileName)
{
	LOG("Saving File: " << fileName.c_str());

	// Load a file here and put the objects in the proper places. We should get some updates!
	tinyxml2::XMLDocument doc;
	XMLElement* defElement = doc.NewElement("Level");
	defElement->InsertFirstChild(defElement);

	GameObjectManager::Instance().save(defElement);

	doc.InsertEndChild(defElement);
	doc.SaveFile(fileName.c_str());
}

