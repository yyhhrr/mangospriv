#include "../pchdef.h"
#include "LuaInclude.h"

#include <luabind/luabind.hpp>

#include "Wrapper/CreatureWrapper.h"
#include <iostream>

#if LUA_THROW_ON_ERROR == 1
#define LUA_CALL(FUN) mLastError = FUN;						\
	if(mLastError) {										\
		std::string errormsg(lua_tostring(mState, -1));		\
		lua_pop(mState, 1);									\
		throw errormsg;										\
	}														\
	(void)(0)
#else
#define LUA_CALL(FUN) mLastError = FUN;						\
	if(mLastError) {										\
		std::string errormsg(lua_tostring(mState, -1));		\
		lua_pop(mState, 1);									\
		return;												\
	}														\
	(void)(0)
#endif

namespace Lua
{
	Script::Script() :
		mState(luaL_newstate())
	{
		luaL_openlibs(mState);
	}

	Script::~Script()
	{
		lua_close(mState);
	}

	void Script::LoadString(const std::string& str)
	{
		LUA_CALL( luaL_loadstring(mState, str.data()) );
	}

	void Script::Push(const Player* plr)
	{
		luabind::object o = luabind::object(mState, plr);
		o.push(mState);
	}

	void Script::Pop()
	{
		lua_pop(mState, 1);
	}

	void Script::Pop(int n)
	{
		lua_pop(mState, n);
	}

	void Script::PCall(int numArgs, int numResults)
	{
		LUA_CALL( lua_pcall(mState, numArgs, numResults, 0) );
	}

	void Script::SetGlobal(const std::string& name)
	{
		lua_setglobal(mState, name.c_str());
	}

	void Script::Test()
	{
		LoadString("local seen={}; function dump(t,i) seen[t]=true; local s={}; local n=0; for k in pairs(t) do n=n+1; s[n]=k; end; table.sort(s); for k,v in ipairs(s) do print(i,v); v=t[v]; if type(v)==\"table\" and not seen[v] then dump(v,i..\"\t\"); end end end dump(_G,\"\")");
		PCall(0, 0);
	}
}