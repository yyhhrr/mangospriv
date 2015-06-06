-- Sound-IDs 7114-7116

-- Questreihe

A Pawn on the Eternal Board -- OK
-- Green Scepter Shard 
Eranikus, Tyrant of the Dream -- OK
Tyrande and Remulos -- OK
The Nightmare's Corruption -- Twilight Corrupter fehlt
The Nightmare Manifests -- Geht gar nicht

Used IN Gates of Ahn'Quiraj Opening Event

DELETE FROM `game_event_npc_gossip` WHERE `guid` IN (135161,135162);
INSERT INTO `game_event_npc_gossip` (`guid`,`event_id`,`textid`) VALUES
(135161,22,16000),
(135162,22,16001); 

AQ Gates data

UPDATE `game_event` SET `end_time`='2020-12-30 22:00:00',`world_event`=1 WHERE `entry`=22;
UPDATE `game_event` SET `length`=0 WHERE `entry`=22;
INSERT INTO `game_event_creature` (`guid`, `event`) VALUES
(135121,22),(135122,22),(135123,22),(135124,22),(135125,22),
(135126,22),(135127,22),(135128,22),(135129,22),(135130,22),
(135131,22),(135132,22),(135133,22),(135134,22),(135135,22),
(135136,22),(135137,22),(135138,22),(135139,22),(135140,22),
(135141,22),(135142,22),(135143,22),(135144,22),(135145,22),
(135146,22),(135147,22),(135148,22),(135149,22),(135150,22),
(135151,22),(135152,22),(135153,22),(135154,22),(135155,22),
(135156,22),(135157,22),(135158,22),(135159,22),(135160,22),
(135161,22),(135162,22); 

Ironforge NPCs

INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(135121,15383,0,1,1,0,0,-4929.00,-1226.00,501.66,4.00,180,5,0,844,0,0,1),
(135122,15431,0,1,1,0,0,-4919.80,-1226.16,501.66,4.00,180,0,0,844,0,0,0),
(135123,15432,0,1,1,0,0,-4913.99,-1233.25,501.66,4.00,180,10,0,881,0,0,1),
(135124,15434,0,1,1,0,0,-4949.20,-1272.96,501.66,1.50,180,0,0,6100,0,0,0),
(135125,15437,0,1,1,0,0,-4943.54,-1278.49,501.66,1.50,180,0,0,2000,0,0,0),
(135126,15445,0,1,1,0,0,-4947.45,-1270.83,501.66,1.50,180,0,0,3900,0,0,0),
(135127,15446,0,1,1,0,0,-4979.34,-1171.01,501.66,4.50,180,0,0,6800,0,0,0),
(135128,15448,0,1,1,0,0,-4969.70,-1181.49,501.66,0.00,180,10,0,3400,0,0,1),
(135129,15450,0,1,1,0,0,-4976.90,-1183.89,501.66,0.00,180,10,0,881,0,0,1),
(135130,15451,0,1,1,0,0,-4990.51,-1157.65,501.66,5.00,180,0,0,844,0,0,0),
(135131,15452,0,1,1,0,0,-4980.24,-1152.63,501.66,5.00,180,10,0,2700,0,0,1),
(135132,15453,0,1,1,0,0,-4999.72,-1143.44,501.66,5.00,180,10,0,881,0,0,1),
(135133,15455,0,1,1,0,0,-4940.09,-1274.89,501.66,1.50,180,0,0,844,0,0,0),
(135134,15456,0,1,1,0,0,-4934.47,-1279.39,501.66,2.00,180,0,0,844,0,0,0),
(135135,15457,0,1,1,0,0,-4934.56,-1273.68,501.66,1.80,180,0,0,881,0,0,0);

Orgrimmar NPCs

INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(135136,15459,1,1,1,0,0,1647.23,-4123.34,31.234,1.70,180,0,0,807,0,0,0),
(135137,15460,1,1,1,0,0,1663.15,-4113.42,34.156,1.70,180,0,0,844,0,0,0),
(135138,15469,1,1,1,0,0,1656.99,-4116.07,33.160,1.70,180,0,0,881,0,0,0),
(135139,15477,1,1,1,0,0,1616.93,-4143.40,34.560,1.50,180,0,0,5960,0,0,0),
(135140,15508,1,1,1,0,0,1623.98,-4148.34,35.990,1.70,180,0,0,6430,0,0,0),
(135141,15512,1,1,1,0,0,1630.03,-4152.75,38.420,1.90,180,0,0,5000,0,0,0),
(135142,15515,1,1,1,0,0,1586.35,-4181.89,40.310,3.14,180,0,0,5960,0,0,0),
(135143,15522,1,1,1,0,0,1590.62,-4159.70,36.920,3.14,180,0,0,5000,0,0,0),
(135144,15525,1,1,1,0,0,1594.49,-4172.66,39.220,3.00,180,0,0,5000,0,0,0),
(135145,15528,1,1,1,0,0,1580.69,-4119.78,34.440,4.61,180,0,0,5960,0,0,0),
(135146,15529,1,1,1,0,0,1572.15,-4122.62,36.280,4.85,180,0,0,5000,0,0,0);

Other NPCs

INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
-- War Effort Commanders
(135151,15701,0,1,1,0,0,-4956.31,-1198.15,501.66,0.00,180,0,0,2900,0,0,2),
(135152,15700,1,1,1,0,0,1582.41,-4209.28,42.508,0.00,180,0,0,3400,0,0,2), 
-- War Effort Recruiters
(135153,15702,1,1,1,0,0,-1204.90,107.38,134.42,1.6,180,0,0,4120,0,0,0),
 (135154,15703,0,1,1,0,0,1573.93,273.84,-43.1,5.3,180,0,0,1700,0,0,0),
(135155,15704,1,1,1,0,0,1648.18,-4405.40,17.43,0,180,15,0,6900,0,0,1),
 (135156,15707,0,1,1,0,0,-4972.66,-945.77,501.66,5,180,0,0,1100,0,0,0),
(135157,15708,0,1,1,0,0,-8813.4,654.195,96.0776,4.5185,180,0,0,6900,0,0,0),
 (135158,15709,1,1,1,0,0,9951.02,2482.53,1316.20,0,180,10,0,881,0,0,1),
(135159,21155,530,1,1,0,0,9526.54,-7189.49,16.14,1.57,180,0,0,6900,2434,0,0),
(135160,21156,530,1,1,0,0,-3934.23,-11606.23,-138.58,4.5,180,0,0,6900,0,0,0),
-- War Effort Ambassadors
(135161,15539,0,1,1,0,0,-4984.12,-1205.16,501.66,3.14,180,0,0,1800,0,0,0),
 (135162,15458,1,1,1,0,0,1632.00,-4116.88,31.24,1.7,180,0,0,910,0,0,0);

War Effort Commanders paths

INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`) VALUES
(1351510,1,-4992.88,-1148.14,501.66),
 (1351510,2,-5018.99,-1182.52,501.66),
 (1351510,3,-4981.63,-1217.63,501.66),
 (1351510,4,-4941.99,-1263.46,501.66),
 (1351510,5,-4920.39,-1242.94,501.66),
 (1351510,6,-4896.23,-1254.96,501.87),
(1351510,7,-4920.39,-1242.94,501.66),
(1351510,8,-4945.89,-1206.33,501.66),
 (1351510,9,-4933.98,-1176.06,501.68),
 (1351510,10,-4956.31,-1198.15,501.66),
(1351520,1,1582.41,-4209.28,42.51),
 (1351520,2,1577.13,-4173.25,43.28),
 (1351520,3,1604.33,-4124.16,29.67),
 (1351520,4,1659.62,-4097.64,33.62),
 (1351520,5,1697.91,-4085.99,39.23),
 (1351520,6,1660.63,-4095.52,34.52),
 (1351520,7,1602.48,-4123.29,29.58),
 (1351520,8,1576.30,-4176.98,37.99); 

War Effort Commanders Mounts

INSERT INTO `creature_template_addon` (`entry`,`mount`) VALUES
 (15701,14347),
(15700,14573),
 (15458,14346),
  (15539,14575);
UPDATE `creature_template` SET `MovementType`=2 WHERE `entry` IN(15700,15701);
UPDATE `creature` SET `spawnMask`=0 WHERE `guid`=86424;
UPDATE `creature_template` SET `faction_A`=64,`faction_H`=64 WHERE `entry`=15707;

EVENT quests

DELETE `creature_questrelation` FROM `creature_questrelation` WHERE `id`=15452;
DELETE `creature_involvedrelation` FROM `creature_involvedrelation` WHERE `id`=15452;
INSERT INTO `creature_questrelation` (`id`,`quest`) VALUES
(15452,8520),(15452,8521);
INSERT INTO `creature_involvedrelation` (`id`,`quest`) VALUES
(15452,8520),(15452,8521);
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `entry`=8520;
INSERT INTO `game_event_quest_condition` (`quest`,`event_id`,`condition_id`,`num`) VALUES 
-- Alliance only quests
(8495,22,1,20),(8494,22,1,20),
 (8500,22,2,20),(8499,22,2,20),
(8504,22,3,20),(8503,22,3,20),
 (8510,22,4,20),(8509,22,4,20),
 (8512,22,5,10),(8511,22,5,10),
 (8514,22,6,10),(8513,22,6,10),
 (8518,22,7,20),(8517,22,7,20),
 (8521,22,8,20),(8520,22,8,20),
 (8525,22,9,20),(8524,22,9,20),
 (8527,22,10,20),(8526,22,10,20), 
-- Horde only Quests
(8543,22,11,20),(8542,22,11,20),
(8546,22,12,20),(8545,22,12,20),
 (8550,22,13,20),(8549,22,13,20),
  (8583,22,14,20),(8582,22,14,20),
(8589,22,15,10),(8588,22,15,10),
(8601,22,16,10),(8600,22,16,10),
  (8605,22,17,20),(8604,22,17,20),
 (8608,22,18,20),(8607,22,18,20),
 (8612,22,19,20),(8611,22,19,20),
 (8616,22,20,20),(8615,22,20,20), 
-- Shared quests
(8493,22,21,20),(8492,22,21,20),(8533,22,21,20),(8532,22,21,20),
 (8506,22,22,20),(8505,22,22,20),(8581,22,22,20),(8580,22,22,20),
(8516,22,23,10),(8515,22,23,10),(8591,22,23,10),(8590,22,23,10),
(8523,22,24,20),(8522,22,24,20),(8610,22,24,20),(8609,22,24,20),
 (8529,22,25,20),(8528,22,25,20),(8614,22,25,20),(8613,22,25,20);

INSERT INTO `game_event_condition` (`event_id`,`condition_id`,`req_num`,`max_world_state_field`,`done_world_state_field`,`description`) VALUES 
-- Alliance reqs
(22,1,28000,0,2000,'Iron'),
(22,2,24000,0,2001,'Thorium'),
 (22,3,33000,0,2002,'Stranglekeep'),
(22,4,20000,0,2003,'Arthas Tears'),
(22,5,180000,0,2004,'Light Leather'),
(22,6,110000,0,2005,'Medium Leather'),
 (22,7,800000,0,2006,'Linen Bandages'),
 (22,8,600000,0,2007,'Silk Bandages'),
 (22,9,14000,0,2008,' Rainbow Fin Albacore'),
 (22,10,20000,0,2009,'Roast Raptor'), 
-- Horde reqs
(22,11,22000,0,2010,'Tin Bars'),
 (22,12,18000,0,2011,'Mithril Bars'),
(22,13,96000,0,2012,'PeaceBloom'),
 (22,14,19000,0,2013,'FireBloom'),
(22,15,60000,0,2014,'Heavy Leather'),
(22,16,60000,0,2015,'Rugged Leather'),
(22,17,250000,0,2016,'Wool Bandages'),
(22,18,250000,0,2017,'Mageweave Bandages'),
 (22,19,10000,0,2018,'Lean Wolf Steaks'),
 (22,20,10000,0,2019,'Baked Salmon'), 
-- World Reqs
(22,21,90000,0,2020,'Copper'),
 (22,22,26000,0,2021,'Purple Lotus'),
 (22,23,80000,0,2022,'Thick Leather'),
  (22,24,400000,0,2023,'Runecloth Bandages'),
 (22,25,17000,0,2024,'Spotted Yellowtail'); 

INSERT INTO `game_event_npc_gossip` (`guid`,`event_id`,`textid`) VALUES
(135161,22,16000),
(135162,22,16001);
INSERT INTO `npc_text` (`ID`,`text0_0`,`text0_1`,`prob0`,`lang0`) VALUES
(16000,'The current situation in the front lines look like this:$B$B Iron $2000w % $B Thorium $2001w % $B Stranglekeep $2002w % $B Arthas Tears $2003w % $B Light Leather $2004w % $B Medium Leather $2005w % $B Linen Bandages $2006w % $B Silk Bandages $2007w % $B Rainbow Fin Albacore $2008w % $B Roast Raptor $2009w % $B$B Allongside with the Horde, we managed to gather: $B Copper $2020w % $B Purple Lotus $2021w % $B Thick Leather $2022w % $B Runecloth Bandage $2023w % $B Spotted Yellowtail $2024w. $B that is the current state of the effort, Aid us $N.','The current situation in the front lines look like this:$B$B Iron $2000w % $B Thorium $2001w % $B Stranglekeep $2002w % $B Arthas Tears $2003w % $B Light Leather $2004w % $B Medium Leather $2005w % $B Linen Bandages $2006w % $B Silk Bandages $2007w % $B Rainbow Fin Albacore $2008w % $B Roast Raptor $2009w % $B$B Allongside with the Horde, we managed to gather: $B Copper $2020w % $B Purple Lotus $2021w % $B Thick Leather $2022w % $B Runecloth Bandage $2023w % $B Spotted Yellowtail $2024w. $B that is the current state of the effort, Aid us $N.',100,0),
(16001,'The current situation in the front lines look like this:$B$B Tin $2010w % $B Mithril $2011w % $B PeaceBloom $2012w % $B FireBloom $2013w % $B Heavy Leather $2014w % $B Rugged Leather $2015w % $B Wool Bandages $2016w % $B Mageweave Bandages $2017w % $B Lean Wolf Steaks $2018w % $B Baked Salmon $2019w % $B$B Allongside with the Alliance, we managed to gather: $B Copper $2020w % $B Purple Lotus $2021w % $B Thick Leather $2022w % $B Runecloth Bandage $2023w % $B Spotted Yellowtail $2024w. $B that is the current state of the effort, Aid us $N.','The current situation in the front lines look like this:$B$B Tin $2010w % $B Mithril $2011w % $B PeaceBloom $2012w % $B FireBloom $2013w % $B Heavy Leather $2014w % $B Rugged Leather $2015w % $B Wool Bandages $2016w % $B Mageweave Bandages $2017w % $B Lean Wolf Steaks $2018w % $B Baked Salmon $2019w % $B$B Allongside with the Alliance, we managed to gather: $B Copper $2020w % $B Purple Lotus $2021w % $B Thick Leather $2022w % $B Runecloth Bandage $2023w % $B Spotted Yellowtail $2024w. $B that is the current state of the effort, Aid us $N.',100,0);
INSERT INTO `npc_gossip` (`npc_guid`,`textid`) VALUES
 (135161,16000),
 (135162,16001);

quest DATA: Bang a gong!

UPDATE `quest_template` SET `CharTitleId`=46 WHERE `entry`=8743;
UPDATE `quest_template` SET `Method`=0 WHERE `entry`=8743;
UPDATE `quest_template` SET `Type`=82 WHERE `entry`=8743;
UPDATE `quest_template` SET `QuestFlags`=2 WHERE `entry`=8743;
UPDATE `gameobject_template` SET `type`=10 WHERE `entry`=180718;
UPDATE `gameobject_template` SET `type`=2 WHERE `entry`=180717;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=180717; 

AQ Doors

INSERT INTO `game_event_gameobject` (`guid`,`event`) VALUES
 (100002,22),(100003,22),(100004,22),
(100005,75),(100006,75),(100007,75),
(100008,76),(100009,76),(100010,76);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(100002,176148,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
 (100003,176147,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
  (100004,176146,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
   (100005,176148,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
 (100006,176147,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
 (100007,176146,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
 (100008,176148,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
  (100009,176147,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1),
  (100010,176146,1,1,1,-8130,1525,17.5,0,0,0,0,0,10,100,1);
INSERT INTO `game_event_creature` (`guid`,`event`) VALUES
   (135164,77),(135165,77),(135166,77),(135167,77),(135168,77),
   (135169,77),(135170,77),(135171,77),(135172,77),(135173,77),
  (135174,77),(135175,77),(135176,77),(135177,77),(135178,77),
     (135179,77),(135180,77),(135181,77),(135182,77),(135183,77),
      (135184,77),(135185,77),(135186,77),(135187,77),(135188,77),
  (135189,77),(135190,77),(135191,77),(135192,77),(135193,77),
    (135194,77),(135195,77),(135196,77),(135197,77),(135198,77),
     (135199,77),(135200,77); 

NPCs

INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES 
-- General Rajaxx
(135164,15341,1,1,1,0,0,-8130.05,1525.05,37.90,0,180,0,0,416375,0,0,0), 
-- Hive Collossus
(135165,15740,1,1,1,0,0,-7298.00,1665.00,-35.6,-1.53589,180,200,0,3331000,0,0,1),
(135166,15741,1,1,1,0,0,-7782.50,516.72,-39.85,-1.53589,180,200,0,3331000,0,0,1),
(135167,15742,1,1,1,0,0,-6530.05,977.13,0.43,-1.53589,180,200,0,3331000,0,0,1), 
-- Anubisah Colossus
(135168,15743,1,1,1,0,0,-8055.51,1478.02,2.60,-1.53589,180,20,0,356070,0,0,1),
(135169,15743,1,1,1,0,0,-8062.05,1527.77,2.61,0,180,15,0,356070,0,0,1),
(135170,15743,1,1,1,0,0,-8006.14,1556.79,2.63,0,180,15,0,356070,0,0,1),
(135171,15743,1,1,1,0,0,-7971.34,1436.62,27.03,-0.15,180,35,0,356070,0,0,1),
(135172,15743,1,1,1,0,0,-8192.27,1628.45,2.59,-1.53589,180,5,0,356070,0,0,1),
(135173,15743,1,1,1,0,0,-8256.22,1528.53,-4.49,0,180,25,0,356070,0,0,1),
(135174,15743,1,1,1,0,0,-8272.40,1436.73,9.99,1,180,10,0,356070,0,0,1), 
-- Qiraji Champions
(135175,15252,1,1,1,0,0,-8117.14,1512.34,3.88,0,180,0,0,266480,0,0,0),
(135176,15252,1,1,1,0,0,-8116.27,1536.37,3.48,0,180,0,0,266480,0,0,0), 
-- Anubisath Sentinels
(135177,15264,1,1,1,0,0,-8090.60,1576.85,14.33,0,180,0,0,110040,0,0,0),
(135178,15264,1,1,1,0,0,-8041.13,1579.14,13.88,3.1415,180,0,0,110040,0,0,0),
(135179,15264,1,1,1,0,0,-8271.16,1681.18,25.95,-0.79,180,0,0,110040,0,0,0),
(135180,15264,1,1,1,0,0,-8288.46,1661.79,24.99,-0.79,180,0,0,110040,0,0,0),
(135181,15264,1,1,1,0,0,-8289.26,1715.53,25.29,0.79,180,0,0,110040,0,0,0),
(135182,15264,1,1,1,0,0,-8324.89,1677.27,25.84,-2.20,180,0,0,110040,0,0,0), 
-- Anubisath Defender
(135183,15277,1,1,1,0,0,-8149.52,1525.93,4.41,0,180,0,0,485550,0,0,0), 
-- Qiraji Warriors
(135184,15537,1,1,1,0,0,-8167.79,1504.59,7.49,0.42,180,15,0,38844,0,0,1),
(135185,15537,1,1,1,0,0,-8175.52,1521.04,4.19,0,180,15,0,38844,0,0,1),
(135186,15537,1,1,1,0,0,-8158.94,1543.86,4.28,-0.2,180,15,0,38844,0,0,1),
(135187,15537,1,1,1,0,0,-8180.07,1562.58,2.36,-0.4,180,15,0,38844,0,0,1),
(135188,15537,1,1,1,0,0,-8226.91,1592.79,4.37,-0.3,180,15,0,38844,0,0,1),
(135189,15537,1,1,1,0,0,-8262.60,1639.70,17.45,-1.53589,180,15,0,38844,0,0,1),
(135190,15537,1,1,1,0,0,-8270.04,1592.54,9.25,-0.2,180,15,0,38844,0,0,1),
(135191,15537,1,1,1,0,0,-8204.95,1558.74,4.02,-0.15,180,15,0,38844,0,0,1),
(135192,15537,1,1,1,0,0,-8193.70,1513.13,5.02,0.2,180,15,0,38844,0,0,1),
(135193,15537,1,1,1,0,0,-8196.39,1536.41,4.25,0,180,15,0,38844,0,0,1),
(135194,15537,1,1,1,0,0,-8216.25,1533.89,1.44,0,180,15,0,38844,0,0,1);