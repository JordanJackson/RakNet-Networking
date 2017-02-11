#ifndef _LUASTATE_H_
#define _LUASTATE_H_
#pragma once

#include "lua.hpp"

typedef int (*luaCallBackFunction)(lua_State* L);

class LuaState
{
public:
	inline static LuaState& Instance()
	{
		if (_luaState == NULL)
		{
			_luaState = new LuaState();
		}
		return *_luaState;
	}

	void registerFunction(const char* _functionName, luaCallBackFunction _func);
	void executeLuaFile(const char* _filename);
	void executeLuaString(const char* _filename);

private:
	lua_State* mLuaState;
	static LuaState* _luaState;

	LuaState();
	~LuaState();
	LuaState(LuaState const&) = delete;
	LuaState& operator=(LuaState const&) = delete;
};

class LuaRegister
{
public:
	LuaRegister(const char* _functionCall, luaCallBackFunction _function)
	{
		LuaState::Instance().registerFunction(_functionCall, _function);
	};
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Macros that defines how to register lua function. </summary>
///-------------------------------------------------------------------------------------------------
#define REGISTER_LUA_FUNCTION(className, functionName)							\
	static LuaRegister className##_mLuaCall_##functionName (#functionName, className##_##functionName);

#define REGISTER_LUA_FUNCTION_0(className, functionName)						\
	static LuaRegister className##_mLuaCall (#functionName, functionName);

#endif

