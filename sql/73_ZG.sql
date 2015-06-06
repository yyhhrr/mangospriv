DELETE FROM event_scripts WHERE id IN(9104, 9066);

INSERT INTO scripted_event_id VALUES
( 9104, "event_gahzranka" );

UPDATE gameobject_template SET data2 = 0, data3 = 0, data11 = 0 WHERE entry = 180526;