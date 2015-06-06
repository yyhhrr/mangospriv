#ifndef __PATH_H
#define __PATH_H

#include "precompiled.h"

const double PI = 3.14159265;

struct PatAdd
{
	uint64 guid;
	Creature* pCreature;
	double abstand;
	double winkel;
	double speed;
	uint32 entry;

	explicit PatAdd(uint32 patEntry) : entry(patEntry), guid(0), pCreature(0), abstand(0.0), winkel(0.0), speed(0.0) {} 
};

struct Pat : public ScriptedAI
{

	Pat(Creature* creature) : ScriptedAI(creature) { }
	void NewAdd(PatAdd& pNewAdd) { Adds.push_back(pNewAdd); }
	virtual void Update(const uint32 time) = 0;

	std::vector<PatAdd> Adds;
	float tmpx, tmpy;

	void AttackMyTarget(Unit* target)
	{
		if(target)
		{
			for(uint32 i = 0; i < Adds.size(); ++i)
				if(Adds[i].pCreature)
					Adds[i].pCreature->AI()->AttackStart(target);
		}
	}

	void updateguard(Creature* pCreature, uint64 guid, double abstand, double winkel, double speed)
	{
		if(pCreature->isAlive())
		{
			getDestination(abstand, winkel);
			pCreature->MonsterMoveWithSpeed(tmpx, tmpy, m_creature->GetPositionZ(), speed);
		}
	}

	void getDestination(double abstand, double winkel)
	{
		double b;
		double _winkel;
		double winkelbogen;
		float x, y, z, o;
		m_creature->GetPosition(x, y, z);
		o = m_creature->GetOrientation();

		b = o * 180 / PI;
		_winkel = winkel + b;
		winkelbogen = _winkel*PI/180;

		tmpx = x + (abstand * cos(winkelbogen));
		tmpy = y + (abstand * sin(winkelbogen));
	}

	void UpdateAI(const uint32 diff)
	{
		if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
		{
			Map* pmap = m_creature->GetMap();
			for(uint32 i = 0; i < Adds.size(); ++i)
			{
				Adds[i].pCreature = pmap->GetCreature(Adds[i].guid);
				if(Adds[i].pCreature)
				{
					updateguard(Adds[i].pCreature, Adds[i].guid, Adds[i].abstand, Adds[i].winkel, Adds[i].speed);
				}
				else
				{
					getDestination(Adds[i].abstand, Adds[i].winkel);
					Adds[i].pCreature = m_creature->SummonCreature(Adds[i].entry, tmpx, tmpy, m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0);
					if(Adds[i].pCreature)
					{
						Adds[i].guid = Adds[i].pCreature->GetGUID();
						Adds[i].pCreature->SetRespawnDelay(30000);
					}
				}
			}
		}

		Update(diff);
	}
};

#endif /*__PATH_H*/