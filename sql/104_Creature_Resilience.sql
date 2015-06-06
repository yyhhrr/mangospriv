CREATE TABLE `creature_template_resilience` (
  `entry` int(11) unsigned NOT NULL,
  `resilience` int(2) unsigned DEFAULT '0',
  `comment` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8