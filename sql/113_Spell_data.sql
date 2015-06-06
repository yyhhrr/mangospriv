# Aegis der Bewahrung
DELETE FROM `spell_bonus_data` WHERE entry=23781;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES ('23781', '0', '0', '0', '0', 'Item - Aegis of Preservation');

# Arcane Shot
UPDATE `spell_bonus_data` SET `direct_bonus` = 0.63 WHERE entry IN(3044, 14281, 14282, 14283, 14284, 14285, 14286, 14287);
# Serpent Sting
UPDATE `spell_bonus_data` SET `dot_bonus` = 0.3 WHERE entry IN(1978, 13549, 13550, 13551, 13552, 13553, 13554, 13555);

# Pala T1 - 8er Bonus
DELETE FROM `spell_proc_event` WHERE entry=21747;
INSERT INTO `spell_proc_event` VALUES (21747,0,0,0,0,0,0,0,0,20,0);
# Knochenmesserschneide
DELETE FROM `spell_proc_event` WHERE entry=21153;
INSERT INTO `spell_proc_event` VALUES (21153,0,0,0,0,0,0,0,0,18,0);

# Hammer of Wrath
DELETE FROM `spell_bonus_data` WHERE entry IN(24239, 24274, 24275);
INSERT INTO `spell_bonus_data` VALUES 
(24239, 0.43, 0, 0, 0, 'Paladin - Hammer of Wrath Rank 3'),
(24274, 0.43, 0, 0, 0, 'Paladin - Hammer of Wrath Rank 2'),
(24275, 0.43, 0, 0, 0, 'Paladin - Hammer of Wrath Rank 1');

# Garrote
DELETE FROM `spell_bonus_data` WHERE entry= 703; # IN(703, 8631, 8632, 8633, 11289, 11290);
INSERT INTO `spell_bonus_data` VALUES 
(703, 0, 0, 0, 0.03, 'Rogue');

# Benediction
UPDATE `spell_affect` SET SpellFamilyMask = 176161280 WHERE entry >= 20101 AND entry <= 20105;

# Pyroclasm
DELETE FROM `spell_proc_event` WHERE entry IN(18073, 18096);
INSERT INTO `spell_proc_event` VALUES (18096, 0, 0, 549755813984, 549755813984, 549755813984, 327680, 0, 0, 0, 0);