-- Palamountquest, Nachtrag: Hm, irgendwie scheint da doch noch so einiges nicht zu stimmen. Vielleicht habe ich als Grundlage eine neuere Datenbankversion benutzt... Hast du vielleicht bei Gelegenheit mal Lust zu schauen, ob die folgenden Einträge in creature_ai_scripts vorhanden sind, bzw. sie direkt zu ergänzen? Bei mir waren die alle schon drin. Den Spielern kamen die Timer auch noch zu lang vor, aber wenn die Gegner keine Spells einsetzen, wären sie ja auch ziemlich einfach. Ich schau mir grad nochmal ein Video an um etwas über die HP rauszufinden.
REPLACE INTO `creature_ai_scripts` VALUES
(1451101,14511,0,0,100,3,3000,5000,5000,7000,11,12739,1,0,0,0,0,0,0,0,0,0,'Shadowed Spirit - Cast Shadow Bolt'),
(1451102,14511,0,0,100,3,5000,7000,7000,12000,11,15245,0,0,0,0,0,0,0,0,0,0,'Shadowed Spirit - Cast Shadow Bolt Volley'),
(1451103,14511,0,0,100,3,1000,3000,25000,55000,11,22417,0,0,0,0,0,0,0,0,0,0,'Shadowed Spirit - Cast Shadow Shield'),
(1451104,14511,0,0,100,3,2000,7000,2000,9000,11,22575,1,0,0,0,0,0,0,0,0,0,'Shadowed Spirit - Cast Shadow Shock'),
(1451201,14512,0,0,100,3,1000,1000,20000,30000,11,18376,1,0,0,0,0,0,0,0,0,0,'Corrupted Spirit - Cast Corruption'),
(1451301,14513,0,0,100,3,3000,5000,9000,20000,11,13738,1,0,0,0,0,0,0,0,0,0,'Malicious Spirit - Cast Rend'),
(1451302,14514,4,0,100,2,0,0,0,0,11,23262,0,0,0,0,0,0,0,0,0,0,'Banal Spirit - Cast Demoralize on Aggro'),
(1451401,14514,0,0,100,3,1000,3000,5000,9000,11,8140,4,0,0,0,0,0,0,0,0,0,'Banal Spirit - Cast Befuddlement'),
(1451801,14518,0,0,100,3,1000,3000,5000,9000,11,8140,4,0,0,0,0,0,0,0,0,0,'Aspect of Banality - Cast Befuddlement'),
(1451802,14518,0,0,100,3,3000,5000,30000,45000,11,16247,4,0,0,0,0,0,0,0,0,0,'Aspect of Banality - Cast Curse of Thorns'),
(1451803,14518,4,0,100,2,0,0,0,0,11,23262,0,0,0,0,0,0,0,0,0,0,'Aspect of Banality - Cast Demoralize on Aggro'),
(1451901,14519,0,0,100,3,1000,1000,20000,30000,11,18376,1,0,0,0,0,0,0,0,0,0,'Aspect of Corruption - Cast Corruption'),
(1451902,14519,0,0,100,3,2000,2000,5000,12000,11,23268,4,0,0,0,0,0,0,0,0,0,'Aspect of Corruption - Cast Shadow Word: Pain'),
(1452001,14520,0,0,100,3,3000,5000,3000,7000,11,13738,1,0,0,0,0,0,0,0,0,0,'Aspect of Malice - Cast Rend'),
(1452002,14520,0,0,100,3,1000,3000,30000,70000,11,16098,0,0,0,0,0,0,0,0,0,0,'Aspect of Malice - Cast Curse of Blood'),
(1452101,14521,0,0,100,3,3000,5000,5000,7000,11,15472,1,0,0,0,0,0,0,0,0,0,'Aspect of Shadow - Cast Shadow Bolt'),
(1452102,14521,0,0,100,3,5000,7000,7000,12000,11,17228,0,0,0,0,0,0,0,0,0,0,'Aspect of Shadow - Cast Shadow Bolt Volley'),
(1452103,14521,0,0,100,3,1000,3000,25000,55000,11,22417,0,0,0,0,0,0,0,0,0,0,'Aspect of Shadow - Cast Shadow Shield'),
(1452104,14521,0,0,100,3,2000,7000,2000,9000,11,17289,1,0,0,0,0,0,0,0,0,0,'Aspect of Shadow - Cast Shadow Shock');
-- Den ganzen Eventgegnern müsste man dann noch in der creature_template den AI-Scriptnamen hinzufügen, also 
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry` IN (14511,14512,14513,14514,14516,14518,14519,14520,14521);
-- Malicious Spirits und Aspect of Malice sollten gegen den Stuneffekt immun sein, die Spirits dieser Welle sollten nach meinen vom Video abgeleiteten Berechnungen etwa 15k Leben haben statt 7k.
UPDATE `creature_template` SET `minhealth`=14502,`maxhealth`=15907, `mechanic_immune_mask`=mechanic_immune_mask | 2064 WHERE `entry`=14513;
UPDATE `creature_template` SET `mechanic_immune_mask`=mechanic_immune_mask | 2064 WHERE `entry`=14520;

UPDATE `creature_template` SET `mechanic_immune_mask`=15416915 WHERE `entry` 
IN(10263,9219,9218,9217,9196,9236,9237,16080,9596,10596,10376,9736,10220,10268,9718,9816,10264,10509,10899,10339,10363,16042,
10430,11490,13280,14327,11492,14326,14322,14321,14325,11501,14324,11489,11488,11487,11496,11486,9016,9319,9018,9024,9033,8983,
9543,9537,9499,9502,9017,9056,9041,9042,9156,9938,8929,9019,9027,9028,9029,9030,9031,9032,9034,9035,9036,9037,9038,9039,9040,
14861,10506,10503,11622,10433,10432,10508,10505,11261,10901,10507,10504,10502,1853,10516,10516,10808,11032,10997,10811,10813,
10436,10437,10438,10435,10439,10440);
UPDATE `creature_template` SET `mechanic_immune_mask`=15412819 WHERE `entry`=14323;  


INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(6559,7770),
(6560,7770),
(6561,7770);
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`, `box_money`, `box_text`, `cond_1`, `cond_1_val_1`, `cond_1_val_2`, `cond_2`, `cond_2_val_1`, `cond_2_val_2`, `cond_3`, `cond_3_val_1`, `cond_3_val_2`) VALUES
(6559,0,0, '<Verwendet das Abschriftenset, um ein Abpausbild zu erstellen.>',1,1,0,0,6559,0,0,NULL,9,8309,0,0,0,0,0,0,0),
(6560,0,0, '<Verwendet das Abschriftenset, um ein Abpausbild zu erstellen.>',1,1,0,0,6560,0,0,NULL,9,8309,0,0,0,0,0,0,0),
(6561,0,0, '<Verwendet das Abschriftenset, um ein Abpausbild zu erstellen.>',1,1,0,0,6561,0,0,NULL,9,8309,0,0,0,0,0,0,0);
INSERT INTO `gossip_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `data_flags`, `dataint`, `dataint2`, `dataint3`, `dataint4`, `x`, `y`, `z`, `o`, `comments`) VALUES
(6561,0,17,20456,1,0,0,0,0,0,0,0,0,0,'create item 20456'),
(6560,0,17,20455,1,0,0,0,0,0,0,0,0,0,'create item 20455'),
(6559,0,17,20454,1,0,0,0,0,0,0,0,0,0,'create item 20454');


INSERT INTO `quest_start_scripts` VALUES
(3982,5,10,8890,300000,0,0,0,0,0,0,0,375.11,-191.82,-70.77,1.92,'What is going on, first wave'),
(3982,5,10,8890,300000,0,0,0,0,0,0,0,376.91,-193.25,-70.37,1.95,'What is going on, first wave'),
(3982,5,10,8891,300000,0,0,0,0,0,0,0,377.86,-192.18,-70.16,1.99,'What is going on, first wave'),
(3982,5,10,8891,300000,0,0,0,0,0,0,0,377.66,-197.33,-70.05,3.44,'What is going on, first wave'),
(3982,40,10,8890,300000,0,0,0,0,0,0,0,375.11,-191.82,-70.77,1.92,'What is going on, second wave'),
(3982,40,10,8890,300000,0,0,0,0,0,0,0,376.91,-193.25,-70.37,1.95,'What is going on, second wave'),
(3982,40,10,8891,300000,0,0,0,0,0,0,0,377.86,-192.18,-70.16,1.99,'What is going on, second wave'),
(3982,40,10,8891,300000,0,0,0,0,0,0,0,377.66,-197.33,-70.05,3.44,'What is going on, second wave'),
(3982,70,7,3982,50,0,0,0,0,0,0,0,0,0,0,0,'What is going on,quest explored');

UPDATE `quest_template` SET `StartScript`=3982 WHERE `entry`=3982;

UPDATE `gameobject_template` SET `data2`= 100 WHERE  `entry`= 103661;
UPDATE `gameobject_template` SET `displayid` = 1709 WHERE `entry`= 174764;

INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(2898,0,0,0,0,'Hunter – Viper Sting');

UPDATE game_graveyard_zone SET faction = 0 WHERE id = 101 and ghost_zone = 1;
UPDATE game_graveyard_zone SET faction = 0 WHERE id = 91 and ghost_zone = 141;

INSERT INTO game_graveyard_zone VALUES
( 101, 1537, 67 ),
( 852, 1537, 469 ),
( 32, 1637, 469 ),
( 429, 85, 0 ),
( 91, 1657, 67 ),
( 429, 1497, 469 ),
( 96, 1497, 67 ),
( 869, 28, 0 );