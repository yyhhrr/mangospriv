#pragma once


enum
{
    SPELL_THUNDERCLAP    = 15588,
    SPELL_FIREBALLVOLLEY = 15285,
    SPELL_MIGHTYBLOW     = 14099
};

struct MANGOS_DLL_DECL mob_phalanxAI : public ScriptedAI
{
    mob_phalanxAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiThunderClapTimer;
    uint32 m_uiFireballVolleyTimer;
    uint32 m_uiMightyBlowTimer;

	bool m_justActivated;
	uint32 m_activationState;
	uint32 m_timer;

    void Reset()
    {
        m_uiThunderClapTimer    = 12000;
        m_uiFireballVolleyTimer = 0;
        m_uiMightyBlowTimer     = 15000;
		m_justActivated = false;
		m_activationState = 0;
		m_timer = 8000;
    }

	void Activate()
	{
		if(m_timer == 8000)
			m_justActivated = true;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
		{
			if(!m_justActivated)
				return;

			if(m_activationState == 0)
			{
				m_creature->GetMotionMaster()->MovePoint(0, 867.56f, -223.15f, -43.68f);
				m_activationState = 1;

			}
			else if(m_activationState == 1)
			{
				if(m_timer < uiDiff)
				{
					m_creature->SetFacingTo(2.12f);
					m_creature->setFaction(54);
					m_creature->MonsterYell(-1540024, 0);
					m_justActivated = false;
					m_timer = 0;
				}
				else
					m_timer -= uiDiff;
			}
		}

        // ThunderClap
        if (m_uiThunderClapTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_THUNDERCLAP);
            m_uiThunderClapTimer = 10000;
        }
        else
            m_uiThunderClapTimer -= uiDiff;

        // FireballVolley
        if (m_creature->GetHealthPercent() < 51.0f)
        {
            if (m_uiFireballVolleyTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIREBALLVOLLEY);
                m_uiFireballVolleyTimer = 15000;
            }
            else
                m_uiFireballVolleyTimer -= uiDiff;
        }

        // MightyBlow
        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTYBLOW);
            m_uiMightyBlowTimer = 10000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};