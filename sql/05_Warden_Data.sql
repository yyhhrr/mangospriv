/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : auth_test

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2013-02-19 17:41:06
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `warden_data_result`
-- ----------------------------
DROP TABLE IF EXISTS `warden_data_result`;
CREATE TABLE `warden_data_result` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `check` int(3) DEFAULT NULL,
  `data` tinytext,
  `str` tinytext,
  `address` int(8) DEFAULT NULL,
  `length` int(2) DEFAULT NULL,
  `result` tinytext,
  `comment` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1010 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of warden_data_result
-- ----------------------------
INSERT INTO `warden_data_result` VALUES ('1', '243', null, null, '8151565', '1', '20', 'Airswim');
INSERT INTO `warden_data_result` VALUES ('2', '243', null, null, '8445948', '4', 'BB8D243F', 'Wallclimb');
INSERT INTO `warden_data_result` VALUES ('3', '243', null, null, '8151657', '4', '488C11C1', 'Swim Gravity');
INSERT INTO `warden_data_result` VALUES ('4', '243', null, null, '8151666', '4', 'D893FEC0', 'Jump Gravity');
INSERT INTO `warden_data_result` VALUES ('5', '243', null, null, '8151646', '1', '30', 'Infinite Jump');
INSERT INTO `warden_data_result` VALUES ('6', '243', null, null, '6382555', '2', '8A47', 'Anti Root');
INSERT INTO `warden_data_result` VALUES ('7', '243', null, null, '6380789', '1', 'F8', 'Anti Move');
INSERT INTO `warden_data_result` VALUES ('8', '243', null, null, '8151647', '1', '75', 'Anti Jump');
INSERT INTO `warden_data_result` VALUES ('9', '243', null, null, '8152026', '3', '8B4F78', 'No Falldamage');
INSERT INTO `warden_data_result` VALUES ('10', '243', null, null, '6504892', '2', '7425', 'Infinite Fly');
