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

//Filename    : ODATE.H
//Description : Header file of object date information

#ifndef __ODATE_H
#define __ODATE_H

#include <win32_compat.h>

class DateInfo
{
public:

   int32_t  julian(int,int,int);
   int32_t  julian(char*);

   char* date_str(int32_t,int=0);
   const char* month_str(int);

   int   year(int32_t julianDate)  { return get_date(julianDate,'Y'); }
   int   month(int32_t julianDate) { return get_date(julianDate,'M'); }
   int   day(int32_t julianDate)   { return get_date(julianDate,'D'); }

   char* time_str(int);
   int   days_in_month(int);
   int   day_year(int,int,int);

	int 	add_months(int inDate, int addMonth);

	int	file_time_to_julian(FILETIME& fileTime);

private:
   int32_t ytoj(int);
   int  month_day(int,int,int&,int&);
   int  get_date(int32_t,char);
};

extern DateInfo date;

#endif
