INSERT INTO spell_script_target VALUES 
( 22904, 0 , 179561 );

UPDATE quest_template SET ExclusiveGroup = 0 WHERE 
entry = 8443 OR
entry = 8442 OR
entry = 8441 OR
entry = 8440 OR
entry = 8439 OR
entry = 8438 OR
entry = 8437 OR
entry = 8436 OR
entry = 8490 OR
entry = 8470;


INSERT INTO game_graveyard_zone VALUES 
( 636, 25, 0 );

UPDATE creature_template SET mindmg = 208, maxdmg = 333 WHERE entry = 14533;