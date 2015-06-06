/* boss_nandos */

#include "precompiled.h"
#include "shadowfang_keep.h"

#define duesteren_worg_rufen	7487
#define sabbernden_worg_rufen	7488
#define werwolfsschrecken_rufen	7489


struct MANGOS_DLL_DECL boss_nandosAI : public ScriptedAI
{
    boss_nandosAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    bool duesteren_worg_rufen_75;
    bool sabbernden_worg_rufen_50;
    bool werwolfsschrecken_rufen_25;

    void Reset()
    {
        duesteren_worg_rufen_75 = false;
        sabbernden_worg_rufen_50 = false;
        werwolfsschrecken_rufen_25 = false;
    }

    void Aggro(Unit *who)
    {
        m_creature->CallForHelp(30.0f);
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (pInstance)
            pInstance->SetData(TYPE_NANDOS, DONE);
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
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->isAlive() && duesteren_worg_rufen_75==false && m_creature->GetHealthPercent() < 75.0f)
        {
            DoCastSpellIfCan(m_creature,duesteren_worg_rufen);
            duesteren_worg_rufen_75=true;
        }

        if (m_creature->isAlive() && sabbernden_worg_rufen_50==false && m_creature->GetHealthPercent() < 50.0f)
        {
            DoCastSpellIfCan(m_creature,sabbernden_worg_rufen);
            sabbernden_worg_rufen_50=true;
        }

        if (m_creature->isAlive() && werwolfsschrecken_rufen_25==false && m_creature->GetHealthPercent() < 25.0f)
        {
            DoCastSpellIfCan(m_creature,werwolfsschrecken_rufen);
            werwolfsschrecken_rufen_25=true;
        }


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_nandos(Creature *_Creature)
{
    return new boss_nandosAI (_Creature);
}

void AddSC_boss_nandos()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_nandos";
    newscript->GetAI = &GetAI_boss_nandos;
    newscript->RegisterSelf();
}

