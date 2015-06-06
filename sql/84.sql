INSERT INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '11353',  '0',  '0', '100', '1','2000',  '2500',  '5000',  '6666','11',  '24437', '0', '0','0', '0', '0', '0','0', '0', '0', '0','gurubashi blooddrinker' );

UPDATE creature_template SET AIName = 'EventAI' WHERE entry = 11353;


INSERT INTO creature_linking_template VALUES
( 11361, 309, 11350, 1, 20 ),
( 11350, 309, 11831, 1, 20 ),
( 15111, 309, 11340, 1, 20 ),
( 11340, 309, 15111, 1, 20 );