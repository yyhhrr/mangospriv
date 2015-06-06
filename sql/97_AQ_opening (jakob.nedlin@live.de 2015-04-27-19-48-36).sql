DELETE FROM `gameobject` WHERE guid=633301;
DELETE FROM `gameobject` WHERE guid=633338;
DELETE FROM `gameobject` WHERE guid=633343;
DELETE FROM `gameobject` WHERE guid=633344;
DELETE FROM `gameobject` WHERE guid=633345;
DELETE FROM `gameobject` WHERE guid=633346;
DELETE FROM `gameobject` WHERE guid=633347;

DELETE FROM `game_event_gameobject` WHERE EVENT='22';

INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('633338', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('633343', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('633344', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('633345', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('633346', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('29294', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('29301', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('29300', '22');
INSERT INTO `game_event_gameobject` (guid, EVENT) VALUES ('29299', '22');

#INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633339','180598','0','-4971.55','-1148.57','501.648','2.28638','0','0','0.909961','0.414693','180','100','1');
#INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633340','180679','0','-4937.29','-1282.74','501.672','2.26893','0','0','0.906308','0.422618','180','100','1');
#INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633341','180680','0','-4913.85','-1226','501.651','2.25148','0','0','0.902585','0.430511','180','100','1');
#INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633342','180681','0','-4958.51','-1179.32','501.659','2.26893','0','0','0.906308','0.422618','180','100','1');
INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633343','180812','1','1590.82','-4155.33','36.2926','-2.58309','0','0','0.961262','-0.275637','180','100','1');
INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633344','180818','1','1637.11','-4147.21','36.0414','-2.54818','0','0','0.956305','-0.292372','180','100','1');
INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633345','180826','1','1579.35','-4109.25','34.5417','-2.53073','0','0','0.953717','-0.300706','180','100','1');
INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633346','180832','1','1619.83','-4092.43','34.5107','-2.58309','0','0','0.961262','-0.275637','10','100','1');
INSERT INTO `gameobject` (guid, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUE ('633338','180838','1','1683.11','-4134.35','39.5419','-2.56563','0','0','0.95882','-0.284015','180','100','1');

DELETE FROM `world_events` WHERE id >= 3 AND id <= 32;

INSERT INTO `world_events` (id, NAME, progress) VALUES ('3', 'AQ War Effort H COPPER BAR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('4', 'AQ War Effort A COPPER BAR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('5', 'AQ War Effort A PURPLE LOTUS', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('6', 'AQ War Effort H PURPLE LOTUS', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('7', 'AQ War Effort A THICK LEATHER', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('8', 'AQ War Effort H THICK LEATHER', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('9', 'AQ War Effort A SPOTTED YELLOWTAIL', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('10', 'AQ War Effort H SPOTTED YELLOWTAIL', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('11', 'AQ War Effort A RUNECLOTH BANDAGE', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('12', 'AQ War Effort H RUNECLOTH BANDAGE', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('13', 'AQ War Effort A IRON BAR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('14', 'AQ War Effort A THORUM BAR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('15', 'AQ War Effort A ARTHAS TEAR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('16', 'AQ War Effort A STRANGLEKELP', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('17', 'AQ War Effort A LIGHT LEATHER', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('18', 'AQ War Effort A MEDIUM LEATHER', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('19', 'AQ War Effort A ROAST RAPTOR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('20', 'AQ War Effort A RAINBOW FIN ALBACORE', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('21', 'AQ War Effort A LINEN BANDAGE', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('22', 'AQ War Effort A SILK BANDAGE', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('23', 'AQ War Effort H TIN BAR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('24', 'AQ War Effort H MITHRIL BAR', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('25', 'AQ War Effort H PEACEBLOOM', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('26', 'AQ War Effort H FIREBLOOM', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('27', 'AQ War Effort H HEAVY LEATHER', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('28', 'AQ War Effort H RUGGED LEATHER', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('29', 'AQ War Effort H LEAN WOLF STEAK', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('30', 'AQ War Effort H BAKED SALMON', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('31', 'AQ War Effort H WOOL BANDAGES', '0');
INSERT INTO `world_events` (id, NAME, progress) VALUES ('32', 'AQ War Effort H MAGEWEAVE BANDAGE', '0');

DELETE FROM `creature` WHERE guid>=349110 AND guid<=349149;
DELETE FROM `game_event_creature` WHERE guid>=349110 AND guid<=349149;

# Horde Collectors
INSERT INTO creature VALUES (349110,15512,1,0,2232,1632.36,-4142,34.5861,2.89257,25,5,0,8200,0,0,0);
INSERT INTO creature VALUES (349111,15508,1,0,2231,1625.46,-4149.13,36.2267,2.26817,25,5,0,6430,0,0,0);
INSERT INTO creature VALUES (349112,15477,1,0,2230,1616.2,-4144.9,34.9202,0.732711,25,5,0,5960,0,0,0);
INSERT INTO creature VALUES (349113,15459,1,0,2229,1652.69,-4123.85,31.7023,2.74615,25,5,0,807,0,0,0);
INSERT INTO creature VALUES (349114,15460,1,0,2228,1669.7,-4118.69,35.55,3.49638,25,5,0,844,0,0,0);
INSERT INTO creature VALUES (349115,15469,1,0,2227,1656.23,-4119.53,32.624,1.16216,25,5,0,881,0,0,0);
INSERT INTO creature VALUES (349116,15533,1,0,2226,1640.36,-4084.25,37.2147,4.3336,25,5,0,807,0,0,0);
INSERT INTO creature VALUES (349117,15535,1,0,2225,1634.41,-4085.65,36.351,5.51425,25,5,0,15260,0,0,0);
INSERT INTO creature VALUES (349118,15532,1,0,2221,1565.18,-4124.19,37.3327,6.18262,25,5,0,7700,0,0,0);
INSERT INTO creature VALUES (349119,15529,1,0,0,1570.48,-4119.11,36.579,5.1710,25,5,0,6000,0,0,0);
INSERT INTO creature VALUES (349120,15528,1,0,2220,1579.01,-4117.4,34.5804,4.94258,25,5,0,5960,0,0,0);
INSERT INTO creature VALUES (349121,15534,1,0,2224,1629.51,-4090.61,35.2169,5.81831,25,5,0,844,0,0,0);
INSERT INTO creature VALUES (349122,15522,1,0,2222,1596.43,-4157.85,36.6147,3.0119,25,5,0,5000,0,0,0);
INSERT INTO creature VALUES (349123,15515,1,0,2222,1590.22,-4179.7,39.83,3.25429,25,5,0,5960,0,0,0);
INSERT INTO creature VALUES (349124,15525,1,0,2223,1595.76,-4171.98,39.2254,2.61769,25,5,0,6000,0,0,0);
# Horde other NPCs
INSERT INTO creature VALUES (349125,15458,1,0,2234,1633.09,-4123.06,31.3259,2.0765,25,5,0,910,0,0,0);
INSERT INTO creature VALUES (349126,3296,1,0,133,1634.12,-4118.74,31.1589,1.97926,25,5,0,4957,0,0,0);
INSERT INTO creature VALUES (349127,3296,1,0,133,1628.84,-4121.72,31.0622,2.18346,25,5,0,4957,0,0,0);
INSERT INTO creature VALUES (349128,15700,1,0,2233,1577.38,-4196.73,40.9912,1.66348,25,0,0,3400,0,0,2);
INSERT INTO creature VALUES (349129,15704,1,0,2235,1656.37,-4401.9,19.4024,4.65391,25,5,0,6900,0,0,0);
INSERT INTO creature VALUES (349130,15703,0,0,2236,1574.83,273.622,-43.1026,5.50895,25,5,0,1700,0,0,0);
INSERT INTO creature VALUES (349131,15702,1,0,2237,-1209.94,100.796,134.472,3.31345,25,5,0,4120,0,0,0);
# Alliance Collectors
INSERT INTO creature VALUES (349132,15455,0,0,847,-4937.95,-1276.57,501.67,2.60858,25,5,0,807,0,0,0);
INSERT INTO creature VALUES (349133,15383,0,0,1419,-4924.8,-1222.76,501.635,3.97831,25,5,0,807,0,0,0);
INSERT INTO creature VALUES (349134,15445,0,0,639,-4948.84,-1274.63,501.673,1.03779,25,5,0,3900,0,0,0);
INSERT INTO creature VALUES (349135,15451,0,0,814,-4972,-1151.69,501.656,3.63642,25,5,0,807,0,0,0);
INSERT INTO creature VALUES (349136,15456,0,0,0,-4941.07,-1278.13,501.671,2.16827,25,5,0,844,0,0,0);
INSERT INTO creature VALUES (349137,15448,0,0,877,-4966.29,-1175.7,501.656,3.59708,25,5,0,3400,0,0,0);
INSERT INTO creature VALUES (349138,15434,0,0,694,-4946.93,-1275.99,501.671,0.826064,25,5,0,6100,0,0,0);
INSERT INTO creature VALUES (349139,15452,0,0,0,-4978.2,-1150.26,501.651,3.47312,25,5,0,2700,0,0,0);
INSERT INTO creature VALUES (349140,15437,0,0,1469,-4945.44,-1281.98,501.676,1.18136,25,5,0,2000,0,0,0);
INSERT INTO creature VALUES (349141,15450,0,0,237,-4966.42,-1180.41,501.66,3.77179,25,5,0,881,0,0,0);
INSERT INTO creature VALUES (349142,15453,0,0,838,-4977.65,-1140.77,501.657,3.74096,25,5,0,881,0,0,0);
INSERT INTO creature VALUES (349143,15457,0,0,1,-4933.93,-1279.2,501.666,2.62883,25,5,0,881,0,0,0);
INSERT INTO creature VALUES (349144,15432,0,0,0,-4927.67,-1219.3,501.634,3.76215,25,5,0,881,0,0,0);
INSERT INTO creature VALUES (349145,15431,0,0,639,-4913.7,-1228.17,501.651,3.75629,25,5,0,844,0,0,0);
# Alliance other NPCs
INSERT INTO creature VALUES (349146,15539,0,0,1411,-4983.9,-1204.67,501.67,2.84738,25,5,0,1800,0,0,0);
INSERT INTO creature VALUES (349147,5595,0,0,783,-4986.37,-1201.86,501.669,2.61085,25,5,0,5194,0,0,0);
INSERT INTO creature VALUES (349148,5595,0,0,783,-4986.64,-1206.56,501.67,3.27922,25,5,0,5194,0,0,0);
INSERT INTO creature VALUES (349149,15701,0,0,1471,-4959.27,-1201.3,501.66,2.09621,25,0,0,2900,0,0,2);

DELETE FROM `creature_addon` WHERE guid=349125;
INSERT INTO `creature_addon` VALUES (349125, 14346, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349118;
INSERT INTO `creature_addon` VALUES (349118, 0, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349122;
INSERT INTO `creature_addon` VALUES (349122, 0, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349128;
INSERT INTO `creature_addon` VALUES (349128, 14573, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349129;
INSERT INTO `creature_addon` VALUES (349129, 0, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349130;
INSERT INTO `creature_addon` VALUES (349130, 0, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349131;
INSERT INTO `creature_addon` VALUES (349131, 0, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349146;
INSERT INTO `creature_addon` VALUES (349146, 14575, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE guid=349149;
INSERT INTO `creature_addon` VALUES (349149, 14347, 0, 0, 0, 0, 0, NULL);

DELETE FROM `creature_equip_template` WHERE entry >= 2220 AND entry <= 2237;
INSERT INTO `creature_equip_template` VALUES (2220, 1159, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2221, 12282, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2222, 4565, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2223, 6660, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2224, 6256, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2225, 13885, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2226, 11411, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2227, 10799, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2228, 2134, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2229, 778, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2230, 3420, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2231, 6654, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2232, 19115, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2233, 2491, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2234, 3492, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2235, 2490, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2236, 4445, 0, 0);
INSERT INTO `creature_equip_template` VALUES (2237, 13953, 0, 0);

INSERT INTO `game_event_creature` VALUES (349110, 22);
INSERT INTO `game_event_creature` VALUES (349111, 22);
INSERT INTO `game_event_creature` VALUES (349112, 22);
INSERT INTO `game_event_creature` VALUES (349113, 22);
INSERT INTO `game_event_creature` VALUES (349114, 22);
INSERT INTO `game_event_creature` VALUES (349115, 22);
INSERT INTO `game_event_creature` VALUES (349116, 22);
INSERT INTO `game_event_creature` VALUES (349117, 22);
INSERT INTO `game_event_creature` VALUES (349118, 22);
INSERT INTO `game_event_creature` VALUES (349119, 22);
INSERT INTO `game_event_creature` VALUES (349120, 22);
INSERT INTO `game_event_creature` VALUES (349121, 22);
INSERT INTO `game_event_creature` VALUES (349122, 22);
INSERT INTO `game_event_creature` VALUES (349123, 22);
INSERT INTO `game_event_creature` VALUES (349124, 22);
INSERT INTO `game_event_creature` VALUES (349125, 22);
INSERT INTO `game_event_creature` VALUES (349126, 22);
INSERT INTO `game_event_creature` VALUES (349127, 22);
INSERT INTO `game_event_creature` VALUES (349128, 22);
INSERT INTO `game_event_creature` VALUES (349129, 22);
INSERT INTO `game_event_creature` VALUES (349130, 22);
INSERT INTO `game_event_creature` VALUES (349131, 22);
INSERT INTO `game_event_creature` VALUES (349132, 22);
INSERT INTO `game_event_creature` VALUES (349133, 22);
INSERT INTO `game_event_creature` VALUES (349134, 22);
INSERT INTO `game_event_creature` VALUES (349135, 22);
INSERT INTO `game_event_creature` VALUES (349136, 22);
INSERT INTO `game_event_creature` VALUES (349137, 22);
INSERT INTO `game_event_creature` VALUES (349138, 22);
INSERT INTO `game_event_creature` VALUES (349139, 22);
INSERT INTO `game_event_creature` VALUES (349140, 22);
INSERT INTO `game_event_creature` VALUES (349141, 22);
INSERT INTO `game_event_creature` VALUES (349142, 22);
INSERT INTO `game_event_creature` VALUES (349143, 22);
INSERT INTO `game_event_creature` VALUES (349144, 22);
INSERT INTO `game_event_creature` VALUES (349145, 22);
INSERT INTO `game_event_creature` VALUES (349146, 22);
INSERT INTO `game_event_creature` VALUES (349147, 22);
INSERT INTO `game_event_creature` VALUES (349148, 22);
INSERT INTO `game_event_creature` VALUES (349149, 22);

DELETE FROM `creature_questrelation` WHERE id=15702 AND quest=8792;
INSERT INTO `creature_questrelation` VALUES (15702, 8792);
DELETE FROM `creature_questrelation` WHERE id=15535 AND quest=8616;
INSERT INTO `creature_questrelation` VALUES (15535, 8616);
DELETE FROM `creature_questrelation` WHERE id=15452 AND quest=8520;
INSERT INTO `creature_questrelation` VALUES (15452, 8520);

UPDATE `creature_template` SET npcflag = (npcflag | 3) WHERE entry=15702;
UPDATE `creature_template` SET minlevel=60, maxlevel=60, minhealth=8000, maxhealth=10200, armor=4750, mindmg=562, maxdmg=726, ScriptName='npc_aq_war_effort_collector' WHERE entry=15525;
UPDATE `creature_template` SET minlevel=60, maxlevel=60, minhealth=8000, maxhealth=10200, armor=4750, mindmg=562, maxdmg=726, ScriptName='npc_aq_war_effort_collector' WHERE entry=15522;
UPDATE `creature_template` SET minlevel=60, maxlevel=60, minhealth=8000, maxhealth=10200, armor=4750, mindmg=562, maxdmg=726, ScriptName='npc_aq_war_effort_collector' WHERE entry=15529;
UPDATE `creature_template` SET minlevel=60, maxlevel=60, minhealth=8000, maxhealth=10200, armor=4750, mindmg=562, maxdmg=726, ScriptName='npc_aq_war_effort_collector' WHERE entry=15512;
UPDATE `creature_template` SET minlevel=60, maxlevel=60, minhealth=8000, maxhealth=10200, armor=4750, mindmg=562, maxdmg=726, ScriptName='npc_aq_war_effort_collector' WHERE entry=15532;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15508;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15477;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15459;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15460;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15469;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15533;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15535;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15528;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15534;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15515;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15383;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15431;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15432;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15434;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15437;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15445;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15446;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15448;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15450;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15451;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15452;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15453;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15455;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15456;
UPDATE `creature_template` SET ScriptName='npc_aq_war_effort_collector' WHERE entry=15457;

UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8583;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8581;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8550;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8533;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8543;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8546;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8612;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8616;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8610;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8608;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8605;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8614;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8591;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8589;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8601;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8510;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8493;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8495;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8512;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8518;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8514;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8506;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8525;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8527;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8523;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8521;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8529;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8504;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8516;
UPDATE `quest_template` SET SpecialFlags=1 WHERE entry=8500;

# Warlord Gorchuk
DELETE FROM `creature_movement` WHERE id=349128;
INSERT INTO `creature_movement` VALUES ('349128','1','1576.64','-4189.98','39.9675','0','0','0','0','0','0','0','0','0','0','1.50483','0','0');
INSERT INTO `creature_movement` VALUES ('349128','2','1577.01','-4183.48','38.8773','0','0','0','0','0','0','0','0','0','0','1.37131','0','0');
INSERT INTO `creature_movement` VALUES ('349128','3','1577.25','-4174.46','37.4801','0','0','0','0','0','0','0','0','0','0','0.991959','0','0');
INSERT INTO `creature_movement` VALUES ('349128','4','1578.09','-4167.71','36.3831','0','0','0','0','0','0','0','0','0','0','1.01499','0','0');
INSERT INTO `creature_movement` VALUES ('349128','5','1581.39','-4159.61','35.1655','0','0','0','0','0','0','0','0','0','0','1.08175','0','0');
INSERT INTO `creature_movement` VALUES ('349128','6','1588.67','-4149.38','33.9235','0','0','0','0','0','0','0','0','0','0','0.908961','0','0');
INSERT INTO `creature_movement` VALUES ('349128','7','1592.66','-4144.2','33.1641','0','0','0','0','0','0','0','0','0','0','0.920741','0','0');
INSERT INTO `creature_movement` VALUES ('349128','8','1598.74','-4136.46','31.7757','0','0','0','0','0','0','0','0','0','0','0.908961','0','0');
INSERT INTO `creature_movement` VALUES ('349128','9','1609.59','-4124.91','30.0022','0','0','0','0','0','0','0','0','0','0','0.798218','0','0');
INSERT INTO `creature_movement` VALUES ('349128','10','1618.42','-4117.79','29.635','0','0','0','0','0','0','0','0','0','0','0.405517','0','0');
INSERT INTO `creature_movement` VALUES ('349128','11','1632.69','-4110.96','29.8144','0','0','0','0','0','0','0','0','0','0','0.341132','0','0');
INSERT INTO `creature_movement` VALUES ('349128','12','1661.24','-4099.46','33.9335','0','0','0','0','0','0','0','0','0','0','0.260237','0','0');
INSERT INTO `creature_movement` VALUES ('349128','13','1627.73','-4104.2','30.8142','0','0','0','0','0','0','0','0','0','0','4.00581','0','0');
INSERT INTO `creature_movement` VALUES ('349128','14','1610.51','-4115.19','29.7288','0','0','0','0','0','0','0','0','0','0','3.89664','0','0');
INSERT INTO `creature_movement` VALUES ('349128','15','1594.74','-4131.56','30.8432','0','0','0','0','0','0','0','0','0','0','4.14404','0','0');
INSERT INTO `creature_movement` VALUES ('349128','16','1585.64','-4145.86','33.215','0','0','0','0','0','0','0','0','0','0','4.1676','0','0');
INSERT INTO `creature_movement` VALUES ('349128','17','1578','-4158.46','35.6619','0','0','0','0','0','0','0','0','0','0','4.39144','0','0');

# Field Marshal Snowfall
DELETE FROM `creature_movement` WHERE id=349149;
INSERT INTO `creature_movement` VALUES ('349149','1','-4970.93','-1185.41','501.66','0','0','0','0','0','0','0','0','0','0','1.3383','0','0');
INSERT INTO `creature_movement` VALUES ('349149','2','-4980.99','-1167.51','501.65','0','0','0','0','0','0','0','0','0','0','0.892198','0','0');
INSERT INTO `creature_movement` VALUES ('349149','3','-4987.88','-1145.44','501.658','0','0','0','0','0','0','0','0','0','0','0.54348','0','0');
INSERT INTO `creature_movement` VALUES ('349149','4','-5005.98','-1158.54','501.658','0','0','0','0','0','0','0','0','0','0','4.88594','0','0');
INSERT INTO `creature_movement` VALUES ('349149','5','-4997.91','-1189.98','501.661','0','0','0','0','0','0','0','0','0','0','5.22995','0','0');
INSERT INTO `creature_movement` VALUES ('349149','6','-4984.3','-1219.86','501.674','0','0','0','0','0','0','0','0','0','0','5.28728','0','0');
INSERT INTO `creature_movement` VALUES ('349149','7','-4960.42','-1251.38','501.669','0','0','0','0','0','0','0','0','0','0','5.13099','0','0');
INSERT INTO `creature_movement` VALUES ('349149','8','-4946.72','-1268.73','501.669','0','0','0','0','0','0','0','0','0','0','5.38311','0','0');
INSERT INTO `creature_movement` VALUES ('349149','9','-4930.44','-1256.3','501.66','0','0','0','0','0','0','0','0','0','0','1.00844','0','0');
INSERT INTO `creature_movement` VALUES ('349149','10','-4923.39','-1234.24','501.658','0','0','0','0','0','0','0','0','0','0','1.20636','0','0');
INSERT INTO `creature_movement` VALUES ('349149','11','-4936.07','-1219.88','501.644','0','0','0','0','0','0','0','0','0','0','2.42687','0','0');

#"You want to know about ration collections? Very well. Last I was told, the Horde had prepared $2095w out of $2096w lean wolf steaks. We've also packed away $2102w of $2103w spotted yellowtail and $210...
#horde amb menuid 6665 text 7942 
#alliance menuid 6772 Text 8082
#alli amb Text 7949
#horde text 8092

DELETE FROM `gossip_menu` WHERE entry>=10267 AND entry<=10280;
DELETE FROM `gossip_menu_option` WHERE menu_id>=10267 AND menu_id<=10280;
DELETE FROM `locales_gossip_menu_option` WHERE menu_id>=10267 AND menu_id<=10280;

UPDATE `creature_template` SET gossip_menu_id=6772, speed_walk=0.75, speed_run=1.76 WHERE entry=15701;
UPDATE `creature_template` SET gossip_menu_id=6665, speed_walk=0.75, speed_run=1.76 WHERE entry=15539;

# Warlord Gorchuk Gossip
UPDATE `creature_template` SET gossip_menu_id=10267, npcflag = (npcflag | 1), speed_walk=0.75, speed_run=1.76 WHERE entry=15700;
INSERT INTO `gossip_menu` VALUES (10267, 8092, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10267','0','0','What is the Ahn\'Qiraj war effort?','1','1','10268','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10267','1','0','How many metal bars have the Horde collected so far?','1','1','10269','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10267','2','0','How many herbs have the Horde collected so far?','1','1','10270','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10267','3','0','How many leather skins have the Horde collected so far?','1','1','10271','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10267','4','0','How many bandages have the Horde collected so far?','1','1','10272','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10267','5','0','How many cooked goods have the Horde collected so far?','1','1','10273','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10267','0','What is the Ahn\'Qiraj war effort?',NULL,'Was ist die Ahn\'Qiraj Kriegsvorbereitung?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10267','1','How many metal bars have the Horde collected so far?',NULL,'Wie viele Barren hat die Horde bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10267','2','How many herbs have the Horde collected so far?',NULL,'Wie viele Kräuter hat die Horde bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10267','3','How many leather skins have the Horde collected so far?',NULL,'Wie viel Leder hat die Horde bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10267','4','How many bandages have the Horde collected so far?',NULL,'Wie viele Stoffe hat die Horde bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10267','5','How many cooked goods have the Horde collected so far?',NULL,'Wie viele gekochte Waren hat die Horde bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10268, 8071, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10268','0','0','I want to ask you about something else.','1','1','10267','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10268','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10269, 8096, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10269','0','0','I want to ask you about something else.','1','1','10267','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10269','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10270, 8097, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10270','0','0','I want to ask you about something else.','1','1','10267','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10270','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10271, 8098, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10271','0','0','I want to ask you about something else.','1','1','10267','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10271','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10272, 8099, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10272','0','0','I want to ask you about something else.','1','1','10267','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10272','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10273, 8100, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10273','0','0','I want to ask you about something else.','1','1','10267','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10273','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);

# Commander Stronghammer Gossip
UPDATE `creature_template` SET gossip_menu_id=10274, npcflag = (npcflag | 1), speed_walk=0.75, speed_run=1.76 WHERE entry=15458;
INSERT INTO `gossip_menu` VALUES (10274, 7949, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10274','0','0','What is the Ahn\'Qiraj war effort?','1','1','10275','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10274','1','0','How many metal bars have the Alliance collected so far?','1','1','10276','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10274','2','0','How many herbs have the Alliance collected so far?','1','1','10277','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10274','3','0','How many leather skins have the Alliance collected so far?','1','1','10278','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10274','4','0','How many bandages have the Alliance collected so far?','1','1','10279','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `gossip_menu_option` VALUES ('10274','5','0','How many cooked goods have the Alliance collected so far?','1','1','10280','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10274','0','What is the Ahn\'Qiraj war effort?',NULL,'Was ist die Ahn\'Qiraj Kriegsvorbereitung?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10274','1','How many metal bars have the Alliance collected so far?',NULL,'Wie viele Barren hat die Allianz bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10274','2','How many herbs have the Alliance collected so far?',NULL,'Wie viele Kräuter hat die Allianz bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10274','3','How many leather skins have the Alliance collected so far?',NULL,'Wie viel Leder hat die Allianz bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10274','4','How many bandages have the Alliance collected so far?',NULL,'Wie viele Stoffe hat die Allianz bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `locales_gossip_menu_option` VALUES ('10274','5','How many cooked goods have the Alliance collected so far?',NULL,'Wie viele gekochte Waren hat die Allianz bisher gesammelt?',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10275, 8071, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10275','0','0','I want to ask you about something else.','1','1','10274','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10275','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10276, 7951, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10276','0','0','I want to ask you about something else.','1','1','10274','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10276','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10277, 7952, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10277','0','0','I want to ask you about something else.','1','1','10274','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10277','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10278, 7953, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10278','0','0','I want to ask you about something else.','1','1','10274','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10278','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10279, 7954, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10279','0','0','I want to ask you about something else.','1','1','10274','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10279','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `gossip_menu` VALUES (10280, 7955, 0, 0, 0, 0, 0, 0);
INSERT INTO `gossip_menu_option` VALUES ('10280','0','0','I want to ask you about something else.','1','1','10274','0','0','0','0','','0','0','0','0','0','0','0','0','0');
INSERT INTO `locales_gossip_menu_option` VALUES ('10280','0','I want to ask you about something else.',NULL,'Ich möchte noch etwas wissen.',NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL);


#INSERT INTO `locales_npc_text` (8092, 
