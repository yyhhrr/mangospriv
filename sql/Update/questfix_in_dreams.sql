
-- Taelan more DPS
UPDATE creature_template SET mindmg = 168, maxdmg = 234, RegenHealth = 0, mechanic_immune_mask = 2048, armor = 5500 WHERE entry = 1842;

UPDATE creature_template SET AIname = 'AggressorAI' WHERE entry = 12128;
UPDATE creature_template SET flags_extra = 0 WHERE entry = 1840;


DELETE FROM script_texts WHERE entry BETWEEN -1001105 AND -1001072;

-- Texte auf deutsch/englisch
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc3`, `sound`, `type`, `language`, `emote`, `comment`) 
VALUES
('-1001105','This I promise... This I vow...','Ich verspreche es... Dies ist mein Schwur...','0','0','0','0','tirion fordring SAY_EPILOG_5'),
('-1001104','Your death will not have been in vain, Taelan. A new Order is born on this day... an Order which will dedicate itself to extinguising the evil that plagues this world. An evil that cannot hide behind politics and pleasantries.','Dein Tod wird nicht umsonst gewesen sein, Taelan. Ein neuer Orden wird an diesem Tag gegründet… ein Orden, der sich dazu verschreibt, das Böse der Welt zu vertreiben. Das Böse kann sich nicht mehr länger hinter Politik und Höflichkeiten verstecken.','0','0','0','0','tirion fordring SAY_EPILOG_4'),
('-1001103','Too long have I sat idle, gripped in this haze... this malaise, lamenting what could have been... what should have been.','Zu lange habe ich gewartet, gehadert…darüber geklagt, was hätte geschehen können… was hätte geschehen sollen.','0','0','0','0','tirion fordring SAY_EPILOG_3'),
('-1001102','%s holds the limp body of Taelan Fordring and softly sobs.','Lord Tirion Fordring hält den regungslosen Körper Taelan Fordrings und schluchzt leise.','0','2','0','0','tirion fordring EMOTE_HOLD_TAELAN'),
('-1001101','Look what they did to my boy.','Seht, was sie mit meinem Jungen gemacht haben.','0','0','0','0','tirion fordring SAY_EPILOG_2'),
('-1001100','%s falls to one knee.','%s fällt auf die Knie.','0','2','0','16','tirion fordring EMOTE_FALL_KNEE'),
('-1001099','A thousand more like him exist. Ten thousand. Should one fall, another will rise to take the seat of power.','Es gibt noch so viele mehr. Tausende mehr. Wenn einer fällt, erhebt sich ein anderer um die Macht zu ergreifen.','0','0','0','0','tirion fordring SAY_EPILOG_1'),
('-1001098','Then come, hermit!','Kommt nur, Einsiedler!','0','0','0','0','isillien SAY_TIRION_5'),
('-1001097','Face me, coward. Face the faith and strength that you once embodied.','Feigling, seht mich an. Stellt euch dem Glauben und der Stärke, die Ihr selbst einst selbst verkörpert habt.','0','0','0','0','tirion fordring SAY_TIRION_4'),
('-1001096','May your soul burn in anguish, Isillien! Light give me strength to battle this fiend.','Möge Eure Seele vor Angst brennen, Isillien! Licht, gib mir Kraft, um diesen Unhold zu bekämpfen.','0','0','0','0','tirion fordring SAY_TIRION_3'),
('-1001095','Tragic. The elder Fordring lives on... You are too late, old man. Retreat back to your cave, hermit, unless you wish to join your son in the Twisting Nether.','Wie tragisch. Der alte Fordring lebt noch…Ihr seid zu spät, alter Mann. Kehrt in euer Loch zurück, Einsiedler, oder wünscht Ihr euerm Sohn im wirbelnden Nether Gesellschaft zu leisten.','0','0','0','0','isillien SAY_TIRION_2'),
('-1001094','What have you done, Isillien? You once fought with honor, for the good of our people... and now... you have murdered my boy...','Was habt Ihr getan, Isillien? Früher habt ihr mit Ehre gekämpft, für das Wohl unserer Völker….und jetzt…habt Ihr meinen Sohn ermordet…','0','0','0','0','tirion fordring SAY_TIRION_1'),
('-1001093','% turns his attention towards you.','%s wendet sich Euch zu.','0','2','0','0','isillien EMOTE_ATTACK_PLAYER'),
('-1001092','Did you really believe that you could defeat me? Your friends are soon to join you, Taelan.','Habt Ihr wirklich gedacht, Ihr könntet mich besiegen? Eure Freunde werden euch schon bald folgen, Taelan.','0','0','0','0','isillien SAY_KILL_TAELAN_2'),
('-1001091','%s laughs.','%s lacht.','0','2','0','11','isillien EMOTE_ISILLIEN_LAUGH'),
('-1001090','Enough!','Genug!','0','0','0','0','isillien SAY_KILL_TAELAN_1'),
('-1001089','The end is now, Fordring.','Das Ende naht, Fordring.','0','0','0','1','isillien SAY_ISILLIEN_ATTACK'),
('-1001088','%s calls for his guardsman.','%s ruft seine Wachen.','0','2','0','0','isillien EMOTE_ISILLIEN_ATTACK'),
('-1001087','The Grand Crusader has charged me with destroying you and your newfound friends, Taelan, but know this: I do this for pleasure, not of obligation or duty.','Der Oberste Kreuzfahrer hat mich damit beauftraugt, eure neugewonnenen Freunde und Euch zu töten, Taelan, aber wisset dies: Ich mache dies mit Freude, nicht weil es mir befohlen wurde oder meine Pflicht ist.','0','0','0','1','isillien SAY_ISILLIEN_6'),
('-1001086','It is as they say: Like father, like son. You are as weak of will as Tirion... perhaps more so. I can only hope my assassins finally succeeded in ending his pitiful life.','Es ist wie man sagt: Wie der Vater, so der Sohn. Ihr seid willensschwach wie Tirion…vielleicht noch mehr. Ich kann nur hoffen, dass meine Assassinen endlich sein armseliges Leben beendet haben.','0','0','0','1','isillien SAY_ISILLIEN_5'),
('-1001085','You disappoint me, Taelan. I had plans for you... grand plans. Alas, it was only a matter of time before your filthy bloodline would catch up with you.','Ihr habt mich enttäuscht, Taelan. Ich hatte Pläne für euch… große Pläne. Leider war es nur eine Frage der Zeit bis eure schmutzige Blutlinie euch einholen würde.','0','0','0','1','isillien SAY_ISILLIEN_4'),
('-1001084','This is not your fight, stranger. Protect yourself from the attacks of the Crimson Elite. I shall battle the Grand Inquisitor.','Dies ist nicht euer Kampf, Fremder. Schützt euch vor den Angriffen der Scharlachroten Elitekämpfern. Ich kümmer mich um den Großinquisitor.','0','0','0','1','taelan fordring SAY_ISILLIEN_3'),
('-1001083','Isillien!','Isillien!','0','1','0','25','taelan fordring SAY_ISILLIEN_2'),
('-1001082','You will not make it to the forest\'s edge, Fordring.','Ihr werdet es nicht bis zum Waldrand schaffen, Fordring.','0','0','0','1','isillien SAY_ISILLIEN_1'),
('-1001081','It\'s not much further. The main road is just up ahead.','Es ist nicht mehr weit. Die Hauptstraße ist gleich vorraus.','0','0','0','1','taelan fordring SAY_REACH_TOWER'),
('-1001080','%s calls for his mount.','%s ruft nach seinem Pferd.','0','2','0','22','taelan fordring EMOTE_TAELAN_MOUNT'),
('-1001079','Halt.','Halt.','0','0','0','0','taelan fordring SAY_TAELAN_MOUNT'),
('-1001078','Remove your disguise, lest you feel the bite of my blade when the fury has taken control.','Entfernt eure Verkleidung, damit Ihr nicht meine Klinge zu spüren bekommt, wenn mein Blutdurst die Kontrolle übernimmt.','0','0','0','1','taelan fordring SAY_ESCORT_START'),
('-1001077','I will lead us through Hearthglen to the forest\'s edge. From there, you will take me to my father.','Ich werde uns durch Hearthglen bis zum Waldrand führen. Von dort bringt Ihr mich zu meinem Vater.','0','0','0','1','taelan fordring SAY_SCARLET_COMPLETE_2'),
('-1001076','NOOOOOOOOOOOOO!','NEEEEEEIIIIIIIN!','0','1','0','15','taelan fordring SAY_SCARLET_COMPLETE_1'),
('-1001075','Sir?','Herr?','0','0','0','1','scarlet cavalier SAY_CAVALIER_WORRY_4'),
('-1001074','What the....','Was zur ...','0','0','0','1','scarlet cavalier SAY_CAVALIER_WORRY_3'),
('-1001073','Hey, what is going on over there? Sir, are you alright?','Hey, was ist hier los? Herr, geht es euch gut?','0','0','0','1','scarlet cavalier SAY_CAVALIER_WORRY_2'),
('-1001072','Something is wrong with the Highlord. Do something!','Etwas stimmt mit dem Hochlord nicht. Unternehmmt etwas!','0','0','0','1','scarlet cavalier SAY_CAVALIER_WORRY_1');
