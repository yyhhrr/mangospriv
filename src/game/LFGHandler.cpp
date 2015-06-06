/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "WorldSession.h"
#include "Object.h"
#include "Chat.h"
#include "Language.h"
#include "ScriptMgr.h"
#include "World.h"
#include "Group.h"
#include "LFGHandler.h"

void Player::AttemptJoin(Player* _player, uint32 update_diff, uint32 p_time)
{
    checkForGroupTimer		= 5000;

	BASIC_LOG("AttemptJoin: Angekommen");

	if (IsInMeetingStoneQueue() && GetAreaIdInMeetingStoneQueue())
	{
		ChatHandler(this).PSendSysMessage("You are in queue for dungeon %s...", GetMeetingStoneQueueDungeonName(GetAreaIdInMeetingStoneQueue()).c_str());
		BASIC_LOG("AttemptJoin: Nachricht is raus");
	}

	if (IsInMeetingStoneQueue())
        timeInMeetingStoneQueue += p_time;
    else
        timeInMeetingStoneQueue = 0;

    if (IsInMeetingStoneQueue())
    {
		BASIC_LOG("IsInMeetingStoneQueue: JA");
        if (checkForGroupTimer <= p_time)
        {
			uint32 dungeonArea = GetAreaIdInMeetingStoneQueue();
			uint32 mapId = GetAreaEntryByAreaID(dungeonArea)->mapid;
			std::vector<Player*> playersInQueueForInstance = GetPlayersInMeetingStoneQueueForInstanceId(dungeonArea);
			for (std::vector<Player*>::const_iterator itr = playersInQueueForInstance.begin(); itr != playersInQueueForInstance.end(); ++itr)
			{
				Player* plr = (*itr);

				if (!plr || plr ==_player || plr->GetTeam() != _player->GetTeam())
				{
					BASIC_LOG("AttemptJoin: Falsches Team oder Spieler selbst");
					continue;
				}

				if (plr->GetGroup())// && plr->GetGroup()->IsLeader(plr->GetGUID()))
				{
					BASIC_LOG("AttemptJoin: Spieler ist in einer Gruppe");
					continue;
				}

				if (!_player->GetGroup())
				{
					Group* group = new Group;
					BASIC_LOG("AttemptJoin: Gruppe festlegen");
					if (group->GetLeaderGuid() == plr->GetObjectGuid())
					{
						BASIC_LOG("AttemptJoin: Leader festlegen");
						for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
						{
							if (Player* grpMember = itr->getSource())
							{   
								plr->SetGroupInvite(group);
								plr->RemoveFromMeetingStoneQueue(0);
								//sObjectMgr.AddGroup(group);
								BASIC_LOG("AttemptJoin: Gruppe erstellt");
							}
						}
					}
				}
			}

            checkForGroupTimer = 5000;
		}
        else
            checkForGroupTimer -= p_time;
	}
}
/*
void WorldSession::HandleMeetingStoneJoinOpcode(WorldPacket & recv_data)
{
    ObjectGuid guid;

    recv_data >> guid;

    BASIC_LOG("WORLD: Recvd CMSG_MEETINGSTONE_JOIN Message guid: %s", guid.GetString().c_str());

    if (!_player->IsSelfMover())
        return;

    GameObject *obj = GetPlayer()->GetMap()->GetGameObject(guid);

    if (!obj)
        return;

    if (obj->GetGoType() != GAMEOBJECT_TYPE_MEETINGSTONE)
    {
        sLog.outError("HandleMeetingStoneJoinOpcode: CMSG_MEETINGSTONE_JOIN for not allowed GameObject type %u (Entry %u), didn't expect this to happen.", obj->GetGoType(), obj->GetEntry());
        return;
    }

    if (Group *grp = _player->GetGroup())
    {
        if(!grp->IsLeader(_player->GetGUID()))
        {
            SendMeetingstoneFailed(MEETINGSTONE_FAIL_PARTYLEADER);

            obj->Use(_player);
            return;
        }

        if(grp->isRaidGroup())
        {
            SendMeetingstoneFailed(MEETINGSTONE_FAIL_RAID_GROUP);

            obj->Use(_player);
            return;
        }

        if(grp->IsFull())
        {
            SendMeetingstoneFailed(MEETINGSTONE_FAIL_FULL_GROUP);

            obj->Use(_player);
            return;
        }
    }

    GameObjectInfo const *gInfo = ObjectMgr::GetGameObjectInfo(obj->GetEntry());

    SendMeetingstoneSetqueue(gInfo->meetingstone.areaID, MEETINGSTONE_STATUS_JOINED_QUEUE);
    obj->Use(_player);

	_player->AttemptJoin(_player, 0, 0);
}*/

void WorldSession::HandleMeetingStoneLeaveOpcode(WorldPacket & recv_data)
{
    BASIC_LOG("WORLD: Recvd CMSG_MEETINGSTONE_LEAVE");

	WorldPacket data(SMSG_MEETINGSTONE_SETQUEUE, 5);
	
	data << uint32(0);
	data << uint8(MEETINGSTONE_STATUS_NONE);

	SendPacket(&data);
}

void WorldSession::SendMeetingstoneFailed(uint8 status)
{
    BASIC_LOG("WORLD: Recvd SMSG_MEETINGSTONE_JOINFAILED");

    WorldPacket data(SMSG_MEETINGSTONE_JOINFAILED, 1);

    data << uint8(status);

    SendPacket(&data);
}

void WorldSession::SendMeetingstoneSetqueue(uint32 areaid, uint8 status)
{
    BASIC_LOG("WORLD: Recvd SMSG_MEETINGSTONE_SETQUEUE");

    WorldPacket data(SMSG_MEETINGSTONE_SETQUEUE, 5);

    data << uint32(areaid);
    data << uint8(status);

    SendPacket(&data);

}

bool Player::IsInMeetingStoneQueue()
{
    for (std::map<uint32, uint32>::iterator itr = meetingStoneQueue.begin(); itr != meetingStoneQueue.end(); ++itr)
        if (itr->first == GetGUIDLow())
            return true;

    return false;
}

bool Player::IsInMeetingStoneQueueForInstanceId(uint32 areaId)
{
    for (std::map<uint32, uint32>::iterator itr = meetingStoneQueue.begin(); itr != meetingStoneQueue.end(); ++itr)
        if (itr->first == GetGUIDLow() && itr->second == areaId)
            return true;

    return false;
}

std::vector<Player*> Player::GetPlayersInMeetingStoneQueueForInstanceId(uint32 areaId)
{
    std::vector<Player*> players;
    for (std::map<uint32, uint32>::iterator itr = meetingStoneQueue.begin(); itr != meetingStoneQueue.end(); ++itr)
        if (itr->second == areaId)
		players.push_back(ObjectAccessor::FindPlayer(ObjectGuid(HIGHGUID_PLAYER, itr->first)));

    return players;
}

uint32 Player::GetSizeOfMeetingStoneQueueForInstanceId(uint32 areaId)
{
    uint32 count = 0;
    for (std::map<uint32, uint32>::iterator itr = meetingStoneQueue.begin(); itr != meetingStoneQueue.end(); ++itr)
        if (itr->second == areaId)
            count++;

    return count;
}

uint32 Player::GetAreaIdInMeetingStoneQueue()
{
    for (std::map<uint32, uint32>::iterator itr = meetingStoneQueue.begin(); itr != meetingStoneQueue.end(); ++itr)
        if (itr->first == GetGUIDLow())
            return itr->second;

    return 0;
}

std::string Player::GetMeetingStoneQueueDungeonName(uint32 _areaId)
{
    return GetAreaEntryByAreaID(_areaId)->area_name[GetSession()->GetSessionDbcLocale()];
}

void Player::RemoveFromMeetingStoneQueue(uint32 arg)
{
    std::map<uint32, uint32>::iterator itr = meetingStoneQueue.find(GetGUIDLow());
    if (itr != meetingStoneQueue.end())
    {
        meetingStoneQueue.erase(itr);
        timeInMeetingStoneQueue = 0;

		WorldPacket data(SMSG_MEETINGSTONE_SETQUEUE, 5);
		data << uint32(0);
		data << uint8(MEETINGSTONE_STATUS_NONE);
		GetSession()->SendPacket(&data);
    }
}