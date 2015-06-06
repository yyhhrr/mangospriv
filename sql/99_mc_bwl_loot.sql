DELETE FROM `gameobject_loot_template` WHERE entry=16719 AND item=40008;
UPDATE `gameobject_loot_template` SET maxcount=2 WHERE entry=16719 AND item=40002;
UPDATE `reference_loot_template` SET ChanceOrQuestChance=10 WHERE entry=34032;
UPDATE `reference_loot_template` SET condition_value1=67 WHERE condition_value1=69 AND lootcondition = 6;

# Set Paladin-T2 Alliance only
UPDATE `reference_loot_template` SET lootcondition = 6, condition_value1 = 469 WHERE item >= 16951 AND item <= 16958;

# Set Shaman-T2 Horde only
UPDATE `reference_loot_template` SET lootcondition = 6, condition_value1 = 67 WHERE item >= 16943 AND item <= 16950;

# BWL Razorgore Bugfix
UPDATE `gameobject_template` SET data10=0 WHERE entry=177808; 