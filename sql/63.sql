UPDATE `creature` SET `spawntimesecs`=100 WHERE `id`=9046;

DELETE FROM `quest_start_scripts` WHERE `id`=3982;
INSERT INTO `quest_start_scripts` VALUES
(3982,5,10,8890,300000,0,0,0,0,0,0,0,375.11,-191.82,-70.77,1.92,'What is going on, first wave'),
(3982,5,10,8891,300000,0,0,0,0,0,0,0,377.86,-192.18,-70.16,1.99,'What is going on, first wave'),
(3982,5,10,8891,300000,0,0,0,0,0,0,0,377.66,-197.33,-70.05,3.44,'What is going on, first wave'),
(3982,40,10,8890,300000,0,0,0,0,0,0,0,375.11,-191.82,-70.77,1.92,'What is going on, second wave'),
(3982,40,10,8890,300000,0,0,0,0,0,0,0,376.91,-193.25,-70.37,1.95,'What is going on, second wave'),
(3982,40,10,8891,300000,0,0,0,0,0,0,0,377.86,-192.18,-70.16,1.99,'What is going on, second wave'),
(3982,40,10,8891,300000,0,0,0,0,0,0,0,377.66,-197.33,-70.05,3.44,'What is going on, second wave'),
(3982,70,7,3982,50,0,0,0,0,0,0,0,0,0,0,0,'What is going on,quest explored');

INSERT INTO `spell_chain` (`spell_id`, `prev_spell`, `first_spell`, `rank`) VALUES
(25037,0,25037,1),
(25722,25037,25037,2),
(22789,25722,25037,3),
(25804,22789,25037,4),
(8099,0,8099,1),
(8100,8099,8099,2),
(8101,8100,8099,3),
(12178,8101,8099,4),
(8115,0,8115,1),
(8116,8115,8115,2),
(8117,8116,8115,3),
(12174,8117,8115,4),
(8112,0,8112,1),
(8113,8112,8112,2),
(8114,8113,8112,3),
(12177,8114,8112,4),
(8118,0,8118,1),
(8119,8118,8118,2),
(8120,8119,8118,3),
(12179,8120,8118,4),
(8091,0,8091,1),
(8094,8091,8091,2),
(8095,8094,8091,3),
(12175,8095,8091,4);

-- Segen der Weisheit: an sich brauchte es bei mir nur die erste Zeile, damit wurden die anderen Werte von den beiden anderen Rängen richtig übernommen. Auf diese Weise stacken die Segen in ihrer großen und kleinen Variante bei mir lokal nicht mehr, ich konnte auch keine Fehler beim Lernen der Skills feststellen: alle Ränge wurden richtig zum richtigen Level beim Trainer gelernt.
UPDATE `spell_chain` SET `prev_spell`=25290, `first_spell`=19742, `rank`=11 WHERE `spell_id`=25894;
UPDATE `spell_chain` SET  `first_spell`=19742, `rank`=12 WHERE `spell_id`=25918;
UPDATE `spell_chain` SET  `first_spell`=19742, `rank`=13 WHERE `spell_id`=27143;
-- Segen der Macht: same same but different
UPDATE `spell_chain` SET `prev_spell`=25291, `first_spell`=19740, `rank`=8 WHERE `spell_id`=25782;
UPDATE `spell_chain` SET  `first_spell`=19740, `rank`=9 WHERE `spell_id`=25916;
-- Segen der Könige: bei mir waren noch keine Ränge vorhanden
INSERT INTO `spell_chain` (`spell_id`, `prev_spell`, `first_spell`, `rank`) VALUES
(20217,0,20217,1),
(25898,20217,20217,2);
-- Segen der Rettung: bei mir waren noch keine Ränge vorhanden
INSERT INTO `spell_chain` (`spell_id`, `prev_spell`, `first_spell`, `rank`) VALUES
(1038,0,1038,1),
(25895,1038,1038,2);