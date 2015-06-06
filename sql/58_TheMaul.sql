INSERT INTO script_texts (entry, content_default, type) VALUES
( -1540100, "It's time for the main event! Skarr the Unbreakable is in the maul!", 1 ),
( -1540101, "Can that be the Razza down there in the Maul? This is going to be exciting!", 1 ),
( -1540102, "I don't know the propper text... Whatever, here is Mushgog!", 1 );

UPDATE creature_template SET ScriptName = "npc_grinbiblix" WHERE entry = 14395;

UPDATE creature_template SET lootid = 24100 WHERE entry = 11498;
UPDATE creature_template SET lootid = 24101 WHERE entry = 11497;
UPDATE creature_template SET lootid = 24102 WHERE entry = 11447;

INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount) VALUES
( 24100, 35015, 100, 1, -35015, 1 ),
( 24100, 24034, 20, 2, -24034, 1 ),
( 24100, 24026, 20, 3, -24026, 1 ),
( 24100, 14047,  28, 0, 1, 4 ),

( 24101, 35015, 20, 2, -35015, 1 ),
( 24101, 24034, 100, 1, -24034, 1 ),
( 24101, 24026, 20, 3, -24026, 1 ),

( 24102, 35015, 20, 3, -35015, 1 ),
( 24102, 24034, 20, 2, -24034, 1 ),
( 24102, 24026, 100, 1, -24026, 1 );

INSERT INTO `game_event` VALUES ('91', '2013-08-07 00:00:00', '2035-12-31 00:00:00', '360', '350', '0', 'Dire Maul - The Maul');
INSERT INTO `game_event_creature` VALUES ('50061', '91');