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

// Filename    : OSERIAL.H
// Description : header file of serial number repository


#ifndef __OSERIAL_H
#define __OSERIAL_H
#include <stdint.h>
class SerialRepository
{
public:
	int32_t last_serial_id;
	int32_t max_history;
	int32_t cur_history_size;
	int32_t *history_array;
	char file_name[64];

	bool is_persistant() { return file_name[0] != '\0'; }	// save to file every update

public:
	SerialRepository( int maxHistorySize=0, const char *fileName=0 );
	~SerialRepository();

	void	init();
	void	deinit();
	int	save();
	int	load();

	// -------- function on serial number -------//

	int32_t	get_serial_number();

	// -------- function on repository -------- //

	void	add(int32_t);
	int	add_unique(int32_t);
	bool	is_exist(int32_t);
	int32_t	count(int32_t);
	bool	remove(int32_t);			// true if found and removed
	bool	remove_first(int32_t);	// remove oldest
	bool	remove_last(int32_t);	// remove latest
};

extern SerialRepository serial_repository;

#endif
