UPDATE creature_template SET ScriptName = "npc_pyrewood_citizen" WHERE 
entry = 3530 OR
entry = 1891 OR
entry = 1894 OR
entry = 1895 OR
entry = 3528 OR
entry = 3532;

DELETE FROM creature_questrelation WHERE id = 4218 and quest = 26;
UPDATE quest_template SET NextQuestId = 31 WHERE NextQuestId = 26;