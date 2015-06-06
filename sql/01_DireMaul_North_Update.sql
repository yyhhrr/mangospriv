INSERT INTO creature_movement(id, point, position_x, position_y, position_z) VALUES
( 138186, 1, 323.46, 256.32, 11.21 ),
( 138186, 2, 384.42, 256.63, 11.43 ),
( 138186, 3, 384.42, 256.63, 11.43 ),
( 138186, 4, 384.42, 256.63, 11.43 );

UPDATE creature SET MovementType = '2' WHERE id = '14321';
UPDATE creature_template SET MovementType = '2' WHERE entry = '14321';


INSERT INTO gameobject(guid, id, map, position_x, position_y, position_z, state) VALUES
( 11111550, 179516, 429, 384.38, 262.83, 11.43, 1 );

UPDATE gameobject_template SET ScriptName = 'Quest_ElfischeLegenden' WHERE entry = '179544';