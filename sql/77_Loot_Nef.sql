UPDATE creature_loot_template SET ChanceOrQuestChance = 40 WHERE mincountOrRef = -115836;

INSERT INTO creature_loot_template VALUES
( 11583, 34002, 100, 0, -34002, 4, 0, 0, 0 );


UPDATE creature_template SET skinloot = 0 WHERE entry IN (9198, 14750, 8662, 14986, 12457);

UPDATE creature_loot_template SET ChanceOrQuestChance = 3 WHERE item = 19437;
UPDATE creature_loot_template SET ChanceOrQuestChance = 4 WHERE item = 19438;
UPDATE creature_loot_template SET ChanceOrQuestChance = 2 WHERE item = 19436;
UPDATE creature_loot_template SET ChanceOrQuestChance = 4 WHERE item = 19439;
UPDATE creature_loot_template SET ChanceOrQuestChance = 2 WHERE item = 19362;
UPDATE creature_loot_template SET ChanceOrQuestChance = 2 WHERE item = 19358;
UPDATE creature_loot_template SET ChanceOrQuestChance = 6 WHERE item = 19354;
UPDATE creature_loot_template SET ChanceOrQuestChance = 2 WHERE item = 19435;
UPDATE creature_loot_template SET ChanceOrQuestChance = 2 WHERE item = 19434;


UPDATE creature_template SET minhealth = 1356025, maxhealth = 1356025, mindmg = 170, maxdmg = 190 WHERE entry = 12017;

UPDATE item_template SET spellid_1 = 26656 WHERE entry = 21176;