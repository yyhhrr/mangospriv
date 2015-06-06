UPDATE `creature_template` SET `mechanic_immune_mask`=mechanic_immune_mask | 33554432 WHERE `entry`=10184;

DELETE FROM `creature_loot_template` WHERE `item`=12235 AND `entry` IN (6556,6557,6559);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES
(6556,12235,100,0,1,1,9,4294,0),
(6557,12235,100,0,1,1,9,4294,0),
(6559,12235,100,0,1,1,9,4294,0);

REPLACE INTO `spell_proc_event` (`entry`, `SchoolMask`) VALUES
(17794,32),
(17797,32),
(17798,32),
(17799,32),
(17800,32);