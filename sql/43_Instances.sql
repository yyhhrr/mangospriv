INSERT INTO creature_ai_scripts (creature_id, event_type, event_flags, action1_type) VALUES
( 8982, 4, 2, 24 );


UPDATE creature_template SET npcflag = 7 WHERE entry = 9087;


DELETE FROM creature_loot_template WHERE entry IN(10981, 10990) AND item IN(17306, 17326, 17327, 17328, 17422, 17423, 17502, 17503, 17504 , 18143, 18144, 18145, 18146, 18147, 18206, 18207);
UPDATE creature_template SET faction_A = 7, faction_H = 7 WHERE entry = 10981;


UPDATE `quest_template` SET `RewSpell`=2652, `RewSpellCast`=19318 WHERE `entry` IN(5658,5659,5660,5661,5662,5663);
UPDATE `quest_template` SET `NextQuestID`=0,`PrevQuestId`=0,`NextQuestInChain` =0 WHERE `entry` IN(5658,5659);
UPDATE `quest_template` SET `ExclusiveGroup`=5658 WHERE `entry`IN (5658,5659,5660,5661,5662,5663);
UPDATE quest_template SET RequiredClasses = 16 WHERE `entry` IN(5658,5659,5660,5661,5662,5663);


DELETE FROM `spell_proc_event` WHERE `entry`=16164;
INSERT INTO `spell_proc_event` VALUES (16164,0,11,2416967683,0,0,0,0,0,0,0);


REPLACE INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(16166,0,11,2416967683,0,0,16384,0,0,0,0),
(16246,0,11,2416967683,0,0,16384,0,0,0,0),
(11213,0,11,2416967683,0,0,16384,0,0,0,0),
(12574,0,11,2416967683,0,0,16384,0,0,0,0),
(12575,0,11,2416967683,0,0,16384,0,0,0,0),
(12576,0,11,2416967683,0,0,16384,0,0,0,0),
(12577,0,11,2416967683,0,0,16384,0,0,0,0);


UPDATE `spell_proc_event` SET `ppmRate`=0, `CustomChance`=2 WHERE `entry`=15600;



--Palaquest
INSERT INTO `creature_ai_summons` (`id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `comment`) VALUES
(13001,14.3925,475.848,-23.3028,1.55431,300000,'Uralter Pferdegeist');
INSERT INTO `creature_ai_scripts` VALUES (1148906,11489,6,0,100,2,0,0,0,0,32,14566,0,13001,0,0,0,0,0,0,0,0, 'Tendris Warpwood - Summon Ancient Equine Spirit on Death');

-- Tür schließen
INSERT INTO `event_scripts` (`id`,`delay`,`command`,`datalong`,`datalong2`,`comments`) VALUES 
(8436,1,12,179724,3000000,'Close Rattlegore door on event start');
-- alte Spawns löschen
DELETE FROM `event_scripts` WHERE `id`=8436 AND `command`=10;
-- neue Spawns mit Wellen, ausgelegt auf ca. 20 Minuten Dauer
INSERT INTO `event_scripts` VALUES
(8436,15,10,14514,3000000,0,0,0,0,0,0,0,131.69,167.65,94.12,0,'First Phase - Banal Spirit 1'),
(8436,15,10,14514,3000000,0,0,0,0,0,0,0,131.97,176.71,94.91,0,'First Phase - Banal Spirit 2'),
(8436,15,10,14514,3000000,0,0,0,0,0,0,0,139.39,176.93,95.30,0,'First Phase - Banal Spirit 3'),
(8436,15,10,14514,3000000,0,0,0,0,0,0,0,142.23,168.75,95.20,0,'First Phase - Banal Spirit 4'),
(8436,15,10,14514,3000000,0,0,0,0,0,0,0,138.51,172.07,95.84,0,'First Phase - Banal Spirit 5'),
(8436,45,10,14514,3000000,0,0,0,0,0,0,0,131.69,167.65,94.12,0,'First Phase - Banal Spirit 1b'),
(8436,45,10,14514,3000000,0,0,0,0,0,0,0,131.97,176.71,94.91,0,'First Phase - Banal Spirit 2b'),
(8436,45,10,14514,3000000,0,0,0,0,0,0,0,139.39,176.93,95.30,0,'First Phase - Banal Spirit 3b'),
(8436,45,10,14514,3000000,0,0,0,0,0,0,0,142.23,168.75,95.20,0,'First Phase - Banal Spirit 4b'),
(8436,45,10,14518,3000000,0,0,0,0,0,0,0,138.51,172.07,95.84,0,'First Phase - Aspect of Banality'),
(8436,100,10,14513,3000000,0,0,0,0,0,0,0,150.03,155.82,93.08,0,'Second Phase - Malicious Spirit 1'),
(8436,100,10,14513,3000000,0,0,0,0,0,0,0,122.70,155.66,92.52,0,'Second Phase - Malicious Spirit 2'),
(8436,100,10,14513,3000000,0,0,0,0,0,0,0,121.63,178.02,93.08,0,'Second Phase - Malicious Spirit 3'),
(8436,100,10,14513,3000000,0,0,0,0,0,0,0,150.99,188.94,93.08,0,'Second Phase - Malicious Spirit 4'),
(8436,340,10,14520,3000000,0,0,0,0,0,0,0,138.51,172.07,95.84,0,'Second Phase - Aspect of Malice'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,155.64,148.26,93.08,0,'Third Phase - Group 1'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,151.38,144.88,93.08,0,'Third Phase - Group 1'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,154.11,147.36,93.08,0,'Third Phase - Group 1'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,142.11,168.61,95.21,0,'Third Phase - Group 2'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,142.22,177.50,95.18,0,'Third Phase - Group 2'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,131.94,170.91,94.93,0,'Third Phase - Group 2'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,106.25,185.98,92.90,0,'Third Phase - Group 3'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,109.92,186.08,92.84,0,'Third Phase - Group 3'),
(8436,400,10,14512,3000000,0,0,0,0,0,0,0,111.35,189.40,92.98,0,'Third Phase - Group 3'),
(8436,800,10,14519,3000000,0,0,0,0,0,0,0,138.51,172.07,95.84,0,'Third Phase - Aspect of Corruption'),
(8436,850,10,14511,3000000,0,0,0,0,0,0,0,154.70,147.05,93.08,0,'Fourth Phase - Shadowed Spirit 1'),
(8436,850,10,14511,3000000,0,0,0,0,0,0,0,155.94,190.02,93.09,0,'Fourth Phase - Shadowed Spirit 2'),
(8436,850,10,14511,3000000,0,0,0,0,0,0,0,111.89,187.70,92.89,0,'Fourth Phase - Shadowed Spirit 3'),
(8436,920,10,14511,3000000,0,0,0,0,0,0,0,154.70,147.05,93.08,0,'Fourth Phase - Single Shadowed Spirit'),
(8436,960,10,14511,3000000,0,0,0,0,0,0,0,155.94,190.02,93.09,0,'Fourth Phase - Single Shadowed Spirit'),
(8436,1000,10,14511,3000000,0,0,0,0,0,0,0,111.89,187.70,92.89,0,'Fourth Phase - Single Shadowed Spirit'),
(8436,1040,10,14521,3000000,0,0,0,0,0,0,0,138.51,172.07,95.84,0,'Fourth Phase - Aspect of Shadow');

DELETE FROM `creature_ai_scripts` WHERE `creature_id`=14516;
REPLACE INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(1451402,14514,8,0,100,2,20355,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Banal Spirit – Cast Holy Blast on hit by Judgement of Wisdom'),
(1451804,14518,8,0,100,2,20355,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Aspect of Banality – Cast Holy Blast on hit by Judgement of Wisdom'),
(1451303,14513,8,0,100,2,20184,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Malicious Spirit – Cast Holy Blast on hit by JoJ'),
(1452003,14520,8,0,100,2,20184,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Aspect of Malice – Cast Holy Blast on hit by JoJ'),
(1451202,14512,8,0,100,2,20286,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Corrputed Spirit – Cast Holy Blast on hit by JoRightousness'),
(1451203,14512,1,0,100,2,1000,1000,1800000,1800000,11,23245,0,1,0,0,0,0,0,0,0,0,'Corrupted Spirit - Cast Corrupted Stamina on spawn'),
(1451903,14519,8,0,100,2,20286,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Aspect of Corruption – Cast Holy Blast on hit by JoRighteousness'),
(1451105,14511,8,0,100,2,20346,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Shadowed Spirit – Cast Holy Blast on hit by JoLight'),
(1452105,14521,8,0,100,2,20346,0,0,0,11,23269,0,1,0,0,0,0,0,0,0,0,'Aspect of Shadow – Cast Holy Blast on hit by JoLight'),
(1452106,14521,6,0,100,2,0,0,0,0,12,14516,1,3000000,0,0,0,0,0,0,0,0,'Aspect of Shadow - Summon Darkreaver in Death'),
(1451602,14516,0,0,100,3,5000,6000,8000,10000,11,15284,1,0,0,0,0,0,0,0,0,0,'Death Knight Darkreaver - Cast Cleave'),
(1451603,14516,0,0,100,3,6000,7000,12000,14000,11,17393,4,0,0,0,0,0,0,0,0,0,'Death Knight Darkreaver - Cast Shadow Bolt'),
(1451604,14516,0,0,100,3,18000,21000,32000,35000,11,7645,5,0,0,0,0,0,0,0,0,0,'Death Knight Darkreaver - Cast Dominate Mind'),
(1451605,14516,6,0,100,2,0,0,0,0,11,23261,0,7,0,0,0,0,0,0,0,0,'Death Knight Darkreaver - Cast Summon Darkreaver\'s Fallen Charger on Death');
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=14516;

UPDATE creature_template SET AIName = "EventAI" WHERE entry = 14521;


INSERT INTO spell_script_target VALUES ( 16054, 0, 175321 );