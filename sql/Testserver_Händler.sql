INSERT INTO creature_template (entry, modelid_1, modelid_2, name, subname, minlevel, maxlevel, minhealth, maxhealth, faction_A, faction_H, npcflag, scale) VALUES
( 800020, 6882, 7335, "Buffzeug", "Testserver H�ndler", 60, 60, 10000, 10000, 35, 35, 16390, 2 ),
( 800021, 6882, 7335, "Sonstiges", "Testserver H�ndler", 60, 60, 10000, 10000, 35, 35, 16390, 2 ),
( 800022, 6882, 7335, "Tr�nke & Verb�nde", "Testserver H�ndler", 60, 60, 10000, 10000, 35, 35, 16390, 2 );

INSERT INTO npc_vendor (entry, item) VALUES
( 800020, 9206 ), -- Elixier der Riesen
( 800020, 13445 ), -- Elixier der �berragenden Verteidigung
( 800020, 13447 ), -- Elixier der Weisen
( 800020, 13453 ), -- Elixier der rohen Gewalt
( 800020, 13452 ), -- Elixier des Mungos
( 800020,  9264 ), -- Elixier der Schattenmacht
( 800020, 21546 ), -- Elixier der gro�en Feuermacht
( 800020, 13511 ), -- Fl�schchen mit destillierter Weisheit
( 800020, 13512 ), -- Fl�schchen der obersten Macht
( 800020, 13510 ), -- Fl�schchen der Titanen
( 800020, 13454 ), -- Gro�es Arkanelixier
( 800020, 13461 ), -- Gro�er Arkanschutztrank
( 800020, 13457 ), -- Gro�er Feuerschutztrank
( 800020, 13456 ), -- Gro�er Frostschutztrank
( 800020, 13458 ), -- Gro�er Naturschutztrank
( 800020, 13459 ), -- Gro�er Schattenschutztrank
( 800020, 20750 ), -- Zauber�l
( 800020, 18262 ), -- Elementarwetzstein

( 800021, 15138 ), -- Onyxiaschuppenumhang
( 800021, 25001 ), -- Zahlschein
( 800021, 17966 ), -- Rucksack aus Onyxias Haut
( 800021, 21342 ), -- Kernteufelsstofftasche
( 800021,  6265 ), -- Seelensplitter
( 800021, 18232 ), -- Feldreparaturbot
( 800021, 15997 ), -- Thoriumpatronen
( 800021, 18042 ), -- Thoriumpfeile
( 800021,  2662 ), -- Ribblys K�cher
( 800021,  2663 ), -- Ribblys Schulterg�rtel

( 800022, 13444 ), -- Erheblicher Manatrank
( 800022, 13446 ), -- Erheblicher Heiltrank
( 800022, 13445 ), -- Elixier der �berragenden Verteidigung
( 800022, 18253 ), -- Erheblicher Verj�ngungstrank
( 800022, 20002 ), -- Gro�er Trank des traumlosen Schlafs
( 800022, 20008 ), -- Trank der lebhaften Aktion
( 800022, 20004 ), -- Erheblicher Trollbluttrank
( 800022, 20007 ), -- Magierbluttrank
( 800022, 13462 ), -- L�uterungstrank
( 800022, 13442 ), -- M�chtiger Wuttrank
( 800022, 14530 ); -- Runenstoffverband


INSERT INTO item_template (entry, name, displayid, Quality, SellPrice, description) VALUES
( 25001, "Zahlschein", 30658, 4, 500000, "Verkaufe mich!" );