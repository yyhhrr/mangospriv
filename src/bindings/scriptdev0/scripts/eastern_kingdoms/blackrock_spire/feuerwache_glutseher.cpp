/*
#################################################################################
#   ____  _ _   _        ____                 _                          _		#
#  / ___|(_) \ | |      / ___| __ _ _ __ ___ (_)_ __   __ _   _ __   ___| |_	#
#  \___ \| |  \| |_____| |  _ / _` | '_ ` _ \| | '_ \ / _` | | '_ \ / _ \ __|	#
#   ___) | | |\  |_____| |_| | (_| | | | | | | | | | | (_| |_| | | |  __/ |_	#
#  |____/|_|_| \_|      \____|\__,_|_| |_| |_|_|_| |_|\__, (_)_| |_|\___|\__|	#
#                                                     |___/						#
#																				#
#														 Schwarzfelsspitze		#
#												   		Feuerwache Glutseher	#
#################################################################################

ScriptName: Feuerwache Glutseher
Instanz: Flammenschlund
Staus in %: 100%
Abgeschlossen?: Ja
*/

#include "precompiled.h"
#include "blackrock_spire.h"

#define SPELL_FEUERNOVA          16079
#define SPELL_FLAMEBUFFET       16536


struct MANGOS_DLL_DECL feuerwache_glutseherAI : public ScriptedAI
{
    feuerwache_glutseherAI(Creature* pCreature) : ScriptedAI(pCreature) {
        Reset();
    }

    uint32 FeuerNova_Timer;
    uint32 FlameBuffet_Timer;
    uint32 feuerwache_glutseher_timer;
    bool free;
    void Reset()
    {
        FeuerNova_Timer = 6000;
        FlameBuffet_Timer = 3000;
        feuerwache_glutseher_timer = 30000;
        free=false;
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_ALTAR_EVENT,DONE);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_ALTAR_EVENT,FAIL);
            m_creature->setFaction(734);
            m_creature->SetVisibility(VISIBILITY_OFF);
        }
    }


    void UpdateAI(const uint32 diff)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (pInstance->GetData(TYPE_ALTAR_EVENT)==IN_PROGRESS)
        {

            if ( feuerwache_glutseher_timer < diff)
            {
				m_creature->MonsterYell("Gut so, lasst mich frei!",LANG_UNIVERSAL,0);
                m_creature->setFaction(754);
                Player* pe;
                if (pe=pInstance->instance->GetPlayer(pInstance->GetData64(DATA_PLAYER)))
                    m_creature->AI()->AttackStart(pe);
            }feuerwache_glutseher_timer -= diff;
        }
        else
        {
            m_creature->setFaction(734);
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            return;
        }

        if (FeuerNova_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FEUERNOVA);
            FeuerNova_Timer = 6000;
        } else FeuerNova_Timer -= diff;

        if (FlameBuffet_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FLAMEBUFFET);
            FlameBuffet_Timer = 12500;
        } else FlameBuffet_Timer -= diff;


        DoMeleeAttackIfReady();

    }
};
CreatureAI* GetAI_feuerwache_glutseher(Creature* pCreature)
{
    return new feuerwache_glutseherAI(pCreature);
}

void AddSC_feuerwache_glutseher()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "feuerwache_glutseher";
    newscript->GetAI = &GetAI_feuerwache_glutseher;
    newscript->RegisterSelf();
}