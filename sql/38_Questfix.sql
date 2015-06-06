INSERT INTO event_scripts (id, command, datalong, datalong2, comments) VALUES
( 12000, 7, 3701, 20, 'Thaurissan Ruins' );
UPDATE gameobject_template SET data0 = 279, data1 = 3701, data2 = 12000 WHERE entry = 153556;


UPDATE quest_template SET PrevQuestId = 4361 WHERE entry = 4362;