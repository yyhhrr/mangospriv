#pragma once


namespace LuaInstance
{
	/* GETTERS */
	int GetData64(lua_State* L, InstanceData* data)
	{
		uint32 _data = Eluna::CHECKVAL<uint32>(L, 2);
		Eluna::Push(L, data->GetData64(_data));

		return 1;
	}

	int GetData(lua_State* L, InstanceData* data)
	{
		uint32 _data = Eluna::CHECKVAL<uint32>(L, 2);
		Eluna::Push(L, data->GetData(_data));

		return 1;
	}

	/* SETTERS */
	int SetData64(lua_State* L, InstanceData* data)
	{
		uint32 _data = Eluna::CHECKVAL<uint32>(L, 2);
		uint32 value = Eluna::CHECKVAL<uint64>(L, 3);
		data->SetData64(_data, value);

		return 0;
	}

	int SetData(lua_State* L, InstanceData* data)
	{
		uint32 _data = Eluna::CHECKVAL<uint32>(L, 2);
		uint32 value = Eluna::CHECKVAL<uint32>(L, 3);
		data->SetData(_data, value);

		return 0;
	}
}