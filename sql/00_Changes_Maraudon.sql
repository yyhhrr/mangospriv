INSERT INTO script_waypoint VALUES
( 13716, 0, 726.106, 77.976, -86.592, 0, 'CelebrasRedeemed 1' ),
( 13716, 1, 657.7, 75.09, -86.82, 0, 'CelebrasRedeemed 2' );

UPDATE creature_template SET ScriptName = 'npc_celebras_the_redeemed', MovementType = '0' WHERE entry = '13716';
UPDATE instance_template SET ScriptName = 'instance_maraudon' WHERE map = '349';
UPDATE gameobject_template SET flags = '64' WHERE entry = '178965';

INSERT INTO gameobject_scripts (id, command, datalong, datalong2) VALUES
( 178965, 15, 21939, 0 );