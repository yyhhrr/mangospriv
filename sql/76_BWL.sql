DELETE FROM creature_formations WHERE leaderGUID IN( 139730, 139728, 139799 );

REPLACE INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '12467',  '11',  '0', '100', '2','0',  '0',  '0',  '0','11',  '22436', '0', '1','0', '0', '0', '0','0', '0', '', '','Death Talon Captain - Cast Aura of Flames on Spawn' ),
(  '12467',  '0',  '0', '100', '3','1000',  '1000',  '18000',  '24000','11',  '22440', '0', '1','0', '0', '0', '0','0', '0', '', '','Death Talon Captain - Cast Commanding Shout' ),
(  '12467',  '9',  '0', '100', '3','0',  '5',  '6000',  '9000','11',  '15496', '1', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Captain - Cast Cleave' ),
(  '12467',  '0',  '0', '100', '3','12000',  '16000',  '17000',  '21000','11',  '22438', '4', '33','0', '0', '0', '0','0', '0', '', '','Death Talon Captain - Cast Mark of Detonation' ),
(  '12467',  '0',  '0', '100', '0','1',  '1',  '',  '','38',  '', '', '','0', '', '', '','0', '', '', '','Death Talon Captain - Set in combat with zone' );

REPLACE INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '12464',  '9',  '0', '100', '3','0',  '30',  '5000',  '8000','11',  '22433', '4', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Seether - Cast Flame Buffet' ),
(  '12464',  '0',  '0', '100', '3','9000',  '16000',  '19000',  '26000','11',  '22428', '0', '1','0', '0', '0', '0','0', '0', '', '','Death Talon Seether - Cast Frenzy' ),
(  '12464',  '0',  '0', '100', '0','1',  '1',  '',  '','38',  '', '', '','0', '', '', '','0', '', '', '','' );

REPLACE INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '12465',  '1',  '0', '100', '2','0',  '0',  '0',  '0','21',  '0', '0', '0','22', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Prevent Combat Movement and Set Phase to 0 on Spawn' ),
(  '12465',  '4',  '0', '100', '2','0',  '0',  '0',  '0','11',  '22425', '1', '0','23', '1', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Cast Fireball Volley and Set Phase 1 on Aggro' ),
(  '12465',  '9',  '5', '100', '3','0',  '40',  '4000',  '5000','11',  '22425', '1', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Cast Fireball Volley (Phase 1)' ),
(  '12465',  '3',  '5', '100', '2','7',  '0',  '0',  '0','21',  '1', '0', '0','23', '1', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Start Combat Movement and Set Phase 2 when Mana is at 7% (Phase 1)' ),
(  '12465',  '9',  '5', '100', '2','35',  '80',  '0',  '0','21',  '1', '0', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Start Combat Movement at 35 Yards (Phase 1)' ),
(  '12465',  '9',  '5', '100', '2','5',  '15',  '0',  '0','21',  '0', '0', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Prevent Combat Movement at 15 Yards (Phase 1)' ),
(  '12465',  '9',  '5', '100', '2','0',  '5',  '0',  '0','21',  '1', '0', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Start Combat Movement Below 5 Yards (Phase 1)' ),
(  '12465',  '3',  '3', '100', '3','100',  '15',  '100',  '100','23',  '-1', '0', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Set Phase 1 when Mana is above 15% (Phase 2)' ),
(  '12465',  '9',  '0', '100', '3','0',  '10',  '8000',  '11000','11',  '22424', '0', '1','0', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Cast Blast Wave' ),
(  '12465',  '7',  '0', '100', '2','0',  '0',  '0',  '0','22',  '0', '0', '0','0', '0', '0', '0','0', '0', '', '','Death Talon Wyrmkin - Set Phase to 0 on Evade' ),
(  '12465',  '0',  '0', '100', '0','1',  '1',  '',  '','38',  '', '', '','0', '', '', '','0', '', '', '','' );

REPLACE INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '12463',  '9',  '0', '100', '3','0',  '40',  '12000',  '17000','11',  '16636', '4', '1','0', '0', '0', '0','0', '0', '', '','Death Talon Flamescale - Cast Berserker Charge' ),
(  '12463',  '9',  '0', '100', '3','0',  '20',  '5000',  '8000','11',  '22423', '4', '32','0', '0', '0', '0','0', '0', '', '','Death Talon Flamescale - Cast Flame Shock' ),
(  '12463',  '0',  '0', '100', '0','1',  '1',  '',  '','38',  '', '', '','0', '', '', '','0', '', '', '','' );


-- Überflüssige Illyanna Ravenoak gelöscht
DELETE FROM `creature` WHERE `guid`=138112 AND `id`=11488;

-- Falsche faction der Eldereth korrigiert, greifen nun nicht mehr andere Mobs der Instanz beim Betreten an
UPDATE `creature_template` SET `faction_H`= 14, `faction_A`= 14 WHERE `entry` IN (11469,11470);

-- Verlässt man MC, wird man zu Lothos geportet
UPDATE `areatrigger_teleport` SET `target_map`=0,`target_position_x`=-7536.94,`target_position_y`=-1063.81,`target_position_z`=180.98,`target_orientation`=0.650 WHERE `id`=2890; 

UPDATE `creature_template` SET `minlevel`=1, `maxlevel`=1 WHERE `entry` IN(11817,11819);

UPDATE `creature_model_info` SET `combat_reach`=bounding_radius+0.2 WHERE `modelid`=2554;

UPDATE creature_template SET mechanic_immune_mask = 1025 WHERE entry = 12458;