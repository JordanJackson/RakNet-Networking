#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_
#pragma once

#include "ISystem.h"

class FileSystem : public ISystem
{
public:
	inline static FileSystem& Instance()
	{
		static FileSystem instance;
		return instance;
	}

	STRCODE levelId;

	void LoadFile(std::string& filename)
	{
		loadFiles.push_back(filename);
	}

	void UnLoadFile(std::string& filename)
	{
		unloadFiles.push_back(filename);
	}

	void SaveFile(std::string& filename)
	{
		saveFileName = filename;
		saveFile = true;
	}

protected:
	void initialize() final;
	void update(float deltaTime);
	void load(std::string& fileName);
	void unload(std::string& fileName);
	void save(std::string& fileName);

private:
	std::vector<std::string> loadFiles;
	std::vector<std::string> unloadFiles;

	std::string saveFileName;
	bool saveFile = false;
	
	std::string defaultFileName = "Resources/default.xml";

	FileSystem() = default;
	~FileSystem() = default;
	FileSystem(FileSystem const&) = delete;

	friend class GameEngine;
};

#endif

