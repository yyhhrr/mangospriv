UPDATE item_template SET requiredhonorrank = 0;

INSERT INTO command VALUES
( 'premade', 4, 'Syntax: .premade <subcommand>' ),
( 'premade pre', 4, 'Info: Gibt dem Spieler pre Raid Gear' ),
( 'premade mc', 4, 'Info: Gibt dem Spieler Ausrüstung aus MC / Onyxia / ZG' ),
( 'premade bwl', 4, 'Info: Gibt dem Spieler Ausrüstung aus BWL' ),
( 'premade aq', 4, 'Info: Gibt dem Spieler Ausrüstung aus AQ' ),
( 'premade naxx', 4, 'Info: Gibt dem Spieler Ausrüstung aus Naxxramas' );


INSERT INTO npc_vendor (entry, item) VALUES
( 800021, 12811 ),
( 800021, 16203 ),
( 800021, 16204 ),
( 800021, 14344 ),
( 800021, 12808 ),
( 800021, 12803 ),
( 800021, 7078 ),
( 800021, 7080 ),
( 800021, 7082 ),
( 800021, 13926 ),
( 800021, 18512 ),
( 800021, 13468 ),
( 800021, 11754 ),
( 800021, 12809 ),
( 800021, 4625 ),
( 800021, 8831 ),
( 800021, 14343 ),
( 800021, 11177 ),
( 800021, 7909 ),
( 800021, 11175 );

INSERT INTO creature(guid, id, map, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType) VALUES 
( 1105301, '90200', '1', '0', '508', '10331.43', '830.01', '1326.29', '2.87', '25', '5', '0', '7423', '7071', '0', '0'),
( 1105302, '90200', '1', '0', '508', '-604.37', '-4264.72', '38.95', '2.78', '25', '5', '0', '7423', '7071', '0', '0'),
( 1105303, '90200', '0', '0', '508', '-8931.71', '-123.14', '81.98', '3.89', '25', '5', '0', '7423', '7071', '0', '0'),
( 1105304, '90200', '0', '0', '508', '-6213.93', '330.77', '383.59', '2.92', '25', '5', '0', '7423', '7071', '0', '0'),
( 1105305, '90200', '0', '0', '508', '1664.72', '1668.28', '121.02', '1.52', '25', '5', '0', '7423', '7071', '0', '0')
( 1105306, '90200', '1', '0', '508', '-2915.33', '-248.27', '53.18', '5.36', '25', '5', '0', '7423', '7071', '0', '0');