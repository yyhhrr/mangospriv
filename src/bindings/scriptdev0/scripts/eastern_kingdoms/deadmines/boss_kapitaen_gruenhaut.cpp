/* boss_kapit�n_gr�nhaut */

#include "precompiled.h"
#include <math.h>

#define PI 3.14159265

#define PIRAT			657
#define WELLENFORMER	1732

#define spalten	15284
#define vergiftete_harpune	5208

long double A,W;


struct MANGOS_DLL_DECL boss_kapitaen_gruenhautAI : public ScriptedAI
{
    boss_kapitaen_gruenhautAI(Creature *c) : ScriptedAI(c) {}

    Creature* pirat;
    uint64 piratGUID;
    Creature* wellenformer;
    uint64 wellenformerGUID;

    Unit* target;

    uint32 spalten_timer;
    uint32 vergiftete_harpune_timer;
    uint32 pat_update;

    double officerx;
    double tmpx;

    double officery;
    double tmpy;

    double officerz;
    double officero;

    double officerspeed;

    void Reset()
    {
        officerx=0;
        officery=0;
        officerz=0;
        officero=0;

        spalten_timer = 2000;
        vergiftete_harpune_timer = 4000;
        pat_update = 1000;

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {

    }

    void JustDied(Unit* Killer)
    {}

    void KilledUnit(Unit *victim)
    {}

    void updateguard(Creature* pcreature, uint64 pcreatureGUID, double abstand, double winkel, double speed)
    {
        if ( (pcreature->isAlive()) )
        {
            patrole(abstand,officerx,officery,officero,winkel);
            pcreature->MonsterMoveWithSpeed(tmpx,tmpy,officerz,1200);

        }
    }

    void patrole(double A, double x, double y, double o, double a)
    {
        double b;
        double winkel;
        double winkelbogen;

        b = o*180/PI ;
        winkel=a+b ;
        winkelbogen =winkel*PI/180;

        tmpx = x + (A* (cos(winkelbogen)));
        tmpy = y + (A* (sin(winkelbogen)));

    }

    void spellonmember(Creature* spellingnpc, Creature* spellednpc, uint32 Spell, uint32 hptrigger)
    {
        if (spellednpc->isAlive() && (spellednpc->GetHealthPercent() <= hptrigger))
            spellingnpc->AI()->DoCastSpellIfCan(spellednpc, Spell,0,m_creature->GetGUID());
    }


    void UpdateAI(const uint32 diff)
    {

        if (m_creature->SelectHostileTarget() || m_creature->getVictim() )
        {
            if (spalten_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),spalten);
                spalten_timer = random(5000,8000);
            } else spalten_timer -= diff;

            if (vergiftete_harpune_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),vergiftete_harpune);
                vergiftete_harpune_timer = random(8000,9000);
            } else vergiftete_harpune_timer -= diff;

            DoMeleeAttackIfReady();
        }
        else	// OUT OF COMBAT
        {

            officerx = m_creature->GetPositionX();
            officery=m_creature->GetPositionY();
            officerz=m_creature->GetPositionZ();
            officero=m_creature->GetOrientation();

            Map* pmap;
            pmap= m_creature->GetMap();

            // CONTROLE PATROULLIE HERE
            /////////////////////////////////////////////////////////////////////////////
            if (pat_update < diff)
            {
                if (pirat = pmap->GetCreature(piratGUID))
                {
                    updateguard(pirat,piratGUID,2,170,1500);
                }
                else
                {
                    pirat = (m_creature->SummonCreature(PIRAT,officerx,officery,officerz,officero,TEMPSUMMON_MANUAL_DESPAWN, 0));
                    piratGUID = pirat->GetGUID();
                }
                /////////////////////////////////////////////////////////////////////////////////

                if (wellenformer = pmap->GetCreature(wellenformerGUID))
                {
                    updateguard(wellenformer,wellenformerGUID,2,190,1500);
                }
                else
                {
                    wellenformer = (m_creature->SummonCreature(WELLENFORMER,officerx,officery,officerz,officero,TEMPSUMMON_MANUAL_DESPAWN, 0));
                    wellenformerGUID = wellenformer->GetGUID();
                }
                pat_update = 1000;
            } else pat_update -= diff;

        }
    }
};

CreatureAI* GetAI_boss_kapitaen_gruenhaut(Creature *_Creature)
{
    return new boss_kapitaen_gruenhautAI (_Creature);
}

void AddSC_boss_kapitaen_gruenhaut()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kapitaen_gruenhaut";
    newscript->GetAI = &GetAI_boss_kapitaen_gruenhaut;
    newscript->RegisterSelf();
}