UPDATE creature_template SET ScriptName = "npc_blizzcon_guy" WHERE
entry = 16076 OR
entry = 7951 OR
entry = 16070 OR
entry = 2943;


DROP TABLE IF EXISTS `premium_key`;
CREATE TABLE `premium_key`
(
  `id` int NOT NULL AUTO_INCREMENT,
  `keycode`  VARCHAR(20) NOT NULL DEFAULT '',
  `used`  tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`, `keycode`)
);

DROP TABLE IF EXISTS `premium_item`;
CREATE TABLE `premium_item`
(
  `id` int NOT NULL AUTO_INCREMENT,
  `itemid`  int NOT NULL DEFAULT '0',
  `name`  VARCHAR(20) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`, `itemid`)
);

INSERT INTO premium_item (itemid, name) VALUES
( 20371, 'Blaues Murlocei' ),
( 13584, 'Diablostein' ),
( 13582, 'Zerglingslasso' ),
( 13583, 'Pandahalsband' );