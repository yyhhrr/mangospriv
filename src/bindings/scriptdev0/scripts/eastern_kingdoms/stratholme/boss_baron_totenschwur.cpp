/*
#################################################################################
#   ____  _ _   _        ____                 _                          _		#
#  / ___|(_) \ | |      / ___| __ _ _ __ ___ (_)_ __   __ _   _ __   ___| |_	#
#  \___ \| |  \| |_____| |  _ / _` | '_ ` _ \| | '_ \ / _` | | '_ \ / _ \ __|	#
#   ___) | | |\  |_____| |_| | (_| | | | | | | | | | | (_| |_| | | |  __/ |_	#
#  |____/|_|_| \_|      \____|\__,_|_| |_| |_|_|_| |_|\__, (_)_| |_|\___|\__|	#
#                                                     |___/						#
#																				#
#														Stratholme				#
#												   	 Baron Totenschwur			#
#################################################################################

ScriptName: Baron Totenschwur
Instanz: Stratholme
Staus in %: 100%
Abgeschlossen?: Ja

*/

#include "precompiled.h"
#include "stratholme.h"

#define SAY_0    "Intruders! More pawns of the Argent Dawn, no doubt. I already count one of their number among my prisoners. Withdraw from my domain before she is executed!"
#define SAY_1    "You're still here? Your foolishness is amusing! The Argent Dawn wench needn't suffer in vain. Leave at once and she shall be spared!"
#define SAY_2    "I shall take great pleasure in taking this poor wretch's life! It's not too late, she needn't suffer in vain. Turn back and her death shall be merciful!"
#define SAY_3    "May this prisoner's death serve as a warning. None shall defy the Scourge and live!"
#define SAY_4    "So you see fit to toy with the Lich King's creations? Ramstein, be sure to give the intruders a proper greeting."
#define SAY_5    "Time to take matters into my own hands. Come. Enter my domain and challenge the might of the Scourge!"

#define ADD_1X 4017.403809f
#define ADD_1Y -3339.703369f
#define ADD_1Z 115.057655f
#define ADD_1O 5.487860f

#define ADD_2X 4013.189209f
#define ADD_2Y -3351.808350f
#define ADD_2Z 115.052254f
#define ADD_2O 0.134280f

#define ADD_3X 4017.738037f
#define ADD_3Y -3363.478016f
#define ADD_3Z 115.057274f
#define ADD_3O 0.723313f

#define ADD_4X 4048.877197f
#define ADD_4Y -3363.223633f
#define ADD_4Z 115.054253f
#define ADD_4O 3.627735f

#define ADD_5X 4051.777588f
#define ADD_5Y -3350.893311f
#define ADD_5Z 115.055351f
#define ADD_5O 3.066176f

#define ADD_6X 4048.375977f
#define ADD_6Y -3339.966309f
#define ADD_6Z 115.055222f
#define ADD_6O 2.457497f

#define SPELL_SCHATTENBLITZ		17393
#define SPELL_SPALTEN			15284
#define SPELL_TOEDLICHER_STOSS	15708

#define SPELL_UNHEILIGE_AURA	17467
#define SPELL_RAISEDEAD			17473                           //triggers death pact (17471)

#define SPELL_RAISE_DEAD1   17475
#define SPELL_RAISE_DEAD2   17476
#define SPELL_RAISE_DEAD3   17477
#define SPELL_RAISE_DEAD4   17478
#define SPELL_RAISE_DEAD5   17479
#define SPELL_RAISE_DEAD6   17480

struct MANGOS_DLL_DECL baron_totenschwurAI : public ScriptedAI
{
    baron_totenschwurAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 Schattenblitz_Timer;
    uint32 Spalten_Timer;
    uint32 Toedlicher_Stoss_Timer;
    uint32 SummonSkeletons_Timer;
    Creature *Summoned;
    uint32 i;
    Unit* target;
    bool resetted;

    void Reset()
    {
        Schattenblitz_Timer = 5000;
        Spalten_Timer = 8000;
        Toedlicher_Stoss_Timer = 12000;
        //        RaiseDead_Timer = 30000;
        SummonSkeletons_Timer = 34000;

    }

    void Aggro(Unit *who)
    {
        //DoCastSpellIfCan(m_creature,SPELL_UNHEILIGE_AURA);
    }

    void JustSummoned(Creature* summoned)
    {
        if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            summoned->AI()->AttackStart(target);
    }

    void JustDied(Unit* Killer)
    {
        if (pInstance)
            pInstance->SetData(TYPE_BARON,IN_PROGRESS);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            if (resetted!=true)
            {
                Reset();
                resetted=true;
            }
            return;
        }

        //ShadowBolt
        if (Schattenblitz_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SCHATTENBLITZ);

            Schattenblitz_Timer = random(8000,10000);
        } else Schattenblitz_Timer -= diff;

        //Cleave
        if (Spalten_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SPALTEN);
            //13 seconds until we should cast this again
            Spalten_Timer = random(6000,8000);
        } else Spalten_Timer -= diff;

        //MortalStrike
        if (Toedlicher_Stoss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_TOEDLICHER_STOSS);
            Toedlicher_Stoss_Timer = random(9000,15000);
        } else Toedlicher_Stoss_Timer -= diff;

        //RaiseDead
        //            if (RaiseDead_Timer < diff)
        //          {
        //      DoCastSpellIfCan(m_creature,SPELL_RAISEDEAD);
        //                RaiseDead_Timer = 45000;
        //            }else RaiseDead_Timer -= diff;

        //SummonSkeletons
        if (SummonSkeletons_Timer < diff)
        {
            m_creature->SummonCreature(11197,ADD_1X,ADD_1Y,ADD_1Z,ADD_1O,TEMPSUMMON_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(11197,ADD_2X,ADD_2Y,ADD_2Z,ADD_2O,TEMPSUMMON_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(11197,ADD_3X,ADD_3Y,ADD_3Z,ADD_3O,TEMPSUMMON_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(11197,ADD_4X,ADD_4Y,ADD_4Z,ADD_4O,TEMPSUMMON_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(11197,ADD_5X,ADD_5Y,ADD_5Z,ADD_5O,TEMPSUMMON_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(11197,ADD_6X,ADD_6Y,ADD_6Z,ADD_6O,TEMPSUMMON_TIMED_DESPAWN,29000);

            //34 seconds until we should cast this again
            SummonSkeletons_Timer = 40000;
        } else SummonSkeletons_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_baron_totenschwur(Creature *_Creature)
{
    return new baron_totenschwurAI (_Creature);
}


void AddSC_baron_totenschwur()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="baron_totenschwur";
    newscript->GetAI = &GetAI_baron_totenschwur;
    newscript->RegisterSelf();
}