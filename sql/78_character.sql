/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : chars

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2014-03-04 21:34:26
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `character_bosskill`
-- ----------------------------
DROP TABLE IF EXISTS `character_bosskill`;
CREATE TABLE `character_bosskill` (
  `guid` int(11) NOT NULL,
  `raid` tinyint(1) NOT NULL,
  `boss` int(6) NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of character_bosskill
-- ----------------------------
INSERT INTO `character_bosskill` VALUES ('4', '1', '1', '2014-02-28 14:21:24');
INSERT INTO `character_bosskill` VALUES ('4', '1', '16', '2014-02-28 14:40:26');
INSERT INTO `character_bosskill` VALUES ('4', '7', '32', '2014-02-28 14:40:30');
INSERT INTO `character_bosskill` VALUES ('4', '3', '16', '2014-02-28 14:45:52');
INSERT INTO `character_bosskill` VALUES ('4', '8', '1', '2014-02-28 14:51:59');
INSERT INTO `character_bosskill` VALUES ('4', '8', '4', '2014-02-28 14:52:25');
INSERT INTO `character_bosskill` VALUES ('4', '8', '8', '2014-02-28 14:53:10');
INSERT INTO `character_bosskill` VALUES ('4', '8', '16', '2014-02-28 14:53:43');
INSERT INTO `character_bosskill` VALUES ('4', '8', '32', '2014-02-28 14:55:55');
INSERT INTO `character_bosskill` VALUES ('4', '8', '2', '2014-02-28 14:56:41');
INSERT INTO `character_bosskill` VALUES ('49', '2', '1', '2014-03-04 01:59:32');

-- ----------------------------
-- Table structure for `character_destroyeditem`
-- ----------------------------
DROP TABLE IF EXISTS `character_destroyeditem`;
CREATE TABLE `character_destroyeditem` (
  `guid` int(11) NOT NULL,
  `itemId` int(11) NOT NULL,
  `amount` int(7) NOT NULL,
  `action` tinyint(4) NOT NULL,
  `date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of character_destroyeditem
-- ----------------------------
INSERT INTO `character_destroyeditem` VALUES ('4', '8952', '20', '2', '2014-02-28 14:19:00');
INSERT INTO `character_destroyeditem` VALUES ('42', '2512', '1', '3', '2014-03-04 21:06:12');

-- ----------------------------
-- Table structure for `character_receiveditem`
-- ----------------------------
DROP TABLE IF EXISTS `character_receiveditem`;
CREATE TABLE `character_receiveditem` (
  `guid` int(11) NOT NULL,
  `itemId` int(11) NOT NULL,
  `amount` int(6) NOT NULL,
  `date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of character_receiveditem
-- ----------------------------
INSERT INTO `character_receiveditem` VALUES ('12', '18808', '1', '2014-02-28 14:22:45');
INSERT INTO `character_receiveditem` VALUES ('51', '18970', '2', '2014-03-04 06:42:13');
INSERT INTO `character_receiveditem` VALUES ('51', '19388', '1', '2014-03-04 07:01:47');
INSERT INTO `character_receiveditem` VALUES ('51', '19438', '1', '2014-03-04 07:02:40');
INSERT INTO `character_receiveditem` VALUES ('51', '19683', '1', '2014-03-04 07:04:10');
