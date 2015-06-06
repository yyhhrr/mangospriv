/*Kugel der Herrschaft hat keine Spells*/
UPDATE gameobject_template SET


/*Auslöser ist angreifbar*/
UPDATE creature_template SET faction_A = 35, faction_H = 35, mechanic_immune_mask = 4294967295 WHERE entry = 14449;