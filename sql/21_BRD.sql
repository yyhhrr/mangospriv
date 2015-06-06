INSERT INTO gossip_texts(entry, content_default, content_loc3) VALUES
( -3469200, "You're good for nothing, Ribbly. It's time to pay for your wickedness!", "Ihr seid ein Nichtsnutz, Ribbly. Es ist Zeit für Euren Wahnsinn zu bezahlen!" );

INSERT INTO script_texts(entry, content_default, content_loc3) VALUES
( -1540020, "Get away from those kegs!", "Lasst die Finger von den Fässern!" ),
( -1540021, "You'll pay for that!", "Dafür werdet ihr bezahlen!" ),
( -1540022, "HALT! There were... reports...of a... disturbance.", "HALT! Es gab... Meldungen... über... Unruhen!" ),
( -1540023, "We... are... investigating.", "Wir... untersuchen." ),
( -1540024, "Violence! Property damage! None shall pass!!", "Gewalt! Sachbeschädigung! Niemand darf passieren!" ),
( -1540025, "Try the boar! Is my new recipe!", "Versucht das Wildschwein! Ist mein eigenes Rezept!" ),
( -1540026, "Enjoy! You won't find better ale anywhere!", "Genießt es! Ihr werdet nirgends besseres Bier finden!" ),
( -1540027, "Drink up! There's more where that came from!", "Trinkt aus! Wo das herkommt gibt es noch mehr!" ),
( -1540028, "Have you tried the Dark Iron Ale? It's the best!", "Habt ihr das Dunkeleisenbier probiert? Das ist das beste!" ),
( -1540029, "What you are doing over there?", "Was macht ihr dort drüben?" ),
( -1540030, "No stealing the goods", "Hier wird nicht gestohlen!" ),
( -1540031, "Hey! Get away from that", "Hey! Lasst das in Ruhe!" ),
( -1540032, "That's it!! You're going down!", "Das wars! Dafür werdet ihr bezahlen!" ),
( -1540033, "No! Get away from me! Help!", "Nein! Lasst mich in Ruhe! Hilfe!" );

UPDATE creature_template SET ScriptName = "npc_blackbreath" WHERE entry = 9537;
UPDATE creature_template SET ScriptName = "npc_patron" WHERE entry = 9547 OR entry = 9545 OR entry = 9554;
UPDATE creature_template SET MovementType = 0 WHERE entry = 8911 OR entry = 8895;
UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 9502;
UPDATE creature_template SET ScriptName = "npc_spazzring" WHERE entry = 9499;
UPDATE creature_template SET ScriptName = "npc_ribbly" WHERE entry = 9543;

UPDATE creature SET position_x = 846.74, position_y = -224.70, position_z = -43.68, orientation = 0.52 WHERE id = 9502;

UPDATE gameobject SET position_x = 913.73, position_y = -146.18, position_z = -49.75 WHERE guid = 43098;
UPDATE gameobject SET position_x = 916.68, position_y = -150.74, position_z = -49.75 WHERE guid = 43099;

UPDATE gameobject_template SET ScriptName = "go_guzzler_food" WHERE entry = 165739 OR entry = 165738;
UPDATE gameobject_template SET ScriptName = "go_lager_keg" WHERE entry = 164911;
