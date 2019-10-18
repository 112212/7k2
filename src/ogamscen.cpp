/*
 * Seven Kingdoms 2: The Fryhtan War
 *
 * Copyright 1999 Enlight Software Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//Filename    : OGAMSCEN.CPP
//Description : Select Game Scenario

#include <cstdlib>
#include <ostr.h>
#include <osys.h>
#include <onews.h>
#include <odate.h>
#include <ogfile.h>
#include <omonsres.h>
#include <ofiletxt.h>
#include <odir.h>
#include <obox.h>
#include <obattle.h>
#include <ogame.h>
#include <oconfig.h>
#include <oworldmt.h>
#include <ot_gmenu.h>

//--------- declare static vars ----------//

static void init_scenario_var(ScenInfo* scenInfo);
static int sort_scenario_func(const void *arg1, const void *arg2);

//---------- Begin of function Game::select_run_scenario ----------//
//
// Select and play a scenario.
//
// [int] countScenarioOnly = 0 = display menu and select scenario, 1 = return number of scenario (default:0)
// return : <int> 1 - ok, a scenario is selected and run
//                0 - cancel
//
int Game::select_run_scenario(int countScenarioOnly)
{
	Directory gameDirList[MAX_SCENARIO_PATH];
	ScenInfo* scenInfoArray = NULL;
	int scenInfoSize = 0;
	int dirId;
	for( dirId = 0; dirId < MAX_SCENARIO_PATH; ++dirId )
	{
		if( DIR_SCENARIO_PATH(dirId)[0] )
		{
			Directory &gameDir = gameDirList[dirId];
			{
				String str;
				str  = DIR_SCENARIO_PATH(dirId);
				str += "*.SCN";
				gameDir.read( str, 1 );     // Read in all file names with the "SCN" extension
			}

			if( gameDir.size() > 0)
			{
				//---- append scenario file names in this directory into an array ----//

				scenInfoArray = (ScenInfo *)mem_resize( scenInfoArray, sizeof(ScenInfo)*(scenInfoSize+gameDir.size()) );
				for( int i = 1; i <= gameDir.size(); ++i, ++scenInfoSize)
				{
					char	txtFileName[20];
					scenInfoArray[scenInfoSize].file_name = gameDir[i]->name;    // keep the pointers to the file name string
					scenInfoArray[scenInfoSize].dir_id    = dirId;

					misc.change_file_ext( txtFileName, gameDir[i]->name, "SCT" );

					String str;
					str  = DIR_SCENARIO_PATH(dirId);
					str += txtFileName;

					if( !misc.is_file_exist(str) )
					{
						scenInfoArray[scenInfoSize].scen_name[0] = '\0'; // no desc
						scenInfoArray[scenInfoSize].goal_difficulty = 0;
						scenInfoArray[scenInfoSize].goal_score_bonus = 0;
					}
					else
					{
						FileTxt fileTxtScen(str);

						//---- get the name of the scenario ----//

						fileTxtScen.read_line( scenInfoArray[scenInfoSize].scen_name, ScenInfo::SCEN_NAME_LEN );

						//---- get the difficulty level and score bonus ----//

						fileTxtScen.get_token();		// skip "Difficulty:"
						scenInfoArray[scenInfoSize].goal_difficulty = (short) fileTxtScen.get_num();

						fileTxtScen.get_token();		// skip "Bonus:"
						scenInfoArray[scenInfoSize].goal_score_bonus = (short) fileTxtScen.get_num();
					}
				}
			}
		}
	}

	if( countScenarioOnly )
	{
		if( scenInfoArray )
		{
			mem_del(scenInfoArray);
		}
		return scenInfoSize;
	}

	if( scenInfoSize == 0 )
	{
		// box.msg( "Sorry, there is no scenario in the game directory." );
		box.msg( text_game_menu.str_none_scenario() );
		// possbly no need mem_del because scenInfoArray is NULL
		if( scenInfoArray )
		{
			mem_del(scenInfoArray);
		}
		return 0;
	}

	//-------- sort by difficulty ---------- //

	game_qsort(scenInfoArray, scenInfoSize, sizeof(ScenInfo), sort_scenario_func);

	//-------- select and run a scenario --------//

	int rc = select_scenario( scenInfoSize, scenInfoArray );

	if( rc )
		run_scenario( scenInfoArray+rc-1 );

	//-------------------------------------------//

	mem_del(scenInfoArray);

	return rc;
}
//------------ End of function Game::select_run_scenario -----------//


//---------- Begin of function Game::run_scenario ----------//
//
// <ScenInfo*> scenInfo - the pointer to ScenInfo of the selected scenario
//
int Game::run_scenario(ScenInfo* scenInfo)
{
	String str;

	str  = DIR_SCENARIO_PATH(scenInfo->dir_id);
	str += scenInfo->file_name;

	if( misc.is_file_exist(str) )
	{
		// ###### begin Gilbert 1/11 #########//
		// save the name in the config
		char playerName[Config::PLAYER_NAME_LEN+1];
		strcpy(playerName, config.player_name);
		// ###### end Gilbert 1/11 #########//

		if( game_file.load_game(NULL, str) > 0 )
		{
			init_scenario_var(scenInfo);

			// set the nation name of the player

			if( nation_array.player_recno )
				nation_array.set_custom_both_name( nation_array.player_recno, playerName);

			strcpy(config.player_name, playerName);

			battle.run_loaded();
		}
		game.deinit();
		return 1;
	}

	return 0;
}
//------------ End of function Game::run_scenario -----------//


//-------- Start of function init_scenario_var ----------//
//
static void init_scenario_var(ScenInfo* scenInfo)
{
	//-------- init config var ----------//

	config.king_undie_flag 	  = 0;
	config.disable_ai_flag 	  = 0;
	config.fast_build		  	  = 0;
	config.show_ai_info	  	  = 0;
	config.show_debug_info	  = 0;
	config.show_all_unit_icon = 0;
	config.show_unit_path	  = 0;
	config.disp_extend_info   = 0;

	sys.set_speed(9, COMMAND_AUTO);

	//------ reset the goal deadline -------//

	info.goal_deadline = date.julian( date.year(info.game_date)+config.goal_year_limit,
								date.month(info.game_date),
								date.day(info.game_date) );

	info.goal_difficulty  = scenInfo->goal_difficulty;
	info.goal_score_bonus = scenInfo->goal_score_bonus;

	info.total_play_time  = 0;

	//------ reset vars in NationBase ---------//

	Nation* playerNation = ~nation_array;		// we reset a whole block of vars in NationBase which are related to financial info

	playerNation->cheat_enabled_flag = 0;

	//------- reset financial vars --------//

	int resetSize = (char*) playerNation->relation_array
						 - (char*)(&(playerNation->cur_year_profit));

	memset( &(playerNation->cur_year_profit), 0, resetSize );

	//------- reset statistic vars -------//

	resetSize = (char*)(&(playerNation->own_firm_destroyed))
						 - (char*)(&(playerNation->enemy_soldier_killed))
						 + sizeof(playerNation->own_firm_destroyed);

	memset( &(playerNation->enemy_soldier_killed), 0, resetSize );

	//--------- update statistic ---------//

	nation_array.update_statistic();

	//------- reset display mode --------//
/*
	sys.view_mode = MODE_NORMAL;

	world.map_matrix->map_mode   = MAP_MODE_TERRAIN;
	world.map_matrix->power_mode = 0;
*/
	//------------ reset news ------------//

	news_array.reset();

/*	// ####### begin Gilbert 5/7 #########//
	// set end condition text
	int i;
	for( i = 0; i < game.win_condition_count; i++ )
		game.win_condition[i].update_strings(0);		// 0=win condition
	for( i = 0; i < game.lose_condition_count; i++ )
		game.lose_condition[i].update_strings(1);		// 1=lose condition
	// ####### end Gilbert 5/7 #########//*/
}
//--------- End of function init_scenario_var -----------//



//-------- Start of function init_scenario_var ----------//
//
int sort_scenario_func(const void *arg1, const void *arg2)
{
	return ((ScenInfo *)arg1)->goal_difficulty - ((ScenInfo *)arg2)->goal_difficulty;
}
//-------- End of function init_scenario_var ----------//


