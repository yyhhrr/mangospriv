/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "precompiled.h"

//battlegrounds
extern void AddSC_battleground();

/*###############################################
#					Azeroth:    				#
#				 World Scripts					#
#												#
#################################################*/

extern void AddSC_areatrigger_scripts();
extern void AddSC_bosses_emerald_dragons();
extern void AddSC_boss_lord_kazzak();
extern void AddSC_generic_creature();
extern void AddSC_go_scripts();
extern void AddSC_guards();
extern void AddSC_npc_professions();
extern void AddSC_npcs_special();
extern void AddSC_spell_scripts();
extern void AddSC_world_map_scripts();
//extern void AddSC_meetingstone_innkeeper();
extern void AddSC_npc_EngineeringPets();
extern void ADDSC_blizzconGuy();
extern void AddSC_item_scripts();
extern void AddSC_world_premium_system();

//Events
extern void AddSC_zulgurub_opening();
extern void AddSC_aq_war_effort();
extern void AddSC_elementar_invension();

//eastern kingdoms

/*###############################################
#             Östliche Königreiche:             #
#                  Alteractal                   #
#                                               #
#################################################*/

extern void AddSC_alterac_valley();
extern void AddSC_boss_balinda_stonehearth();
extern void AddSC_boss_drekthar();
extern void AddSC_boss_galvangar();
extern void AddSC_boss_vanndar_stormpike();
extern void AddSC_instance_alterac_valley();

/*###############################################
#					Kalimdor:					#
#				Schwarzfelstiefen				#
#												#
#################################################*/

extern void AddSC_blackrock_depths();
extern void AddSC_blackrock_grim_guzzler();
extern void AddSC_boss_anubshiah();
extern void AddSC_boss_baelgar();
extern void AddSC_botschafter_flammenschlag();
extern void AddSC_boss_draganthaurissan();
extern void AddSC_boss_fineous_dunkelader();
extern void AddSC_boss_general_angerforge();
extern void AddSC_boss_golemlord_argelmach();
extern void AddSC_boss_gorosh_the_dervish();
extern void AddSC_boss_grizzle();
extern void AddSC_boss_high_interrogator_gerstahn();
extern void AddSC_boss_lord_incendius();
extern void AddSC_boss_lord_roccor();
extern void AddSC_boss_magmus();
extern void AddSC_boss_stahlgriff();
extern void AddSC_boss_tomb_of_seven();
extern void AddSC_npc_brandwache_bruecke();
extern void AddSC_instance_blackrock_depths();
extern void AddSC_boss_pyromant_weisenkorn();


/*###############################################
#					Kalimdor:					#
#				Schwarzfelsspitze				#
#												#
#################################################*/

extern void AddSC_boss_drakkisath();
extern void AddSC_boss_halycon();
extern void AddSC_boss_highlordomokk();
extern void AddSC_boss_mothersmolderweb();
extern void AddSC_boss_overlordwyrmthalak();
extern void AddSC_boss_shadowvosh();
extern void AddSC_boss_thebeast();
extern void AddSC_boss_warmastervoone();
extern void AddSC_boss_quatermasterzigris();
extern void AddSC_boss_pyroguard_emberseer();
extern void AddSC_boss_gyth();
extern void AddSC_boss_rend_blackhand();
extern void AddSC_instance_blackrock_spire();

//Pechschwingenhort - BWL:
extern void AddSC_boss_razorgore();
extern void AddSC_boss_vaelastrasz();
extern void AddSC_boss_broodlord();
extern void AddSC_boss_firemaw();
extern void AddSC_boss_ebonroc();
extern void AddSC_boss_flamegor();
extern void AddSC_boss_chromaggus();
extern void AddSC_boss_nefarian();
extern void AddSC_blackwing_lair();
extern void AddSC_instance_blackwing_lair();

extern void AddSC_boss_mr_smite();
extern void AddSC_deadmines();
extern void AddSC_instance_deadmines(); 

//gnomeregan
extern void AddSC_gnomeregan();                             
extern void AddSC_boss_thermaplugg();
extern void AddSC_instance_gnomeregan();

//molten_core
extern void AddSC_instance_molten_core();
extern void AddSC_molten_core();

extern void AddSC_boss_sulfuron();
extern void AddSC_boss_lucifron();
extern void AddSC_boss_shazzrah();
extern void AddSC_boss_ragnaros();
extern void AddSC_boss_majordomo();
extern void AddSC_boss_magmadar();
extern void AddSC_boss_golemagg();
extern void AddSC_boss_gehennas();
extern void AddSC_boss_garr();
extern void AddSC_boss_baron_geddon();
extern void AddSC_boss_major_rag();

extern void AddSC_boss_anubrekhan();                        // naxxramas
extern void AddSC_boss_four_horsemen();
extern void AddSC_boss_faerlina();
extern void AddSC_boss_gluth();
extern void AddSC_boss_gothik();
extern void AddSC_boss_grobbulus();
extern void AddSC_boss_kelthuzad();
extern void AddSC_boss_loatheb();
extern void AddSC_boss_maexxna();
extern void AddSC_boss_noth();
extern void AddSC_boss_heigan();
extern void AddSC_boss_patchwerk();
extern void AddSC_boss_razuvious();
extern void AddSC_boss_sapphiron();
extern void AddSC_boss_thaddius();
extern void AddSC_instance_naxxramas();
extern void AddSC_boss_arcanist_doan();                     //scarlet_monastery
extern void AddSC_boss_azshir_the_sleepless();
extern void AddSC_boss_bloodmage_thalnos();
extern void AddSC_boss_herod();
extern void AddSC_boss_high_inquisitor_fairbanks();
extern void AddSC_boss_houndmaster_loksey();
extern void AddSC_boss_interrogator_vishas();
extern void AddSC_boss_mograine_and_whitemane();
extern void AddSC_instance_scarlet_monastery();
extern void AddSC_instance_scholomance();
extern void AddSC_scholomance();
//Scholomance
extern void AddSC_blutdiener_von_kirtonos();
extern void AddSC_blutrippe();
extern void AddSC_der_ravenier();
extern void AddSC_doktor_theolen_krastinov();
extern void AddSC_dunkelmeister_gandling();
extern void AddSC_hueter_des_wissens_polkelt();
extern void AddSC_instrukteurin_malicia();
extern void AddSC_jandice_barov();
extern void AddSC_kirtonos();
extern void AddSC_kormok();
extern void AddSC_lady_illucia_barov();
extern void AddSC_lord_alexei_barov();
extern void AddSC_marduk();
extern void AddSC_ras_frostraunen();
extern void AddSC_vectus();
extern void AddSC_SpectralTutor();

extern void AddSC_shadowfang_keep();                        //shadowfang_keep
extern void AddSC_instance_shadowfang_keep();
extern void AddSC_boss_magistrate_barthilas();              //stratholme
extern void AddSC_boss_maleki_the_pallid();
extern void AddSC_boss_nerubenkan();
extern void AddSC_boss_cannon_master_willey();
extern void AddSC_boss_baroness_anastari();
extern void AddSC_boss_ramstein_the_gorger();
extern void AddSC_boss_timmy_the_cruel();
extern void AddSC_boss_postmaster_malown();
extern void AddSC_boss_baron_rivendare();
extern void AddSC_boss_dathrohan_balnazzar();
extern void AddSC_instance_stratholme();
extern void AddSC_stratholme();
extern void AddSC_instance_sunken_temple();                 //sunken_temple
extern void AddSC_sunken_temple();
extern void AddSC_boss_archaedas();                         //uldaman
extern void AddSC_uldaman();
extern void AddSC_instance_uldaman();
extern void AddSC_boss_arlokk();                            //zulgurub
extern void AddSC_boss_gahzranka();
extern void AddSC_boss_grilek();
extern void AddSC_boss_hakkar();
extern void AddSC_boss_hazzarah();
extern void AddSC_boss_jeklik();
extern void AddSC_boss_jindo();
extern void AddSC_boss_mandokir();
extern void AddSC_boss_marli();
extern void AddSC_boss_ouro();
extern void AddSC_boss_renataki();
extern void AddSC_boss_thekal();
extern void AddSC_boss_venoxis();
extern void AddSC_boss_wushoolay();
extern void AddSC_instance_zulgurub();
extern void AddSC_zulgurub();

//extern void AddSC_alterac_mountains();
extern void AddSC_arathi_highlands();
extern void AddSC_blasted_lands();
extern void AddSC_boss_kruul();
extern void AddSC_burning_steppes();
extern void AddSC_deeprun_tram();
extern void AddSC_dun_morogh();
extern void AddSC_eastern_plaguelands();
extern void AddSC_elwynn_forest();
extern void AddSC_hinterlands();
extern void AddSC_ironforge();
extern void AddSC_loch_modan();
extern void AddSC_redridge_mountains();
extern void AddSC_searing_gorge();
extern void AddSC_silverpine_forest();
extern void AddSC_stormwind_city();
extern void AddSC_stranglethorn_vale();
extern void AddSC_swamp_of_sorrows();
extern void AddSC_tirisfal_glades();
extern void AddSC_undercity();
extern void AddSC_western_plaguelands();
extern void AddSC_westfall();
extern void AddSC_wetlands();

//#############################################
//dire maul
extern void AddSC_instance_dire_maul();
extern void AddSC_dire_maul();
	
extern void AddSC_boss_alzzin_der_wildformer();
extern void AddSC_boss_cho_rush_der_beobachter();
extern void AddSC_boss_guard_fengus();
extern void AddSC_boss_hauptmann_krombruch();
extern void AddSC_boss_hydrobrut();
extern void AddSC_boss_illyanna_rabeneiche();
extern void AddSC_boss_immolthar();
extern void AddSC_boss_koenig_gordok();
extern void AddSC_boss_lethtendris();
extern void AddSC_boss_mol_dar();
extern void AddSC_boss_prince_tortheldrin();
extern void AddSC_npc_pusillin();
extern void AddSC_boss_tendris_warpwood();
extern void AddSC_boss_wache_slip_kik();
extern void AddSC_boss_zevrim_dornhuf();
    
extern void AddSC_QuestElfischeLegenden();
extern void AddSC_theMaul();
//#############################################

/*###############################################
#					Kalimdor:					#
#				Blackfathom Deeps				#
#												#
#################################################*/

extern void AddSC_blackfathom_deeps();
extern void AddSC_boss_gelihast();
extern void AddSC_boss_ghamoo();
extern void AddSC_boss_lady_sarevess();
extern void AddSC_boss_lord_kelris();
extern void AddSC_instance_blackfathom_deeps();

/*###############################################
#					Kalimdor:					#
#					Maraudon					#
#												#
#################################################*/

extern void AddSC_boss_celebras_the_cursed();
extern void AddSC_boss_faulschnapper();
extern void AddSC_npc_khan_geister();
extern void AddSC_boss_landslide();
extern void AddSC_boss_lord_schlangenzunge();
extern void AddSC_boss_noxxion();
extern void AddSC_boss_ptheradras();
extern void AddSC_boss_tueftler_gizlock();
extern void AddSC_rar_meshlok_der_ernter();
extern void AddCelebrasTheRedeemed();

/*###############################################
#					Kalimdor:					#
#					Onyxia						#
#												#
#################################################*/

extern void AddSC_instance_onyxias_lair();
extern void AddSC_boss_onyxia();


extern void AddSC_boss_amnennar_the_coldbringer();          //razorfen_downs
extern void AddSC_razorfen_downs();

/*###############################################
#					Kalimdor:					#
#			  Kral der Klingenhauer				#
#												#
#################################################*/

extern void AddSC_boss_agathelos_der_tobende();
extern void AddSC_boss_aggem_dornfluch();
extern void AddSC_boss_charlga_klingenflanke();
extern void AddSC_boss_oberanfuehrer_rammhauer();
extern void AddSC_boss_roogug();
extern void AddSC_boss_todessprecher_jargba();
extern void AddSC_instance_razorfen_kraul();
extern void AddSC_razorfen_kraul();


extern void AddSC_boss_ayamiss();                           //ruins_of_ahnqiraj
extern void AddSC_boss_buru();
extern void AddSC_boss_kurinnaxx();
extern void AddSC_boss_moam();
extern void AddSC_boss_ossirian();
extern void AddSC_boss_rajaxx();
extern void AddSC_ruins_of_ahnqiraj();
extern void AddSC_instance_ruins_of_ahnqiraj();
extern void AddSC_boss_cthun();                             //temple_of_ahnqiraj
extern void AddSC_boss_fankriss();
extern void AddSC_boss_huhuran();
extern void AddSC_bug_trio();
extern void AddSC_boss_sartura();
extern void AddSC_boss_viscidus();
extern void AddSC_boss_skeram();
extern void AddSC_boss_twinemperors();
extern void AddSC_mob_anubisath_sentinel();
extern void AddSC_instance_temple_of_ahnqiraj();

/*###############################################
#					Kalimdor:					#
#			   Höhlen des Wehklagens			#
#												#
#################################################*/

extern void AddSC_boss_lady_anacondra();
extern void AddSC_boss_lord_kobrahn();
extern void AddSC_boss_lord_pythas();
extern void AddSC_boss_lord_serpentis();
extern void AddSC_boss_mutanus_der_verschlinger();
extern void AddSC_boss_skum();
extern void AddSC_boss_verdan_der_ewiglebende();
extern void AddSC_wailing_caverns();
extern void AddSC_instance_wailing_caverns();

/*###############################################
#					Kalimdor:					#
#				   Zul'Farrak					#
#												#
#################################################*/

extern void AddSC_boss_antu_sul();
extern void AddSC_boss_gahz_rilla();
extern void AddSC_boss_haeuptling_ukorz_sandskalp();
extern void AddSC_boss_henker_der_sandwueter();
extern void AddSC_boss_hexendoktor_zum_rah();
extern void AddSC_boss_nekrum_der_ausweider();
extern void AddSC_boss_ruuzlu();
extern void AddSC_boss_schattenpriester_sezz_ziz();
extern void AddSC_boss_theka_der_maertyrer();
extern void AddSC_boss_wasserbeschwoererin_velratha();
extern void AddSC_instance_zulfarrak();
extern void AddSC_rar_karaburan();
extern void AddSC_rar_zerillis();
extern void AddSC_zulfarrak();


extern void AddSC_ashenvale();
extern void AddSC_azshara();
extern void AddSC_boss_azuregos();
extern void AddSC_darkshore();
extern void AddSC_desolace();
extern void AddSC_durotar();
extern void AddSC_dustwallow_marsh();
extern void AddSC_felwood();
extern void AddSC_feralas();
extern void AddSC_moonglade();
extern void AddSC_mulgore();
extern void AddSC_orgrimmar();
extern void AddSC_silithus();
extern void AddSC_stonetalon_mountains();
extern void AddSC_tanaris();
extern void AddSC_teldrassil();
extern void AddSC_the_barrens();
extern void AddSC_thousand_needles();
extern void AddSC_thunder_bluff();
extern void AddSC_ungoro_crater();
extern void AddSC_winterspring();

/*###############################################
#					  World:					#
#					Epic Quests					#
#												#
#################################################*/

extern void AddSC_Priest_Epic_Quest();
extern void AddSC_Hunter_Epic_Quest();

void AddScripts()
{
    //battlegrounds
    AddSC_battleground();

/*###############################################
#					Azeroth:    				#
#				 World Scripts					#
#												#
#################################################*/

    AddSC_areatrigger_scripts();
    AddSC_bosses_emerald_dragons();
    AddSC_boss_lord_kazzak();
    AddSC_generic_creature();
    AddSC_go_scripts();
    AddSC_guards();
    AddSC_npc_professions();
    AddSC_npcs_special();
    AddSC_spell_scripts();
    AddSC_world_map_scripts();
	//AddSC_meetingstone_innkeeper();
	AddSC_npc_EngineeringPets();
	ADDSC_blizzconGuy();
	AddSC_item_scripts();
	AddSC_world_premium_system();

	//Events
	AddSC_zulgurub_opening();
    AddSC_aq_war_effort();
	AddSC_elementar_invension();

    /*###############################################
    #             Östliche Königreiche:             #
    #                  Alteractal                   #
    #                                               #
    #################################################*/

    AddSC_alterac_valley();
    AddSC_boss_balinda_stonehearth();
    AddSC_boss_drekthar();
    AddSC_boss_galvangar();
    AddSC_boss_vanndar_stormpike();
    AddSC_instance_alterac_valley();

/*###############################################
#			  Östliche Königreiche:				#
#				Schwarzfelstiefen				#
#												#
#################################################*/

AddSC_blackrock_depths();
AddSC_blackrock_grim_guzzler();
AddSC_boss_anubshiah();
AddSC_boss_baelgar();
AddSC_botschafter_flammenschlag();
AddSC_boss_draganthaurissan();
AddSC_boss_fineous_dunkelader();
AddSC_boss_general_angerforge();
AddSC_boss_golemlord_argelmach();
AddSC_boss_gorosh_the_dervish();
AddSC_boss_grizzle();
AddSC_boss_high_interrogator_gerstahn();
AddSC_boss_lord_incendius();
AddSC_boss_lord_roccor();
AddSC_boss_magmus();
AddSC_boss_stahlgriff();
AddSC_boss_tomb_of_seven();
AddSC_npc_brandwache_bruecke();
AddSC_instance_blackrock_depths();
AddSC_boss_pyromant_weisenkorn();


    AddSC_boss_drakkisath();                                //blackrock_spire
    AddSC_boss_halycon();
    AddSC_boss_highlordomokk();
    AddSC_boss_mothersmolderweb();
    AddSC_boss_overlordwyrmthalak();
    AddSC_boss_shadowvosh();
    AddSC_boss_thebeast();
    AddSC_boss_warmastervoone();
    AddSC_boss_quatermasterzigris();
    AddSC_boss_pyroguard_emberseer();
    AddSC_boss_gyth();
	AddSC_boss_rend_blackhand();
    AddSC_instance_blackrock_spire();

    AddSC_boss_razorgore();                                 //blackwing_lair
    AddSC_boss_vaelastrasz();
    AddSC_boss_broodlord();
    AddSC_boss_firemaw();
    AddSC_boss_ebonroc();
    AddSC_boss_flamegor();
    AddSC_boss_chromaggus();
    AddSC_boss_nefarian();
	AddSC_blackwing_lair();
    AddSC_instance_blackwing_lair();

	AddSC_deadmines();
	AddSC_boss_mr_smite();
	AddSC_instance_deadmines(); 

	//gnomeregan
    AddSC_gnomeregan();                                    
    AddSC_boss_thermaplugg();
    AddSC_instance_gnomeregan();

	// Molten Core

    AddSC_instance_molten_core();
    AddSC_molten_core();

    AddSC_boss_sulfuron();
    AddSC_boss_lucifron();
    AddSC_boss_shazzrah();
    AddSC_boss_ragnaros();
    AddSC_boss_majordomo();
    AddSC_boss_magmadar();
    AddSC_boss_golemagg();
    AddSC_boss_gehennas();
    AddSC_boss_garr();
    AddSC_boss_baron_geddon();
    AddSC_boss_major_rag();

    AddSC_boss_anubrekhan();                                // naxxramas
    AddSC_boss_four_horsemen();
    AddSC_boss_faerlina();
    AddSC_boss_gluth();
    AddSC_boss_gothik();
    AddSC_boss_grobbulus();
    AddSC_boss_kelthuzad();
    AddSC_boss_loatheb();
    AddSC_boss_maexxna();
    AddSC_boss_noth();
    AddSC_boss_heigan();
    AddSC_boss_patchwerk();
    AddSC_boss_razuvious();
    AddSC_boss_sapphiron();
    AddSC_boss_thaddius();
    AddSC_instance_naxxramas();
    AddSC_boss_arcanist_doan();                             //scarlet_monastery
    AddSC_boss_azshir_the_sleepless();
    AddSC_boss_bloodmage_thalnos();
    AddSC_boss_herod();
    AddSC_boss_high_inquisitor_fairbanks();
    AddSC_boss_houndmaster_loksey();
    AddSC_boss_interrogator_vishas();
    AddSC_boss_mograine_and_whitemane();
    AddSC_instance_scarlet_monastery();


	// scholomance
    AddSC_instance_scholomance();
    AddSC_scholomance();

    AddSC_blutdiener_von_kirtonos();
    AddSC_blutrippe();
    AddSC_der_ravenier();
    AddSC_doktor_theolen_krastinov();
    AddSC_dunkelmeister_gandling();
    AddSC_hueter_des_wissens_polkelt();
    AddSC_instrukteurin_malicia();
    AddSC_jandice_barov();
    AddSC_kirtonos();
    AddSC_kormok();
    AddSC_lady_illucia_barov();
    AddSC_lord_alexei_barov();
    AddSC_marduk();
    AddSC_ras_frostraunen();
    AddSC_vectus();
	AddSC_SpectralTutor();

//#################################################
	// dire maul
    AddSC_instance_dire_maul();
    AddSC_dire_maul();
	
    AddSC_boss_alzzin_der_wildformer();
    AddSC_boss_cho_rush_der_beobachter();
    AddSC_boss_guard_fengus();
    AddSC_boss_hauptmann_krombruch();
    AddSC_boss_hydrobrut();
	AddSC_boss_illyanna_rabeneiche();
	AddSC_boss_immolthar();
    AddSC_boss_koenig_gordok();
    AddSC_boss_lethtendris();
    AddSC_boss_mol_dar();
	AddSC_boss_prince_tortheldrin();
	AddSC_npc_pusillin();
	AddSC_boss_tendris_warpwood();
    AddSC_boss_wache_slip_kik();
    AddSC_boss_zevrim_dornhuf();
    
	AddSC_QuestElfischeLegenden();
	AddSC_theMaul();
//#################################################
    AddSC_shadowfang_keep();                                //shadowfang_keep
    AddSC_instance_shadowfang_keep();
    AddSC_boss_magistrate_barthilas();                      //stratholme
    AddSC_boss_maleki_the_pallid();
    AddSC_boss_nerubenkan();
    AddSC_boss_cannon_master_willey();
    AddSC_boss_baroness_anastari();
    AddSC_boss_ramstein_the_gorger();
    AddSC_boss_timmy_the_cruel();
    AddSC_boss_postmaster_malown();
    AddSC_boss_baron_rivendare();
    AddSC_boss_dathrohan_balnazzar();
    AddSC_instance_stratholme();
    AddSC_stratholme();
    AddSC_instance_sunken_temple();                         //sunken_temple
    AddSC_sunken_temple();
    AddSC_boss_archaedas();                                 //uldaman
    AddSC_uldaman();
    AddSC_instance_uldaman();
    AddSC_boss_arlokk();                                    //zulgurub
    AddSC_boss_gahzranka();
    AddSC_boss_grilek();
    AddSC_boss_hakkar();
    AddSC_boss_hazzarah();
    AddSC_boss_jeklik();
    AddSC_boss_jindo();
    AddSC_boss_mandokir();
    AddSC_boss_marli();
    AddSC_boss_ouro();
    AddSC_boss_renataki();
    AddSC_boss_thekal();
    AddSC_boss_venoxis();
    AddSC_boss_wushoolay();
    AddSC_instance_zulgurub();
	AddSC_zulgurub();

    //AddSC_alterac_mountains();
    AddSC_arathi_highlands();
    AddSC_blasted_lands();
    AddSC_burning_steppes();
    AddSC_deeprun_tram();
    AddSC_dun_morogh();
    AddSC_eastern_plaguelands();
    AddSC_elwynn_forest();
    AddSC_hinterlands();
    AddSC_ironforge();
    AddSC_loch_modan();
    AddSC_redridge_mountains();
    AddSC_searing_gorge();
    AddSC_silverpine_forest();
    AddSC_stormwind_city();
    AddSC_stranglethorn_vale();
    AddSC_swamp_of_sorrows();
    AddSC_tirisfal_glades();
    AddSC_undercity();
    AddSC_western_plaguelands();
    AddSC_westfall();
    AddSC_wetlands();

    //kalimdor

/*###############################################
#					Kalimdor:					#
#				Blackfathom Deeps				#
#												#
#################################################*/

	AddSC_blackfathom_deeps();
	AddSC_boss_gelihast();
	AddSC_boss_ghamoo();
	AddSC_boss_lady_sarevess();
	AddSC_boss_lord_kelris();
	AddSC_instance_blackfathom_deeps();

/*###############################################
#					Kalimdor:					#
#					Maraudon					#
#												#
#################################################*/

	AddSC_boss_faulschnapper();
	AddSC_npc_khan_geister();
	AddSC_boss_landslide();
	AddSC_boss_lord_schlangenzunge();
	AddSC_boss_noxxion();
	AddSC_boss_ptheradras();
	AddSC_boss_tueftler_gizlock();
	AddSC_rar_meshlok_der_ernter();
	AddCelebrasTheRedeemed();


    AddSC_instance_onyxias_lair();                          //onyxias_lair
    AddSC_boss_onyxia();
    AddSC_boss_amnennar_the_coldbringer();                  //razorfen_downs
    AddSC_razorfen_downs();


/*###############################################
#					Kalimdor:					#
#			  Kral der Klingenhauer				#
#												#
#################################################*/

	AddSC_boss_agathelos_der_tobende();
	AddSC_boss_aggem_dornfluch();
	AddSC_boss_charlga_klingenflanke();
	AddSC_boss_oberanfuehrer_rammhauer();
	AddSC_boss_roogug();
	AddSC_boss_todessprecher_jargba();
	AddSC_instance_razorfen_kraul();
	AddSC_razorfen_kraul();


    AddSC_boss_ayamiss();                                   //ruins_of_ahnqiraj
    AddSC_boss_buru();
    AddSC_boss_kurinnaxx();
    AddSC_boss_moam();
    AddSC_boss_ossirian();
    AddSC_boss_rajaxx();
    AddSC_ruins_of_ahnqiraj();
    AddSC_instance_ruins_of_ahnqiraj();
    AddSC_boss_cthun();                                     //temple_of_ahnqiraj
    AddSC_boss_fankriss();
    AddSC_boss_huhuran();
    AddSC_bug_trio();
    AddSC_boss_viscidus();
    AddSC_boss_sartura();
    AddSC_boss_skeram();
    AddSC_boss_twinemperors();
    AddSC_mob_anubisath_sentinel();
    AddSC_instance_temple_of_ahnqiraj();


/*###############################################
#					Kalimdor:					#
#			   Höhlen des Wehklagens			#
#												#
#################################################*/

	AddSC_boss_lady_anacondra();
	AddSC_boss_lord_kobrahn();
	AddSC_boss_lord_pythas();
	AddSC_boss_lord_serpentis();
	AddSC_boss_mutanus_der_verschlinger();
	AddSC_boss_skum();
	AddSC_boss_verdan_der_ewiglebende();
    AddSC_wailing_caverns();
    AddSC_instance_wailing_caverns();

/*###############################################
#					Kalimdor:					#
#					Zul'Farrak					#
#												#
#################################################*/

	AddSC_instance_zulfarrak();
	AddSC_zulfarrak();
	AddSC_boss_wasserbeschwoererin_velratha();
    AddSC_boss_antu_sul();
    AddSC_boss_gahz_rilla();
    AddSC_boss_haeuptling_ukorz_sandskalp();
    AddSC_boss_henker_der_sandwueter();
    AddSC_boss_hexendoktor_zum_rah();
    AddSC_boss_nekrum_der_ausweider();
    AddSC_boss_ruuzlu();
    AddSC_boss_schattenpriester_sezz_ziz();
    AddSC_boss_theka_der_maertyrer();
    AddSC_rar_karaburan();
    AddSC_rar_zerillis();

    AddSC_ashenvale();
    AddSC_azshara();
    AddSC_boss_azuregos();
    AddSC_darkshore();
    AddSC_desolace();
    AddSC_durotar();
    AddSC_dustwallow_marsh();
    AddSC_felwood();
    AddSC_feralas();
    AddSC_moonglade();
    AddSC_mulgore();
    AddSC_orgrimmar();
    AddSC_silithus();
    AddSC_stonetalon_mountains();
    AddSC_tanaris();
    AddSC_teldrassil();
    AddSC_the_barrens();
    AddSC_thousand_needles();
    AddSC_thunder_bluff();
    AddSC_ungoro_crater();
    AddSC_winterspring();

/*###############################################
#					  World:					#
#					Epic Quests					#
#												#
#################################################*/

	AddSC_Priest_Epic_Quest();
	AddSC_Hunter_Epic_Quest();
}
