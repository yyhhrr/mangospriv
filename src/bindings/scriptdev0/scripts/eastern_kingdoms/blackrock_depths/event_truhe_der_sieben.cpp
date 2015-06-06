/* Bosse des Events : Truhe der Sieben  */

#include "precompiled.h"
#include "blackrock_depths.h"

#define FACTION_NEUTRAL             734
#define FACTION_HOSTILE             754

Player* tombofsevenplayer;
ScriptedInstance* pInstance;

///////////////////////////////////////////////////////////////////////////
//						HASS'REL
///////////////////////////////////////////////////////////////////////////

#define SPELL_MANABRAND				14033
#define SPELL_SCHATTENBLITZ         15232
#define SPELL_SCHATTENSCHILD        12040
#define SPELL_SCHLAG				15580

struct MANGOS_DLL_DECL hass_relAI : public ScriptedAI
{
    hass_relAI(Creature *c) : ScriptedAI(c) {}

    uint32 manaburn_Timer;
    uint32 schattenblitz_Timer;
    uint32 schattenschild_Timer;
    uint32 schlag_Timer;
    Unit* target;

    void Reset()
    {
        manaburn_Timer = 8000;
        schattenblitz_Timer = 2000;
        schattenschild_Timer = 30000;
        schlag_Timer = 1000;

    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature,SPELL_SCHATTENSCHILD);
    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_TOMB_OF_SEVEN,IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_TOMB_OF_SEVEN,FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }


        if (manaburn_Timer < diff)
        {
            int mana =0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (target->getPowerType()!=POWER_MANA)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                mana++;
                if (mana > 10)
                {
                    goto end;
                }
            }
            DoCastSpellIfCan(target,SPELL_MANABRAND);
			end:
            manaburn_Timer = random(5000,10000);
        } else manaburn_Timer -= diff;


        if (schattenblitz_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_SCHATTENBLITZ);
            schattenblitz_Timer = random(4000,8000);
        } else schattenblitz_Timer -= diff;


        if (schattenschild_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SCHATTENSCHILD);
            schattenschild_Timer = random(30000,35000);
        } else schattenschild_Timer -= diff;


        if (schlag_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SCHLAG);
            schlag_Timer = random(5000,10000);
        }
        else
        {
            schlag_Timer -= diff;
            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_hass_rel(Creature *_Creature)
{
    return new hass_relAI (_Creature);
}


///////////////////////////////////////////////////////////////////////////
//						ZORN'REL
///////////////////////////////////////////////////////////////////////////
#define SPELL_RUESTUNG_ZERREISSEN		11971
#define SPELL_SCHILDBLOCK           12169
#define SPELL_SCHILDWALL            15062
//#define SPELL_SCHLAG				15580  Ist schon definiert !

struct MANGOS_DLL_DECL zorn_relAI : public ScriptedAI
{
    zorn_relAI(Creature *c) : ScriptedAI(c) {}

    uint32 ruestung_zerreissen_Timer;
    uint32 schildblock_Timer;
    uint32 schildwall_Timer;
    uint32 schlag_Timer;
    Unit* target;

    void Reset()
    {
        ruestung_zerreissen_Timer = 8000;
        schildblock_Timer = 2000;
        schildwall_Timer = 6000;
        schlag_Timer = 5000;
    }

    void Aggro(Unit *who)
    {

    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_TOMB_OF_SEVEN, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }

        if (ruestung_zerreissen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_RUESTUNG_ZERREISSEN);
            ruestung_zerreissen_Timer = random(10000,15000);
        } else ruestung_zerreissen_Timer -= diff;

        if (schildblock_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SCHILDBLOCK);
            schildblock_Timer = random(5000,7000);
        } else schildblock_Timer -= diff;

        if (schildwall_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SCHILDWALL);
            schildwall_Timer = random(20000,30000);
        } else schildwall_Timer -= diff;

        if (schlag_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SCHLAG);
            schlag_Timer = random(4000,8000);
        }
        else
        {
            schlag_Timer -= diff;
            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_zorn_rel(Creature *_Creature)
{
    return new zorn_relAI (_Creature);
}

///////////////////////////////////////////////////////////////////////////
//						B�S'REL
///////////////////////////////////////////////////////////////////////////

#define SPELL_GEBET_DER_HEILUNG		15585
#define SPELL_GEDANKENSCHLAG        15587
#define SPELL_HEILEN				15586
#define SPELL_MACHTWORT_SCHILD		11974

struct MANGOS_DLL_DECL boes_relAI : public ScriptedAI
{
    boes_relAI(Creature *c) : ScriptedAI(c) {}

    uint32 gebet_der_heilung_Timer;
    uint32 gedankenschlag_Timer;
    uint32 heilen_Timer;
    uint32 machtwort_schild_Timer;
    Unit* target;

    void Reset()
    {
        gebet_der_heilung_Timer = 10000;
        gedankenschlag_Timer = 3000;
        heilen_Timer = 8000;
        machtwort_schild_Timer = 12000;

    }

    void Aggro(Unit *who)
    {

    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_TOMB_OF_SEVEN, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_TOMB_OF_SEVEN,FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }


        if (gebet_der_heilung_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_GEBET_DER_HEILUNG);
            gebet_der_heilung_Timer = random(10000,20000);
        } else gebet_der_heilung_Timer -= diff;


        if (gedankenschlag_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_GEDANKENSCHLAG);

            gedankenschlag_Timer = random(8000,12000);
        } else gedankenschlag_Timer -= diff;


        if (heilen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_HEILEN);
            heilen_Timer = random(10000,15000);
        } else heilen_Timer -= diff;


        if (machtwort_schild_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_MACHTWORT_SCHILD);
            machtwort_schild_Timer = random(20000,30000);
        } else machtwort_schild_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boes_rel(Creature *_Creature)
{
    return new boes_relAI (_Creature);
}

///////////////////////////////////////////////////////////////////////////
//						DUNK'REL
///////////////////////////////////////////////////////////////////////////

#define SPELL_KNIESEHNE             9080
#define SPELL_SPALTEN               40504
#define SPELL_TOLLKUEHNHEIT			13847
#define SPELL_TOEDLICHER_STOSS        13737




#define    FACTION_NEUTRAL 734
#define    FACTION_HOSTILE 754

#define    SPELL_SMELT_DARK_IRON 14891
#define    SPELL_LEARN_SMELT 14894
#define    QUEST_SPECTRAL_CHALICE 4083
#define    SKILLPOINT_MIN 230


#define GOSSIP_ITEM_TEACH_1 "Teach me the art of smelting dark iron"
#define GOSSIP_ITEM_TEACH_2 "Continue..."
#define GOSSIP_ITEM_TRIBUTE "I want to pay tribute"

bool GossipHello_dunk_rel(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_TOMB_OF_SEVEN) == NOT_STARTED)
        {
            if (pPlayer->GetQuestRewardStatus(QUEST_SPECTRAL_CHALICE) && pPlayer->GetSkillValue(SKILL_MINING) >= SKILLPOINT_MIN && !pPlayer->HasSpell(SPELL_SMELT_DARK_IRON))
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TEACH_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (!pPlayer->GetQuestRewardStatus(QUEST_SPECTRAL_CHALICE) && pPlayer->GetSkillValue(SKILL_MINING) >= SKILLPOINT_MIN)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TRIBUTE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        }
    }
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_dunk_rel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TEACH_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
        pPlayer->SEND_GOSSIP_MENU(2606, pCreature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+11:
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, SPELL_LEARN_SMELT, false);
        break;
    case GOSSIP_ACTION_INFO_DEF+2:
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[PH] Continue...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
        pPlayer->SEND_GOSSIP_MENU(2604, pCreature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+22:
        pPlayer->CLOSE_GOSSIP_MENU();
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
        {
            //are 5 minutes expected? go template may have data to despawn when used at quest
            pInstance->DoRespawnGameObject(pInstance->GetData64(DATA_GO_CHALICE),MINUTE*5);
            pInstance->DoRespawnGameObject(pInstance->GetData64(DATA_GO_CHALICE),MINUTE*5);

        }
        break;
    }
    return true;
}





struct MANGOS_DLL_DECL dunk_relAI : public ScriptedAI
{
    dunk_relAI(Creature *c) : ScriptedAI(c) {}

    uint32 kniesehne_Timer;
    uint32 spalten_Timer;
    uint32 tollkuehnheit_Timer;
    uint32 toedlicher_stoss_Timer;
    Unit* target;

    void Reset()
    {
        kniesehne_Timer = 3000;
        spalten_Timer = 6000;
        tollkuehnheit_Timer = 20000;
        toedlicher_stoss_Timer = 4000;

    }

    void Aggro(Unit *who)
    {

    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_TOMB_OF_SEVEN,IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_TOMB_OF_SEVEN,FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }


        if (kniesehne_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KNIESEHNE);
            kniesehne_Timer = random(14000,17000);
        } else kniesehne_Timer -= diff;


        if (spalten_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SPALTEN);
            spalten_Timer = random(5000,15000);
        } else spalten_Timer -= diff;


        if (tollkuehnheit_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_TOLLKUEHNHEIT);
            tollkuehnheit_Timer = random(20000,30000);
        } else tollkuehnheit_Timer -= diff;


        if (toedlicher_stoss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_TOEDLICHER_STOSS);
            toedlicher_stoss_Timer = random(7000,13000);
        } else toedlicher_stoss_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_dunk_rel(Creature *_Creature)
{
    return new dunk_relAI (_Creature);
}

///////////////////////////////////////////////////////////////////////////
//						WUT'REL
///////////////////////////////////////////////////////////////////////////

#define SPELL_FROSTBLITZ        12675
#define SPELL_FROSTNOVA			12674
#define SPELL_FROSTRUESTUNG		12544
#define SPELL_FROSTZAUBERSCHUTZ	15044
#define SPELL_KAELTEKEGEL		15244

struct MANGOS_DLL_DECL wut_relAI : public ScriptedAI
{
    wut_relAI(Creature *c) : ScriptedAI(c) {}

    uint32 frostblitz_Timer;
    uint32 frostnova_Timer;
    uint32 frostruestung_Timer;
    uint32 frostzauberschutz_Timer;
    uint32 kaeltekegel_Timer;
    Unit* target;

    void Reset()
    {
        frostblitz_Timer = 2000;
        frostnova_Timer = 6000;
        frostruestung_Timer = 1800000;
        frostzauberschutz_Timer = 8000;
        kaeltekegel_Timer = 25000;

    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature,SPELL_FROSTRUESTUNG);
    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_TOMB_OF_SEVEN,IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_TOMB_OF_SEVEN,FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }


        if (frostblitz_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target, SPELL_FROSTBLITZ);
            frostblitz_Timer = random(4000,7000);
        } else frostblitz_Timer -= diff;


        if (frostnova_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTNOVA);
            frostnova_Timer = random(10000,20000);
        } else frostnova_Timer -= diff;


        if (frostruestung_Timer < diff)
        {
            DoCastSpellIfCan(target,SPELL_FROSTRUESTUNG);

            frostruestung_Timer = random(1800000,1805000);
        } else frostruestung_Timer -= diff;


        if (frostzauberschutz_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FROSTZAUBERSCHUTZ);
            frostzauberschutz_Timer = random(30000,50000);
        } else frostzauberschutz_Timer -= diff;


        if (kaeltekegel_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KAELTEKEGEL);
            kaeltekegel_Timer = random(10000,15000);
        } else kaeltekegel_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_wut_rel(Creature *_Creature)
{
    return new wut_relAI (_Creature);
}

///////////////////////////////////////////////////////////////////////////
//						UN'REL
///////////////////////////////////////////////////////////////////////////

#define SPELL_DAEMONENRUESTUNG			13787
#define SPELL_FEUERBRAND				12742
#define SPELL_FLUCH_DER_SCHWAECHE		12493
#define SPELL_LEERWANDLER_BESCHWOEREN	15092
#define SPELL_SCHATTENBLITZSALVE		15245
#define SPELL_VERBANNEN					8994

#define  MAX_DWARF                      7


#define GOSSIP_ITEM_CHALLENGE   "Your bondage is at an end, Doom'rel. I challenge you!"
#define SAY_DOOMREL_START_EVENT        -1230003

bool GossipHello_boss_doomrel(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_TOMB_OF_SEVEN) == NOT_STARTED)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_CHALLENGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    pPlayer->SEND_GOSSIP_MENU(2601, pCreature->GetGUID());
    return true;
}

bool GossipSelect_boss_doomrel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        pPlayer->CLOSE_GOSSIP_MENU();
        DoScriptText(SAY_DOOMREL_START_EVENT, pCreature, pPlayer);
        // start event
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
            pInstance->SetData(TYPE_TOMB_OF_SEVEN, IN_PROGRESS);

        break;
    }
    return true;
}


struct MANGOS_DLL_DECL un_relAI : public ScriptedAI
{
    un_relAI(Creature *c) : ScriptedAI(c) {}

    //uint32 d�monenr�stung_Timer;
    uint32 feuerbrand_Timer;
    uint32 fluch_der_schwaeche_Timer;
    uint32 leerwandler_beschwoeren_Timer;
    uint32 schattenblitzsalve_Timer;
    uint32 verbannen_Timer;
    Unit* target;

    Creature* hassrel;
    Creature* zornrel;
    Creature* boesrel;
    Creature* dunkrel;
    Creature* wutrel;
    Creature* unrel;
    Creature* trottrel;


    uint32 m_uiCallToFight_Timer;
    uint8 m_uiDwarfRound;
    bool m_bHasSummoned;

    ScriptedInstance* m_pInstance;

    void Reset()
    {

        feuerbrand_Timer = 3000;
        fluch_der_schwaeche_Timer = 6000;
        leerwandler_beschwoeren_Timer = 10000;
        schattenblitzsalve_Timer = 5000;
        verbannen_Timer = 9000;
        //m_creature->setFaction(FACTION_HOSTILE);

        m_uiCallToFight_Timer = 0;
        m_uiDwarfRound = 0;
        m_bHasSummoned = false;

    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature,SPELL_DAEMONENRUESTUNG);
    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_TOMB_OF_SEVEN,IN_PROGRESS);


    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_TOMB_OF_SEVEN,FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }


    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }


        if (feuerbrand_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FEUERBRAND);
            feuerbrand_Timer = random(5000,8000);
        } else feuerbrand_Timer -= diff;


        if (fluch_der_schwaeche_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FLUCH_DER_SCHWAECHE);
            fluch_der_schwaeche_Timer = 8000;
        } else fluch_der_schwaeche_Timer -= diff;


        if (leerwandler_beschwoeren_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_LEERWANDLER_BESCHWOEREN);
            leerwandler_beschwoeren_Timer = random(15000,25000);
        } else leerwandler_beschwoeren_Timer -= diff;


        if (schattenblitzsalve_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SCHATTENBLITZSALVE);
            schattenblitzsalve_Timer = random(15000,25000);
        } else schattenblitzsalve_Timer -= diff;


        if (verbannen_Timer < diff)
        {
            int c=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (target==m_creature->getVictim() && c<10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                c++;
            }
            DoCastSpellIfCan(m_creature,SPELL_VERBANNEN);
            verbannen_Timer = random(20000,25000);
        } else verbannen_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

bool GossipHello_un_rel(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_TOMB_OF_SEVEN) == NOT_STARTED)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_CHALLENGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    pPlayer->SEND_GOSSIP_MENU(2601, pCreature->GetGUID());
    return true;
}

bool GossipSelect_un_rel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        pPlayer->CLOSE_GOSSIP_MENU();
        DoScriptText(SAY_DOOMREL_START_EVENT, pCreature, pPlayer);
        // start event
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
        {
            pInstance->SetData(TYPE_TOMB_OF_SEVEN, IN_PROGRESS);
            pInstance->SetData64(DATA_PLAYER,pPlayer->GetGUID());
        }
        break;
    }
    return true;
}


CreatureAI* GetAI_un_rel(Creature *_Creature)
{
    return new un_relAI (_Creature);
}

///////////////////////////////////////////////////////////////////////////
//						TROTT'REL
///////////////////////////////////////////////////////////////////////////

#define SPELL_BLUTUNG			15583
#define SPELL_ENTRINNEN         15087
#define SPELL_FINSTERER_STOSS    15581
#define SPELL_MEUCHELN          15582
#define SPELL_SOLARPLEXUS		12540

float tx, ty, tz, dx, dy, dz;

struct MANGOS_DLL_DECL trott_relAI : public ScriptedAI
{
    trott_relAI(Creature *c) : ScriptedAI(c) {}

    uint32 blutung_Timer;
    uint32 entrinnen_Timer;
    uint32 finsterer_stoss_Timer;
    uint32 meucheln_Timer;
    uint32 solarplexus_Timer;
    Unit* target;

    void Reset()
    {
        blutung_Timer = 10000;
        entrinnen_Timer = 18000;
        finsterer_stoss_Timer = 5000;
        solarplexus_Timer = 16000;
        meucheln_Timer = 6000;

    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_TOMB_OF_SEVEN,IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_TOMB_OF_SEVEN,FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }


        if (blutung_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_BLUTUNG);
            blutung_Timer = random(14000,18000);
        } else blutung_Timer -= diff;


        if (entrinnen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_ENTRINNEN);

            entrinnen_Timer = random(30000,40000);
        } else entrinnen_Timer -= diff;


        if (finsterer_stoss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FINSTERER_STOSS);
            finsterer_stoss_Timer = random(5000,7000);
        } else finsterer_stoss_Timer -= diff;


        if (solarplexus_Timer < diff)
        {
            int i=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)>5 && i>10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(target,SPELL_SOLARPLEXUS);
            solarplexus_Timer = random(8000,10000);
        } else solarplexus_Timer -= diff;

        if (meucheln_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MEUCHELN);
            meucheln_Timer = random(6000,12000);
        } else meucheln_Timer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_trott_rel(Creature *_Creature)
{
    return new trott_relAI (_Creature);
}


void AddSC_truhe_der_sieben()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "hass_rel";
    newscript->GetAI = &GetAI_hass_rel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "zorn_rel";
    newscript->GetAI = &GetAI_zorn_rel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boes_rel";
    newscript->GetAI = &GetAI_boes_rel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "dunk_rel";
    newscript->GetAI = &GetAI_dunk_rel;
    newscript->pGossipHello = &GossipHello_dunk_rel;
    newscript->pGossipSelect = &GossipSelect_dunk_rel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "wut_rel";
    newscript->GetAI = &GetAI_wut_rel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "un_rel";
    newscript->GetAI = &GetAI_un_rel;
    newscript->pGossipHello = &GossipHello_un_rel;
    newscript->pGossipSelect = &GossipSelect_un_rel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "trott_rel";
    newscript->GetAI = &GetAI_trott_rel;
    newscript->RegisterSelf();
}

