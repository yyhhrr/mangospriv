local npcId = 90200

local locations = {
	[1] = { "Horde Cities", 1,
			{ "Orgrimmar", 1, 1503, -4415.5, 22 },
			{ "Undercity", 0, 1831, 238.5, 61.6 },
			{ "Thunderbluff", 1, -1278, 122, 132 },
	},
	[2] = { "Alliance Cities", 0,
			{ "Stormwind", 0, -8905, 560, 94 },
			{ "Ironforge", 0, -4795, -1117, 499 },
			{ "Darnassus", 1, 9952, 2280.5, 1342 },
	},
	[3] = { "Raids", 2,
		{ "Blackwing Lair", 0, -7661.42, -1219.53, 287.78 },
		{ "Molten Core", 0, -7515.40, -1046.68, 182.30 },
		{ "Naxxramas", 0, 3120.15, -3724.92, 137.65 },
		{ "Zul'Gurub", 0, -11916.70, -1215.71, 92.28 },
		{ "Onyxia's Lair", 1, -4708.27, -3727.63, 54.55 },
		{ "Ahn'Qiraj", 1, -8098.66, 1525.15, 2.77 },
	},
	[4] = { "Instances 50+", 2,
		{ "Blackrock Depths", 0, -7179.33, -921.21, 165.82 },
		{ "UBRS / LBRS", 0, -7526.08, -1227.56, 285.73 },
		{ "Scholomance", 0, 1266.44, -2557.00, 94.12 },
		{ "Stratholme", 0, 3352.91, -3379.03, 144.78 },
		{ "Stratholme (hinten)", 0, 3174.16, -4038.08, 105.47 },
		{ "Sunken Temple", 0, -10177.90, -3994.89, -111.25 },
		{ "Dire Maul", 1, -3979.65, 1128.52, 161.02 },
	},
	[5] = { "Instances 30-50", 2,
		{ "Scarlet Monastery", 0, 2892.03, -811.03, 160.33 },
		{ "Uldaman", 0, -6071.37, -2955.15, 209.78 },
		{ "Uldaman (hinten)", 0, -6622.90, -3766.03, 266.54 },
		{ "Maraudon", 1, -1419.13, 2908.13, 137.46 },
		{ "Razorfen Downs", 1, -4657.57, -2519.60, 81.05 },
		{ "Zul'Farrak", 1, -6801.18, -2893.02, 9.00 },
	},
	[6] = { "Instances 10-30", 2,
		{ "Deadmines", 0, -11208.36, 1673.23, 24.64 },
		{ "Gnomeregan", 0, -5163.54, 925.42, 257.17 },
		{ "Shadowfang Keep", 0, -234.67, 1561.63, 76.89 },
		{ "The Stockade", 0, -8770.50, 841.37, 90.25 },
		{ "Blackfathom Deeps", 1, 4249.99, 740.10, -25.67 },
		{ "Ragefire Chasm", 1, 1811.78, -4410.5, -18.47 },
		{ "Razorfen Kraul", 1, -4470.27, -1677.77, 81.39 },
		{ "Wailing Caverns", 1, -731.60, -2218.38, 17 },
	},
};

local function GossipHello(event, player, unit)
        if (#locations <= 10) then
                for i, v in ipairs(locations) do
                        if(v[2] == 2 or v[2] == player:GetTeam()) then
                                player:GossipMenuAddItem(0, v[1], 0, i)
                        end
                end
                player:GossipSendMenu(1, unit)
        else
                print("This teleporter only supports 10 different menus.")
        end
end    
 
local function GossipSelect(event, player, unit, sender, intid, code)
        if(intid == 0) then
                GossipHello(event, player, unit)
        elseif(intid <= 10) then
                for i, v in ipairs(locations[intid]) do
                        if (i > 2) then
                                player:GossipMenuAddItem(0, v[1], 0, intid..i)
                        end
                end
                player:GossipMenuAddItem(0, "Back", 0, 0)
                player:GossipSendMenu(1, unit)
        elseif(intid > 10) then
                for i = 1, #locations do
                        for j, v in ipairs(locations[i]) do
                                if(intid == tonumber(i..j)) then
                                        player:GossipComplete()
                                        player:Teleport(v[2], v[3], v[4], v[5], 0)
                                end
                        end
                end
        end
end
 
RegisterCreatureGossipEvent(npcId, 1, GossipHello)
RegisterCreatureGossipEvent(npcId, 2, GossipSelect)