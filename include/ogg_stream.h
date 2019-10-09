/*
 * Seven Kingdoms 2: The Fryhtan War
 * WavStream
 *
 * Copyright 2010 Unavowed <unavowed@vexillium.org>
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

#ifndef OGG_STREAM_H
#define OGG_STREAM_H

#include <audio_stream.h>
#include <input_stream.h>
#include <sndfile.h>

class OggStream: public AudioStream
{
private:
   SNDFILE* file;
   SF_INFO* info;
   short* frames;
   int cursor;
   
public:
   OggStream();
   ~OggStream();
   
   bool open(const char *file_name);
   void close();
   long read(void *buffer, size_t frame_count);
   bool seek(size_t frame_no);
   
   int32_t frame_rate() const;
   int channels() const;
   int sample_size() const;
};

#endif
