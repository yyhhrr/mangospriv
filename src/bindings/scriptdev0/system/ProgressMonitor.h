#pragma once


namespace ProgressMonitor
{
	enum class Raids : uint32
	{
		MoltenCore = 1,
		Onyxia,
		BlackWingLair,
		ZulGurub,
		Aq20,
		Aq40,
		Naxxramas,
		Worldbosses
	};

	enum class Bosses : uint32
	{
		/* Molten Core */
		Lucifron = 1,
		Magmadar = 2,
		Gehennas = 4,
		Garr = 8,
		Geddon = 16,
		Shazzrah = 32,
		Sulfuronherold = 64,
		Golemagg = 128,
		Majordomus = 256,
		Ragnaros = 512,

		/* Onyxia */
		Onyxia = 1,

		/* Black Wing Lair */
		Razorgore = 1,
		Vaelstrasz = 2,
		Lashlayer = 4,
		Firemaw = 8,
		Ebonroc = 16,
		Flamegor = 32,
		Chromaggus = 64,
		Nefarian = 128,

		/* Zul'Gurub */
		Jeklik = 1,
		Venoxis = 2,
		Mandokir = 4,
		Marli  = 8,
		EdgeOfMadness = 16,
		Thekal = 32,
		Gahzranka = 64,
		Arlokk = 128,
		Jindo = 256,
		Hakkar = 512,

		/* Ruins of Ahn'Qiraj */
		Kurinaxx = 1,
		Rajaxx = 2,
		Moam = 4,
		Buru = 8,
		Ayamiss = 16,
		Ossiran = 32,

		/* Temple of Ahn'Qiraj */
		Skeram = 1,
		Yauj = 2,
		Sartura = 4,
		Fankriss = 8,
		Huhuran = 16,
		Viscidus = 32,
		Imperators = 64,
		Ouro = 128,
		CThun = 256,

		/* Naxxramas */
		AnubRekhan = 1,
		Faerlina = 2,
		Maexxna = 4,
		Patchwerk = 8,
		Grobbulus = 16,
		Gluth = 32,
		Thaddius = 64,
		Noth = 128,
		Heigan = 256,
		Loatheb = 512,
		Razuvious = 1024,
		Gothik = 2048,
		FourHorsemen = 4096,
		Sapphiron = 8192,
		KelThuzad = 16384,

		/* World Bosses */
		Azuregos = 1,
		LordKazzak = 2,
		Emeriss = 4,
		Lethon = 8,
		Ysondre = 16,
		Taerar = 32,
	};

	void Track(Player* plr, Raids raid, Bosses boss);
}