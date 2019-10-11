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

// Filename    : OVOLUME.H
// Description : audio volume unit

#ifndef __OVOLUME_H
#define __OVOLUME_H

class DsVolume;
class AbsVolume;
class RelVolume;
class PosVolume;

class DsVolume
{
public:
	int32_t	ds_vol;			// -10,000 to 0 (DSBVOLUME_MIN to DSBVOLUME_MAX)
	int32_t	ds_pan;			// -10,000 to 10,000

public:
	DsVolume(int32_t dsVol, int32_t dsPan);
	DsVolume(AbsVolume &);
	DsVolume(RelVolume &);
};

class AbsVolume
{
public:
	int32_t	abs_vol;
	int32_t	ds_pan;

public:
	AbsVolume(int32_t absVol, int32_t dsPan);
	AbsVolume(DsVolume &);
};

class RelVolume
{
public:
	int32_t	rel_vol;			// 0 to 100
	int32_t	ds_pan;			// -10,000 to 10,000

public:
	RelVolume()	{}
	RelVolume(int32_t relVol, int32_t dsPan);
	RelVolume(PosVolume &);
	RelVolume(PosVolume &, int drop, int limit);
};

class PosVolume
{
public:
	int32_t	x;
	int32_t	y;

public:
	PosVolume(int32_t relLocX, int32_t relLocY);
};

extern RelVolume DEF_REL_VOLUME;

#endif