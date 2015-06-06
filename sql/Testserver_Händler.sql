INSERT INTO creature_template (entry, modelid_1, modelid_2, name, subname, minlevel, maxlevel, minhealth, maxhealth, faction_A, faction_H, npcflag, scale) VALUES
( 800020, 6882, 7335, "Buffzeug", "Testserver Händler", 60, 60, 10000, 10000, 35, 35, 16390, 2 ),
( 800021, 6882, 7335, "Sonstiges", "Testserver Händler", 60, 60, 10000, 10000, 35, 35, 16390, 2 ),
( 800022, 6882, 7335, "Tränke & Verbände", "Testserver Händler", 60, 60, 10000, 10000, 35, 35, 16390, 2 );

INSERT INTO npc_vendor (entry, item) VALUES
( 800020, 9206 ), -- Elixier der Riesen
( 800020, 13445 ), -- Elixier der überragenden Verteidigung
( 800020, 13447 ), -- Elixier der Weisen
( 800020, 13453 ), -- Elixier der rohen Gewalt
( 800020, 13452 ), -- Elixier des Mungos
( 800020,  9264 ), -- Elixier der Schattenmacht
( 800020, 21546 ), -- Elixier der großen Feuermacht
( 800020, 13511 ), -- Fläschchen mit destillierter Weisheit
( 800020, 13512 ), -- Fläschchen der obersten Macht
( 800020, 13510 ), -- Fläschchen der Titanen
( 800020, 13454 ), -- Großes Arkanelixier
( 800020, 13461 ), -- Großer Arkanschutztrank
( 800020, 13457 ), -- Großer Feuerschutztrank
( 800020, 13456 ), -- Großer Frostschutztrank
( 800020, 13458 ), -- Großer Naturschutztrank
( 800020, 13459 ), -- Großer Schattenschutztrank
( 800020, 20750 ), -- Zauberöl
( 800020, 18262 ), -- Elementarwetzstein

( 800021, 15138 ), -- Onyxiaschuppenumhang
( 800021, 25001 ), -- Zahlschein
( 800021, 17966 ), -- Rucksack aus Onyxias Haut
( 800021, 21342 ), -- Kernteufelsstofftasche
( 800021,  6265 ), -- Seelensplitter
( 800021, 18232 ), -- Feldreparaturbot
( 800021, 15997 ), -- Thoriumpatronen
( 800021, 18042 ), -- Thoriumpfeile
( 800021,  2662 ), -- Ribblys Köcher
( 800021,  2663 ), -- Ribblys Schultergürtel

( 800022, 13444 ), -- Erheblicher Manatrank
( 800022, 13446 ), -- Erheblicher Heiltrank
( 800022, 13445 ), -- Elixier der überragenden Verteidigung
( 800022, 18253 ), -- Erheblicher Verjüngungstrank
( 800022, 20002 ), -- Großer Trank des traumlosen Schlafs
( 800022, 20008 ), -- Trank der lebhaften Aktion
( 800022, 20004 ), -- Erheblicher Trollbluttrank
( 800022, 20007 ), -- Magierbluttrank
( 800022, 13462 ), -- Läuterungstrank
( 800022, 13442 ), -- Mächtiger Wuttrank
( 800022, 14530 ); -- Runenstoffverband


INSERT INTO item_template (entry, name, displayid, Quality, SellPrice, description) VALUES
( 25001, "Zahlschein", 30658, 4, 500000, "Verkaufe mich!" );