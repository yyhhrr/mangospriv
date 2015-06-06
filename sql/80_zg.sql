DELETE FROM creature_ai_scripts WHERE creature_id IN(11350, '14989');
INSERT INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '11350',  '2',  '0', '100', '2','30',  '0',  '0',  '0','11',  '8269', '0', '0','1', '-46', '0', '0','0', '0', '0', '0','Gurubashi Axe Thrower - Cast Enrage at 30% HP' ),
(  '11350',  '0',  '0', '100', '3','5000',  '5000',  '15000',  '18000','11',  '24018', '0', '0','0', '0', '0', '0','0', '0', '0', '0','Gurubashi Axe Thrower - Cast Axe Flurry' ),
(  '14989',  '11',  '0', '100', '0','0',  '0',  '0',  '0','11',  '26744', '0', '0','0', '0', '0', '0','0', '0', '0', '0','Poisonous Cloud - Cast Poisonous Blood' );

UPDATE creature_template SET minlevel = 63, maxlevel = 63, unit_flags = 131072 + 33554432, AIName = '', ScriptName = 'mob_poisonous_cloud' WHERE entry = 14989;

UPDATE creature SET spawntimesecs = 90 WHERE guid IN(51428, 51426);
UPDATE creature_model_info SET combat_reach = 3 WHERE modelid = 15295;