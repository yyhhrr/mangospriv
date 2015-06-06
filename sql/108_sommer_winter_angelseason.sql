-- Winterfischsaison vom 23.9. bis 21.3 -- Alle 4 Jahre verschiebt sich der Zeitraum um einen Tag nach vorne (Schaltjahrbedingt)
DELETE FROM `game_event` WHERE entry = 97;
DELETE FROM `game_event` WHERE entry = 98;
INSERT INTO `game_event`
(`entry`, `start_time`,`end_time`,`occurence`,`length`,`holiday`,`description`)
VALUES
(97,'2010-09-23 01:00:00','2020-12-30 23:00:00',525600,256320,0,'Winterangelsaison');
 
 
-- Sommerfischsaison vom 21.3. bis 23.9 -- Alle 4 Jahre verschiebt sich der Zeitraum um einen Tag nach vorne (Schaltjahrbedingt)
INSERT INTO `game_event`
(`entry`, `start_time`,`end_time`,`occurence`,`length`,`holiday`,`description`)
VALUES
(98,'2010-03-21 01:00:00','2020-12-30 23:00:00',525600,269280,0,'Sommerangelsaison');
 
 
-- Winterkalmer im Winter
UPDATE reference_loot_template SET condition_value1 = 97 WHERE entry = 13755 AND item = 13755;
 
-- Sommerbarsch im Sommer
UPDATE reference_loot_template SET condition_value1 = 98 WHERE entry = 13755 AND item = 13756;