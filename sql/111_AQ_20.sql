UPDATE `creature_template` SET rank=3, ScriptName='' WHERE entry=15341;
UPDATE `creature_template` SET armor=5500, ScriptName='npc_general_andorov', equipment_id=2238, npcflag=1, scale=1, RegenHealth=0 WHERE entry=15471;
UPDATE `creature_template` SET armor=3500, ScriptName='npc_kaldorei_elite', equipment_id=2239, RegenHealth=0 WHERE entry=15473;
UPDATE `creature_template` SET faction_A=14, faction_H=14, ScriptName='mob_buru_egg' WHERE entry=15514;
UPDATE `creature_template` SET ScriptName='boss_buru' WHERE entry=15370;
DELETE FROM `creature_ai_scripts` WHERE creature_id=15964;
DELETE FROM `creature_model_info` WHERE modelid=15742;
INSERT INTO `creature_model_info` VALUES (15742, 6, 7, 2, 0, 0);

#UPDATE creature_linking SET flag = flag | 3 WHERE guid >= 1105302 AND guid <= 1105614 AND flag & 512 = 512;
#UPDATE creature_linking SET flag = flag & ~3 WHERE master_guid IN(1105411, 1105444, 1105412);
DELETE FROM `creature_linking` WHERE guid IN(1105337, 1105338);
INSERT INTO `creature_linking` VALUES (1105338, 1105339, 3);
INSERT INTO `creature_linking` VALUES (1105337, 1105336, 3);

UPDATE `gameobject_template` SET size = 1.4, flags = 2 WHERE entry = 180691;

DELETE FROM `gossip_texts` WHERE entry IN(-3509000, -3509001);
INSERT INTO `gossip_texts` (`entry`, `content_default`, `content_loc3`) VALUES ('-3509000', 'Let\'s find out.', 'Das werden wir ja sehen.'); 
INSERT INTO `gossip_texts` (`entry`, `content_default`, `content_loc3`) VALUES ('-3509001', 'Let\'s see what you have.', 'Zeigt mal was Ihr habt.');

DELETE FROM `script_texts` WHERE entry=-1509029;
INSERT INTO `script_texts` VALUES (-1509029, 'Come get some!', NULL, NULL, 'Jetzt gibt\'s Prügel', NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 'andorov INTRO4');

DELETE FROM `script_texts` WHERE entry=-1509030;
INSERT INTO `script_texts` VALUES (-1509030, 'Kill first, ask questions later... Incoming!', NULL, NULL, 'Zuerst töten, dann Fragen stellen... Hier kommen sie!', NULL, NULL, NULL, NULL, NULL, 0, 1, 0, 0, 'andorov WAVE1');

DELETE FROM `script_texts` WHERE entry=-1509031;
INSERT INTO `script_texts` VALUES (-1509031, 'I lied...', NULL, NULL, 'Ich habe gelogen...', NULL, NULL, NULL, NULL, NULL, 0, 1, 0, 0, 'andorov INTRO2');

UPDATE `script_texts` SET `content_loc3`='%s entzieht Euch Euer Mana und versteinert Euch.' WHERE entry=-1509028;
UPDATE `script_texts` SET `content_default`='Remember, Rajaxx, when I said I\'d kill you last?', `content_loc3`='Erinnerst du dich, Rajaxx? Ich habe dir doch versprochen, dich zuletzt zu töten.' WHERE entry=-1509004;
UPDATE `script_texts` SET `content_loc3`='Hier kommen sie. Bleibt am Leben, Welpen.' WHERE entry=-1509003;
UPDATE `script_texts` SET `content_loc3`='%s behält %s im Blickfeld!' WHERE entry=-1509002;

UPDATE `creature_ai_texts` SET `content_loc3`='Tötet $N!' WHERE entry=-1119;
UPDATE `creature_ai_texts` SET `content_loc3`='Unverschämter Narr! Ich werde Euch höchstpersönlich töten!' WHERE entry=-701;
UPDATE `creature_ai_texts` SET `content_loc3`='Soldaten, Hauptmänner, gebt den Kampf nicht auf!' WHERE entry=-702;;
UPDATE `creature_ai_texts` SET `content_loc3`='Ihr verschwendet meine Zeit, %n!' WHERE entry=-703;

DELETE FROM `game_graveyard_zone` WHERE ghost_zone = 3429;
INSERT INTO `game_graveyard_zone` VALUES (913, 3429, 0);

#POLYMORPH + CHARM Immunity
UPDATE `creature_template` SET mechanic_immune_mask = mechanic_immune_mask | 65537 WHERE entry IN(15344, 15387, 15385, 15388, 15386, 15390, 15389, 15392, 15391);

#TRASH FIX
UPDATE `creature_ai_scripts` SET event_type=30, event_param1=0, event_param2=15324, event_param3=0, event_param4=0, COMMENT='Qiraji Gladiator - Cast Vengeance and Emote at Recieve Death Event' WHERE id=1532403;
DELETE FROM `creature_ai_scripts` WHERE id=1532404;
INSERT INTO `creature_ai_scripts` VALUES ('1532404','15324','4','0','100','0','0','0','0','0','46','1','0','0','0','0','0','0','0','0','0','0','Qiraji Gladiator - Set Send Death Event on Aggro');
UPDATE `creature_template` SET ScriptName='mob_hive_zara_stinger' WHERE entry=15327;
UPDATE `creature_template` SET armor=4091, mindmg=1587, maxdmg=2105, attackpower=211 WHERE entry=15343;
UPDATE `creature_template` SET armor=4091, mindmg=2540, maxdmg=3367, attackpower=468 WHERE entry=15324;
UPDATE `creature_template` SET armor=4091 WHERE entry IN(15387, 15344, 15336);
UPDATE `creature_template` SET armor=530 WHERE entry = 15514;
UPDATE `creature_template` SET armor=4691, mechanic_immune_mask = mechanic_immune_mask | 612506619 WHERE entry IN(15385, 15388, 15386, 15390, 15389, 15392, 15391);
UPDATE `creature_template` SET mechanic_immune_mask = mechanic_immune_mask | 1 WHERE entry=15343;
UPDATE `creature_template` SET AIName='', ScriptName='npc_obsidian_destroyer' WHERE entry=15338;
DELETE FROM `creature_ai_scripts` WHERE creature_id=15262;

DELETE FROM `creature_equip_template` WHERE entry IN(2238, 2239);
DELETE FROM `creature_equip_template_raw` WHERE entry IN(2238, 2239);
#INSERT INTO `creature_equip_template` VALUES (2238, 9425, 9425, 0);
INSERT INTO `creature_equip_template_raw` VALUES (2238, 21238, 21238, 0, 33492738, 50266116, 0, 781, 1815, 0);
INSERT INTO `creature_equip_template_raw` VALUES (2239, 5175, 0, 0, 33490946, 0, 0, 17, 0, 0);
DELETE FROM creature WHERE guid=9000441;
INSERT INTO creature VALUES (9000441,15323,509,0,0,-9218.12,1385.47,21.3865,2.26028,21600,5,0,15738,0,0,0);
DELETE FROM creature WHERE guid=9000442;
INSERT INTO creature VALUES (9000442,15323,509,0,0,-9317.34,1379.56,21.4228,0.824878,21600,5,0,15738,0,0,0);
DELETE FROM creature WHERE guid=9000443;
INSERT INTO creature VALUES (9000443,15320,509,0,0,-9314.58,1388.6,21.4249,5.31321,21600,5,0,31456,0,0,0);
DELETE FROM creature WHERE guid=9000444;
INSERT INTO creature VALUES (9000444,15320,509,0,0,-9307.45,1379.85,21.4251,2.7533,21600,5,0,31456,0,0,0);
UPDATE `creature_template` SET `scale`='1.8' WHERE (`entry`='15370');
UPDATE `creature_template` SET `scale`='2' WHERE (`entry`='15319');

UPDATE `creature_template` SET `speed_walk`='4' WHERE (`entry`='15343');
UPDATE `creature_template` SET `scale`='0.4' WHERE (`entry`='15168');
UPDATE `creature_template` SET `scale`='1.8' WHERE (`entry`='15348');
UPDATE `creature_template` SET `speed_walk`='1.2' WHERE (`entry`='15348');
UPDATE `creature_template` SET `baseattacktime`='1400' WHERE (`entry`='15348');
UPDATE `creature_template` SET `scale`='0.7' WHERE (`entry`='15462');
UPDATE `creature_template` SET `scale`='1' WHERE (`entry`='15461');

UPDATE `creature_template` SET `speed_walk`='1.4' WHERE entry IN(15325, 15327);
UPDATE `creature_template` SET `mindmg`='1992',`maxdmg`='2393',`attackpower`='921', `baseattacktime`='1300' WHERE (`entry`='15341');
UPDATE `creature_template` SET `armor`='3675',`scale`='1.5',`mindmg`='669',`maxdmg`='1098',`baseattacktime`='1200' WHERE (`entry`='15471');
UPDATE `creature_template` SET `mindmg`='580',`maxdmg`='914',`baseattacktime`='1800' WHERE (`entry`='15473');
UPDATE `creature_template` SET `ScriptName`='boss_rajaxx' WHERE (`entry`='15341');

#Buru
UPDATE `creature_template` SET minhealth=666250, maxhealth=666250 WHERE `entry`=15370;
#Moam
UPDATE `creature_template` SET minhealth=346450, maxhealth=346450, rank=3 WHERE `entry`=15340;
#Ossirian
UPDATE `creature_template` SET minhealth=499650, maxhealth=499650, rank=3 WHERE `entry`=15339;

-- Loot Trash Fixes:
-- Nicht 100% Bliizlike weil eventuell einzelne Monster höhere Dropchancen haben aber das kann man nachträglich immernoch anpassen
-- hauptsache es rennen nicht gleich in der ersten Woche mit den Questitems rum und das AH platzt vor lauter Kristellen und Götzen.

-- Götzen:
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20866');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20870');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20868');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20873');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20869');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20871');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20867');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='0.4' WHERE (`item`='20872');

-- Kristalle
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20862');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20859');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20863');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20861');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20865');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20860');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20864');
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`='1.6' WHERE (`item`='20858');

-- Nicht mehr Festwurzelbar
UPDATE `creature_template` SET `mechanic_immune_mask`='577' WHERE (`entry`='15327');
UPDATE `creature_template` SET `mechanic_immune_mask`='735825' WHERE (`entry`='15555');