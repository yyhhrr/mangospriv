UPDATE gameobject_loot_template SET item = 40025, groupid = 0, ChanceOrQuestChance = 100, mincountOrRef = -40025 WHERE item = 11751;
DELETE FROM gameobject_loot_template WHERE item = 11752 OR item = 11753;

INSERT INTO reference_loot_template VALUES
( 40025, 11751, 0, 1, 1, 1, 0, 0, 0 ),
( 40025, 11752, 0, 1, 1, 1, 0, 0, 0 ),
( 40025, 11753, 0, 1, 1, 1, 0, 0, 0 );

INSERT INTO creature_loot_template VALUES
( 11502, 34002, 100, 0, -34002, 4, 0, 0, 0 );