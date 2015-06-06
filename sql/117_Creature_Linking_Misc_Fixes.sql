#Zul'Gurub
DELETE FROM `creature_linking` WHERE guid IN(49650, 49575, 49573, 49577, 49403, 49609, 49574, 49421, 49576, 49402, 49379, 49386, 49399, 49398, 49397, 49400, 49391);
INSERT INTO `creature_linking` VALUES 
(49575, 49650, 3),
(49573, 49650, 3),
(49577, 49650, 3),
(49403, 49650, 3),
(49574, 49609, 3),
(49421, 49609, 3),
(49576, 49609, 3),
(49402, 49609, 3),
(49379, 49655, 1),
(49399, 49400, 3),
(49398, 49400, 3),
(49397, 49400, 3),
(49400, 49655, 1),
(49391, 49655, 1),
(49386, 49655, 1);

DELETE FROM `creature_linking` WHERE (guid >= 9000271 AND guid <= 9000286) OR (master_guid >= 9000271 AND master_guid <= 9000286);
INSERT INTO `creature_linking` VALUES 
(49309, 9000281, 3),
(49296, 9000281, 3),
(49308, 9000281, 3),
(49297, 9000281, 3),
(49300, 9000281, 3),
(49307, 9000281, 3),
(49299, 9000281, 3),
(9000284, 9000281, 3),
(49813, 9000278, 3),
(51350, 9000278, 3),
(51348, 9000278, 3),
(51351, 9000278, 3),
(51349, 9000278, 3),
(49812, 9000278, 3),
(49811, 9000278, 3),
(51352, 9000278, 3);

UPDATE `creature_template` SET flags_extra = flags_extra | 8192 WHERE entry IN(14825, 14882, 14883);

# Molten Core
DELETE FROM `creature_linking` WHERE guid IN(56705, 56714, 56716, 56747, 56702, 56708, 56710, 56719, 56700, 56707);
INSERT INTO `creature_linking` VALUES
(56705, 56704, 3),
(56714, 56715, 3),
(56716, 56717, 3),
(56747, 56748, 3),
(56702, 56703, 3),
(56708, 56709, 3),
(56710, 56711, 3),
(56719, 56718, 3),
(56700, 56701, 3),
(56707, 56706, 3);

# UBRS
DELETE FROM `creature_linking` WHERE guid IN(348613, 348619, 348617, 348615, 348596, 348598, 1104466, 1104464, 348604);
INSERT INTO `creature_linking` VALUES
(348613, 348596, 3),
(348619, 348596, 3),
(348617, 348596, 3),
(348615, 348596, 3),
(348596, 1105166, 1),
(348604, 348598, 3),
(348598, 1105166, 1),
(1104466, 1105166, 1),
(1104464, 1105166, 1);

# Misc
UPDATE `creature_template` SET NAME='Tyrande Whisperwind' WHERE entry=7999;
UPDATE `locales_creature` SET name_loc3='Tyrande Whisperwind' WHERE entry=7999;

# Other Stuff
# Quest 7491
UPDATE `db_script_string` SET content_loc3='Bewohner von Orgrimmar, kommt, versammelt euch und feiert unsere Helden, die gemeinsam gegen den schwarzen Drachenschwarm einen Sieg erringen konnten! Die gefürchtete Brutmutter Onyxia wurde bezwungen!' WHERE entry = 2000005236;
UPDATE `db_script_string` SET content_loc3='Erfeut euch an der mutigen Tat von $N! Lasst $g seinen:ihren; Siegesschrei eure Herzen höher schlagen!' WHERE entry = 2000005237;
#2000005237
UPDATE `quest_end_scripts` SET datalong2=14000 WHERE id=7491 AND delay=0 AND command=3;
UPDATE `quest_end_scripts` SET datalong2=1000 WHERE id=7491 AND delay=14;
UPDATE `quest_end_scripts` SET datalong2=15000 WHERE id=7491 AND delay=40;
UPDATE `quest_end_scripts` SET datalong2=1000 WHERE id=7491 AND delay=55;
DELETE FROM quest_end_scripts WHERE id = 7491 AND command = 29;
INSERT INTO quest_end_scripts VALUES
(7491, 0, 29, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'npc_flag removed'),
(7491, 56, 29, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'npc_flag readded');

# Quest 7784
DELETE FROM `quest_end_scripts` WHERE id = 7784;
UPDATE `gameobject_template` SET `type`='9', `flags`='4' WHERE (`entry`='179881');
DELETE FROM `gameobject` WHERE guid=633352;
INSERT INTO `gameobject` VALUES (633352, 179881, 1, 1539, -4422, 8, 4.8548, 0, 0, 0.551627, -0.755623, -3600, 100, 0);
INSERT INTO `quest_end_scripts` VALUES (7784, 0, 3, 0, 14000, 0, 0, 0, 0, 0, 0, 0, 1542.71, -4422.18, 10.6873, 3.38925, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 14, 3, 0, 250, 0, 0, 0, 0, 0, 0, 0, 1543.57, -4422.14, 10.5473, 4.26497, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 18, 1, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 18, 0, 6, 0, 0, 0, 0, 2000009993, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 43, 1, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 43, 0, 6, 0, 0, 0, 0, 2000009992, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 45, 15, 22888, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 45, 9, 633352, 3600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 75, 3, 0, 14000, 0, 0, 0, 0, 0, 0, 0, 1566.46, -4395.17, 6.93236, 3.29579, '');
INSERT INTO `quest_end_scripts` VALUES (7784, 89, 3, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 1565.79, -4395.27, 6.986, 3.35, '');
INSERT INTO quest_end_scripts VALUES
(7784, 0, 29, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'npc_flag removed'),
(7784, 90, 29, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'npc_flag readded');
DELETE FROM `db_script_string` WHERE entry IN (2000009992, 2000009993); 
INSERT INTO `db_script_string` VALUES (2000009993, 'NEFARIAN IS SLAIN! People of Orgrimmar, bow down before the might of $N and his allies for they have laid a blow against the Black Dragonflight that is sure to stir the Aspects from their malaise! This defeat shall surely be felt by the father of the Black Flight: Deathwing reels in pain and anguish this day!', NULL, NULL, 'NEFARIAN WURDE ERSCHLAGEN! Einwohner von Orgrimmar, kniet nieder vor $N und $g seinen:ihren; mächtigen Verbündeten, die gemeinsam eine großen Sieg gegen den schwarzen Drachenschwarm errungen haben! Diese Tat wird selbst Deathwing, dem Vater der Blackwing Angst und Schrecken verbreiten!', NULL, NULL, NULL, NULL, NULL);
INSERT INTO `db_script_string` VALUES (2000009992, 'Be lifted by $N accomplishment! Revel in his rallying cry!', NULL, NULL, 'Erfeut euch an der mutigen Tat von $N! Lasst $g seinen:ihren; Siegesschrei eure Herzen höher schlagen!', NULL, NULL, NULL, NULL, NULL);
UPDATE `quest_template` SET `CompleteScript`='7784' WHERE (`entry`='7784');
	
# Quest 7495
UPDATE `locales_quest` SET EndText_loc3 = 'Bei Uthers Barte' WHERE entry = 7495;

# Quest 7781
UPDATE `locales_quest` SET EndText_loc3 = 'Unfassbar!' WHERE entry = 7781;

# Quest 7782
UPDATE `gameobject_template` SET `type`='9', `flags`='4' WHERE (`entry`='179882');
DELETE FROM `gameobject` WHERE guid=633353;
INSERT INTO `gameobject` VALUES (633353, 179882, 0, -8926.46, 495.391, 105.972, 2.48063, 0, 0, 0.945886, 0.324499, -3600, 100, 0);
DELETE FROM `quest_end_scripts` WHERE id = 7782;

UPDATE `quest_template` SET `CompleteScript`='7782' WHERE (`entry`='7782');

INSERT INTO quest_end_scripts VALUES
(7782, 0, 29, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'npc_flag removed'),
(7782, 24, 29, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'npc_flag readded');
INSERT INTO `quest_end_scripts` VALUES (7782, 2, 1, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7782, 2, 0, 6, 0, 0, 0, 0, 2000000163, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7782, 20, 1, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7782, 20, 0, 6, 0, 0, 0, 0, 2000000164, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7782, 22, 15, 22888, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `quest_end_scripts` VALUES (7782, 22, 9, 633353, 3600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

DELETE FROM `db_script_string` WHERE entry IN (2000000163, 2000000164); 
INSERT INTO `db_script_string` VALUES (2000000163, 'Citizens of the Alliance, the Lord of Blackrock is slain! Nefarian has been subdued by the combined might of $N and $g his:her; allies!', NULL, NULL, 'Angehörige der Allianz, der Herrscher des Blackrocks wurde besiegt! Die vereinte Macht von $N und $g seiner:ihrer; Verbündeten bezwangen Nefarian!', NULL, NULL, NULL, NULL, NULL);
INSERT INTO `db_script_string` VALUES (2000000164, 'Let your spirits rise! Rally around your champion, bask in $g his:her; glory! Revel in the rallying cry of the dragon slayer!', NULL, NULL, 'Erhebt Eure Gemüter! Feiert Euren Helden, erfreut euch an $g seinen:ihren; Ruhm! Lasst den Siegesschrei des Drachentöters Eure Herzen höher schlagen!', NULL, NULL, NULL, NULL, NULL);

UPDATE `locales_quest` SET OfferRewardText_loc3 = '<Feldmarschall Afrasiabi verbeugt sich.> $B$BSucht Euch eine Belohnung aus und lasst die Feierlichkeiten beginnen!' WHERE entry = 7782;

# Albtraumdrachen können nicht gekickt werden
UPDATE `creature_template` SET mechanic_immune_mask = mechanic_immune_mask | 33554432 WHERE entry IN(14887, 14888, 14889, 14890);