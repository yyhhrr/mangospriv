-- Löschen aller Looteinträge für die 3 Drachen

DELETE FROM reference_loot_template WHERE entry IN (SELECT item FROM creature_loot_template WHERE entry IN (11981, 11983, 14601));
DELETE FROM reference_loot_template WHERE entry IN (99999,99998,99997,99996);		
DELETE FROM creature_loot_template WHERE entry IN (11981,11983,14601) 
		OR item IN (16899 ,16907,16913,16920,16928,16940,16948,16956,16964,19357,19367,19430,19431,19432,19433,19353,19355,19394,19395,
                    19396,19397,19343,19344,19365,19398,19399,19400,19401,19402,19345,19368,19403,19405,19406,19407);


-- Set und geteilter Drachenloot

INSERT INTO `creature_loot_template` (`entry`, `item`,  `ChanceOrQuestChance`, `groupid`,  `mincountOrRef`,  `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) 
VALUES
(11981, 99999, 100, 0, -99999, 1, 0, 0, 0), -- Flammenmaul
(11983, 99999, 100, 0, -99999, 1, 0, 0, 0), -- Feuerschwinge
(14601, 99999, 100, 0, -99999, 1, 0, 0, 0), -- Schattenschwinge
(11981, 99998, 100, 0, -99998, 1, 0, 0, 0), -- Drachen-Spezifischer Loot Flammenmaul
(11983, 99997, 100,  0, -99997, 1, 0, 0, 0), -- Drachen-Spezifischer Loot Feuerschwinge
(14601, 99996,  100,  0, -99996, 1, 0, 0, 0); -- Drachen-Spezifischer Loot Schattenschwinge

INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) 
VALUES 
(99999,  16899,  0,  1, 1, 1, 0, 0, 0), -- Druide
(99999,  16907,  0,  1, 1, 1, 0, 0, 0), -- Schurke
(99999,  16913,  0,  1, 1, 1, 0, 0, 0), -- Magier
(99999,  16920,  0,  1, 1, 1, 0, 0, 0), -- Priester
(99999,  16928,  0,  1, 1, 1, 0, 0, 0), -- Hexer
(99999,  16940,  0,  1, 1, 1, 0, 0, 0), -- Jäger
(99999,  16948,  0,  1, 1, 1, 6, 67, 0), -- Schamane
(99999,  16956,  0,  1, 1, 1, 6, 469, 0), -- Pala
(99999,  16964,  0,  1, 1, 1, 0, 0, 0), -- Krieger
(99999,  19353,  4,  1, 1, 1, 0, 0, 0), -- Drake Talon Cleaver
(99999,  19355,  4,  1, 1, 1, 0, 0, 0), -- Shadow Wing Focus Staff
(99999,  19394,  8,  1, 1, 1, 0, 0, 0), -- Drake Talon Pauldrons
(99999,  19395,  8,  1, 1, 1, 0, 0, 0), -- Rejuvenating Gem
(99999,  19396,  8,  1, 1, 1, 0, 0, 0), -- Taut Dragonhide Belt
(99999,  19397,  8,  1, 1, 1, 0, 0, 0); -- Ring of Blackrock


INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) 
VALUES 
(99998,  19357,  10,  1, 1, 1, 0, 0, 0), -- Herald of Woe
(99998,  19367,  10,  1, 1, 1, 0, 0, 0), -- Dragon's Touch
(99998,  19430,  20,  1, 1, 1, 0, 0, 0), -- Shroud of Pure Thought
(99998,  19431,  20,  1, 1, 1, 0, 0, 0), -- Styleen's Impeding Scarab
(99998,  19432,  20,  1, 1, 1, 0, 0, 0), -- Circle of Applied Force
(99998,  19433,  20,  1, 1, 1, 0, 0, 0); -- Emberweave Leggings


INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) 
VALUES
(99997,  19343,  0,  1, 1, 1, 6, 469, 0), -- Scrolls of Blinding Light
(99997,  19344,  0,  1, 1, 1, 6, 67, 0), -- Natural Alignment Crystal
(99997,  19365,  0,  1, 1, 1, 0, 0, 0), -- Claw of the Black Drake
(99997,  19398,  0,  1, 1, 1, 0, 0, 0), -- Cloak of Firemaw
(99997,  19399,  0,  1, 1, 1, 0, 0, 0), -- Black Ash Robe
(99997,  19400,  0,  1, 1, 1, 0, 0, 0), -- Firemaw's Clutch
(99997,  19401,  0,  1, 1, 1, 0, 0, 0), -- Primalist's Linked Legguards
(99997,  19402,  0,  1, 1, 1, 0, 0, 0); -- Legguards of the Fallen Crusader

	
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) 
VALUES
(99996,  19345,  18,  1, 1, 1, 0, 0, 0), -- Aegis of Preservation
(99996,  19368,  10,  1, 1, 1, 0, 0, 0), -- Dragonbreath Hand Cannon
(99996,  19403,  18,  1, 1, 1, 0, 0, 0), -- Band of Forced Concentration
(99996,  19405,  18,  1, 1, 1, 0, 0, 0), -- Malfurion's Blessed Bulwark
(99996,  19406,  18,  1, 1, 1, 0, 0, 0), -- Drake Fang Talisman
(99996, 19407,  18,  1, 1, 1, 0, 0, 0); -- Ebony Flame Gloves