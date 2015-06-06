INSERT INTO npc_vendor (entry, item) VALUES
( 7940, 14469 ),
( 7940, 14488 ),
( 7940, 14472 );

UPDATE item_template SET class = 7, subclass = 3 WHERE entry IN(18637, 18638, 18639, 18634, 7506, 16022);

UPDATE creature_template SET unit_flags = 0 WHERE entry = 684;