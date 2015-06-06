// Flüssiges Feuer
UPDATE gameobject_template SET data2 = 5, data5 = 1 WHERE entry = 180125;

INSERT INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '11370',  '6',  '0', '100', '0','0',  '0',  '0',  '0','11',  '24598', '0', '6','0', '0', '0', '0','0', '0', '0', '0','summon spiders on death' );

UPDATE creature_template SET AIName = "EventAI" WHERE entry = 11370;