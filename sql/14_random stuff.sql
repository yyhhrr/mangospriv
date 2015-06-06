UPDATE creature_template SET ScriptName = "npc_battle_chicken" WHERE entry = 8836;
UPDATE creature_template SET ScriptName = "npc_exploding_sheep" WHERE entry = 2675;

UPDATE item_template SET spellppmRate_1 = 2.1 WHERE entry = 14576;

/*Musste Aschenschwinge löschen, weil er den Server gecrasht hat*/
INSERT INTO creature VALUES
( 31041, 10321, 1, 6374, 0, -5103.36, -3949.53, 41.4934, 1.23918, 300, 0, 0, 31291, 0, 0, 0 );