UPDATE `creature_template` SET `mechanic_immune_mask`=48971346 WHERE `entry` IN(11352, 14750);
UPDATE `creature_template` SET `mechanic_immune_mask`=617299803 WHERE `entry` IN(11348, 11347);


INSERT INTO creature_linking_template VALUES
( 11368, 309, 14750, 3, 20 ),
( 11368, 309, 14517, 3, 35 );


UPDATE creature_template SET ScriptName = "blooddrinker" WHERE entry = 11353;

INSERT INTO spell_bonus_data VALUES
(  3044, 0.1,  0, 0, 0, 'Hunter - Arcane Shot R1' ),
( 14281, 0.1,  0, 0, 0, 'Hunter - Arcane Shot R2' ),
( 14282, 0.1,  0, 0, 0, 'Hunter - Arcane Shot R3' ),
( 14283, 0.43, 0, 0, 0, 'Hunter - Arcane Shot R4' ),
( 14284, 0.43, 0, 0, 0, 'Hunter - Arcane Shot R5' ),
( 14285, 0.43, 0, 0, 0, 'Hunter - Arcane Shot R6' ),
( 14286, 0.43, 0, 0, 0, 'Hunter - Arcane Shot R7' ),
( 14287, 0.43, 0, 0, 0, 'Hunter - Arcane Shot R8' ),

(  1978, 0, 0.05, 0, 0, 'Hunter - Serpent Sting R1' ),
( 13549, 0, 0.05, 0, 0, 'Hunter - Serpent Sting R2' ),
( 13550, 0, 0.05, 0, 0, 'Hunter - Serpent Sting R3' ),
( 13551, 0, 0.2,  0, 0, 'Hunter - Serpent Sting R4' ),
( 13552, 0, 0.2,  0, 0, 'Hunter - Serpent Sting R5' ),
( 13553, 0, 0.2,  0, 0, 'Hunter - Serpent Sting R6' ),
( 13554, 0, 0.2,  0, 0, 'Hunter - Serpent Sting R7' ),
( 13555, 0, 0.2,  0, 0, 'Hunter - Serpent Sting R8' ),

(   136, 0, 0.05, 0, 0, 'Hunter - Mend Pet R1' ),
(  3111, 0, 0.05, 0, 0, 'Hunter - Mend Pet R2' ),
(  3661, 0, 0.05, 0, 0, 'Hunter - Mend Pet R3' ),
(  3662, 0, 0.2,  0, 0, 'Hunter - Mend Pet R4' ),
( 13542, 0, 0.2,  0, 0, 'Hunter - Mend Pet R5' ),
( 13543, 0, 0.2,  0, 0, 'Hunter - Mend Pet R6' ),
( 13544, 0, 0.2,  0, 0, 'Hunter - Mend Pet R7' ),

(  1510, 0, 0.1667, 0, 0, 'Hunter - Volley R1' ),
( 14294, 0, 0.1667, 0, 0, 'Hunter - Volley R2' ),
( 14295, 0, 0.1667, 0, 0, 'Hunter - Volley R3' ),

( 23722, 100, 0, 0, 0, 'Hunter Trinket - Arcane Detonation' );

UPDATE creature_template SET dmg_multiplier = 11 WHERE entry = 12435;
UPDATE creature_template SET unit_flags = 33555202 WHERE entry = 12434;

DELETE FROM creature_ai_scripts WHERE creature_id IN (14825, 14883, 14989, 11357);
INSERT INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '14883',  '0',  '0', '100', '33','6300',  '7000',  '4000',  '7000','11',  '19717', '4', '0','11', '17937', '4', '0','11', '0', '0', '0','Voodoo Slave - Cast random' ),
(  '14883',  '0',  '0', '100', '1','3000',  '4000',  '10000',  '11000','11',  '12740', '0', '0','0', '0', '0', '0','0', '0', '0', '0','Voodoo Slave - Summon Infernal' ),
(  '14825',  '0',  '0', '100', '1','4000',  '12000',  '7000',  '14000','11',  '24673', '4', '0','0', '0', '0', '0','0', '0', '0', '0','Withered Mistress - Cast Curse of Blood' ),
(  '14989',  '29',  '0', '100', '1','26744',  '1',  '1',  '1','11',  '26744', '0', '0','0', '0', '0', '0','0', '0', '0', '0','Poisonous Cloud - Cast Poisonous Blood' ),
(  '11357',  '11',  '0', '100', '2','0',  '0',  '0',  '0','11',  '8876', '0', '1','0', '0', '0', '0','0', '0', '0', '0','Son of Hakkar - Cast Thrash on Spawn' ),
(  '11357',  '0',  '0', '100', '3','8000',  '15000',  '11000',  '16000','11',  '16790', '4', '0','0', '0', '0', '0','0', '0', '0', '0','Son of Hakkar - Cast Knockdown' ),
(  '11357',  '6',  '0', '100', '1','0',  '0',  '0',  '0','11',  '24319', '0', '7','0', '0', '0', '0','0', '0', '0', '0','Son of Hakkar - Summon Poison Cloud on Death' );

UPDATE creature_template SET AIName = 'EventAI' WHERE entry IN(14825, 14989);

UPDATE creature_model_info SET bounding_radius = 6 WHERE modelid = 13992;

UPDATE creature_template SET dmg_multiplier = 5 WHERE entry IN(12460, 12461);
UPDATE creature_template SET dmg_multiplier = 30 WHERE entry IN (14601, 11981, 11983);

UPDATE creature SET DeathState = 1 WHERE guid IN (139968, 139969);
UPDATE creature_template SET faction_A = 1114, faction_H = 1114 WHERE entry IN (14263, 14261, 14264, 14262, 14265, 14302);

UPDATE creature_model_info SET bounding_radius = 2.35, combat_reach = 2.4 WHERE modelid = 15295;

UPDATE creature_template SET modelid_1 = 11686, minlevel = 60, maxlevel = 60, unit_class = 1, unit_flags = 33554432, type_flags = 1024 WHERE ENTRY = 14989;

DELETE FROM creature_loot_template WHERE item = 19975;

UPDATE pool_creature SET chance = 20 WHERE pool_entry = 2004;

UPDATE creature_template SET ScriptName = "mob_chained_spirit" WHERE entry = 15117;

UPDATE creature SET spwantimessec = 60 WHERE id IN (51428, 51429, 51393, 51425, 51426);

DELETE FROM creature_loot_template WHERE item = 18147;

INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount) VALUES
( 13144, 18147, -100, 0, 1, 1 ),
( 13145, 18147, -100, 0, 1, 1 ),
( 13146, 18147, -100, 0, 1, 1 ),
( 13147, 18147, -100, 0, 1, 1 ),

UPDATE creature_template SET InhabitType = 4 WHERE entry = 10415;