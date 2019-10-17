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

// Filename    : OCRC_STO.H
// Description : store of crc of objects

#include <onationa.h>
#include <ounit.h>
#include <ofirma.h>
#include <otown.h>
#include <obullet.h>
#include <orebel.h>
#include <ospy.h>
#include <osite.h>
#include <otalkres.h>
#include <oremote.h>
#include <ocrc_sto.h>
#include <crc.h>

CrcStore::CrcStore() :
	nations(0), units(0), firms(0), towns(0), bullets(0), rebels(0), spies(0), sites(0), talk_msgs(0)
{
}

void CrcStore::init()
{
	deinit();
}

void CrcStore::deinit()
{
	nations.clear();
	units.clear();
	firms.clear();
	towns.clear();
	bullets.clear();
	rebels.clear();
	spies.clear();
	sites.clear();
	talk_msgs.clear();
}
#include <string>

int ocrc_sto_object_size = 0;
const char* ocrc_sto_class_name = "";

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr) {
	size_t pos = data.find(toSearch);
	while(pos != std::string::npos) {
		data.replace(pos, toSearch.size(), replaceStr);
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

void dump_binary(std::string filename, int size, void* bin=0) {
	
	#define DUMP_DIR "memory_dump"
	std::string fname(DUMP_DIR);
	fname = fname + "/" + filename;
	
	// create folder path
	std::string folder = fname;
	size_t t = folder.rfind('/');
	if(t != folder.npos) {
		folder = folder.substr(0, t);
	}
#ifndef NO_WINDOWS
	findAndReplaceAll(folder, "/", "\\");
#endif
	if(!misc.is_file_exist((char*)folder.c_str()))
		misc.mkpath((char*)folder.c_str());
	
	
	FILE* f = fopen(fname.c_str(),"wb");
	if(f) {
		extern void *omp_crc_ptr;
		fwrite(bin?bin:omp_crc_ptr, size, 1, f);
		fclose(f);
	}
}

void CrcStore::record_nations()
{
	nations.clear();
	*(short *)nations.reserve(sizeof(short)) = nation_array.size();

	for( short nationRecno = 1; nationRecno <= nation_array.size(); ++nationRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !nation_array.is_deleted(nationRecno) )
			checkNum = nation_array[nationRecno]->crc8();
		*(CRC_TYPE *)nations.reserve(sizeof(CRC_TYPE)) = checkNum;
		
		
		dump_binary(std::string("nations/") + ocrc_sto_class_name + std::to_string(nationRecno) + ".dump", ocrc_sto_object_size);
	}
}

void CrcStore::record_units()
{
	units.clear();
	*(short *)units.reserve(sizeof(short)) = unit_array.size();

	for( short unitRecno = 1; unitRecno <= unit_array.size(); ++unitRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !unit_array.is_deleted(unitRecno) )
			checkNum = unit_array[unitRecno]->crc8();
		*(CRC_TYPE *)units.reserve(sizeof(CRC_TYPE)) = checkNum;
		
		dump_binary(std::string("units/") + ocrc_sto_class_name + std::to_string(unitRecno) + ".dump", ocrc_sto_object_size);
	}
}

void CrcStore::record_firms()
{
	firms.clear();
	*(short *)firms.reserve(sizeof(short)) = firm_array.size();

	for( short firmRecno = 1; firmRecno <= firm_array.size(); ++firmRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !firm_array.is_deleted(firmRecno) )
			checkNum = firm_array[firmRecno]->crc8();
		dump_binary(std::string("firms/") + ocrc_sto_class_name + std::to_string(firmRecno) + ".dump", ocrc_sto_object_size);
		*(CRC_TYPE *)firms.reserve(sizeof(CRC_TYPE)) = checkNum;
	}
}

void CrcStore::record_towns()
{
	towns.clear();
	*(short *)towns.reserve(sizeof(short)) = town_array.size();

	for( short townRecno = 1; townRecno <= town_array.size(); ++townRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !town_array.is_deleted(townRecno) )
			checkNum = town_array[townRecno]->crc8();
		dump_binary(std::string("towns/") + ocrc_sto_class_name + std::to_string(townRecno) + ".dump", ocrc_sto_object_size);
		*(CRC_TYPE *)towns.reserve(sizeof(CRC_TYPE)) = checkNum;
	}
}

void CrcStore::record_bullets()
{
	bullets.clear();
	*(short *)bullets.reserve(sizeof(short)) = bullet_array.size();

	for( short bulletRecno = 1; bulletRecno <= bullet_array.size(); ++bulletRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !bullet_array.is_deleted(bulletRecno) )
			checkNum = bullet_array[bulletRecno]->crc8();
		*(CRC_TYPE *)bullets.reserve(sizeof(CRC_TYPE)) = checkNum;
	}
}


void CrcStore::record_rebels()
{
	rebels.clear();
	*(short *)rebels.reserve(sizeof(short)) = rebel_array.size();

	for( short rebelRecno = 1; rebelRecno <= rebel_array.size(); ++rebelRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !rebel_array.is_deleted(rebelRecno) )
		{
			checkNum = rebel_array[rebelRecno]->crc8();
		}
		*(CRC_TYPE *)rebels.reserve(sizeof(CRC_TYPE)) = checkNum;
	}
}


void CrcStore::record_spies()
{
	spies.clear();
	*(short *)spies.reserve(sizeof(short)) = spy_array.size();

	for( short spyRecno = 1; spyRecno <= spy_array.size(); ++spyRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !spy_array.is_deleted(spyRecno) )
		{
			checkNum = spy_array[spyRecno]->crc8();
		}
		*(CRC_TYPE *)spies.reserve(sizeof(CRC_TYPE)) = checkNum;
	}
}


void CrcStore::record_sites()
{
	sites.clear();
	*(short *)sites.reserve(sizeof(short)) = site_array.size();

	for( short siteRecno = 1; siteRecno <= site_array.size(); ++siteRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !site_array.is_deleted(siteRecno) )
		{
			 checkNum = site_array[siteRecno]->crc8();
		}
		*(CRC_TYPE *)sites.reserve(sizeof(CRC_TYPE)) = checkNum;
	}
}


void CrcStore::record_talk_msgs()
{
	talk_msgs.clear();
	*(short *)talk_msgs.reserve(sizeof(short)) = talk_res.talk_msg_count();

	for( short talkRecno = 1; talkRecno <= talk_res.talk_msg_count(); ++talkRecno)
	{
		CRC_TYPE checkNum = 0;
		if( !talk_res.is_talk_msg_deleted(talkRecno) )
		{
			 checkNum = talk_res.get_talk_msg(talkRecno)->crc8();
		}
		*(CRC_TYPE *)talk_msgs.reserve(sizeof(CRC_TYPE)) = checkNum;
	}
}

extern int desync_flags;
void CrcStore::record_all()
{
	int d = MSG_COMPARE_NATION;
	if( ((1 << (MSG_COMPARE_NATION-d)) & desync_flags) == 0)
		record_nations();
	if( ((1 << (MSG_COMPARE_UNIT-d)) & desync_flags) == 0)
		record_units();
	if( ((1 << (MSG_COMPARE_FIRM-d)) & desync_flags) == 0)
		record_firms();
	if( ((1 << (MSG_COMPARE_TOWN-d)) & desync_flags) == 0)
		record_towns();
	if( ((1 << (MSG_COMPARE_BULLET-d)) & desync_flags) == 0)
		record_bullets();
	if( ((1 << (MSG_COMPARE_REBEL-d)) & desync_flags) == 0)
		record_rebels();
	if( ((1 << (MSG_COMPARE_SPY-d)) & desync_flags) == 0)
		record_spies();
	if( ((1 << (MSG_COMPARE_SITE-d)) & desync_flags) == 0)
		record_sites();
	if( ((1 << (MSG_COMPARE_TALK-d)) & desync_flags) == 0)
		record_talk_msgs();
}

void CrcStore::dump_nations() {
	
}
void CrcStore::dump_units() {
}

void CrcStore::dump_firms() {
	
}
void CrcStore::dump_towns() {
	
}
void CrcStore::dump_bullets() {
	
}
void CrcStore::dump_rebels() {
	
}
void CrcStore::dump_spies() {
	
}
void CrcStore::dump_sites() {
	
}


void CrcStore::send_all()
{
	char *charPtr;
	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_NATION, nations.length() );
	memcpy(charPtr, nations.queue_buf, nations.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_UNIT, units.length() );
	memcpy(charPtr, units.queue_buf, units.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_FIRM, firms.length() );
	memcpy(charPtr, firms.queue_buf, firms.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_TOWN, towns.length() );
	memcpy(charPtr, towns.queue_buf, towns.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_BULLET, bullets.length() );
	memcpy(charPtr, bullets.queue_buf, bullets.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_REBEL, rebels.length() );
	memcpy(charPtr, rebels.queue_buf, rebels.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_SPY, spies.length() );
	memcpy(charPtr, spies.queue_buf, spies.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_SITE, sites.length() );
	memcpy(charPtr, sites.queue_buf, sites.length() );

	charPtr = (char *)remote.new_send_queue_msg(MSG_COMPARE_TALK, talk_msgs.length() );
	memcpy(charPtr, talk_msgs.queue_buf, talk_msgs.length() );
}

// return 0 if equal
// otherwise not equal
int CrcStore::compare_remote(DWORD remoteMsgId, char *dataPtr)
{
	VLenQueue *vq = NULL;
	const char *arrayName;

	switch(remoteMsgId)
	{
	case MSG_COMPARE_NATION:
		vq = &nations;
		arrayName = "nation_array";
		break;
	case MSG_COMPARE_UNIT:
		vq = &units;
		arrayName = "unit_array";
		break;
	case MSG_COMPARE_FIRM:
		vq = &firms;
		arrayName = "firm_array";
		break;
	case MSG_COMPARE_TOWN:
		vq = &towns;
		arrayName = "town_array";
		break;
	case MSG_COMPARE_BULLET:
		vq = &bullets;
		arrayName = "bullet_array";
		break;
	case MSG_COMPARE_REBEL:
		vq = &rebels;
		arrayName = "rebel_array";
		break;
	case MSG_COMPARE_SPY:
		vq = &spies;
		arrayName = "spy_array";
		break;
	case MSG_COMPARE_SITE:
		vq = &sites;
		arrayName = "site_array";
		break;
	case MSG_COMPARE_TALK:
		vq = &talk_msgs;
		arrayName = "talk_res";
		break;
	default:
		err_here();
		return 0;
	}

	err_when(vq->length() < sizeof(short));
	int rc = memcmp(vq->queue_buf, dataPtr, vq->length() );
	if( rc )
	{
		char *p1;
		char *p2;
		int diffOffset;

		// found out where is the first difference
		for( diffOffset = 0, p1 = vq->queue_buf, p2 = dataPtr; 
			diffOffset < vq->length() && *p1 == *p2; 
			++diffOffset, ++p1, ++p2);

		// ###### patch begin Gilbert 23/1 #######//
		// err.run("%s discrepency, offset : %d", arrayName, diffOffset);
		crc_error_string = arrayName;
		crc_error_string += "discrepency, offset : ";
		crc_error_string += diffOffset;
		// ###### patch end Gilbert 23/1 #######//
		diffOffset = 0;		// dummy code
	}
	return rc;
}
