/* boss_handlanger_pein */

#include "precompiled.h"
#include "deadmines.h"

#define schmetterwurf	6435
#define peins_stampfen	6432
#define hauen	3391


struct MANGOS_DLL_DECL boss_handlanger_peinAI : public ScriptedAI
{
    boss_handlanger_peinAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 schmetterwurf_timer;
    uint32 peins_stampfen_timer;
    uint32 hauen_timer;
    uint32 chest_timer_open;
    uint32 wait;

    uint64 truhe;

    bool stampfen66;
    bool stampfen33;
    bool chest;
    bool open;


    void Reset()
    {
        schmetterwurf_timer = 3000;
        hauen_timer = 5000;
        chest_timer_open = 1700;
        stampfen66=false;
        stampfen33=false;
        chest=false;
        wait = -1;
        open = false;
    }

    void Aggro(Unit *who)
    {
        //ScriptedInstance* pInstance = (ScriptedInstance*) m_creature->GetInstanceData();
        //GameObject* Truhe= pInstance->instance->GetGameObject(pInstance->GetData64(GO_SMITE_TRUHE));

    }

    void JustDied(Unit* Killer)
    {
    }

    void KilledUnit(Unit *victim)
    {
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*) m_creature->GetInstanceData();

        if (chest==true)
        {
            m_creature->CombatStop(true);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            m_creature->MonsterMoveWithSpeed(1.908f, -780.556f, 9.8111f, 1500, 0);

            if (chest_timer_open < diff && open == false)
            {
                // �ffne Kiste , Equip �ndern
                //pInstance->SetData(TYPE_SMITE_TRUHE,IN_PROGRESS);

                /*	m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 45479);
                	m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO , 218171138);
                	m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO* + 1, 3);

                	m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 45481);
                	m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO + 2, 218171138);
                	m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO + 2 + 1, 3); */
                open = true;
                wait = 6000;
            } else chest_timer_open -=diff;

            if (wait < diff)
            {
                //pInstance->SetData(TYPE_SMITE_TRUHE,DONE);
                chest = false;
                open = false;
                m_creature->Attack(m_creature->getVictim(),true);

            } else wait -=diff;


        }
        else
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            {
                Reset();
                return;
            }

            if (schmetterwurf_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),schmetterwurf);
                schmetterwurf_timer = random(6000,7000);
            } else schmetterwurf_timer -= diff;

            if (m_creature->isAlive() && stampfen66==false && m_creature->GetHealthPercent() < 66.0f)
            {
                DoCastSpellIfCan(m_creature->getVictim(),peins_stampfen);
                stampfen66=true;
                chest_timer_open = 1700;
                wait = -1;
                chest=true;
            }

            if (m_creature->isAlive() && stampfen33==false && m_creature->GetHealthPercent() < 33.0f)
            {
                DoCastSpellIfCan(m_creature->getVictim(),peins_stampfen);
                stampfen33=true;
                chest_timer_open = 1700;
                wait = -1;
                chest=true;
            }

            if (hauen_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),hauen);
                hauen_timer = random(6000,8000);
            } else hauen_timer -= diff;


            DoMeleeAttackIfReady();

        }
    }
};

CreatureAI* GetAI_boss_handlanger_pein(Creature *_Creature)
{
    return new boss_handlanger_peinAI (_Creature);
}

void AddSC_boss_handlanger_pein()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_handlanger_pein";
    newscript->GetAI = &GetAI_boss_handlanger_pein;
    newscript->RegisterSelf();
}