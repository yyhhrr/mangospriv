INSERT INTO script_texts (entry, content_default, content_loc3, sound) VALUES
( -1540035, "Intruders have breached the hatchery! Sound the alarm! Protect the eggs at all costs!", "Eindringlinge sind in die Brutstätte vorgedrungen, schlagt Alarm! Beschützt die Eier um jeden Preis!", 8272 ),
( -1540036, "Foolish mortal, you serve me now!", "Sterbliche... ihr dient jetzt mir!", 8273 ),
( -1540037, "Ich habe die Kontrolle wiedererlangt. Alle Mann zurück auf ihre Posten!", "Ich habe die Kontrolle wiedererlangt. Alle Mann zurück auf ihre Posten!", 8274 );

UPDATE gameobject_template SET ScriptName = "DragonOrb1" WHERE entry = 177808;
UPDATE gameobject_template SET data6 = 2147483647 WHERE entry = 177807;

UPDATE creature_template set npcflag = 7 WHERE entry = 11057 OR entry = 11056;