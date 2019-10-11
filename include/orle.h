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

// Filename    : ORLE.H
// Description : run-lenght compress

#ifndef __ORLE_H
#define __ORLE_H
#include <stdint.h>

class File;

class Rle
{
public:
	Rle();
	~Rle();

	// nul output, to find output size
	int32_t compress( unsigned char *inPtr, int32_t inByteLen, unsigned char rleKey = 0xff );

	// compressed data in memory
	int32_t compress( unsigned char *inPtr, int32_t inByteLen, unsigned char *outPtr, unsigned char rleKey = 0xff );
	int32_t expand( unsigned char *inPtr, unsigned char *outPtr, unsigned char rleKey = 0xff );

	// compressed data in file
	int32_t compress( unsigned char *inPtr, int32_t inByteLen, File *outFile, unsigned char rleKey = 0xff );
	int32_t expand( File *inFile, unsigned char *outPtr, unsigned char rleKey = 0xff );
};


#endif
