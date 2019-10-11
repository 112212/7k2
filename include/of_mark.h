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

//Filename    : OF_MARK.H
//Description : Header of FirmMarket

#ifndef __OF_MARK_H
#define __OF_MARK_H

#ifndef __OFIRM_H
#include <ofirm.h>
#endif

#ifndef __OINFO_H
#include <oinfo.h>
#endif

#ifndef __ORAWRES_H
#include <orawres.h>
#endif

#ifndef __TOWN_H
#include <otown.h>
#endif

//---------- Define constant -----------//

#define  PEASANT_GOODS_MONTH_DEMAND  0.5	 // No. of unit of goods a peasant buys in a month
#define  WORKER_GOODS_MONTH_DEMAND     1   // No. of unit of goods a worker buys in a month

#define  MAX_MARKET_GOODS		6		// Maximum no. of types of goods the market trades for

#define  MAX_MARKET_STOCK 		500

//------- Define class MarketInfo --------//

#pragma pack(1)
struct MarketGoods
{
	char		raw_id;
	char		product_raw_id;
	short		input_firm_recno;

	float		stock_qty;

	float 	cur_month_supply; 		// supply from direct linked firms only. One of its uses is determining whether we have enough supply to export
	float		last_month_supply;
	float		supply_30days()		{ return last_month_supply*(30-info.game_day)/30 +
											  cur_month_supply; }
	float		month_demand;

	float 	cur_month_sale_qty;
	float		last_month_sale_qty;
	float		sale_qty_30days()			{ return last_month_sale_qty*(30-info.game_day)/30 +
												  cur_month_sale_qty; }
	float 	cur_year_sales;
	float		last_year_sales;
	float		sales_365days()		{ return last_year_sales*(365-info.year_day)/365 +
											  cur_year_sales; }
};
#pragma pack()

//------- Define class FirmMarket --------//

#pragma pack(1)
class FirmMarket : public Firm
{
public:
	float			 max_stock_qty;		// maximum stock qty of each market goods

	MarketGoods  market_goods_array[MAX_MARKET_GOODS];
	MarketGoods* market_raw_array(int rawId);
	MarketGoods* market_product_array(int productId);	// pointers to market_goods_array

	int			 free_slot_count();
	int			 stock_value_index();		// for AI, a 0-100 index number telling the total value of the market's stock

	short			 next_output_link_id;
	short			 next_output_firm_recno;

	//------------ ai vars -----------//

	int32_t			 no_linked_town_since_date;
	int32_t			 last_import_new_goods_date;

public:
	FirmMarket();
	~FirmMarket();

	void 		init_derived();

	void		draw(int displayLayer=1);

	// -------- interface function -------//

	void 		put_info(int refreshFlag);
	void 		detect_info();
	int		should_show_info();
	void		disp_edit_mode(int& refreshFlag, int& dispY1, int dispBG=1);
	int		detect_edit_mode();

	// -------- process function ------//

	void		next_day();
	void		next_month();
	void		next_year();

	// -------- caravan function --------//

	short		hire_caravan(char remoteAction);
	int		can_hire_caravan();

	// ------- goods function ---------//

	void		sell_goods();
	void		set_goods(int isRaw, int goodsId, int position);
	void		clear_market_goods(int position);

	// ------ type casting function -----------//

	virtual 	FirmMarket*	cast_to_FirmMarket() { return this; };

	// -------- ai function --------//

	void		process_ai();	// ai process entry point
	int 		is_market_linked_to_town(int ownBaseTownOnly=0);

	// ------- save game function -------//
//	int		read_derived_file(File* filePtr);

	//-------------- multiplayer checking codes ---------------//

	virtual	UCHAR crc8();
	virtual	void	clear_ptr();

private:
	void		disp_market_info(int dispY1, int refreshFlag);
	void		detect_market_info();
	void 		disp_income(int dispY1, int refreshFlag);
	void 		input_goods(int maxInputQty);
	void 		set_next_output_firm();
	void		update_trade_link();
	void 		free_unused_slot();

	//------------------ AI actions --------------------//

	int		think_del();
	void 		ai_update_link_status();
	int  		think_import_new_product();
	int 		think_increase_existing_product_supply();
	int 		think_import_specific_product(int productId);
	int 		think_mft_specific_product(int rawId);
	int 		think_export_product();
	void		think_demand_trade_treaty();
	void 		think_market_build_factory();
	int		ai_create_new_trade(Firm* firmPtr, int stop1PickUpType, int stop2PickUpType);
};
#pragma pack()

//--------------------------------------//

#endif
