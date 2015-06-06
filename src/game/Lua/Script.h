#pragma once

#define LUA_THROW_ON_ERROR 0

class Object;
class WorldObject;
class Unit;
class Creature;
class Player;

namespace Lua
{
	class Script
	{
	private:
		int mLastError = 0;

	public:
		lua_State* mState;
		Script();
		~Script();

		void LoadString(const std::string& str);
		
		template <typename FUN>
		void LoadWrapper(FUN f)
		{
			f(mState);
		}

		void Push(const Player* plr);

		void Pop();
		void Pop(int n);

		void PCall(int numArgs, int numResults);

		void SetGlobal(const std::string& name);

		void Test();
	};
}