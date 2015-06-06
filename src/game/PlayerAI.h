#pragma once

class PlayerAI
{
	 // The spell which charmed the player
	uint32 mSpellId;
	std::mt19937 rnd_generator;
	std::uniform_int_distribution<int> dist;

protected:
	// The charmed player
	Player* mPlayer;
	// Timer for abilities. 2000ms initial, then every 3000ms - 6000ms
	uint32 mTimer;
	// The players abilities
	std::vector<uint32> mSpells;
    // Current Targetguid
    ObjectGuid mTarget;

public:
	PlayerAI(Player* player, uint32 spellId);
	virtual ~PlayerAI();

    Unit* SelectTarget();

	virtual void UpdateAI(uint32 timeDelta);
	bool CastSpell(uint32 spellId, bool triggered = false);
	void Chase();

	bool CheckCharmer();
	void UnCharm();

	void MovementInform();
};
