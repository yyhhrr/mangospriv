/*Alex's Test Beatdown Staff*/
UPDATE item_template SET sheath = 2 WHERE entry = 19879;

/*Scharlachrote NPCs haben fälschlicherweise die Faction Horde*/
UPDATE creature_template SET faction_a = 56, faction_h = 56 WHERE faction_a = 67;

/*Der Läuterer ist nicht immun gegen alles*/
UPDATE creature_template SET mindmg = 2000, maxdmg = 3000, mechanic_immune_mask = 4294967295 WHERE entry = 14503;

/*Die Skelette der Priesterquest haben (lokal) kein ScriptName*/
UPDATE creature_template SET ScriptName = "mob_scourge_archer" WHERE entry = 14489;
UPDATE creature_template SET ScriptName = "mob_scourge_footsoldier" WHERE entry = 14486;