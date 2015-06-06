UPDATE locales_creature INNER JOIN creature_template ON locales_creature.entry = creature_template.entry SET name_loc3 = name, subname_loc3 = subname;
UPDATE creature_template INNER JOIN locales_creature ON locales_creature.entry = creature_template.entry SET name = name_loc1, subname = subname_loc1;

UPDATE locales_gameobject INNER JOIN gameobject_template ON locales_gameobject.entry = gameobject_template.entry SET name_loc3 = name;
UPDATE gameobject_template INNER JOIN locales_gameobject ON locales_gameobject.entry = gameobject_template.entry SET name = name_loc1;

UPDATE locales_item INNER JOIN item_template ON locales_item.entry = item_template.entry SET name_loc3 = name, description_loc3 = description;
UPDATE item_template INNER JOIN locales_item ON locales_item.entry = item_template.entry SET name = name_loc1, description = description_loc1;

UPDATE locales_quest INNER JOIN quest_template ON locales_quest.entry = quest_template.entry SET Title_loc3 = Title, Details_loc3 = Details, Objectives_loc3 = Objectives, OfferRewardText_loc3 = OfferRewardText, RequestItemsText_loc3 = RequestItemsText, EndText_loc3 = EndText, ObjectiveText1_loc3 = ObjectiveText1, ObjectiveText2_loc3 = ObjectiveText2, ObjectiveText3_loc3 = ObjectiveText3, ObjectiveText4_loc3 = ObjectiveText4;
UPDATE quest_template INNER JOIN locales_quest ON locales_quest.entry = quest_template.entry SET Title = Title_loc1, Details = Details_loc1, Objectives = Objectives_loc1, OfferRewardText = OfferRewardText_loc1, RequestItemsText = RequestItemsText_loc1, EndText = EndText_loc1, ObjectiveText1 = ObjectiveText1_loc1, ObjectiveText2 = ObjectiveText2_loc1, ObjectiveText3 = ObjectiveText3_loc1, ObjectiveText4 = ObjectiveText4_loc1;