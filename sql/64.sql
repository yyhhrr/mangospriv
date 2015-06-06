UPDATE `spell_chain` SET `rank`=6, `prev_spell`=19854 WHERE `spell_id`=25894;
UPDATE `spell_chain` SET `rank`=7, `prev_spell`=25894 WHERE `spell_id`=25290;
UPDATE `spell_chain` SET `rank`=8, `prev_spell`=25290 WHERE `spell_id`=25918;
UPDATE `spell_chain` SET `rank`=7, `prev_spell`=19838 WHERE `spell_id`=25782;
UPDATE `spell_chain` SET `rank`=8, `prev_spell`=25782 WHERE `spell_id`=25291;
UPDATE `spell_chain` SET `rank`=9, `prev_spell`=25291 WHERE `spell_id`=25916;


UPDATE `gameobject` SET `spawntimesecs` =7200 WHERE `id`=153451; 

-- Baron Geddon hat InhabitType = 4 und kann somit fliegen. Mögliche Ursache warum er durch Wände läuft in MC.
UPDATE creature_template SET InhabitType = 1 WHERE entry = 12056;

-- Jeklik darf fliegen - sonst kommt sie von der Mauer nicht runter.
-- UPDATE creature_template SET InhabitType = 7 WHERE entry = 14517;



UPDATE creature_model_info SET bounding_radius = 2.38, combat_reach = 1 WHERE modelid = 15295;