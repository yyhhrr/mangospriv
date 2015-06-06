/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : chars

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2014-10-21 12:40:33
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `gameobject_instance`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_instance`;
CREATE TABLE `gameobject_instance` (
  `instanceID` int(11) NOT NULL,
  `guid` int(10) NOT NULL,
  `state` int(11) NOT NULL,
  PRIMARY KEY (`instanceID`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of gameobject_instance
-- ----------------------------
