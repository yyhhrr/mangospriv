/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : zp_characters

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2013-08-04 09:52:33
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `harvey`
-- ----------------------------
DROP TABLE IF EXISTS `harvey`;
CREATE TABLE `harvey` (
  `id` int(9) NOT NULL AUTO_INCREMENT,
  `type` int(2) DEFAULT '0',
  `uuid` bigint(20) NOT NULL,
  `expiration` bigint(20) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
