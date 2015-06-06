ALTER TABLE `characters`
ADD COLUMN `isPvpFlagged`  tinyint(1) NOT NULL DEFAULT 0 AFTER `honor_current_ranking`;