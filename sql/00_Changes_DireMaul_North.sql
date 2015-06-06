UPDATE `creature_template` SET `faction_a` = '45', `faction_h` = '45' WHERE `name` LIKE 'gordok%';
UPDATE `creature_template` SET `ScriptName` = "boss_cho_rush_der_beobachter" WHERE `entry` = '14324';
UPDATE `creature_template` SET `ScriptName` = "boss_koenig_gordok" WHERE `entry` = '11501';
UPDATE `creature_template` SET `ScriptName` = "mizzle_the_crafty" WHERE `entry` = '14353';
UPDATE `creature_template` SET `ScriptName` = "knot_quest_cache" WHERE `entry` = '14338';
UPDATE `gameobject_template` SET `data1` = '150000', data15 = '1' WHERE `entry` = '179564';
DELETE FROM `gameobject` WHERE `id` = '179501';
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES ('150000', '8766', '100', '0', '15', '20', '0', '0', '0');
