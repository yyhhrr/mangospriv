INSERT INTO creature_template (entry, modelid_1, name, subname, minlevel, maxlevel) VALUES
( 90001, 11686, "Scharlachroter Kommandant Mograine", "Trigger", 1, 1 );

INSERT INTO creature ( id, map, position_x, position_y, position_z) VALUES
( 90001, 189, 855, 1321, 2 );

UPDATE creature_template SET ScriptName = "highlord_mograine_event", name = "Hochlord Mograine", subname = "Der Aschenbringer", minlevel = 60, maxlevel = 60, rank = 3 WHERE entry = 16440;

INSERT INTO script_texts (entry, content_default) VALUES
( -1540000, "Bow down! Kneel before the Ashbringer! A new dawn approaches, brothers and sisters! Our message will be delivered to the filth of this world through the chosen one!" ),
( -1540001, "And so it begins..." ),
( -1540002, "Kneel! Kneel before the Ashbringer!" ),
( -1540003, "Have you come to save this world? To cleanse it?" ),
( -1540004, "Take me with you, sir." ),
( -1540005, "I am unworthy, sir." ),
( -1540006, "You hold my father's blade, $N. My soldiers are yours to control, my Lord. Take them... Lead them... The impure must be purged. They must be cleansed of their taint." ),
( -1540007, "Renault..." ),
( -1540008, "Father... But... How?" ),
( -1540009, "Did you think that your betrayal would be forgotten? Lost in the carefully planned cover up of my death? Blood of my blood, the blade felt your cruelty long after my heart had stopped beating. And in death, I knew what you had done. But now, the chains of Kel'Thuzad hold me no more. I come to serve justice. I AM ASHBRINGER." ),
( -1540010, "Forgive me, father! Please..." ),
( -1540011, "You are forgiven..." );

INSERT INTO npc_text (ID, text0_0) VALUES
( 90010, "At last, the curse is lifted. Thank you, hero." ),
( 90011, "You mean, you don't know? The sword that you carry on your back - it is known as Ashbringer; named after its original owner." ),
( 90012, "Aye, the Highlord Mograine: A founder of the original order of the Scarlet Crusade. A knight of unwavering faith and purity; Mograine would be betrayed by his own son and slain by Kel'Thuzad's forces inside Stratholme. It is how I ended up here..." ),
( 90013, "It was High General Abbendis, High Inquisitor Isillien, and Highlord Mograine that formed the Crusade. In its infancy, the Crusade was a noble order. The madness and insane zealotry that you see now did not exist. It was not until the one known as Grand Crusader appeared that the wheels of corruption were set in motion." ),
( 90014, "The Highlord was the lynchpin of the Crusade. Aye, Mograine was called the Ashbringer because of his exploits versus the armies of the Lich King. With only blade and faith, Mograine would walk into whole battalions of undead and emerge unscathed - the ashes of his foes being the only indication that he had been there at all. Do you not understand? The very face of death feared him! It trembled in his presence!" ),
( 90015, "The only way a hero can die, $R: Through tragedy. The Grand Crusader struck a deal with Kel'Thuzad himself! An ambush would be staged that would result in the death of Mograine. The type of betrayal that could only be a result of the actions of one's most trusted and loved companions." ),
( 90016, "<High Inquisitor Fairbanks nods.>$BAye, the lesser Mograine, the one known as the Scarlet Commander, through - what I suspect - the dealings of the Grand Crusader. He led his father to the ambush like a lamb to the slaughter." ),
( 90017, "<High Inquisitor Fairbanks lifts up his tabard revealing several gruesome scars.>$BBecause I was there... I was the Highlord's most trusted advisor. I should have known... I felt that something was amiss yet I allowed it to happen. Would you believe that there were a thousand or more Scourge?" ),
( 90018, "This was the Ashbringer, fool! As the Scourge began to materialize around us, Mograine's blade began to glow... to hum... the younger Mograine would take that as a sign to make his escape. They descended upon us with a hunger the likes of which I had never seen. Yet..." ),
( 90019, "It was not enough.$B<Fairbanks smirks briefly, lost in a memory.>$BA thousand came and a thousand died. By the Light! By the might of Mograine! He would smite them down as fast as they could come. Through the chaos, I noticed that the lesser Mograine was still there, off in the distance. I called him, \"Help us, Renault! Help your father, boy!\"" ),
( 90020, "<High Inquisitor Fairbanks shakes his head.>$BNo... He stood in the background, watching as the legion of undead descended upon us. Soon after, my powers were exhausted. I was the first to fall... Surely they would tear me limb from limb as I lay there unconscious; but they ignored me completely, focusing all of their attention on the Highlord." ),
( 90021, "It was all I could do to feign death as the corpses of the Scourge piled upon me. There was darkness and only the muffled sounds of the battle above me. The clashing of iron, the gnashing and grinding... gruesome, terrible sounds. And then there was silence. He called to me! \"Fairbanks! Fairbanks, where are you? Talk to me, Fairbanks!\" And then came the sound of incredulousness. The bite of betrayal, $R." ),
( 90022, "The boy had picked up Ashbringer and driven it through his father's heart as his back was turned. His last words will haunt me forever: \"What have you done, Renault? Why would you do this?\"" ),
( 90023, "The blade and Mograine were a singular entity. Do you understand? This act corrupted the blade and lead to Mograine's own corruption as a death knight of Kel'Thuzad. I swore that if I lived, I would expose the perpetrators of this heinous crime. For two days I remained under the rot and contagion of Scourge - gathering as much strength as possible to escape the razed city." ),
( 90024, "Aye, I did. Much to the dismay of the lesser Mograine, I made my way back to the Scarlet Monastery. I shouted and screamed. I told the tale to any that would listen. And I would be murdered in cold blood for my actions, dragged to this chamber - the dark secret of the order. But some did listen... some heard my words. Thus was born the Argent Dawn..." ),
( 90025, "I'm afraid that the blade which you hold in your hands is beyond saving. The hatred runs too deep. But do not lose hope, $C. Where one chapter has ended, a new one begins.$BFind his son - a more devout and pious man you may never meet. It is rumored that he is able to build the Ashbringer anew, without requiring the old, tainted blade." ),
( 90026, "<High Inquisitor Fairbanks shakes his head.>$BNo, $R; only one of his sons is dead. The other lives...$B<High Inquisitor Fairbanks points to the sky.>$BThe Outland... Find him there..." );


INSERT INTO gossip_texts (entry, content_default) VALUES
( -3000200, "Curse? What's going on here, Fairbanks?" ),
( -3000201, "Mograine?" ),
( -3000202, "What do you mean?" ),
( -3000203, "I still do not fully understand." ),
( -3000204, "Incredible story. So how did he die?" ),
( -3000205, "You mean..." ),
( -3000206, "How do you know all of this?" ),
( -3000207, "A thousand? For one man?" ),
( -3000208, "Yet? Yet what??" ),
( -3000209, "And did he?"  ),
( -3000210, "Continue please, Fairbanks." ),
( -3000211, "You mean..." ),
( -3000212, "You were right, Fairbanks. That is tragic." ),
( -3000213, "And you did..." ),
( -3000214, "You tell an incredible tale, Fairbanks. What of the blade? Is it beyond redemption?" ),
( -3000215, "But his son is dead." );

UPDATE creature_template SET ScriptName = "cathedral_trash_ai" WHERE
entry = 4295 OR entry = 4294 OR entry = 4300 OR entry = 4298 OR
entry = 4301 OR entry = 4299 OR entry = 4540 OR entry = 4302 OR
entry = 4303;


UPDATE creature_template SET npcflag = 1, unit_flags = 557120 WHERE entry = 4542;