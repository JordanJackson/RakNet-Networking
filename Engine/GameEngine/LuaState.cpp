#include "Core.h"
#include "LuaState.h"
#include <iostream>

LuaState* LuaState::_luaState = NULL;

LuaState::LuaState(void)
{
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);
}

LuaState::~LuaState(void)
{
	lua_close(mLuaState);
}

void LuaState::registerFunction(const char* _functionName, luaCallBackFunction _func)
{
	lua_register(mLuaState, _functionName, _func);
}

void LuaState::executeLuaFile(const char* _filename)
{
	int status = luaL_loadfile(mLuaState, _filename);

	if (status == 0)
	{
		// execute Lua program
		status = lua_pcall(mLuaState, 0, LUA_MULTRET, 0);
	}

	if (status != 0)
	{
		//std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(mLuaState, 1); // remove error message
	}
}

void LuaState::executeLuaString(const char* _lua)
{
	int status = luaL_loadstring(mLuaState, _lua);

	if (status == 0)
	{
		// execute Lua program
		status = lua_pcall(mLuaState, 0, LUA_MULTRET, 0);
	}

	if (status != 0)
	{
		std::cerr << "-- " << lua_tostring(mLuaState, -1) << std::endl;
		lua_pop(mLuaState, 1); // remove error message
	}
}

