/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos-zero>
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
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "MapManager.h"
#include "Transports.h"
#include "BattleGround.h"
#include "WaypointMovementGenerator.h"
#include "MapPersistentStateMgr.h"
#include "ObjectMgr.h"

#if defined(WIN32) && !defined(__MINGW32__)
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define DELTA_EPOCH_IN_USEC  11644473600000000ULL

uint32 mTimeStamp()
{
	/* We subtract 20 years from the epoch so that it doesn't overflow uint32
	* TODO: Remember to update code in 20 years */
	const uint32 YEAR_IN_SECONDS = 31556952;

	FILETIME ft;
	uint64 t;
	GetSystemTimeAsFileTime(&ft);

	t = (uint64)ft.dwHighDateTime << 32;
	t |= ft.dwLowDateTime;
	t /= 10;
	t -= DELTA_EPOCH_IN_USEC;

	return uint32((((t / 1000000L) * 1000) + ((t % 1000000L) / 1000)) - ((YEAR_IN_SECONDS * 20) * 1000));
}

#else
#include <time.h>

uint32 mTimeStamp()
{
	struct timeval tp;
	const uint32 YEAR_IN_SECONDS = 31556952;
	gettimeofday(&tp, NULL);
	uint32 return_val = (((tp.tv_sec * 1000) + (tp.tv_usec / 1000)) - ((YEAR_IN_SECONDS * 20) * 1000));
	return return_val;
}

#endif

void WorldSession::HandleMoveWorldportAckOpcode( WorldPacket & /*recv_data*/ )
{
    DEBUG_LOG( "WORLD: got MSG_MOVE_WORLDPORT_ACK." );
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    // ignore unexpected far teleports
    if(!GetPlayer()->IsBeingTeleportedFar())
        return;

    // get start teleport coordinates (will used later in fail case)
    WorldLocation old_loc;
    GetPlayer()->GetPosition(old_loc);

    // get the teleport destination
    WorldLocation &loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check (only cheating case possible)
    if (!MapManager::IsValidMapCoord(loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation))
    {
        sLog.outError("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far to a not valid location "
            "(map:%u, x:%f, y:%f, z:%f) We port him to his homebind instead..",
            GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);
        // stop teleportation else we would try this again and again in LogoutPlayer...
        GetPlayer()->SetSemaphoreTeleportFar(false);
        // and teleport the player to a valid place
        GetPlayer()->TeleportToHomebind();
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.mapid);

    Map* map = NULL;

    // prevent crash at attempt landing to not existed battleground instance
    if(mEntry->IsBattleGround())
    {
        if (GetPlayer()->GetBattleGroundId())
            map = sMapMgr.FindMap(loc.mapid, GetPlayer()->GetBattleGroundId());

        if (!map)
        {
            DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far to nonexisten battleground instance "
                " (map:%u, x:%f, y:%f, z:%f) Trying to port him to his previous place..",
                GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);

            GetPlayer()->SetSemaphoreTeleportFar(false);

            // Teleport to previous place, if cannot be ported back TP to homebind place
            if (!GetPlayer()->TeleportTo(old_loc))
            {
                DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s cannot be ported to his previous place, teleporting him to his homebind place...",
                    GetPlayer()->GetGuidStr().c_str());
                GetPlayer()->TeleportToHomebind();
            }
            return;
        }
    }

    InstanceTemplate const* mInstance = ObjectMgr::GetInstanceTemplate(loc.mapid);

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    GetPlayer()->SetSemaphoreTeleportFar(false);

    // relocate the player to the teleport destination
    if (!map)
        map = sMapMgr.CreateMap(loc.mapid, GetPlayer());

    GetPlayer()->SetMap(map);
    GetPlayer()->Relocate(loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    // the CanEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!GetPlayer()->GetMap()->Add(GetPlayer()))
    {
        // if player wasn't added to map, reset his map pointer!
        GetPlayer()->ResetMap();

        DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far but couldn't be added to map "
            " (map:%u, x:%f, y:%f, z:%f) Trying to port him to his previous place..",
            GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);

        // Teleport to previous place, if cannot be ported back TP to homebind place
        if (!GetPlayer()->TeleportTo(old_loc))
        {
            DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s cannot be ported to his previous place, teleporting him to his homebind place...",
                GetPlayer()->GetGuidStr().c_str());
            GetPlayer()->TeleportToHomebind();
        }
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // only add to bg group and object, if the player was invited (else he entered through command)
    if(_player->InBattleGround())
    {
        // cleanup seting if outdated
        if(!mEntry->IsBattleGround())
        {
            // We're not in BG
            _player->SetBattleGroundId(0, BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            _player->SetBGTeam(TEAM_NONE);
        }
        // join to bg case
        else if(BattleGround *bg = _player->GetBattleGround())
        {
            if(_player->IsInvitedForBattleGroundInstance(_player->GetBattleGroundId()))
                bg->AddPlayer(_player);
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // flight fast teleport case
    if(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if(!_player->InBattleGround())
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Reset(*GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->m_taxi.ClearTaxiDestinations();
    }

    if(mEntry->IsRaid() && mInstance)
    {
        if (time_t timeReset = sMapPersistentStateMgr.GetScheduler().GetResetTimeFor(mEntry->MapID))
        {
            uint32 timeleft = uint32(timeReset - time(NULL));
            GetPlayer()->SendInstanceResetWarning(mEntry->MapID, timeleft);
        }
    }

    // mount allow check
    if(!mEntry->IsMountAllowed())
        _player->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

    // honorless target
    if(GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAckOpcode(WorldPacket& recv_data)
{
    DEBUG_LOG("MSG_MOVE_TELEPORT_ACK");

    ObjectGuid guid;

    recv_data >> guid;

    uint32 counter, time;
    recv_data >> counter >> time;
    DEBUG_LOG("Guid: %s", guid.GetString().c_str());
    DEBUG_LOG("Counter %u, time %u", counter, time/IN_MILLISECONDS);

    Unit *mover = _player->GetMover();
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    if(!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if(guid != plMover->GetObjectGuid())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->SetPosition(dest.coord_x, dest.coord_y, dest.coord_z, dest.orientation, true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    // new zone
    if(old_zone != newzone)
    {
        // honorless target
        if(plMover->pvpInfo.inHostileArea)
            plMover->CastSpell(plMover, 2479, true);
    }

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

static const ACE_Time_Value ApplicationStartTime;

inline uint32 getMSTime_2()
{
    return (ACE_OS::gettimeofday() - ApplicationStartTime).msec();
}

void WorldSession::HandleMovementOpcodes( WorldPacket & recv_data )
{
	if(this->_player == nullptr || this->m_Socket == nullptr)
		return;

    uint32 opcode = recv_data.GetOpcode();
    BASIC_LOG("WORLD: Recvd %s (%u, 0x%X) opcode from %s", LookupOpcodeName(opcode), opcode, opcode, _player->GetName());

    Unit *mover = _player->GetMover();
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if(plMover && plMover->IsBeingTeleported())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    /* extract packet */
    MovementInfo movementInfo;
	movementInfo.Read(recv_data);
    /*----------------*/

    // Calculate timestamp
    uint32 move_time, mstime;
    mstime = mTimeStamp();
    if (m_clientTimeDelay == 0)
        m_clientTimeDelay = mstime - movementInfo.time;
    move_time = (movementInfo.time - (mstime + m_clientTimeDelay)) + mstime;
    movementInfo.UpdateTime(move_time);

    /* if(movementInfo.time - (mstime + m_clientTimeDelay) < 0)
	{
	move_time = mstime + 500;
	move_time -= (movementInfo.time - (mstime + m_clientTimeDelay));
	movementInfo.UpdateTime(move_time);
	}
	else
	{
	int calc_var = (movementInfo.time - (mstime + m_clientTimeDelay));
	if(calc_var < 0)
	calc_var *= -1;
	calc_var += 500 + mstime;
	move_time = calc_var;*/

	move_time = (movementInfo.GetTime() - (mstime - m_clientTimeDelay)) + 500 + mstime;
	movementInfo.UpdateTime(move_time);

	if (!VerifyMovementInfo(movementInfo, opcode))
        return;

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (opcode == MSG_MOVE_FALL_LAND && plMover && !plMover->IsTaxiFlying())
        plMover->HandleFall(movementInfo);

    /* process position-change */
    HandleMoverRelocation(movementInfo);

    if (plMover)
        plMover->UpdateFallInformationIfNeed(movementInfo, opcode);

	WorldPacket data(opcode, recv_data.size());
	data << mover->GetPackGUID();				// write guid
    movementInfo.Write(data);					// write data
    mover->SendMessageToSetExcept(&data, _player);
}

void WorldSession::HandleForceSpeedChangeAckOpcodes(WorldPacket &recv_data)
{
    uint32 opcode = recv_data.GetOpcode();
    BASIC_LOG("WORLD: Recvd %s (%u, 0x%X) opcode from %s", LookupOpcodeName(opcode), opcode, opcode, _player->GetName());

    /* extract packet */
    ObjectGuid guid;
    MovementInfo movementInfo;
    float  newspeed;

    recv_data >> guid;
    recv_data >> Unused<uint32>();                          // counter or moveEvent
    recv_data >> movementInfo;
    recv_data >> newspeed;

    // now can skip not our packet
    if(_player->GetObjectGuid() != guid)
        return;
    /*----------------*/

    // client ACK send one packet for mounted/run case and need skip all except last from its
    // in other cases anti-cheat check can be fail in false case
    UnitMoveType move_type;
    UnitMoveType force_move_type;

    static char const* move_type_name[MAX_MOVE_TYPE] = {  "Walk", "Run", "RunBack", "Swim", "SwimBack", "TurnRate" };

    switch(opcode)
    {
        case CMSG_FORCE_WALK_SPEED_CHANGE_ACK:          move_type = MOVE_WALK;          force_move_type = MOVE_WALK;        break;
        case CMSG_FORCE_RUN_SPEED_CHANGE_ACK:           move_type = MOVE_RUN;           force_move_type = MOVE_RUN;         break;
        case CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK:      move_type = MOVE_RUN_BACK;      force_move_type = MOVE_RUN_BACK;    break;
        case CMSG_FORCE_SWIM_SPEED_CHANGE_ACK:          move_type = MOVE_SWIM;          force_move_type = MOVE_SWIM;        break;
        case CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:     move_type = MOVE_SWIM_BACK;     force_move_type = MOVE_SWIM_BACK;   break;
        case CMSG_FORCE_TURN_RATE_CHANGE_ACK:           move_type = MOVE_TURN_RATE;     force_move_type = MOVE_TURN_RATE;   break;
        default:
            sLog.outError("WorldSession::HandleForceSpeedChangeAck: Unknown move type opcode: %u", opcode);
            return;
    }

    // skip all forced speed changes except last and unexpected
    // in run/mounted case used one ACK and it must be skipped.m_forced_speed_changes[MOVE_RUN} store both.
    if(_player->m_forced_speed_changes[force_move_type] > 0)
    {
        --_player->m_forced_speed_changes[force_move_type];
        if(_player->m_forced_speed_changes[force_move_type] > 0)
            return;
    }

    if (!_player->GetTransport() && fabs(_player->GetSpeed(move_type) - newspeed) > 0.01f)
    {
        if(_player->GetSpeed(move_type) > newspeed)         // must be greater - just correct
        {
            sLog.outError("%sSpeedChange player %s is NOT correct (must be %f instead %f), force set to correct value",
                move_type_name[move_type], _player->GetName(), _player->GetSpeed(move_type), newspeed);
            _player->SetSpeedRate(move_type,_player->GetSpeedRate(move_type),true);
        }
        else                                                // must be lesser - cheating
        {
            BASIC_LOG("Player %s from account id %u kicked for incorrect speed (must be %f instead %f)",
                _player->GetName(),_player->GetSession()->GetAccountId(),_player->GetSpeed(move_type), newspeed);
            _player->GetSession()->KickPlayer();
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPacket &recv_data)
{
	if (_player)
		BASIC_LOG("WORLD: Recvd CMSG_SET_ACTIVE_MOVER from %s", _player->GetName());
	else
		return;

    ObjectGuid guid;
    recv_data >> guid;

    if(_player && _player->GetMover() && _player->GetMover()->GetObjectGuid() != guid)
    {
        sLog.outError("HandleSetActiveMoverOpcode: incorrect mover guid: mover is %s and should be %s",
            _player->GetMover()->GetGuidStr().c_str(), guid.GetString().c_str());
        return;
    }
}

void WorldSession::HandleMoveNotActiveMoverOpcode(WorldPacket &recv_data)
{
    DEBUG_LOG("WORLD: Recvd CMSG_MOVE_NOT_ACTIVE_MOVER");
    recv_data.hexlike();

    ObjectGuid old_mover_guid;
    MovementInfo mi;

    recv_data >> old_mover_guid;
    recv_data >> mi;

    if(_player->GetMover()->GetObjectGuid() != old_mover_guid)
    {
        sLog.outError("HandleMoveNotActiveMover: incorrect mover guid: mover is %s and should be %s instead of %s",
            _player->GetMover()->GetGuidStr().c_str(),
            _player->GetGuidStr().c_str(),
            old_mover_guid.GetString().c_str());
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    _player->m_movementInfo = mi;
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recvdata*/)
{
    //DEBUG_LOG("WORLD: Recvd CMSG_MOUNTSPECIAL_ANIM");

    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
    data << GetPlayer()->GetObjectGuid();

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveKnockBackAck( WorldPacket & recv_data )
{
    DEBUG_LOG("CMSG_MOVE_KNOCK_BACK_ACK");

    Unit *mover = _player->GetMover();
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if(plMover && plMover->IsBeingTeleported())
    {
        recv_data.rpos(recv_data.wpos()); // prevent warnings spam
        return;
    }

    ObjectGuid guid;
    MovementInfo movementInfo;		// Sent in addition to knockback data
    recv_data >> guid;
    recv_data >> Unused<uint32>();	// Always set to zero?
    movementInfo.Read(recv_data);

    // Calculate timestamp (should probably move this into its own function?
    int32 move_time, mstime;
    mstime = mTimeStamp();
    if(m_clientTimeDelay == 0)
        m_clientTimeDelay = mstime - movementInfo.time;

    /* The 500 delay lets the client sync the movement correctly.
	* Yes it slows things a bit, but removing it causes stutter.
	* Fixes itself after a short while */
    move_time = (movementInfo.time - (mstime - m_clientTimeDelay)) + 500 + mstime;
    movementInfo.UpdateTime(move_time);

    /* Make sure input is valid */
    if (!VerifyMovementInfo(movementInfo, guid))
        return;

    HandleMoverRelocation(movementInfo);

    /* Weird size, maybe needs correcting */
    WorldPacket data(MSG_MOVE_KNOCK_BACK, uint16(recv_data.size() + 4));
    data.appendPackGUID(guid);

    /* Includes data shown below (but in different order) */
    movementInfo.Write(data);

    /* This is sent in addition to the rest of the movement data (yes, angle+velocity are sent twice) */
    data << movementInfo.jump.sinAngle;
    data << movementInfo.jump.cosAngle;
    data << movementInfo.jump.xyspeed;
    data << movementInfo.jump.velocity;

    /* Do we really need to send the data to everyone? Seemed to work better */
    mover->SendMessageToSet(&data, false);
}

void WorldSession::SendKnockBack(float angle, float horizontalSpeed, float verticalSpeed)
{
	float vsin = sin(angle);
	float vcos = cos(angle);

	WorldPacket data(SMSG_MOVE_KNOCK_BACK, 9 + 4 + 4 + 4 + 4 + 4);
	data << GetPlayer()->GetPackGUID();
	data << uint32(0);                                  // Sequence
	data << float(vcos);                                // x direction
	data << float(vsin);                                // y direction
	data << float(horizontalSpeed);                     // Horizontal speed
	data << float(-verticalSpeed);                      // Z Movement speed (vertical)
	SendPacket(&data);
}

void WorldSession::HandleMoveHoverAck( WorldPacket& recv_data )
{
    DEBUG_LOG("CMSG_MOVE_HOVER_ACK");

    MovementInfo movementInfo;

    recv_data >> Unused<uint64>();                          // guid
    recv_data >> Unused<uint32>();                          // unk
    recv_data >> movementInfo;
    recv_data >> Unused<uint32>();                          // unk2
}

void WorldSession::HandleMoveWaterWalkAck(WorldPacket& recv_data)
{
    DEBUG_LOG("CMSG_MOVE_WATER_WALK_ACK");

    MovementInfo movementInfo;

    recv_data.read_skip<uint64>();                          // guid
    recv_data.read_skip<uint32>();                          // unk
    recv_data >> movementInfo;
    recv_data >> Unused<uint32>();                          // unk2
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recv_data)
{
    if (!_player->isAlive() || _player->isInCombat())
        return;

    ObjectGuid summonerGuid;
    recv_data >> summonerGuid;

    _player->SummonIfPossible(true);
}

bool WorldSession::VerifyMovementInfo(MovementInfo const& movementInfo, ObjectGuid const& guid, uint32 opcode)
{
    // ignore wrong guid (player attempt cheating own session for not own guid possible...)
    if (guid != _player->GetMover()->GetObjectGuid())
        return false;

	return VerifyMovementInfo(movementInfo, opcode);
}

bool WorldSession::VerifyMovementInfo(MovementInfo const& movementInfo, uint32 opcode)
{
	float x, y, z, o;
	x = movementInfo.GetPos()->x;
	y = movementInfo.GetPos()->y;
	z = movementInfo.GetPos()->z;
	o = movementInfo.GetPos()->o;

	uint32 mapid = _player->GetMapId();

    if (!MaNGOS::IsValidMapCoord(x, y, z, o))
        return false;

	if(_player->isGameMaster() == false)
	{
		if((_player->GetCharm() && _player->GetCharm()->GetTypeId() != TYPEID_PLAYER))
		{
			//TODO: some possessed creatures are faster than others, do the math!
		}
		else
			SpeedhackCheck(movementInfo);
	}

	if (opcode == MSG_MOVE_STOP_SWIM)
	{
		GridMapLiquidData data;
		auto status = _player->GetMap()->GetTerrain()->getLiquidStatus(x, y, z, MAP_ALL_LIQUIDS, &data);

		if (status != LIQUID_MAP_UNDER_WATER && status != LIQUID_MAP_IN_WATER)
		{
			sLog.outWarden("received MSG_MOVE_STOP_SWIM without simming flag (player: \"%s\", account: %u)", _player->GetName(), _player->GetAccountId());
			KickPlayer(); //Causes extrem lag -> kick player
		}
	}

	if (movementInfo.HasMovementFlag(MOVEFLAG_HOVER) && _player->HasAuraType(SPELL_AURA_HOVER) == false)
		return false;

	if((movementInfo.HasMovementFlag(MOVEFLAG_LEVITATING) || movementInfo.HasMovementFlag(MOVEFLAG_SAFE_FALL)) && (!_player->HasAuraType(SPELL_AURA_FEATHER_FALL) || _player->HasAuraType(SPELL_AURA_SAFE_FALL)))
		return false;

	if(movementInfo.HasMovementFlag(MOVEFLAG_WATERWALKING) && _player->HasAuraType(AuraType::SPELL_AURA_WATER_WALK) == false && _player->isAlive())
		return false;

    if (movementInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT))
    {
        // transports size limited
        // (also received at zeppelin/lift leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if( movementInfo.GetTransportPos()->x > 50 || movementInfo.GetTransportPos()->y > 50 || movementInfo.GetTransportPos()->z > 100 )
            return false;

        if( !MaNGOS::IsValidMapCoord(movementInfo.GetPos()->x + movementInfo.GetTransportPos()->x, movementInfo.GetPos()->y + movementInfo.GetTransportPos()->y,
            movementInfo.GetPos()->z + movementInfo.GetTransportPos()->z, movementInfo.GetPos()->o + movementInfo.GetTransportPos()->o) )
        {
            return false;
        }
    }

    return true;
}

bool WorldSession::SpeedhackCheck(MovementInfo const& movementInfo)
{
	/*
	Altes Tracking eventuell f�r die Zukunft:
	sLog.outWarden("player fell further than 18 units. units moved: %f (player: \"%s\", account: %u)", dist, _player->GetName(), _player->GetAccountId());
	//KickPlayer();
	std::stringstream ss;
	ss << "M\303\266glicher SpeedHack / Teleporthack { " << x << " " << y << " " << z << "} {" << position->x << " " << position->y << " " << position->z << "} Map: " << _player->GetMapId();
	CharacterDatabase.PExecute("INSERT INTO character_warden (id, guid, account, name, result, timestamp) VALUES('0', '%u', '%u', '%s', '%s', CURRENT_TIMESTAMP)", _player->GetGUIDLow(), _player->GetAccountId(), _player->GetName(), ss.str().c_str());
	_player->TeleportTo(pMapId, x, y, z, o);
	_player->SetClientControl(_player, 0);
	*/

	Player* pPlayer = _player;
	int pAntiCheatCount = pPlayer->GetAntiCheatCounter();
	
	if (_player->GetSession()->GetSecurity() > SEC_PLAYER)
		return false;

	if (pPlayer->IsTaxiFlying())
		return false;

	if (movementInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT))
		return false;

	if (pPlayer->GetAntiCheatImmuneTimer() > 0)
		return false;

	debug_log("AntiCheatCounter: %u", pAntiCheatCount);

	if (pAntiCheatCount >= 3)
		pPlayer->GetSession()->KickPlayer();

	int pMapId = pPlayer->GetMapId();

	const Position* position = movementInfo.GetPos();
	float x, y, z, o;
	pPlayer->GetMover()->GetPosition(x, y, z);
	o = pPlayer->GetMover()->GetOrientation();

	float pDistance = sqrt(pow(position->x - x, 2) + pow(position->y - y, 2) + pow(position->z - z, 2));
	float pDistanceZ = sqrt(pow(position->z - z, 2)) / 2;
	float pDistanceZFalling = sqrt(pow(position->z - z, 2));

	debug_log("Distance: %f %d", pDistance, pPlayer->IsMounted());
	int32 main_speed_mod_mount = 0;
	int32 main_speed_mod_normal = 0;

	if (pPlayer->IsMounted())
	{
		float pPaladinSpellBonusTalent = 0;
		float pSpellBonusKarotte = 0;
		float pSpellBonusSporen = 0;
		float pSpellBonusEnchantment = 0;

		float pSpellBonusRidingSum = 0;

		if (pPlayer->HasAura(26023))
			pPaladinSpellBonusTalent = 0.80f;
		else if (pPlayer->HasAura(26022))
			pPaladinSpellBonusTalent = 0.40f;

		if (pPlayer->HasAura(13587))
			pSpellBonusKarotte = 0.15f;

		if (pPlayer->HasAura(7215))
			pSpellBonusSporen = 0.15f;

		if (pPlayer->HasAura(7215))
			pSpellBonusEnchantment = 0.15f;

		pSpellBonusRidingSum = pPaladinSpellBonusTalent + pSpellBonusKarotte + pSpellBonusSporen + pSpellBonusEnchantment;
		debug_log("pRidingSpells: %f", pSpellBonusRidingSum);
		main_speed_mod_mount = pPlayer->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED);
		debug_log("AURA MOUNT: %i", main_speed_mod_mount);
		// Fallend
		if (pPlayer->HasMovementFlag(MOVEFLAG_FALLING) || pPlayer->HasMovementFlag(MOVEFLAG_FALLINGFAR) || movementInfo.HasMovementFlag(MOVEFLAG_FALLING) || movementInfo.HasMovementFlag(MOVEFLAG_FALLINGFAR))
		{
			float pMaxDistance = ((((3.70 / 100) * main_speed_mod_mount) + 3.70) + pDistanceZFalling) + pSpellBonusRidingSum;
			debug_log("Schnelles Reiten fallend: %f -- Erlaubt: %f", pDistance, pMaxDistance);
			if (pDistance > pMaxDistance)
			{
				pAntiCheatCount = pAntiCheatCount + 1;
				pPlayer->SetAntiCheatCounter(pAntiCheatCount);
				pPlayer->TeleportTo(pMapId, x, y, z, o, true);
				debug_log("Hackverdacht mit Mount!");
				return true;
			}
		}
		else
		{
			// Normales Reiten:
			if (main_speed_mod_mount == 60)
			{
				float pMaxDistance = ((((3.70 / 100) * main_speed_mod_mount) + 3.70) + pDistanceZ) + pSpellBonusRidingSum;
				debug_log("Normales Reiten: %f -- Erlaubt: %f", pDistance, pMaxDistance);
				if (pDistance > pMaxDistance)
				{
					pAntiCheatCount = pAntiCheatCount + 1;
					pPlayer->SetAntiCheatCounter(pAntiCheatCount);
					pPlayer->TeleportTo(pMapId, x, y, z, o, true);
					debug_log("Hackverdacht!");
					return true;
				}
			}
			//Schnelles Reiten:
			else if (main_speed_mod_mount == 100)
			{
				float pMaxDistance = ((((3.70 / 100) * main_speed_mod_mount) + 3.70) + pDistanceZ) + pSpellBonusRidingSum;
				debug_log("Schnelles Reiten: %f -- Erlaubt: %f", pDistance, pMaxDistance);
				if (pDistance > pMaxDistance)
				{
					pAntiCheatCount = pAntiCheatCount + 1;
					pPlayer->SetAntiCheatCounter(pAntiCheatCount);
					pPlayer->TeleportTo(pMapId, x, y, z, o, true);
					debug_log("Hackverdacht!");
					return true;
				}
			}
		}
	}
	else
	{
		float pSpellBonusToleranz = 0.05f;
		main_speed_mod_normal = pPlayer->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_SPEED);

		// Fallend
		if (pPlayer->HasMovementFlag(MOVEFLAG_FALLING) || pPlayer->HasMovementFlag(MOVEFLAG_FALLINGFAR) || movementInfo.HasMovementFlag(MOVEFLAG_FALLING) || movementInfo.HasMovementFlag(MOVEFLAG_FALLINGFAR))
		{
			// 3.5 + main_speed_mod_normal% (0.18) = 3.68 ~ 3.70
			float pMaxDistance = ((((3.70 / 100) * main_speed_mod_normal) + 3.70) + pDistanceZFalling) + pSpellBonusToleranz;
			if (pDistance > pMaxDistance)
			{
				debug_log("Laufen im fallen: %f -- Erlaubt: %f", pDistance, pMaxDistance);
				debug_log("Hackverdacht ohne Mount!");
			}
		}
		else
		{
			float pDruidSpellSetBonus = 0.0f;
			float pSpellBonusSwim = 0.55f;

			if (pPlayer->HasAura(23218))
				pDruidSpellSetBonus = 0.55f;

			if (pPlayer->HasAura(7840) && pPlayer->HasMovementFlag(MOVEFLAG_SWIMMING))
				pSpellBonusSwim = 3.70f;

			debug_log("Druid: %f", pSpellBonusSwim);

			if (main_speed_mod_normal > 0)
			{
				debug_log("Speed Aura Add: %i", main_speed_mod_normal);

				if (pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
				{
					float pSpellBonusNightelfRacial = 0.0f;

					if (pPlayer->getRace() == 4)
						pSpellBonusNightelfRacial = 0.85f;

					debug_log("Nachtelf: %f", pSpellBonusNightelfRacial);

					float pMaxDistance = ((((3.70 / 100) * main_speed_mod_normal) + 3.90) + pDistanceZ) + pSpellBonusToleranz + pSpellBonusNightelfRacial;
					debug_log("Laufen mit Speed als Geist: %f -- Erlaubt: %f", pDistance, pMaxDistance);
					if (pDistance > pMaxDistance)
					{
						pAntiCheatCount = pAntiCheatCount + 1;
						pPlayer->SetAntiCheatCounter(pAntiCheatCount);
						pPlayer->TeleportTo(pMapId, x, y, z, o, true);
						debug_log("Hackverdacht im Laufen mit Speed als Geist!");
						return true;
					}
				}
				else
				{
					float pMaxDistance = ((((3.70 / 100) * main_speed_mod_normal) + 3.70) + pDistanceZ) + pSpellBonusToleranz + pDruidSpellSetBonus + pSpellBonusSwim;
					debug_log("Laufen mit Speed: %f -- Erlaubt: %f", pDistance, pMaxDistance);
					if (pDistance > pMaxDistance)
					{
						pAntiCheatCount = pAntiCheatCount + 1;
						pPlayer->SetAntiCheatCounter(pAntiCheatCount);
						pPlayer->TeleportTo(pMapId, x, y, z, o, true);
						debug_log("Hackverdacht im Laufen mit Speed!");
						return true;
					}
				}
			}
			else
			{
				// 3.5 + 10% (0.18) = 3.68 ~ 3.70 + pDistanceZ
				float pMaxDistance = (3.70 + pDistanceZ) + pSpellBonusToleranz + pDruidSpellSetBonus;
				debug_log("%f -- %f", pDistance, pMaxDistance);
				if (pDistance > pMaxDistance)
				{
					pAntiCheatCount = pAntiCheatCount + 1;
					pPlayer->SetAntiCheatCounter(pAntiCheatCount);
					pPlayer->TeleportTo(pMapId, x, y, z, o, true);
					debug_log("Hackverdacht im Laufen!");
					return true;
				}
			}
		}
	}
	return true;
}

void WorldSession::HandleMoverRelocation(MovementInfo& movementInfo)
{
	if(this->_player == nullptr || this->m_Socket == nullptr)
		return;

	movementInfo.UpdateTime(WorldTimer::getMSTime());

    Unit *mover = _player->GetMover();

    if (Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL)
    {
        if (movementInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT))
        {
            if (!plMover->m_transport)
            {
                // elevators also cause the client to send MOVEFLAG_ONTRANSPORT - just unmount if the guid can be found in the transport list
                for (MapManager::TransportSet::const_iterator iter = sMapMgr.m_Transports.begin(); iter != sMapMgr.m_Transports.end(); ++iter)
                {
                    if ((*iter)->GetObjectGuid() == movementInfo.GetTransportGuid())
                    {
                        plMover->m_transport = (*iter);
                        (*iter)->AddPassenger(plMover);
                        break;
                    }
                }
            }
        }
        else if (plMover->m_transport)               // if we were on a transport, leave
        {
            plMover->m_transport->RemovePassenger(plMover);
            plMover->m_transport = NULL;
            movementInfo.ClearTransportData();
        }

        if (movementInfo.HasMovementFlag(MOVEFLAG_SWIMMING) != plMover->IsInWater())
        {
            // now client not include swimming flag in case jumping under water
            plMover->SetInWater( !plMover->IsInWater() || plMover->GetTerrain()->IsUnderWater(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z) );
        }

        plMover->SetPosition(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o);
        plMover->m_movementInfo = movementInfo;

        /* Movement should cancel looting */
        if(ObjectGuid lootGUID = plMover->GetLootGuid())
            plMover->SendLootRelease(lootGUID);

        if(movementInfo.GetPos()->z < -500.0f)
        {
			if (plMover->GetBattleGround()
                && plMover->GetBattleGround()->HandlePlayerUnderMap(_player))
            {
                // do nothing, the handle already did if returned true
            }
            else
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                // TODO: discard movement packets after the player is rooted
                if(plMover->isAlive())
                {
                    plMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, plMover->GetMaxHealth());
                    // pl can be alive if GM/etc
                    if(!plMover->isAlive())
                    {
                        // change the death state to CORPSE to prevent the death timer from
                        // starting in the next player update
                        plMover->KillPlayer();
                        plMover->BuildPlayerRepop();
                    }
                }

                // cancel the death timer here if started
                plMover->RepopAtGraveyard();
                plMover->ResurrectPlayer(0.5f);
                plMover->SpawnCorpseBones();
            }
        }
    }
    else                                                    // creature charmed
    {
		if (mover && mover->IsInWorld() && mover->GetMap())
			mover->GetMap()->CreatureRelocation(static_cast<Creature*>(mover), movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o);
	}
}
