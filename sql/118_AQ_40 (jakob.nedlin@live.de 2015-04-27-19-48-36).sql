# Disenchanting Fix
# DELETE FROM `disenchant_loot_template` WHERE entry=50 AND item=14344;
# INSERT INTO `disenchant_loot_template` VALUES (50,14344,99.5,1,1,1,0,0,0);

# Testrealm only
UPDATE `areatrigger_teleport` SET target_position_x=-8479.716, target_position_y=2029.192, target_position_z=125.155, target_orientation=0.4 WHERE id=4010;
#UPDATE `areatrigger_teleport` SET target_position_x=-8231.33, target_position_y=2010.6, target_position_z=129.861, target_orientation=0 WHERE id=4010;

# Resilience (Sollte nach Release langsam wieder erhöht werden)
DELETE FROM `creature_template_resilience` WHERE entry IN(15263, 15544, 15543, 15511, 15516, 15984, 15510);
INSERT INTO `creature_template_resilience` VALUES
(15263, 40, 'Ahn\'Qiraj 40 - Skeram'),
(15544, 40, 'Ahn\'Qiraj 40 - Vem'),
(15543, 40, 'Ahn\'Qiraj 40 - Prinzessin Yauj'),
(15511, 40, 'Ahn\'Qiraj 40 - Lord Kri'),
(15516, 40, 'Ahn\'Qiraj 40 - Sartura'),
(15984, 35, 'Ahn\'Qiraj 40 - Sarturas Add'),
(15510, 40, 'Ahn\'Qiraj 40 - Fankriss');

# Misc
DELETE FROM `game_graveyard_zone` WHERE ghost_zone = 3428;
INSERT INTO `game_graveyard_zone` VALUES (913, 3428, 0);
UPDATE `gameobject_template` SET flags=2 WHERE entry=180690;

# Trash
UPDATE `creature_template` SET AIName='', ScriptName='npc_obsidian_eradicator' WHERE entry=15262;
DELETE FROM `creature_ai_scripts` WHERE creature_id=15262;
UPDATE `creature_template` SET AIName='', mindmg=2395, maxdmg=2675, ScriptName='npc_vekniss_guardian' WHERE entry=15233;
DELETE FROM `creature_ai_scripts` WHERE creature_id=15233;
UPDATE `creature_template` SET mindmg=2395, maxdmg=2675 WHERE entry=15230;

# Skeram
DELETE FROM `spell_target_position` WHERE id IN(4801, 8195, 20449);
INSERT INTO `spell_target_position` VALUES (4801, 531, -8306.22, 2062.50, 133.07, 0.4),
(8195, 531, -8346.64, 2081.41, 125.65, 0.4),
(20449, 531, -8330.28, 2123.08, 133.06, 0.4);
UPDATE `creature_model_info` SET bounding_radius=3, combat_reach=4 WHERE modelid = 15345;
UPDATE `creature_template` SET minhealth=466375, maxhealth=467509, flags_extra = flags_extra & ~1  WHERE entry=15263;

# Bug-Trio
DELETE FROM `creature_linking` WHERE guid IN(1105638, 1105639, 1105640);
INSERT INTO `creature_linking` VALUES (1105639, 1105638, 15), (1105638, 1105640, 15), (1105640, 1105639, 15);;
UPDATE `creature_template` SET flags_extra = flags_extra & ~1  WHERE entry IN(15544, 15543, 15511);
UPDATE `creature_template` SET AIName='', ScriptName='npc_kri_poison_cloud' WHERE entry=15933;
DELETE FROM `creature_movement_template` WHERE entry=15544;
INSERT INTO `creature_movement_template` VALUES
(15544,1,-8549.55,2161.72,-3.6664,0,0,0,0,0,0,0,0,0,0,4.46724,0,0),
(15544,2,-8558.18,2154.86,-5.82032,0,0,0,0,0,0,0,0,0,0,3.83499,0,0),
(15544,3,-8571.58,2153.97,-7.28693,0,0,0,0,0,0,0,0,0,0,3.12029,0,0),
(15544,4,-8589.82,2156.38,-7.04311,0,0,0,0,0,0,0,0,0,0,2.63727,0,0),
(15544,5,-8599.43,2164.46,-3.94408,0,0,0,0,0,0,0,0,0,0,2.4527,0,0),
(15544,6,-8589.82,2156.38,-7.04311,0,0,0,0,0,0,0,0,0,0,2.63727,0,0),
(15544,7,-8571.58,2153.97,-7.28693,0,0,0,0,0,0,0,0,0,0,3.12029,0,0),
(15544,8,-8558.18,2154.86,-5.82032,0,0,0,0,0,0,0,0,0,0,3.83499,0,0),
(15544,9,-8549.55,2161.72,-3.6664,0,0,0,0,0,0,0,0,0,0,4.46724,0,0),
(15544,10,-8550.37,2166.34,-3.70582,0,0,0,0,0,0,0,0,0,0,1.78903,0,0),
(15544,11,-8557.03,2161.73,-4.60029,0,0,0,0,0,0,0,0,0,0,3.66221,0,0),
(15544,12,-8581.15,2162.06,-6.36318,0,0,0,0,0,0,0,0,0,0,2.86503,0,0),
(15544,13,-8585.25,2158.23,-7.15473,0,0,0,0,0,0,0,0,0,0,2.80612,0,0),
(15544,14,-8591.83,2160.24,-5.88721,0,0,0,0,0,0,0,0,0,0,2.45662,0,0),
(15544,15,-8599.43,2164.46,-3.94408,0,0,0,0,0,0,0,0,0,0,2.4527,0,0),
(15544,16,-8596.97,2157.21,-5.77788,0,0,0,0,0,0,0,0,0,0,1.18428,0,0),
(15544,17,-8581.15,2162.06,-6.36318,0,0,0,0,0,0,0,0,0,0,5.45293,0,0),
(15544,18,-8581.15,2162.06,-6.36318,0,0,0,0,0,0,0,0,0,0,5.45293,0,0),
(15544,19,-8577.69,2155.71,-7.37716,0,0,0,0,0,0,0,0,0,0,1.76547,0,0),
(15544,20,-8576.25,2169.75,-4.48843,0,0,0,0,0,0,0,0,0,0,1.67515,0,0),
(15544,21,-8567.24,2160.48,-6.17982,0,0,0,0,0,0,0,0,0,0,0.826923,0,0),
(15544,22,-8556.39,2159.72,-4.7979,0,0,0,0,0,0,0,0,0,0,1.19999,0,0),
(15544,23,-8549.55,2161.72,-3.6664,0,0,0,0,0,0,0,0,0,0,0.92117,0,0),
(15544,24,-8556.39,2159.72,-4.7979,0,0,0,0,0,0,0,0,0,0,1.25889,0,0),
(15544,25,-8571.55,2160.55,-6.43027,0,0,0,0,0,0,0,0,0,0,1.68693,0,0),
(15544,26,-8576.25,2169.75,-4.48843,0,0,0,0,0,0,0,0,0,0,2.25635,0,0),
(15544,27,-8579.52,2158.82,-7.01833,0,0,0,0,0,0,0,0,0,0,4.87958,0,0),
(15544,28,-8586.59,2162.1,-6.0648,0,0,0,0,0,0,0,0,0,0,2.76685,0,0),
(15544,29,-8591.83,2160.24,-5.88721,0,0,0,0,0,0,0,0,0,0,2.15817,0,0),
(15544,30,-8594.46,2167.18,-4.24164,0,0,0,0,0,0,0,0,0,0,1.87935,0,0),
(15544,31,-8591.83,2160.24,-5.88721,0,0,0,0,0,0,0,0,0,0,1.69872,0,0),
(15544,32,-8584.88,2154.09,-7.61168,0,0,0,0,0,0,0,0,0,0,0.552035,0,0),
(15544,33,-8581.15,2162.06,-6.36318,0,0,0,0,0,0,0,0,0,0,1.89899,0,0),
(15544,34,-8566.11,2157.76,-6.48814,0,0,0,0,0,0,0,0,0,0,6.22261,0,0),
(15544,35,-8550.37,2166.34,-3.70582,0,0,0,0,0,0,0,0,0,0,0.944732,0,0);

# Sartura
UPDATE `creature_template` SET mechanic_immune_mask = mechanic_immune_mask & ~2048  WHERE entry=15516;
UPDATE `creature_template` SET mechanic_immune_mask = 617297755  WHERE entry=15984;
DELETE FROM `creature_movement_template` WHERE entry=15516;
INSERT INTO `creature_movement_template` VALUES
(15516,1,-8337.2,1596.45,-31.2387,0,0,0,0,0,0,0,0,0,0,0.923705,0,0),
(15516,2,-8263.57,1746.7,-14.9944,0,0,0,0,0,0,0,0,0,0,5.22764,0,0),
(15516,3,-8233.96,1643.88,-32.8831,0,0,0,0,0,0,0,0,0,0,3.42121,0,0),
(15516,4,-8338.47,1698.45,-20.2435,0,0,0,0,0,0,0,0,0,0,6.22508,0,0),
(15516,5,-8223.09,1710.64,-27.4231,0,0,0,0,0,0,0,0,0,0,3.04422,0,0);

# Fankriss
UPDATE `creature_template` SET flags_extra =  flags_extra | 256 WHERE entry=15510;
DELETE FROM `spell_target_position` WHERE id IN(518, 720, 731, 1121, 25831, 25832);
INSERT INTO `spell_target_position` VALUES
(518,531,-8028.5,1050.9,-54,0),
(720,531,-8043.01,1254.21,-84.19,0),
(731,531,-8003,1222.9,-82.1,0),
(1121,531,-8022.68,1150.08,-89.33,0),
(25831,531,-8158.03,1139.3,-83.95,0),
(25832,531,-8029.25,1237.78,-85.2285,0);

# Viscidus
UPDATE `creature_template` SET ScriptName = 'boss_viscidus' WHERE entry = 15299;
UPDATE `creature_template` SET ScriptName = 'npc_glob_of_viscidus' WHERE entry = 15667;