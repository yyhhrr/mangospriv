/* 50/50 Chance, dass einer der beiden Rare Mobs spawnt. Wenn einer spawnt, dann spawnt der andere nicht. */
UPDATE pool_creature SET chance = 50 WHERE guid = 1104484 OR guid = 40458;

/*Palazeug dropt nun nicht mehr für Hordler und Schamiezeug nicht mehr für Allies. Yay!*/
UPDATE reference_loot_template SET lootcondition = 6 WHERE 
item = 16854 OR
item = 16856 OR
item = 16853 OR
item = 16860 OR
item = 16855 OR
item = 16859 OR
item = 16842 OR
item = 16844 OR
item = 16841 OR
item = 16839 OR
item = 16843 OR
item = 16837 OR
item = 16946 OR
item = 16954;

UPDATE reference_loot_template SET condition_value1 = 469 WHERE
item = 16854 OR
item = 16856 OR
item = 16853 OR
item = 16860 OR
item = 16855 OR
item = 16859 OR
item = 16954;

UPDATE reference_loot_template SET condition_value1 = 67 WHERE
item = 16842 OR
item = 16844 OR
item = 16841 OR
item = 16839 OR
item = 16843 OR
item = 16837 OR
item = 16946;


/* Update Ragnaros Loot. Nun garantiert 2 T2 Hosen, 2 Random Epics und zu 33% ein weiteres Random Epic */
DELETE FROM reference_loot_template WHERE entry = 40003;
DELETE FROM creature_loot_template WHERE item = 40003;
DELETE FROM reference_loot_template WHERE entry = 40009;
DELETE FROM creature_loot_template WHERE item = 40009;

INSERT INTO reference_loot_template VALUES
( 40003, 17102, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 17107, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 18817, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 19137, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 17104, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 17076, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 18816, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 17106, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 18815, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 18814, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 19138, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 17063, 0, 1, 1, 1, 0, 0, 0 ),
( 40003, 17082, 0, 1, 1, 1, 0, 0, 0 ),

( 40022, 17102, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 17107, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 18817, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 19137, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 17104, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 17076, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 18816, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 17106, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 18815, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 18814, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 19138, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 17063, 0, 1, 1, 1, 0, 0, 0 ),
( 40022, 17082, 0, 1, 1, 1, 0, 0, 0 ),

( 40023, 16915, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16922, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16930, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16909, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16901, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16938, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16946, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16962, 0, 1, 1, 1, 0, 0, 0 ),
( 40023, 16954, 0, 1, 1, 1, 0, 0, 0 ),

( 40024, 17102, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 17107, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 18817, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 19137, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 17104, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 17076, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 18816, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 17106, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 18815, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 18814, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 19138, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 17063, 0, 1, 1, 1, 0, 0, 0 ),
( 40024, 17082, 0, 1, 1, 1, 0, 0, 0 );

INSERT INTO creature_loot_template VALUES
( 11502, 40003, 100, 0, -40003, 1, 0, 0, 0 ),
( 11502, 40022, 100, 0, -40022, 1, 0, 0, 0 ),
( 11502, 40023, 100, 0, -40023, 1, 0, 0, 0 ),
( 11502, 40024,  33, 0, -40024, 1, 0, 0, 0 );


/* Knockback dummy, wirft Spieler innerhalb von 20m weg */
UPDATE creature_template SET 
ScriptName = "ragnaros_knockback_dummy",
unit_class = 0,
unit_flags = 0,
spell1 = 0,
faction_A = 14, faction_H = 14,
minlevel = 60, maxlevel = 60
WHERE entry = 13148;