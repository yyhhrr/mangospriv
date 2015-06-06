REPLACE INTO script_texts (entry, content_default, type) VALUES
( -1000753, "We must act quickly or all shall be lost!", 0 ),
( -1000754, "My forces cannot overcome the Qiraji defenses. We will not be able to get close enough to place your precious barrier, dragon.", 0 ),
( -1000758, "%s glances at her companions.", 2 ),
( -1000755, "There is a way...", 0 ),
( -1000756, "%s nods knowingly." , 2 ),
( -1000757, "Aye, Fandral, remember these words: Let not your grief guide your faith. These thoughts you hold... dark places you go, night elf. Absolution cannot be had through misguided vengeance.", 0 ),
( -1000759, "We will push them back, Anachronos. This I vow. Uphold your end of this task. Let not your hands falter as you seal our fates behind the barrier.", 0 ),
( -1000760, "Succumb to the endless dream, little ones. Let it consume you!", 1 ),
( -1000761, "Anachronos, this diversion will give you and the young druid time enough to seal the gate. Do not falter. Now, let us see how they deal with chaotic magic.", 0 ),
( -1000762, "Let them feel the wrath of the Blue Flight! May Malygos protect me!", 1 ),
( -1000763, "Do not forget the sacrifices made on this day, night elf. We have all suffered immensely at the hands of these beasts.", 0 ),
( -1000764, "Alexstrasza grant me the resolve to drive our enemies back!", 1 ),
( -1000765, "NOW, STAGHELM! WE GO NOW! Prepare your magic!", 0 ),
( -1000766, "It is done, dragon. Lead the way.", 0 ),
( -1000767, "Stay close...", 0 ),
( -1000768, "FINISH THE SPELL, STAGHELM! I CANNOT HOLD THE GLYPHS OF WARDING IN PLACE MUCH LONGER! CALL FORTH THE ROOTS!", 0 ),
( -1000770, "Ancient ones guide my hand... Wake from your slumber! WAKE AND SEAL THIS CURSED PLACE!", 0 ),
( -1000771, "%s falls to one knee - exhausted.", 2 ),
( -1000772, "It... It is over, Lord Staghelm. We are victorious. Albeit the cost for this victory was great.", 0 ),
( -1000773, "There is but one duty that remains...", 0 ),
( -1000774, "Before I leave this place, I make one final offering to you, Lord Staghelm. Should a time arise in which you must gain entry to this accursed fortress use the Scepter of the Shifting Sands on the sacred gong. The magic holding the barrier together will dissipate and the horrors of Ahn'Qiraj will be unleashed upon the world once more.", 0 ),
( -1000775, "%s hands the Scepter of the Shifting Sands to Fandral Staghelm.", 2 ),
( -1000776, "After the savagery that my people have witnessed and felt, you expect me to accept another burden, dragen? Surely your are mad.", 0 ),
( -1000777, "I want nothing to do with Silithus, the Qiraj and least of all, any damned dragons!", 0 ),
( -1000778, "%s hurls the Scepter of the Shifting Sands into the barrier, shattering it.", 2 ),
( -1000779, "Lord Staghelm, where are you going? You would shatter our bond for the sake of pride?", 0 ),
( -1000780, "My son's soul will find no comfort in this hollow victory, dragen. I will have him back. Though it takes a millennia. I WILL have my son back!", 0 ),
( -1000781, "%s shakes his head in disappointment.", 2 ),
( -1000782, "%s kneels down to pick up the fragments of the shattered scepter.", 2 ),
( -1000783, "And now you know all there is to know, mortal...", 0 );


UPDATE creature_template SET faction_A = 16, faction_H = 16 WHERE entry IN(15414, 15421, 15422, 15424);
UPDATE creature_template SET minhealth = 100000, maxhealth = 100000, faction_A = 1608, faction_H = 1608 WHERE entry = 15423;
UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry IN(15378, 15379, 15380, 15381);

INSERT INTO scripted_event_id VALUES
( 9425, "AQ_SUMMON_GLYPHS" ),
( 9426, "AQ_SUMMON_ROOTS" ),
( 9427, "AQ_SUMMON_DOOR" ),
( 9527, "AQ_PLACE_LOOT" ),
( 9542, "AQ_SUMMON_MAW" );

INSERT INTO npc_text (ID, text0_0) VALUES
( 90200, "You seek the scepter shard charged to the protection of the Blue Flight?$B$B<Azuregos appears to be smiling.>" ),
( 90201, "Of course I do not have the shard. That would be asinine. I walk amongst savages in this cursed land! I cannot take five steps without some crazed orc or human trying to thrust a sharp stick into my hide. At any given time you could very well be speaking to my ghostly spirit.$B$BAnyhow... I hid it.$B$BGo on... ask me where." ),
( 90202, "You are going to love this story, $R. See, here is the thing. As soon as Malygos charged me with protecting the scepter shard, I knew I was in for an eternity of headache and pain.$B$BAll I want to do is study these magic rich shorelines and take in what artifacts might remain from the Sundering. You know, there was once a thriving Highborne civilization here! You don't have to tell me that there is going to be some solid magical items around here, $R. I can smell it!" ),
( 90203, "As I was saying, I held on to that scepter shard for a good five-hundred years and it was nothing but trouble. It attracted all kinds of attention - the wrong kind of attention. I want to be left alone to my studies not babysit some would be hero's ticket to glory. So... I gave it to the fish." ),
( 90204, "Not just any fish, mortal. This fish was a minnnow. A very special minnow" ),
( 90205, "Exactly!" ),
( 90206, "Genious, I know... It will never be found; but, in case it is found, I gave the minnow some special powers." ),
( 90207, "Correct. You've been listening! This minnow is really quite a terrible creature to of my own creation. When I am relaxing I like to think of the unlucky fishermen who have run into this minnow and been devoured whole. I bet they were quite surprised!$B$B<Azuregos laughs.>" ),
( 90208, "Genius is often misunderstood, $R. Now, about the scepter shard... If you want that shard and do not wish to spend the next ten thousand years searching for it, you are going to have to listen very carefully." ),
( 90209, "Two words: Arcanite buoy." ),
( 90210, "Right then... You have to build an arcanite buoy and place it in the ocean. My minnow will be attracted to the magical emanations. When he gets near the buoy - BLAMMO! It will explode in a glorious pulse of arcane energy, revealing the minnow's true form. Also, you may or may not incur the wrath of Neptulon. A 50/50 chance I would say." ),
( 90211, "If i did not know better I would think that you were mocking me, mortal; but yes, that is mostly correct. You may remain fully clothed." ),
( 90212, "Take this ledger to an old acquaintance of mine in Tanaris. His name is Narain Soothfancy - terrible, terrible psychic but an amazing engineer! He should be able to make sense of it all.$BNo need to thank me, $N. It's the least I could do.$B$BGood day!" ),
( 90213, "Shouldn't you be saving the world?" ),
( 90214, "It... It's foggy, $N. I can't see anything! Wait... Wait a minute. I see... I see Doctor Weavil's hideout. It... YES! I believe he is holding a chapter of the book!$BLet me see if I can zoom this thing out to get a better vantage point.$B$B<Narain appears to be going cross-eyed>$B$BAlcaz Island! That devious bastard is on Alcaz Island!" );


INSERT INTO script_texts (entry, content_default) VALUES
( -1540200, "How did you know? I mean, yes... Yes I am looking for that shard. Do you have it?" ),
( -1540201, "Alright. Where?" ),
( -1540202, "By Bronzebeard's... um, beard! What are you talking about?" ),
( -1540203, "Fish? You gave a piece of what could be the key to saving all life on Kalimdor to a fish?" ),
( -1540204, "A minnow? The oceans are filled with minnows! There could be a hundred million million minnows out there!" ),
( -1540205, "..." ),
( -1540206, "You put the piece on a minnow and placed the minnow somewhere in the waters of the sea between here and the Eastern Kingdoms? And this minnow has special powers?" ),
( -1540207, "You're insane." ),
( -1540208, "I'm all ears." ),
( -1540209, "Come again." ),
( -1540210, "Ok, let me get this straight. You put the scepter entrusted to your Flight by Anachronos on a minnow of your own making and now you expect me to build an... an arcanite buoy or something... to force your minnow out of hiding? AND potentially incur the wrath of an Elemental Lord? Did I miss anything? Perhaps I am to do this without any clothes on, during a solar eclipse, on a leap year?" ),
( -1540211, "FINE! And how, dare I ask, am i supposed to acquire an arcanite buoy?" ),
( -1540212, "But..." );

UPDATE creature_template SET ScriptName = "AQ_OPENING_AZUREGOS" WHERE entry = 15481;
UPDATE creature_template SET ScriptName = "npc_aq_opening_siren" WHERE entry = 15526;

UPDATE gameobject_template SET displayId = 6484, name = "Tasche voller Gold" WHERE entry = 180660;

INSERT INTO gameobject_template (entry, type, displayId, name, data0, data1) VALUES
( 901000, 8, 0, "Sammelpunkt", 1348, 5 );

INSERT INTO gameobject (id, map, position_x, position_y, position_z) VALUES
( 901000, 1, 5084.37, -5116.33, 931.06 );

DELETE FROM event_scripts WHERE id = 9527;


INSERT INTO script_texts (entry, content_default, type, emote) VALUES
( -1901020, "No hello for your old friend, Narain? Who were you expecting???", 0, 25 ),
( -1901021, "So... You thought you could fool me, did you? The greatest criminal mastermind Azeroth has ever known???", 0, 1 ),
( -1901022, "I see right through your disguise. Number Two! Number Two kill!", 0, 1 );

UPDATE creature_template SET ScriptName = "npc_aq_weavil_flying_machine" WHERE entry = 15553;
UPDATE creature_model_info SET combat_reach = 3 WHERE modelid = 10133;

INSERT INTO creature_ai_scripts (creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(  '28125',  '0',  '0', '100', '1','5000',  '7000',  '5000',  '7000','11',  '28125', '0', '0','0', '', '', '','0', '', '', '','' );

UPDATE creature_template SET AIName = "EventAI" WHERE entry = 15554;

UPDATE creature_template SET ScriptName = "npc_aq_narain" WHERE entry = 11811;

REPLACE INTO gameobject VALUES
( 45065, 180666, 0, -8332.34, 403.836, 124.478, -0.401426, 0, 0, 0.199368, -0.979925, 900, 100, 1 ),
( 11111625, 180669, 1, 3542.150879, -6585.05564, 0, 0, 0, 0, 0, 0, 0, 0, 1 );

UPDATE quest_template SET NextQuestID = 8728 WHERE entry IN (8587, 8578, 8620);

UPDATE creature_template SET InhabitType = 7, unit_flags = 32768, flags_extra = 0, ScriptName = "aq_opening_maws" WHERE entry = 15571;
UPDATE creature_model_info SET combat_reach = 4 WHERE modelid = 15555;
UPDATE creature_loot_template SET ChanceOrQuestChance = -100 WHERE item = 21137;