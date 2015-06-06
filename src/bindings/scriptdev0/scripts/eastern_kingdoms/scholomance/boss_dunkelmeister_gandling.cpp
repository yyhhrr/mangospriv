/* Dunkelmeister Gandling */


#include "precompiled.h"
#include "scholomance.h"


//	   | Zauberbezeichnung				SpellID
#define SPELL_ARKANE_GESCHOSSE			15791
#define SPELL_FLUCH_DES_DUNKELMEISTERS	18702
#define SPELL_SCHATTENSCHILD			12040
//#define SPELL_SCHATTENPORTAL			17950


struct MANGOS_DLL_DECL dunkelmeister_gandlingAI : public ScriptedAI
{
    dunkelmeister_gandlingAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 arkane_geschosse_timer;
    uint32 fluch_des_dunkelmeisters_timer;
    uint32 schattenschild_timer;
    uint32 schattenportal_timer;

    Creature* addone;
    Creature* addtwo;
    Creature* addthree;

    uint64 addoneGUID;
    uint64 addtwoGUID;
    uint64 addthreeGUID;

    int raum;
    Unit* target;
    Unit* porttarget;
    bool portalrdy;

    void Reset()
    {
        arkane_geschosse_timer = 3000;
        fluch_des_dunkelmeisters_timer = 4000;
        schattenschild_timer = 2000;
        schattenportal_timer = 5000;

        addoneGUID = 0;
        addtwoGUID = 0;
        addthreeGUID = 0;

        portalrdy = true;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
			pInstance->SetData(TYPE_GANDLING, IN_PROGRESS);

        m_creature->MonsterYell("Soso... Ihr fordert mich tats\303\244chlich heraus? Nun, wie Ihr wollt!",LANG_UNIVERSAL,NULL);

        portalrdy=true;
    }

    void JustDied(Unit *killer)
    {
        if (pInstance)
			pInstance->SetData(TYPE_GANDLING, DONE);

        m_creature->MonsterYell("Alle M\303\274he... umsonst, wir... werden alle... untergehen!",LANG_UNIVERSAL,NULL);

		if (pInstance)
		{
			if (portalrdy==false)
			{
				switch (raum)
				{
				case 0:
					portalrdy=true;
					pInstance->SetData(TYPE_RAUM_ONE,DONE);
					break;
				case 1:
					portalrdy=true;
					pInstance->SetData(TYPE_RAUM_TWO,DONE);
					break;
				case 2:
					portalrdy=true;
					pInstance->SetData(TYPE_RAUM_THREE,DONE);
					break;
				case 3:
					portalrdy=true;
					pInstance->SetData(TYPE_RAUM_FOUR,DONE);
					break;
				case 4:
					portalrdy=true;
					pInstance->SetData(TYPE_RAUM_FIVE,DONE);
					break;
				case 5:
					portalrdy=true;
					pInstance->SetData(TYPE_RAUM_SIX,DONE);
					break;
				}
			}
		}
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_GANDLING, FAIL);

		if (pInstance)
		{
			if (portalrdy==false)
			{
				if (porttarget->isDead())
				{
					switch (raum)
					{
					case 0:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_ONE, DONE);
						break;
					case 1:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_TWO, DONE);
						break;
					case 2:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_THREE, DONE);
						break;
					case 3:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_FOUR, DONE);
						break;
					case 4:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_FIVE, DONE);
						break;
					case 5:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_SIX, DONE);
						break;
					}
				}
			}
		}
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (arkane_geschosse_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_ARKANE_GESCHOSSE);
            arkane_geschosse_timer = random(1000,2000);
        } else arkane_geschosse_timer -= diff;

        if (fluch_des_dunkelmeisters_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FLUCH_DES_DUNKELMEISTERS);
            fluch_des_dunkelmeisters_timer = random(40000,60000);
        } else fluch_des_dunkelmeisters_timer -= diff;

        if (schattenschild_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SCHATTENSCHILD);
            schattenschild_timer = random(20000,30000);
        } else schattenschild_timer -= diff;

        Map* pmap;
        pmap= m_creature->GetMap();

        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 3 && pInstance)
        {
            if (schattenportal_timer < diff && portalrdy == true)
            {
                //int i=0;
                porttarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                /*while(m_creature->getVictim()==portporttarget && i>10)
                {
                portporttarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
                }*/

                if (m_creature->getThreatManager().getThreat(porttarget))
                    m_creature->getThreatManager().modifyThreatPercent(porttarget, -100);

                switch (rand()%6)
                {
                case 0: // oben links
                    DoTeleportPlayer(porttarget, 250.0696f, 0.3921f, 84.8408f, 3.149f);
                    addone = m_creature->SummonCreature(16119, 222.977829f, -4.305364f, 85.228249f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    addoneGUID = addone->GetGUID();
                    addone->AI()->AttackStart(porttarget);
                    //addtwo = m_creature->SummonCreature(16119, 222.977829f, -0.172f, 85.228249f, 0.0f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,120000);
                    //addtwoGUID = addtwo->GetGUID();
                    //addtwo->AI()->AttackStart(porttarget);
                    addthree = m_creature->SummonCreature(16119, 222.977829f, 4.938f, 85.228249f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    addthreeGUID = addthree->GetGUID();
                    addthree->AI()->AttackStart(porttarget);
                    pInstance->SetData(TYPE_RAUM_ONE, IN_PROGRESS);
                    portalrdy=false;
                    raum = 0;
                    break;

                case 1: // oben mitte
                    DoTeleportPlayer(porttarget, 181.4220f, -91.9481f, 84.8410f, 1.608f);
                    addone = m_creature->SummonCreature(16119, 172.004f, -45.902f, 86.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addoneGUID = addone->GetGUID();
                    addone->AI()->AttackStart(porttarget);
                    //addtwo = m_creature->SummonCreature(16119, 181.312f, -48.902f, 86.0f, 0.0f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                    //addtwoGUID = addtwo->GetGUID();
                    //addtwo->AI()->AttackStart(porttarget);
                    addthree = m_creature->SummonCreature(16119, 190.294f, -45.603f, 86.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addthreeGUID = addthree->GetGUID();
                    addthree->AI()->AttackStart(porttarget);
                    pInstance->SetData(TYPE_RAUM_TWO, IN_PROGRESS);
                    portalrdy=false;
                    raum = 1;
                    break;

                case 2: // oben rechts
                    DoTeleportPlayer(porttarget, 95.1547f, -1.8173f, 85.2289f, 0.043f);
                    addone = m_creature->SummonCreature(16119, 140.351f, 8.488f, 89.5f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addoneGUID = addone->GetGUID();
                    addone->AI()->AttackStart(porttarget);
                    //addtwo = m_creature->SummonCreature(16119, 137.189f, -0.750f, 86.5f, 0.0f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                    //addtwoGUID = addtwo->GetGUID();
                    //addtwo->AI()->AttackStart(porttarget);
                    addthree = m_creature->SummonCreature(16119, 140.539f, -9.759f, 89.5f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addthreeGUID = addthree->GetGUID();
                    addthree->AI()->AttackStart(porttarget);
                    pInstance->SetData(TYPE_RAUM_THREE, IN_PROGRESS);
                    portalrdy=false;
                    raum = 2;
                    break;

                case 3: // unten links
                    DoTeleportPlayer(porttarget, 250.0696f, 0.3921f, 72.6722f, 3.149f);
                    addone = m_creature->SummonCreature(16119, 218.397f, -6.095f, 73.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addoneGUID = addone->GetGUID();
                    addone->AI()->AttackStart(porttarget);
                    //addtwo = m_creature->SummonCreature(16119, 218.397f, 0.104f, 73.0f, 0.0f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                    //addtwoGUID = addtwo->GetGUID();
                    //addtwo->AI()->AttackStart(porttarget);
                    addthree = m_creature->SummonCreature(16119, 218.397f, 7.208f, 73.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addthreeGUID = addthree->GetGUID();
                    addthree->AI()->AttackStart(porttarget);
                    pInstance->SetData(TYPE_RAUM_FOUR, IN_PROGRESS);
                    portalrdy=false;
                    raum = 3;
                    break;

                case 4: // unten mitte
                    DoTeleportPlayer(porttarget, 181.4220f, -91.9481f, 70.7734f, 1.608f);
                    addone = m_creature->SummonCreature(16119, 169.658f, -65.203f, 75.8f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addoneGUID = addone->GetGUID();
                    addone->AI()->AttackStart(porttarget);
                    //addtwo = m_creature->SummonCreature(16119, 183.374f, -70.315f, 71.2f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                    //addtwoGUID = addtwo->GetGUID();
                    //addtwo->AI()->AttackStart(porttarget);
                    addthree = m_creature->SummonCreature(16119, 194.506f, -65.857f, 75.8f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addthreeGUID = addthree->GetGUID();
                    addthree->AI()->AttackStart(porttarget);
                    pInstance->SetData(TYPE_RAUM_FIVE, IN_PROGRESS);
                    portalrdy=false;
                    raum = 4;
                    break;

                case 5: // unten rechts
                    DoTeleportPlayer(porttarget, 106.1541f, -1.8994f, 75.3663f, 0.043f);
                    addone = m_creature->SummonCreature(16119, 143.801f, 8.251f, 75.9f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addoneGUID = addone->GetGUID();
                    addone->AI()->AttackStart(porttarget);
                    //addtwo = m_creature->SummonCreature(16119, 143.801f, -0.799f, 75.9f, 0.0f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    //addtwoGUID = addtwo->GetGUID();
                    //addtwo->AI()->AttackStart(porttarget);
                    addthree = m_creature->SummonCreature(16119, 143.801f, -10.619f, 75.9f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    addthreeGUID = addthree->GetGUID();
                    addthree->AI()->AttackStart(porttarget);
                    pInstance->SetData(TYPE_RAUM_SIX, IN_PROGRESS);
                    portalrdy=false;
                    raum = 5;
                    break;
                }
                schattenportal_timer = random(40000,70000);
            } else schattenportal_timer -= diff;
        }

        if (portalrdy==false)
        {
            if (porttarget->isDead())
            {
                switch (raum)
                {
                case 0:
                    portalrdy=true;
                    pInstance->SetData(TYPE_RAUM_ONE, DONE);
                    break;
                case 1:
                    portalrdy=true;
                    pInstance->SetData(TYPE_RAUM_TWO, DONE);
                    break;
                case 2:
                    portalrdy=true;
                    pInstance->SetData(TYPE_RAUM_THREE, DONE);
                    break;
                case 3:
                    portalrdy=true;
                    pInstance->SetData(TYPE_RAUM_FOUR, DONE);
                    break;
                case 4:
                    portalrdy=true;
                    pInstance->SetData(TYPE_RAUM_FIVE, DONE);
                    break;
                case 5:
                    portalrdy=true;
                    pInstance->SetData(TYPE_RAUM_SIX, DONE);
                    break;
                }

            }

            if ((addone = pmap->GetCreature(addoneGUID)) && (addthree = pmap->GetCreature(addthreeGUID)))// && (addtwo = pmap->GetCreature(addtwoGUID)))
            {
                if (addone->isDead() && addthree->isDead())// && addtwo->isDead())
                {
					switch (raum)
					{
					case 0:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_ONE, DONE);
						break;
					case 1:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_TWO, DONE);
						break;
					case 2:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_THREE, DONE);
						break;
					case 3:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_FOUR, DONE);
						break;
					case 4:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_FIVE, DONE);
						break;
					case 5:
						portalrdy=true;
						pInstance->SetData(TYPE_RAUM_SIX, DONE);
						break;
					}
                }
            }
        }

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_dunkelmeister_gandling(Creature *_Creature)
{
    return new dunkelmeister_gandlingAI (_Creature);
}

void AddSC_dunkelmeister_gandling()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="dunkelmeister_gandling";
    newscript->GetAI = &GetAI_dunkelmeister_gandling;
    newscript->RegisterSelf();
}