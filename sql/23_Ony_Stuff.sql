INSERT INTO creature (id, map, position_x, position_y, position_z, orientation, curhealth) VALUES
( 14524, 1, 6248.44, -1204.82, 370.91, 2.16, 11122 ),
( 14525, 1, 6188.29, -1166.57, 371.07, 1.78, 11122 ),
( 14526, 1, 6217.13, -1119.14, 370.16, 4.23, 11122 );

UPDATE quest_template SET RewItemCount1 = 1, RewItemId1 = 18513 WHERE entry = 7507;
UPDATE quest_template SET PrevQuestId = 7508 WHERE entry = 7509;
UPDATE quest_template SET RewItemCount2 = 1, RewItemId2 = 18715 WHERE entry = 7636;

UPDATE reference_loot_template SET condition_value1 = 67 WHERE item = 16947;
UPDATE reference_loot_template SET condition_value1 = 469 WHERE item = 16955;