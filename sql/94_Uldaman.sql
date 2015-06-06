UPDATE `creature_loot_template` SET `lootcondition` = '0' WHERE `entry` = '6910' AND `item` = '7741'; 
DELETE FROM `gameobject_scripts` WHERE `id`='14393';
INSERT INTO `gameobject_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `datalong3`, `datalong4`, `data_flags`, `dataint`, `dataint2`, `dataint3`, `dataint4`, `x`, `y`, `z`, `o`, `comments`) VALUES('14393','26','11','14394','0','0','0','0','0','0','0','0','0','0','0','0','');
INSERT INTO `gameobject_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `datalong3`, `datalong4`, `data_flags`, `dataint`, `dataint2`, `dataint3`, `dataint4`, `x`, `y`, `z`, `o`, `comments`) VALUES('14393','26','20','2','7228','100','0','0','0','0','0','0','-232.51','251.52','-49.44','5.01','');
DELETE FROM `creature_movement_template` WHERE `entry`='7228';
INSERT INTO `creature_movement_template` VALUES ('7228', '1', '-232.51','251.52','-49.44', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5.01', '0', '0');