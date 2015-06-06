UPDATE creature_template set faction_A = 80, faction_H = 80 WHERE entry IN ( 14263, 14302, 14261, 14264, 14262, 14265 );

UPDATE creature_template set mindmg = 891, maxdmg = 1144 WHERE entry = 14302;
UPDATE creature_template set mindmg = 517, maxdmg = 637 WHERE entry IN( 14263, 14264, 14262 );
UPDATE creature_template set mindmg = 524, maxdmg = 673 WHERE entry IN( 14261, 14265 );