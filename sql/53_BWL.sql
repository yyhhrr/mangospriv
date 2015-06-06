UPDATE creature_template SET faction_A = 103, faction_H = 103, minlevel = 60, maxlevel = 60, rank = 1, mingold = 0, maxgold = 0 WHERE entry IN(14302, 14265, 14262, 14264, 14261, 14263);

UPDATE creature_template SET mindmg = 891, maxdmg = 1144 WHERE entry = 14302;
UPDATE creature_template SET mindmg = 524, maxdmg = 673 WHERE entry = 14265;
UPDATE creature_template SET mindmg = 517, maxdmg = 637 WHERE entry = 14262;
UPDATE creature_template SET mindmg = 517, maxdmg = 637 WHERE entry = 14264;
UPDATE creature_template SET mindmg = 524, maxdmg = 673 WHERE entry = 14261;
UPDATE creature_template SET mindmg = 517, maxdmg = 637 WHERE entry = 14263;

UPDATE creature_template SET speed_walk = 1.2, speed_run = 1.5 WHERE entry IN(11983, 14601, 11981);


UPDATE `creature_template` SET `mechanic_immune_mask`=15416914 WHERE `entry` = 12435;
UPDATE `creature_template` SET `mechanic_immune_mask`=15416915 WHERE `entry` 
IN ( 12017, 11983, 11981, 14601, 14020, 10162, 11583 );

UPDATE `creature_template` SET `mechanic_immune_mask`=1024 WHERE `entry` 
IN ( 12468, 14022, 14023, 14024, 14025, 12458 );

UPDATE creature_model_info SET bounding_radius = 5, combat_reach = 6 WHERE modelid = 13992;
-- Chromaggus
UPDATE `creature_model_info` SET `bounding_radius`='5',`combat_reach`='6' WHERE (`modelid`='14367');
