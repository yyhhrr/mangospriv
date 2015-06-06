INSERT INTO spell_proc_event (entry, CustomChance) VALUES
( 8516, 20 ),
( 10608, 20 ),
( 10610, 20 );

UPDATE spell_bonus_data SET dot_bonus = 0 WHERE entry = 755;

INSERT INTO spell_bonus_data (entry, comments) VALUES
( 3698, "Warlock - Health Funnel Rank 2" ),
( 3699, "Warlock - Health Funnel Rank 3" ),
( 3700, "Warlock - Health Funnel Rank 4" ),
( 11693, "Warlock - Health Funnel Rank 5" ),
( 11694, "Warlock - Health Funnel Rank 6" ),
( 11695, "Warlock - Health Funnel Rank 7" );


INSERT INTO spell_bonus_data (entry, direct_bonus, comments) VALUES
( 6789, 0.21428, "Warlock - Death Coil Rank 1" ),
( 17925, 0.21428, "Warlock - Death Coil Rank 2" ),
( 17926, 0.21428, "Warlock - Death Coil Rank 3" );

UPDATE spell_bonus_data SET direct_bonus = 1, dot_bonus = 0.166666 WHERE entry = 172;

UPDATE creature_template SET unit_flags = 32768 WHERE entry = 417;

DELETE FROM gameobject WHERE guid = 7569;


UPDATE creature_template SET mindmg = mindmg * 3, maxdmg = maxdmg * 3 WHERE entry = 12422;