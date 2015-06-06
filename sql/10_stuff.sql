/*
	Ich mag die Idee hier hinter. Vielleicht sollte man sowas als Spenderitem einführen?
	Muss ja nicht unbedingt das hier sein, es können ja die Mounts sein, die mit 1.4
	unverkäuflich gemacht wurden. Siehe: http://www.wowpedia.org/Removed_mounts
*/

INSERT INTO item_template (entry, class, subclass, name, displayid, quality, flags, spellid_1, spellcooldown_1, spellcategory_1, spellcategorycooldown_1, bonding) VALUES
( 25000, 15, 0, "Nights Testmount", 31434, 5, 64, 24576, 1, 1, 1, 1);

INSERT INTO creature_template (entry, modelid_1, name, minlevel, maxlevel, minhealth, maxhealth, faction_A, faction_H, type) VALUES
( 15135, 8318, "Nights Testmount", 1, 1, 1, 1, 35, 35, 6);


/* Hab mich beim letzten Patch mit der Fraktion der scharlachroten NPCs gerirrt. Damit sollte es stimmen */
UPDATE creature_template SET faction_a = 67, faction_h = 67 WHERE
ENTRY = 575 OR ENTRY = 1506 OR ENTRY = 1507 OR ENTRY = 1535 OR
ENTRY = 1536 OR ENTRY = 1537 OR ENTRY = 1538 OR ENTRY = 1539 OR
ENTRY = 1540 OR ENTRY = 1660 OR ENTRY = 1664 OR ENTRY = 1665 OR
ENTRY = 1667 OR ENTRY = 1801 OR ENTRY = 1826 OR ENTRY = 1827 OR
ENTRY = 1831 OR ENTRY = 1832 OR ENTRY = 1833 OR ENTRY = 1834 OR
ENTRY = 1835 OR ENTRY = 1836 OR ENTRY = 1837 OR ENTRY = 1838 OR
ENTRY = 1839 OR ENTRY = 1841 OR ENTRY = 1842 OR ENTRY = 1845 OR
ENTRY = 1846 OR ENTRY = 1883 OR ENTRY = 1884 OR ENTRY = 1885 OR
ENTRY = 1931 OR ENTRY = 1934 OR ENTRY = 1935 OR ENTRY = 1936 OR
ENTRY = 3974 OR ENTRY = 3975 OR ENTRY = 3976 OR ENTRY = 3977 OR
ENTRY = 3983 OR ENTRY = 4280 OR ENTRY = 4281 OR ENTRY = 4282 OR
ENTRY = 4286 OR ENTRY = 4287 OR ENTRY = 4288 OR ENTRY = 4289 OR
ENTRY = 4290 OR ENTRY = 4291 OR ENTRY = 4292 OR ENTRY = 4293 OR
ENTRY = 4294 OR ENTRY = 4295 OR ENTRY = 4296 OR ENTRY = 4297 OR
ENTRY = 4298 OR ENTRY = 4299 OR ENTRY = 4300 OR ENTRY = 4301 OR
ENTRY = 4302 OR ENTRY = 4303 OR ENTRY = 4304 OR ENTRY = 4306 OR
ENTRY = 4493 OR ENTRY = 4494 OR ENTRY = 4540 OR ENTRY = 4542 OR
ENTRY = 5487 OR ENTRY = 6575 OR ENTRY = 7566 OR ENTRY = 9448 OR
ENTRY = 9449 OR ENTRY = 9450 OR ENTRY = 9451 OR ENTRY = 9452 OR
ENTRY = 10605 OR ENTRY = 10608 OR ENTRY = 10828 OR ENTRY = 10943 OR
ENTRY = 10979 OR ENTRY = 11054 OR ENTRY = 11078 OR ENTRY = 11613 OR
ENTRY = 11514 OR ENTRY = 11898 OR ENTRY = 12339 OR ENTRY = 12352 OR
ENTRY = 13158 OR ENTRY = 15162 OR ENTRY = 16114;