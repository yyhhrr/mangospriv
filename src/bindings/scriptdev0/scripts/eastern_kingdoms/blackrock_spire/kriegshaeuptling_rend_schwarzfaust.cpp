/* kriegshaeuptling_rend_schwarzfaust */

#include "precompiled.h"
#include "blackrock_spire.h"

#define raserei	8269
#define spalten	15284
#define toedlicher_stoss	15708
#define wirbelwind	15589
#define wirbelwind_kanalisieren	13736

#define CDB 10447
#define CW	10442
#define GDF	10742




struct MANGOS_DLL_DECL kriegshaeuptling_rend_schwarzfaustAI : public ScriptedAI
{
    kriegshaeuptling_rend_schwarzfaustAI(Creature *c) : ScriptedAI(c)
    {
        m_pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (m_creature->GetPositionZ() > 120)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            EventMode = true;
        }
        else
            EventMode = false;

        Reset();
    }
    ScriptedInstance* m_pInstance;
    Unit* target;
    Creature* tmp;

    uint32 doorstatus;

    uint32 raserei_timer;
    uint32 spalten_timer;
    uint32 toedlicher_stoss_timer;
    uint32 wirbelwind_timer;
    uint32 wirbelwind_kanalisieren_timer;

    uint64 BigDoor;

    bool EventMode;
    uint32 eventphase;
    uint32 eventtimer;

    void Reset()
    {
        raserei_timer = 10000;
        spalten_timer = 5000;
        toedlicher_stoss_timer = 8000;
        wirbelwind_timer = 10000;
        wirbelwind_kanalisieren_timer = 20000;
        eventphase=0;
        eventtimer=1000;
        doorstatus = 0;

    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Habt ihr auch eure Klingen geschaerft ?",LANG_UNIVERSAL,NULL);
    }

    void JustDied(Unit* Killer)
    {
//	m_creature->Yell("SIR VICROR!!!!",LANG_UNIVERSAL,0);
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        if (pInstance)
            pInstance->SetData(TYPE_REND_SCHWARZFAUST,DONE);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_REND_SCHWARZFAUST,FAIL);
            m_creature->ForcedDespawn(0);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        Map* pmap;
        pmap= m_creature->GetMap();

        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (EventMode==true && pInstance->GetData(TYPE_REND_SCHWARZFAUST) == FAIL)
        {
//		m_creature->Yell("IHR VERSAGER !",LANG_UNIVERSAL,0);
            m_creature->SetVisibility(VISIBILITY_ON);
            if (doorstatus==1) pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
            Reset();
            pInstance->SetData(TYPE_REND_SCHWARZFAUST,SPECIAL);



        }
        if (EventMode==true && pInstance->GetData(TYPE_REND_SCHWARZFAUST) == IN_PROGRESS)
        {

            Player* pe;
            pe=pInstance->instance->GetPlayer(pInstance->GetData64(DATA_PLAYER));
            if (!pe || pe->isDead())
            {
                debug_log("SD2: pe");
                Map::PlayerList const& players = pInstance->instance->GetPlayers();

                if (!players.isEmpty())
                {
                    debug_log("SD2: pe");
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        debug_log("SD2: pex");
                        if (Player* pPlayer = itr->getSource())
                        {
                            debug_log("SD2: pea");
                            if (pPlayer->isAlive())
                            {
                                pInstance->SetData64(DATA_PLAYER,pPlayer->GetGUID());
                                debug_log("SD2: peb");
                                return;
                            }
                        }
                    }
                    pInstance->SetData(TYPE_REND_SCHWARZFAUST,FAIL);
                    return;
                }
            }

            if (eventtimer < diff)
            {

                {
                    switch (eventphase)
                    {

                    case 0:
//					m_creature->Yell("Seht euch diese Weicheier an, Sir Victor! Lasst das Spektakel beginnen!",LANG_UNIVERSAL,0);
                        eventphase++;
                        eventtimer = 5000;
                        break;
                        // WELLE 1
                    case 1:
                        // door open
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=1;
                        eventphase++;
                        eventtimer = 2000;
                        break;
                    case 2:
                        // spawn welle 1
                        tmp=m_creature->SummonCreature(10447,195,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-424,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-416,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        eventphase++;
                        eventtimer = 3500;
                        break;
                    case 3:
                        // door close
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=0;
                        eventphase++;
                        eventtimer = 40000;
                        break;

                        // WELLE 2
                    case 4:
                        // door open
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=1;
                        eventphase++;
                        eventtimer = 2000;
                        break;
                    case 5:
                        // spawn welle 2
                        tmp=m_creature->SummonCreature(10447,195,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-425,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-422,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-418,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-414,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        eventphase++;
                        eventtimer = 3500;
                        break;
                    case 6:
                        // door close
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=0;
                        eventphase++;
                        eventtimer = 38000;
                        break;

                        // WELLE 3
                    case 7:
                        // door open
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=1;
                        eventphase++;
                        eventtimer = 2000;
                        break;
                    case 8:
                        tmp=m_creature->SummonCreature(10447,195,-424,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10742,195,-416,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-424,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-416,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        eventphase++;
                        eventtimer = 3500;
                        break;
                    case 9:
                        // door close
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=0;
                        eventphase++;
                        eventtimer = 38000;
                        break;

                        // WELLE 4
                    case 10:
                        // door open
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=1;
                        eventphase++;
                        eventtimer = 2000;
                        break;
                    case 11:
                        // spawn welle 4
                        tmp=m_creature->SummonCreature(10447,195,-424,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10742,195,-416,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-425,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-422,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-418,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,199,-414,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        eventphase++;
                        eventtimer = 3500;
                        break;
                    case 12:
                        // door close
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=0;
                        eventphase++;
                        eventtimer = 38000;
                        break;

                        // WELLE 5
                    case 13:
                        // door open
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=1;
                        eventphase++;
                        eventtimer = 2000;
                        break;
                    case 14:
                        // spawn welle 5
                        tmp=m_creature->SummonCreature(10447,199,-425,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10742,199,-422,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10447,199,-418,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10742,199,-414,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-424,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-416,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        eventphase++;
                        eventtimer = 3500;
                        break;
                    case 15:
                        // door close
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=0;
                        eventphase++;
                        eventtimer = 38000;
                        break;

                        // WELLE 6
                    case 16:
                        // door open
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=1;
                        eventphase++;
                        eventtimer = 2000;
                        break;
                    case 17:
                        // spawn welle 6
                        tmp=m_creature->SummonCreature(10447,199,-425,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10742,199,-422,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10447,199,-418,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10742,199,-414,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,195,-424,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,195,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,195,-416,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-424,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        tmp=m_creature->SummonCreature(10442,200,-416,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                        tmp->AI()->AttackStart(pe);
                        eventphase++;
                        eventtimer = 3500;
                        break;
                    case 18:
                        // door close
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=0;
                        eventphase++;
                        eventtimer = 48000;
                        break;

                        // BOSS
                    case 19:
                        // door open
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=1;
						m_creature->MonsterYell("Nun kuemmere ich mich persoenlich um diese Nichtsnutze!",LANG_UNIVERSAL,0);
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        eventphase++;
                        eventtimer = 2000;
                        break;
                    case 20:
                        // spawn boss
                        tmp=m_creature->SummonCreature(10339,200,-420,111,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,600000);
                        tmp->AI()->AttackStart(pe);
                        eventphase++;
                        eventtimer = 3500;
                        break;
                    case 21:
                        // door close
                        pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR),0,false);
                        doorstatus=0;
                        eventphase++;
                        eventtimer = 1000;
                        break;

                    }
                }
            } else eventtimer-=diff;
            return;
        }
        else
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            {
                return;
            }

            if (raserei_timer < diff)
            {
                DoCastSpellIfCan(m_creature,raserei);
                raserei_timer = random(120000,120000);
            } else raserei_timer -= diff;

            if (spalten_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),spalten);
                spalten_timer = random(8000,12000);
            } else spalten_timer -= diff;

            if (toedlicher_stoss_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),toedlicher_stoss);
                toedlicher_stoss_timer = random(15000,20000);
            } else toedlicher_stoss_timer -= diff;

            if (wirbelwind_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),wirbelwind);
                wirbelwind_timer = random(15000,15000);
            } else wirbelwind_timer -= diff;

            if (wirbelwind_kanalisieren_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),wirbelwind_kanalisieren);
                wirbelwind_kanalisieren_timer = random(15000,15000);
            } else wirbelwind_kanalisieren_timer -= diff;


            DoMeleeAttackIfReady();

        }
    }
};

CreatureAI* GetAI_kriegshaeuptling_rend_schwarzfaust(Creature *_Creature)
{
    return new kriegshaeuptling_rend_schwarzfaustAI (_Creature);
}

void AddSC_kriegshaeuptling_rend_schwarzfaust()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="kriegshaeuptling_rend_schwarzfaust";
    newscript->GetAI = &GetAI_kriegshaeuptling_rend_schwarzfaust;
    newscript->RegisterSelf();
}

