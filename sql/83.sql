INSERT INTO creature_linking_template VALUES
( 14880, 309, 14532, 1, 20 ),
( 14532, 309, 11338, 1, 20 ),
( 11338, 309, 11370, 1, 20 ),
( 11338, 309, 14532, 1, 20 );

UPDATE creature_template SET AIName = 'EventAI' WHERE entry IN(5709, 5710, 5711, 5712, 5714, 5715, 5716, 5717, 5719, 5720, 5721, 5722, 8443, 8497, 8580, 9456);

UPDATE creature_template SET AIName = '' WHERE entry IN(8717, 9683, 9708, 10375, 12101, 15554, 800005);