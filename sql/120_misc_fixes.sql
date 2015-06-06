# Sunken Tempel
DELETE FROM scripted_event_id WHERE id=8502;
INSERT INTO scripted_event_id VALUES
(8502,'event_avatar_of_hakkar');
UPDATE `creature_template` SET armor=3247 WHERE entry=8443;
UPDATE `creature_template` SET ScriptName='npc_nightmare_suppressor', MovementType=0 WHERE entry=8497;
UPDATE `spell_script_target` SET targetEntry=8440 WHERE entry=12623;

DELETE FROM `creature_equip_template` WHERE entry=2240;
INSERT INTO `creature_equip_template` VALUES (2240, 11932, 0, 0);
UPDATE `creature_template` SET AIName='', equipment_id=2240 WHERE entry=5710;

DELETE FROM `script_texts` WHERE entry IN(-1999949, -1999947, -1999948);
INSERT INTO `script_texts` (`entry`, `type`, `content_default`, `content_loc3`, `comment`) VALUES ('-1999947', 1, 'No! You must not summon our god!', 'Nein! Ihr dürft unseren Gott nicht herbeirufen!', 'Nightmare Suppressor YELL LINE1'); 
INSERT INTO `script_texts` (`entry`, `type`, `content_default`, `content_loc3`, `comment`) VALUES ('-1999948', 1, 'Stop, Infidels!', 'Halt, Ungläubige!', 'Nightmare Suppressor YELL LINE2'); 
INSERT INTO `script_texts` (`entry`, `type`, `content_default`, `content_loc3`, `comment`) VALUES ('-1999949', 1, 'No! You must not do this!', 'Nein! Das dürft Ihr nicht tun!', 'Nightmare Suppressor YELL LINE3'); 
DELETE FROM `script_texts` WHERE entry IN(-1999950, -1999951, -1999952);
INSERT INTO `script_texts` (`entry`, `type`, `content_default`, `content_loc3`, `sound`, `comment`) VALUES 
('-1999950', 1, 'The Soulflayer comes!', 'Der Seelenschinder kommt!', 5862, 'Jammal\'an AGGRO'),
('-1999951', 1, 'Join us!', 'Kommt zu uns!', 5864, 'Jammal\'an CAST'),
('-1999952', 1, 'Hakkar shall live again!', 'Hakkar wird wieder leben!', 5865, 'Jammal\'an LOW HEALTH'); 

DELETE FROM `creature` WHERE guid=9000067;
UPDATE `gameobject` SET spawntimesecs=-604800 WHERE id IN(148883, 148937);