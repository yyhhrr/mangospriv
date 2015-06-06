DELETE FROM creature WHERE guid=1105301;
INSERT INTO creature VALUES (1105301, 14890, 0, 0, 0, -10372.5, -421.983, 63.6243, 3.10504, 300, 0, 0, 0, 8640, 0, 0);
UPDATE creature_template SET faction_A = 50, faction_H = 50, skinloot = 100009 WHERE entry IN(14887, 14888, 14889, 14890);
UPDATE `gameobject_template` SET data2=3 WHERE entry=180517;

# Ysondre: Druid Mana
UPDATE `creature_template` SET minmana=900, maxmana=1100 WHERE entry=15260;
# Hitbox
UPDATE `creature_model_info` SET bounding_radius=2.8, combat_reach=3.8 WHERE modelid>=15363 AND modelid<=15366;

# Trash Movement
UPDATE `creature` SET MovementType=1, spawndist=5 WHERE id IN(12475, 12476, 12474);
UPDATE `creature_template` SET MovementType=1 WHERE entry IN(12475, 12476, 12474);

# Skinning Loot
DELETE FROM `skinning_loot_template` WHERE entry=100009;
INSERT INTO `skinning_loot_template` VALUES (100009, 20381, 100, 1, 3, 5, 0, 0, 0);