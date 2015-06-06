#include "../../pchdef.h"
#include "../LuaInclude.h"
#include <luabind/luabind.hpp>

#include "PlayerWrapper.h"
#include "../../Creature.h"
#include "../../SpellAuras.h"
#include "../../CreatureAI.h"
#include "../../CreatureGroups.h"
#include "../../DynamicObject.h"
#include "../../InstanceData.h"
#include "../../Spell.h"
#include "../../Totem.h"
#include "../../ZoneScript.h"


namespace Lua
{
	namespace UnitWrapper
	{
		void Wrap(lua_State* L)
		{
			using namespace luabind;

			open(L);

			module(L)
			[
				class_<Player>("Unit")

			];
		}
	}
}