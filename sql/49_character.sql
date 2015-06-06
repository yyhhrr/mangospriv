ALTER TABLE characters ADD COLUMN honor_current_ranking
int NOT NULL DEFAULT 0
AFTER deleteInfos_Name;