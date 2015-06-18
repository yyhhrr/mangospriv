-- Simone + Pet linking
DELETE FROM creature_linking_template WHERE entry = 14528 AND master_entry = 14527;
INSERT INTO `creature_linking_template` (`entry`, `map`, `master_entry`, `flag`, `search_range`) VALUES('14528','1','14527','643','0');

-- Franklin neutral
UPDATE creature_template SET faction_H = 7, faction_A = 7 WHERE entry = 14529;