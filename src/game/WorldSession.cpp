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

/** \file
    \ingroup u2w
*/

#include "WorldSocket.h"                                    // must be first to make ACE happy with ACE includes in it
#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "BattleGroundMgr.h"
#include "MapManager.h"
#include "SocialMgr.h"
#include "Warden-System\WardenWin.h"
#include "Warden-System\WardenMac.h"
#include "LuaEngine/LuaEngine.h"

// select opcodes appropriate for processing in Map::Update context for current session state
static bool MapSessionFilterHelper(WorldSession* session, OpcodeHandler const& opHandle)
{
    // we do not process thread-unsafe packets
    if (opHandle.packetProcessing == PROCESS_THREADUNSAFE)
        return false;

    // we do not process not loggined player packets
    Player * plr = session->GetPlayer();
    if (!plr)
        return false;

    // in Map::Update() we do not process packets where player is not in world!
    return plr->IsInWorld();
}


bool MapSessionFilter::Process(WorldPacket * packet)
{
    OpcodeHandler const& opHandle = opcodeTable[packet->GetOpcode()];
    if (opHandle.packetProcessing == PROCESS_INPLACE)
        return true;

    // let's check if our opcode can be really processed in Map::Update()
    return MapSessionFilterHelper(m_pSession, opHandle);
}

// we should process ALL packets when player is not in world/logged in
// OR packet handler is not thread-safe!
bool WorldSessionFilter::Process(WorldPacket* packet)
{
    OpcodeHandler const& opHandle = opcodeTable[packet->GetOpcode()];
    // check if packet handler is supposed to be safe
    if (opHandle.packetProcessing == PROCESS_INPLACE)
        return true;

    // let's check if our opcode can't be processed in Map::Update()
    return !MapSessionFilterHelper(m_pSession, opHandle);
}

/// WorldSession constructor
WorldSession::WorldSession(uint32 id, WorldSocket *sock, AccountTypes sec, bool ispremium, time_t mute_time, LocaleConstant locale) :
m_muteTime(mute_time),
_player(NULL), m_Socket(sock), _security(sec), _ispremium(ispremium), _accountId(id), _logoutTime(0),
m_inQueue(false), m_playerLoading(false), m_playerLogout(false), m_playerRecentlyLogout(false), m_playerSave(false),
m_sessionDbcLocale(sWorld.GetAvailableDbcLocale(locale)), m_sessionDbLocaleIndex(sObjectMgr.GetIndexForLocale(locale)),
m_latency(0), m_tutorialState(TUTORIALDATA_UNCHANGED), m_Warden(nullptr), AntiDOS(this)
{
    if (sock)
    {
        m_Address = sock->GetRemoteAddress ();
        sock->AddReference ();
    }
}

/// WorldSession destructor
WorldSession::~WorldSession()
{
    ///- unload player if not unloaded
    if (_player)
        LogoutPlayer (true);

    /// - If have unclosed socket, close it
    if (m_Socket)
    {
        m_Socket->CloseSocket ();
        m_Socket->RemoveReference ();
        m_Socket = NULL;
    }

	if(m_Warden)
	{
		delete m_Warden;
		m_Warden = nullptr;
	}

    ///- empty incoming packet queue
    WorldPacket* packet;
    while(_recvQueue.next(packet))
        delete packet;
}

void WorldSession::SizeError(WorldPacket const& packet, uint32 size) const
{
    sLog.outError("Client (account %u) send packet %s (%u) with size " SIZEFMTD " but expected %u (attempt crash server?), skipped",
        GetAccountId(),LookupOpcodeName(packet.GetOpcode()),packet.GetOpcode(),packet.size(),size);
}

/// Get the player name
char const* WorldSession::GetPlayerName() const
{
    return GetPlayer() ? GetPlayer()->GetName() : "<none>";
}

/// Send a packet to the client
void WorldSession::SendPacket(WorldPacket const* packet)
{
    if (!m_Socket)
        return;

    #ifdef MANGOS_DEBUG

    // Code for network use statistic
    static uint64 sendPacketCount = 0;
    static uint64 sendPacketBytes = 0;

    static time_t firstTime = time(NULL);
    static time_t lastTime = firstTime;                     // next 60 secs start time

    static uint64 sendLastPacketCount = 0;
    static uint64 sendLastPacketBytes = 0;

    time_t cur_time = time(NULL);

    if((cur_time - lastTime) < 60)
    {
        sendPacketCount+=1;
        sendPacketBytes+=packet->size();

        sendLastPacketCount+=1;
        sendLastPacketBytes+=packet->size();
    }
    else
    {
        uint64 minTime = uint64(cur_time - lastTime);
        uint64 fullTime = uint64(lastTime - firstTime);
        DETAIL_LOG("Send all time packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f time: %u",sendPacketCount,sendPacketBytes,float(sendPacketCount)/fullTime,float(sendPacketBytes)/fullTime,uint32(fullTime));
        DETAIL_LOG("Send last min packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f",sendLastPacketCount,sendLastPacketBytes,float(sendLastPacketCount)/minTime,float(sendLastPacketBytes)/minTime);

        lastTime = cur_time;
        sendLastPacketCount = 1;
        sendLastPacketBytes = packet->wpos();               // wpos is real written size
    }

    #endif                                                  // !MANGOS_DEBUG

    if (m_Socket->SendPacket (*packet) == -1)
        m_Socket->CloseSocket ();
}

/// Add an incoming packet to the queue
void WorldSession::QueuePacket(WorldPacket* new_packet)
{
    _recvQueue.add(new_packet);
}

/// Logging helper for unexpected opcodes
void WorldSession::LogUnexpectedOpcode(WorldPacket* packet, const char *reason)
{
    sLog.outError( "SESSION: received unexpected opcode %s (0x%.4X) %s",
        LookupOpcodeName(packet->GetOpcode()),
        packet->GetOpcode(),
        reason);
}

/// Logging helper for unexpected opcodes
void WorldSession::LogUnprocessedTail(WorldPacket *packet)
{
    sLog.outError( "SESSION: opcode %s (0x%.4X) have unprocessed tail data (read stop at " SIZEFMTD " from " SIZEFMTD ")",
        LookupOpcodeName(packet->GetOpcode()),
        packet->GetOpcode(),
        packet->rpos(),packet->wpos());
}

/// Update the WorldSession (triggered by World update)
bool WorldSession::Update(PacketFilter& updater)
{
    ///- Retrieve packets from the receive queue and call the appropriate handlers
    /// not process packets if socket already closed
    WorldPacket* packet;

    time_t currentTime = time(NULL);

    while (m_Socket && !m_Socket->IsClosed() && _recvQueue.next(packet, updater))
    {
        /*#if 1
        sLog.outError( "MOEP: %s (0x%.4X)",
                        LookupOpcodeName(packet->GetOpcode()),
                        packet->GetOpcode());
        #endif*/

        OpcodeHandler const& opHandle = opcodeTable[packet->GetOpcode()];
        try
        {
            switch (opHandle.status)
            {
                case STATUS_LOGGEDIN:
                    if(!_player)
                    {
                        // skip STATUS_LOGGEDIN opcode unexpected errors if player logout sometime ago - this can be network lag delayed packets
                        if(!m_playerRecentlyLogout)
                            LogUnexpectedOpcode(packet, "the player has not logged in yet");
                    }
                    else if (_player->IsInWorld() && AntiDOS.EvaluateOpcode(*packet, currentTime))
                        ExecuteOpcode(opHandle, packet);

                    // lag can cause STATUS_LOGGEDIN opcodes to arrive after the player started a transfer
                    break;
                case STATUS_LOGGEDIN_OR_RECENTLY_LOGGEDOUT:
                    if(!_player && !m_playerRecentlyLogout)
                    {
                        LogUnexpectedOpcode(packet, "the player has not logged in yet and not recently logout");
                    }
                    else if (AntiDOS.EvaluateOpcode(*packet, currentTime))
                        // not expected _player or must checked in packet hanlder
                        ExecuteOpcode(opHandle, packet);
                    break;
                case STATUS_TRANSFER:
                    if(!_player)
                        LogUnexpectedOpcode(packet, "the player has not logged in yet");
                    else if(_player->IsInWorld())
                        LogUnexpectedOpcode(packet, "the player is still in world");
                    else if (AntiDOS.EvaluateOpcode(*packet, currentTime))
                        ExecuteOpcode(opHandle, packet);
                    break;
                case STATUS_AUTHED:
                    // prevent cheating with skip queue wait
                    if(m_inQueue)
                    {
                        LogUnexpectedOpcode(packet, "the player not pass queue yet");
                        break;
                    }

                    // single from authed time opcodes send in to after logout time
                    // and before other STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT opcodes.
                    m_playerRecentlyLogout = false;

                    if (AntiDOS.EvaluateOpcode(*packet, currentTime))
                        ExecuteOpcode(opHandle, packet);
                    break;
                case STATUS_NEVER:
                    sLog.outError( "SESSION: received not allowed opcode %s (0x%.4X)",
                        opHandle.name,
                        packet->GetOpcode());
                    break;
                case STATUS_UNHANDLED:
                    DEBUG_LOG("SESSION: received not handled opcode %s (0x%.4X)",
                        opHandle.name,
                        packet->GetOpcode());
                    break;
                default:
                    sLog.outError("SESSION: received wrong-status-req opcode %s (0x%.4X)",
                        opHandle.name,
                        packet->GetOpcode());
                    break;
            }
        }
        catch (ByteBufferException &)
        {
            sLog.outError("WorldSession::Update ByteBufferException occured while parsing a packet (opcode: %u) from client %s, accountid=%i.",
                    packet->GetOpcode(), GetRemoteAddress().c_str(), GetAccountId());
            if (sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG))
            {
                DEBUG_LOG("Dumping error causing packet:");
                packet->hexlike();
            }

            if (sWorld.getConfig(CONFIG_BOOL_KICK_PLAYER_ON_BAD_PACKET))
            {
                DETAIL_LOG("Disconnecting session [account id %u / address %s] for badly formatted packet.",
                    GetAccountId(), GetRemoteAddress().c_str());

                KickPlayer();
            }
        }

        delete packet;
    }

    ///- Cleanup socket pointer if need
    if (m_Socket && m_Socket->IsClosed())
    {
		if (m_Socket == nullptr)
			return false;

        m_Socket->RemoveReference();
        m_Socket = NULL;
    }

	if(m_Socket && !m_Socket->IsClosed() && m_Warden)
		m_Warden->Update();

    //check if we are safe to proceed with logout
    //logout procedure should happen only in World::UpdateSessions() method!!!
    if(updater.ProcessLogout())
    {
        ///- If necessary, log the player out
        time_t currTime = time(NULL);
        if (!m_Socket || (ShouldLogOut(currTime) && !m_playerLoading))
            LogoutPlayer(true);

        if (!m_Socket)
            return false;                                       //Will remove this session from the world session map
    }

    return true;
}

/// %Log the player out
void WorldSession::LogoutPlayer(bool Save)
{
    // finish pending transfers before starting the logout
    while(_player && _player->IsBeingTeleportedFar())
        HandleMoveWorldportAckOpcode();

    m_playerLogout = true;
    m_playerSave = Save;

    if (_player)
    {
        sLog.outChar("Account: %d (IP: %s) Logout Character:[%s] (guid: %u)", GetAccountId(), GetRemoteAddress().c_str(), _player->GetName() ,_player->GetGUIDLow());

        if (ObjectGuid lootGuid = GetPlayer()->GetLootGuid())
            DoLootRelease(lootGuid);

        ///- If the player just died before logging out, make him appear as a ghost
        //FIXME: logout must be delayed in case lost connection with client in time of combat
        if (_player->GetDeathTimer())
        {
            _player->getHostileRefManager().deleteReferences();
            _player->BuildPlayerRepop();
            _player->RepopAtGraveyard();
        }
        else if (_player->isInCombat())
        {
            _player->CombatStop();
            _player->getHostileRefManager().setOnlineOfflineState(false);
            _player->RemoveAllAurasOnDeath();

            // build set of player who attack _player or who have pet attacking of _player
            std::set<Player*> aset;
            for(Unit::AttackerSet::const_iterator itr = _player->getAttackers().begin(); itr != _player->getAttackers().end(); ++itr)
            {
				auto* map = _player->GetMap();
				if (!map)
					break;

				Unit* i = map->GetUnit(*itr);
				Unit* owner = i->GetOwner();           // including player controlled case
                if(owner)
                {
                    if(owner->GetTypeId()==TYPEID_PLAYER)
                        aset.insert((Player*)owner);
                }
                else
				{
					if(i->GetTypeId()==TYPEID_PLAYER)
						aset.insert((Player*)(*itr));
				}
            }

            _player->SetPvPDeath(!aset.empty());
            _player->KillPlayer();
            _player->BuildPlayerRepop();
            _player->RepopAtGraveyard();

            // give honor to all attackers from set like group case
            for(std::set<Player*>::const_iterator itr = aset.begin(); itr != aset.end(); ++itr)
                (*itr)->RewardHonor(_player,aset.size());

            // give bg rewards and update counters like kill by first from attackers
            // this can't be called for all attackers.
            if(!aset.empty())
                if(BattleGround *bg = _player->GetBattleGround())
                    bg->HandleKillPlayer(_player,*aset.begin());
        }
        else if(_player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
        {
            // this will kill character by SPELL_AURA_SPIRIT_OF_REDEMPTION
            _player->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);
            //_player->SetDeathPvP(*); set at SPELL_AURA_SPIRIT_OF_REDEMPTION apply time
            _player->KillPlayer();
            _player->BuildPlayerRepop();
            _player->RepopAtGraveyard();
        }
        //drop a flag if player is carrying it
        if(BattleGround *bg = _player->GetBattleGround())
            bg->EventPlayerLoggedOut(_player);

        ///- Teleport to home if the player is in an invalid instance
        if(!_player->m_InstanceValid && !_player->isGameMaster())
        {
            _player->TeleportToHomebind();
            //this is a bad place to call for far teleport because we need player to be in world for successful logout
            //maybe we should implement delayed far teleport logout?
        }

        // FG: finish pending transfers after starting the logout
        // this should fix players beeing able to logout and login back with full hp at death position
        while(_player->IsBeingTeleportedFar())
            HandleMoveWorldportAckOpcode();

        for (int i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
        {
            if(BattleGroundQueueTypeId bgQueueTypeId = _player->GetBattleGroundQueueTypeId(i))
            {
                _player->RemoveBattleGroundQueueId(bgQueueTypeId);
                sBattleGroundMgr.m_BattleGroundQueues[ bgQueueTypeId ].RemovePlayer(_player->GetObjectGuid(), true);
            }
        }

        ///- Reset the online field in the account table
        // no point resetting online in character table here as Player::SaveToDB() will set it to 1 since player has not been removed from world at this stage
        // No SQL injection as AccountID is uint32
        static SqlStatementID id;

        SqlStatement stmt = LoginDatabase.CreateStatement(id, "UPDATE account SET active_realm_id = ? WHERE id = ?");
        stmt.PExecute(uint32(0), GetAccountId());

        ///- If the player is in a guild, update the guild roster and broadcast a logout message to other guild members
        if (Guild* guild = sGuildMgr.GetGuildById(_player->GetGuildId()))
        {
            if (MemberSlot* slot = guild->GetMemberSlot(_player->GetObjectGuid()))
            {
                slot->SetMemberStats(_player);
                slot->UpdateLogoutTime();
            }

            guild->BroadcastEvent(GE_SIGNED_OFF, _player->GetObjectGuid(), _player->GetName());
        }

        ///- Remove pet
        _player->RemovePet(PET_SAVE_AS_CURRENT);

        ///- empty buyback items and save the player in the database
        // some save parts only correctly work in case player present in map/player_lists (pets, etc)
        if(Save)
            _player->SaveToDB();

        ///- Leave all channels before player delete...
        _player->CleanupChannels();

        ///- If the player is in a group (or invited), remove him. If the group if then only 1 person, disband the group.
        _player->UninviteFromGroup();

        // remove player from the group if he is:
        // a) in group; b) not in raid group; c) logging out normally (not being kicked or disconnected)
        if(_player->GetGroup() && !_player->GetGroup()->isRaidGroup() && m_Socket)
            _player->RemoveFromGroup();

        ///- Send update to group
        if(_player->GetGroup())
            _player->GetGroup()->SendUpdate();

        ///- Broadcast a logout message to the player's friends
        sSocialMgr.SendFriendStatus(_player, FRIEND_OFFLINE, _player->GetObjectGuid(), true);
        sSocialMgr.RemovePlayerSocial (_player->GetGUIDLow ());

		///- Used by Eluna
		sEluna->OnLogout(_player);

        ///- Remove the player from the world
        // the player may not be in the world when logging out
        // e.g if he got disconnected during a transfer to another map
        // calls to GetMap in this case may cause crashes
        if (_player->IsInWorld())
        {
            Map* _map = _player->GetMap();
            _map->Remove(_player, true);
        }
        else
        {
            _player->CleanupsBeforeDelete();
            Map::DeleteFromWorld(_player);
        }

        SetPlayer(NULL);                                    // deleted in Remove/DeleteFromWorld call

        ///- Send the 'logout complete' packet to the client
        WorldPacket data( SMSG_LOGOUT_COMPLETE, 0 );
        SendPacket( &data );

        ///- Since each account can only have one online character at any given time, ensure all characters for active account are marked as offline
        //No SQL injection as AccountId is uint32

        static SqlStatementID updChars;

        stmt = CharacterDatabase.CreateStatement(updChars, "UPDATE characters SET online = 0 WHERE account = ?");
        stmt.PExecute(GetAccountId());

        DEBUG_LOG( "SESSION: Sent SMSG_LOGOUT_COMPLETE Message" );
    }

    m_playerLogout = false;
    m_playerSave = false;
    m_playerRecentlyLogout = true;
    LogoutRequest(0);
}

/// Kick a player out of the World
void WorldSession::KickPlayer()
{
	if (_player)
	{
		_player->InterruptNonMeleeSpells(true);
		_player->SaveToDB();
	}

    if (m_Socket)
        m_Socket->CloseSocket();
}

/// Cancel channeling handler
void WorldSession::SendAreaTriggerMessage(const char* Text, ...)
{
    va_list ap;
    char szStr [1024];
    szStr[0] = '\0';

    va_start(ap, Text);
    vsnprintf( szStr, 1024, Text, ap );
    va_end(ap);

    uint32 length = strlen(szStr)+1;
    WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 4+length);
    data << length;
    data << szStr;
    SendPacket(&data);
}

void WorldSession::SendNotification(const char *format,...)
{
    if(format)
    {
        va_list ap;
        char szStr [1024];
        szStr[0] = '\0';
        va_start(ap, format);
        vsnprintf( szStr, 1024, format, ap );
        va_end(ap);

        WorldPacket data(SMSG_NOTIFICATION, (strlen(szStr)+1));
        data << szStr;
        SendPacket(&data);
    }
}

void WorldSession::SendNotification(int32 string_id,...)
{
    char const* format = GetMangosString(string_id);
    if(format)
    {
        va_list ap;
        char szStr [1024];
        szStr[0] = '\0';
        va_start(ap, string_id);
        vsnprintf( szStr, 1024, format, ap );
        va_end(ap);

        WorldPacket data(SMSG_NOTIFICATION, (strlen(szStr)+1));
        data << szStr;
        SendPacket(&data);
    }
}

const char * WorldSession::GetMangosString( int32 entry ) const
{
    return sObjectMgr.GetMangosString(entry,GetSessionDbLocaleIndex());
}

void WorldSession::Handle_NULL( WorldPacket& recvPacket )
{
    DEBUG_LOG("SESSION: received unimplemented opcode %s (0x%.4X)",
        LookupOpcodeName(recvPacket.GetOpcode()),
        recvPacket.GetOpcode());
}

void WorldSession::Handle_EarlyProccess( WorldPacket& recvPacket )
{
    sLog.outError( "SESSION: received opcode %s (0x%.4X) that must be processed in WorldSocket::OnRead",
        LookupOpcodeName(recvPacket.GetOpcode()),
        recvPacket.GetOpcode());
}

void WorldSession::Handle_ServerSide( WorldPacket& recvPacket )
{
    sLog.outError("SESSION: received server-side opcode %s (0x%.4X)",
        LookupOpcodeName(recvPacket.GetOpcode()),
        recvPacket.GetOpcode());
}

void WorldSession::Handle_Deprecated( WorldPacket& recvPacket )
{
    sLog.outError( "SESSION: received deprecated opcode %s (0x%.4X)",
        LookupOpcodeName(recvPacket.GetOpcode()),
        recvPacket.GetOpcode());
}

void WorldSession::SendAuthWaitQue(uint32 position)
{
    if(position == 0)
    {
        WorldPacket packet( SMSG_AUTH_RESPONSE, 1 );
        packet << uint8( AUTH_OK );
        SendPacket(&packet);
    }
    else
    {
        WorldPacket packet( SMSG_AUTH_RESPONSE, 6 );
        packet << uint8( AUTH_WAIT_QUEUE );
		packet << uint32(position);
		packet << uint8(0); //unk
        SendPacket(&packet);
    }
}

void WorldSession::LoadTutorialsData()
{
    for ( int aX = 0 ; aX < 8 ; ++aX )
        m_Tutorials[ aX ] = 0;

    QueryResult *result = CharacterDatabase.PQuery("SELECT tut0,tut1,tut2,tut3,tut4,tut5,tut6,tut7 FROM character_tutorial WHERE account = '%u'", GetAccountId());

    if(!result)
    {
        m_tutorialState = TUTORIALDATA_NEW;
        return;
    }

    do
    {
        Field *fields = result->Fetch();

        for (int iI = 0; iI < 8; ++iI)
            m_Tutorials[iI] = fields[iI].GetUInt32();
    }
    while( result->NextRow() );

    delete result;

    m_tutorialState = TUTORIALDATA_UNCHANGED;
}

void WorldSession::SendTutorialsData()
{
    WorldPacket data(SMSG_TUTORIAL_FLAGS, 4*8);
    for(uint32 i = 0; i < 8; ++i)
        data << m_Tutorials[i];
    SendPacket(&data);
}

void WorldSession::SaveTutorialsData()
{
    static SqlStatementID updTutorial ;
    static SqlStatementID insTutorial ;

    switch(m_tutorialState)
    {
        case TUTORIALDATA_CHANGED:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(updTutorial, "UPDATE character_tutorial SET tut0=?, tut1=?, tut2=?, tut3=?, tut4=?, tut5=?, tut6=?, tut7=? WHERE account = ?");
                for (int i = 0; i < 8; ++i)
                    stmt.addUInt32(m_Tutorials[i]);

                stmt.addUInt32(GetAccountId());
                stmt.Execute();
            }
            break;

        case TUTORIALDATA_NEW:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insTutorial, "INSERT INTO character_tutorial (account,tut0,tut1,tut2,tut3,tut4,tut5,tut6,tut7) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");

                stmt.addUInt32(GetAccountId());
                for (int i = 0; i < 8; ++i)
                    stmt.addUInt32(m_Tutorials[i]);

                stmt.Execute();
            }
            break;
        case TUTORIALDATA_UNCHANGED:
            break;
    }

    m_tutorialState = TUTORIALDATA_UNCHANGED;
}

void WorldSession::InitWarden(BigNumber* K, std::string os)
{
	if(os == "Win")
		m_Warden = reinterpret_cast<WardenBase*>(new WardenWin());
	else
		m_Warden = reinterpret_cast<WardenBase*>(new WardenMac());

	m_Warden->Init(this, K);
}

void WorldSession::ExecuteOpcode( OpcodeHandler const& opHandle, WorldPacket* packet )
{
	if (!sEluna->OnPacketReceive(this, *packet))
		return;

    // need prevent do internal far teleports in handlers because some handlers do lot steps
    // or call code that can do far teleports in some conditions unexpectedly for generic way work code
    if (_player)
        _player->SetCanDelayTeleport(true);

    (this->*opHandle.handler)(*packet);

    if (_player)
    {
        // can be not set in fact for login opcode, but this not create porblems.
        _player->SetCanDelayTeleport(false);

        //we should execute delayed teleports only for alive(!) players
        //because we don't want player's ghost teleported from graveyard
        if (_player->IsHasDelayedTeleport())
            _player->TeleportTo(_player->m_teleport_dest, _player->m_teleport_options);
    }

    if (packet->rpos() < packet->wpos() && sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG))
        LogUnprocessedTail(packet);
}

void WorldSession::SendPlaySpellVisual(ObjectGuid guid, uint32 spellArtKit)
{
	WorldPacket data(SMSG_PLAY_SPELL_VISUAL, 8 + 4); // visual effect on guid
	data << guid;
	data << spellArtKit; // index from SpellVisualKit.dbc
	SendPacket(&data);
}

bool WorldSession::DosProtection::EvaluateOpcode(WorldPacket& p, time_t time, uint32 factor) const
{
    uint32 maxPacketCounterAllowed = GetMaxPacketCounterAllowed(p.GetOpcode());
    // Return true if there no limit for the opcode
    if (!maxPacketCounterAllowed)
        return true;
    PacketCounter& packetCounter = _PacketThrottlingMap[p.GetOpcode()];
    if (packetCounter.lastReceiveTime != time)
    {
        packetCounter.lastReceiveTime = time;
        packetCounter.amountCounter = 0;
    }
    packetCounter.amountCounter += factor;
    // Check if player is flooding some packets
    if (packetCounter.amountCounter <= maxPacketCounterAllowed)
        return true;
    sLog.outWarden("network", "AntiDOS: Account %u, IP: %s, Ping: %u, Character: %s, flooding packet (opc: %s (0x%X), count: %u)",
        Session->GetAccountId(), Session->GetRemoteAddress().c_str(), Session->GetLatency(), Session->GetPlayerName(),
        opcodeTable[p.GetOpcode()].name, p.GetOpcode(), packetCounter.amountCounter);
    switch (_policy)
    {
    case POLICY_LOG:
        return true;
    case POLICY_KICK:
    {
        sLog.outWarden("network", "AntiDOS: Player kicked!");
        Session->KickPlayer();
        return false;
    }
    case POLICY_BAN:
    {
        BanMode bm = (BanMode)sWorld.getConfig(CONFIG_UINT32_PACKET_SPOOF_BANMODE);
        uint32 duration = sWorld.getConfig(CONFIG_UINT32_PACKET_SPOOF_BANDURATION); // in seconds
        std::string nameOrIp = "";
        switch (bm)
        {
        case BAN_CHARACTER: // not supported, ban account
        case BAN_ACCOUNT: (void)sAccountMgr.GetName(Session->GetAccountId(), nameOrIp); break;
        case BAN_IP: nameOrIp = Session->GetRemoteAddress(); break;
        }
        sWorld.BanAccount(bm, nameOrIp, duration, "DOS (Packet Flooding/Spoofing", "Server: AutoDOS");
        sLog.outWarden("network", "AntiDOS: Player automatically banned for %u seconds.", duration);
        Session->KickPlayer();
        return false;
    }
    default: // invalid policy
        return true;
    }
}
uint32 WorldSession::DosProtection::GetMaxPacketCounterAllowed(uint16 opcode) const
{
    uint32 maxPacketCounterAllowed;
    switch (opcode)
    {
        // CPU usage sending 2000 packets/second on a 3.70 GHz 4 cores on Win x64
        // [% CPU mysqld] [%CPU worldserver RelWithDebInfo]
    case CMSG_PLAYER_LOGIN: // 0 0.5
    case CMSG_PET_NAME_QUERY: // 0 1
    case CMSG_NPC_TEXT_QUERY: // 0 1
    case CMSG_ATTACKSTOP: // 0 1
    //case CMSG_QUERY_QUESTS_COMPLETED: // 0 1
    case CMSG_QUERY_TIME: // 0 1
    //case CMSG_CORPSE_MAP_POSITION_QUERY: // 0 1
    case CMSG_MOVE_TIME_SKIPPED: // 0 1
    case MSG_QUERY_NEXT_MAIL_TIME: // 0 1
    case CMSG_SETSHEATHED: // 0 1
    case MSG_RAID_TARGET_UPDATE: // 0 1
    case CMSG_PLAYER_LOGOUT: // 0 1
    case CMSG_LOGOUT_REQUEST: // 0 1
    case CMSG_PET_RENAME: // 0 1
    case CMSG_QUESTGIVER_CANCEL: // 0 1
    case CMSG_QUESTGIVER_REQUEST_REWARD: // 0 1
    case CMSG_COMPLETE_CINEMATIC: // 0 1
    case CMSG_BANKER_ACTIVATE: // 0 1
    case CMSG_BUY_BANK_SLOT: // 0 1
    case CMSG_OPT_OUT_OF_LOOT: // 0 1
    case CMSG_DUEL_ACCEPTED: // 0 1
    case CMSG_DUEL_CANCELLED: // 0 1
    //case CMSG_CALENDAR_COMPLAIN: // 0 1
    case CMSG_QUEST_QUERY: // 0 1.5
    case CMSG_ITEM_QUERY_SINGLE: // 0 1.5
    case CMSG_ITEM_NAME_QUERY: // 0 1.5
    case CMSG_GAMEOBJECT_QUERY: // 0 1.5
    case CMSG_CREATURE_QUERY: // 0 1.5
    case CMSG_QUESTGIVER_STATUS_QUERY: // 0 1.5
    case CMSG_GUILD_QUERY: // 0 1.5
    //case CMSG_ARENA_TEAM_QUERY: // 0 1.5
    case CMSG_TAXINODE_STATUS_QUERY: // 0 1.5
    case CMSG_TAXIQUERYAVAILABLENODES: // 0 1.5
    case CMSG_QUESTGIVER_QUERY_QUEST: // 0 1.5
    case CMSG_PAGE_TEXT_QUERY: // 0 1.5
    //case MSG_QUERY_GUILD_BANK_TEXT: // 0 1.5
    case MSG_CORPSE_QUERY: // 0 1.5
    case MSG_MOVE_SET_FACING: // 0 1.5
    case CMSG_REQUEST_PARTY_MEMBER_STATS: // 0 1.5
    case CMSG_QUESTGIVER_COMPLETE_QUEST: // 0 1.5
    case CMSG_SET_ACTION_BUTTON: // 0 1.5
    case CMSG_RESET_INSTANCES: // 0 1.5
    //case CMSG_HEARTH_AND_RESURRECT: // 0 1.5
    case CMSG_TOGGLE_PVP: // 0 1.5
    case CMSG_PET_ABANDON: // 0 1.5
    case CMSG_ACTIVATETAXIEXPRESS: // 0 1.5
    case CMSG_ACTIVATETAXI: // 0 1.5
    case CMSG_SELF_RES: // 0 1.5
    case CMSG_UNLEARN_SKILL: // 0 1.5
    //case CMSG_EQUIPMENT_SET_SAVE: // 0 1.5
    //case CMSG_DELETEEQUIPMENT_SET: // 0 1.5
    //case CMSG_DISMISS_CRITTER: // 0 1.5
    case CMSG_REPOP_REQUEST: // 0 1.5
    case CMSG_GROUP_INVITE: // 0 1.5
    case CMSG_GROUP_DECLINE: // 0 1.5
    case CMSG_GROUP_ACCEPT: // 0 1.5
    case CMSG_GROUP_UNINVITE_GUID: // 0 1.5
    case CMSG_GROUP_UNINVITE: // 0 1.5
    case CMSG_GROUP_DISBAND: // 0 1.5
    //case CMSG_BATTLEMASTER_JOIN_ARENA: // 0 1.5
    case CMSG_LEAVE_BATTLEFIELD: // 0 1.5
    //case MSG_GUILD_BANK_LOG_QUERY: // 0 2
    case CMSG_LOGOUT_CANCEL: // 0 2
    //case CMSG_REALM_SPLIT: // 0 2
    //case CMSG_ALTER_APPEARANCE: // 0 2
    case CMSG_QUEST_CONFIRM_ACCEPT: // 0 2
    case MSG_GUILD_EVENT_LOG_QUERY: // 0 2.5
    //case CMSG_READY_FOR_ACCOUNT_DATA_TIMES: // 0 2.5
    case CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY: // 0 2.5
    case CMSG_BEGIN_TRADE: // 0 2.5
    case CMSG_INITIATE_TRADE: // 0 3
    case CMSG_MESSAGECHAT: // 0 3.5
    case CMSG_INSPECT: // 0 3.5
    case CMSG_AREA_SPIRIT_HEALER_QUERY: // not profiled
    case CMSG_STANDSTATECHANGE: // not profiled
    case MSG_RANDOM_ROLL: // not profiled
    //case CMSG_TIME_SYNC_RESP: // not profiled
    case CMSG_TRAINER_BUY_SPELL: // not profiled
    {
        // "0" is a magic number meaning there's no limit for the opcode.
        // All the opcodes above must cause little CPU usage and no sync/async database queries at all
        maxPacketCounterAllowed = 0;
        break;
    }
    case CMSG_NAME_QUERY: // 0 1
    {
        maxPacketCounterAllowed = 1000;
        break;
    }
    case CMSG_QUESTGIVER_ACCEPT_QUEST: // 0 4
    case CMSG_QUESTLOG_REMOVE_QUEST: // 0 4
    case CMSG_QUESTGIVER_CHOOSE_REWARD: // 0 4
    //case CMSG_CONTACT_LIST: // 0 5
    //case CMSG_LEARN_PREVIEW_TALENTS: // 0 6
    case CMSG_AUTOBANK_ITEM: // 0 6
    case CMSG_AUTOSTORE_BANK_ITEM: // 0 6
    case CMSG_WHO: // 0 7
    //case CMSG_PLAYER_VEHICLE_ENTER: // 0 8
    //case CMSG_LEARN_PREVIEW_TALENTS_PET: // not profiled
    case MSG_MOVE_HEARTBEAT:
    {
        maxPacketCounterAllowed = 200;
        break;
    }
    case CMSG_GUILD_SET_PUBLIC_NOTE: // 1 2 1 async db query
    case CMSG_GUILD_SET_OFFICER_NOTE: // 1 2 1 async db query
    //case CMSG_SET_CONTACT_NOTES: // 1 2.5 1 async db query
    //case CMSG_CALENDAR_GET_CALENDAR: // 0 1.5 medium upload bandwidth usage
    //case CMSG_GUILD_BANK_QUERY_TAB: // 0 3.5 medium upload bandwidth usage
    //case CMSG_QUERY_INSPECT_ACHIEVEMENTS: // 0 13 high upload bandwidth usage
    //case CMSG_GAMEOBJ_REPORT_USE: // not profiled
    case CMSG_GAMEOBJ_USE: // not profiled
    case MSG_PETITION_DECLINE: // not profiled
    {
        maxPacketCounterAllowed = 50;
        break;
    }
    //case CMSG_QUEST_POI_QUERY: // 0 25 very high upload bandwidth usage
    //{
    //    maxPacketCounterAllowed = MAX_QUEST_LOG_SIZE;
    //    break;
    //}
    //case CMSG_GM_REPORT_LAG: // 1 3 1 async db query
    case CMSG_SPELLCLICK: // not profiled
    //case CMSG_REMOVE_GLYPH: // not profiled
    //case CMSG_DISMISS_CONTROLLED_VEHICLE: // not profiled
    {
        maxPacketCounterAllowed = 20;
        break;
    }
    case CMSG_PETITION_SIGN: // 9 4 2 sync 1 async db queries
    case CMSG_TURN_IN_PETITION: // 8 5.5 2 sync db query
    case CMSG_GROUP_CHANGE_SUB_GROUP: // 6 5 1 sync 1 async db queries
    case CMSG_PETITION_QUERY: // 4 3.5 1 sync db query
    //case CMSG_CHAR_RACE_CHANGE: // 5 4 1 sync db query
    //case CMSG_CHAR_CUSTOMIZE: // 5 5 1 sync db query
    //case CMSG_CHAR_FACTION_CHANGE: // 5 5 1 sync db query
    case CMSG_CHAR_DELETE: // 4 4 1 sync db query
    case CMSG_DEL_FRIEND: // 7 5 1 async db query
    case CMSG_ADD_FRIEND: // 6 4 1 async db query
    case CMSG_CHAR_RENAME: // 5 3 1 async db query
    case CMSG_GMSURVEY_SUBMIT: // 2 3 1 async db query
    case CMSG_BUG: // 1 1 1 async db query
    case CMSG_GROUP_SET_LEADER: // 1 2 1 async db query
    case CMSG_GROUP_RAID_CONVERT: // 1 5 1 async db query
    case CMSG_GROUP_ASSISTANT_LEADER: // 1 2 1 async db query
    //case CMSG_CALENDAR_ADD_EVENT: // 21 10 2 async db query
    case CMSG_PETITION_BUY: // not profiled 1 sync 1 async db queries
    //case CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE: // not profiled
    //case CMSG_REQUEST_VEHICLE_PREV_SEAT: // not profiled
    //case CMSG_REQUEST_VEHICLE_NEXT_SEAT: // not profiled
    //case CMSG_REQUEST_VEHICLE_SWITCH_SEAT: // not profiled
    //case CMSG_REQUEST_VEHICLE_EXIT: // not profiled
    //case CMSG_CONTROLLER_EJECT_PASSENGER: // not profiled
    //case CMSG_ITEM_REFUND: // not profiled
    //case CMSG_SOCKET_GEMS: // not profiled
    case CMSG_WRAP_ITEM: // not profiled
    //case CMSG_REPORT_PVP_AFK: // not profiled
    {
        maxPacketCounterAllowed = 10;
        break;
    }
    case CMSG_CHAR_CREATE: // 7 5 3 async db queries
    case CMSG_CHAR_ENUM: // 22 3 2 async db queries
    case CMSG_GMTICKET_CREATE: // 1 25 1 async db query
    case CMSG_GMTICKET_UPDATETEXT: // 0 15 1 async db query
    case CMSG_GMTICKET_DELETETICKET: // 1 25 1 async db query
    //case CMSG_GMRESPONSE_RESOLVE: // 1 25 1 async db query
    //case CMSG_CALENDAR_UPDATE_EVENT: // not profiled
    //case CMSG_CALENDAR_REMOVE_EVENT: // not profiled
    //case CMSG_CALENDAR_COPY_EVENT: // not profiled
    //case CMSG_CALENDAR_EVENT_INVITE: // not profiled
    //case CMSG_CALENDAR_EVENT_SIGNUP: // not profiled
    //case CMSG_CALENDAR_EVENT_RSVP: // not profiled
    //case CMSG_CALENDAR_EVENT_REMOVE_INVITE: // not profiled
    //case CMSG_CALENDAR_EVENT_MODERATOR_STATUS: // not profiled
    //case CMSG_ARENA_TEAM_INVITE: // not profiled
    //case CMSG_ARENA_TEAM_ACCEPT: // not profiled
    //case CMSG_ARENA_TEAM_DECLINE: // not profiled
    //case CMSG_ARENA_TEAM_LEAVE: // not profiled
    //case CMSG_ARENA_TEAM_DISBAND: // not profiled
    //case CMSG_ARENA_TEAM_REMOVE: // not profiled
    //case CMSG_ARENA_TEAM_LEADER: // not profiled
    case CMSG_LOOT_METHOD: // not profiled
    case CMSG_GUILD_INVITE: // not profiled
    case CMSG_GUILD_ACCEPT: // not profiled
    case CMSG_GUILD_DECLINE: // not profiled
    case CMSG_GUILD_LEAVE: // not profiled
    case CMSG_GUILD_DISBAND: // not profiled
    case CMSG_GUILD_LEADER: // not profiled
    case CMSG_GUILD_MOTD: // not profiled
    case CMSG_GUILD_RANK: // not profiled
    case CMSG_GUILD_ADD_RANK: // not profiled
    case CMSG_GUILD_DEL_RANK: // not profiled
    case CMSG_GUILD_INFO_TEXT: // not profiled
    //case CMSG_GUILD_BANK_DEPOSIT_MONEY: // not profiled
    //case CMSG_GUILD_BANK_WITHDRAW_MONEY: // not profiled
    //case CMSG_GUILD_BANK_BUY_TAB: // not profiled
    //case CMSG_GUILD_BANK_UPDATE_TAB: // not profiled
    //case CMSG_SET_GUILD_BANK_TEXT: // not profiled
    case MSG_SAVE_GUILD_EMBLEM: // not profiled
    case MSG_PETITION_RENAME: // not profiled
    case MSG_TALENT_WIPE_CONFIRM: // not profiled
    //case MSG_SET_DUNGEON_DIFFICULTY: // not profiled
    //case MSG_SET_RAID_DIFFICULTY: // not profiled
    case MSG_PARTY_ASSIGNMENT: // not profiled
    case MSG_RAID_READY_CHECK: // not profiled
    {
        maxPacketCounterAllowed = 3;
        break;
    }
    //case CMSG_ITEM_REFUND_INFO: // not profiled
    //{
    //    maxPacketCounterAllowed = PLAYER_SLOTS_COUNT;
    //    break;
    //}
    default:
    {
        maxPacketCounterAllowed = 100;
        break;
    }
    }
    return maxPacketCounterAllowed;
}