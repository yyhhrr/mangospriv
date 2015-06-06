UPDATE creature_template SET ScriptName = "mob_molthor" WHERE entry = 14875;

INSERT INTO script_texts (entry, content_default, type, language) VALUES
( -1800000, 'Begin the ritual, my servants. We must banish the heart of Hakkar. Back into the void!', 0, 0 ),
( -1800001, 'All hail $N, slayer of Hakkar and hero of Azeroth!', 0, 0 );

INSERT INTO creature_template (entry, modelid_1, name, minlevel, maxlevel, minhealth, maxhealth, faction_A, faction_H, scale, unit_flags, InhabitType, ScriptName) VALUES
( 90002, 11686, 'zg hakkar quest dummy', 1, 1, 1, 1, 35, 35, 2, 33555200, 7, 'ZG_HAKKAR_QUEST_DUMMY' );

INSERT INTO spell_script_target VALUES
( 24217, 1, 90002 );

INSERT INTO creature VALUES
( 9000086, 90002, 0, 11686, 0, -11820.90, 1337.82, 10.0, 0.0, 300, 0, 0, 1, 0, 0, 2 );

INSERT INTO creature_movement VALUES
( 9000086, 1, -11820.90, 1337.82, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.09772, 0, 0 );