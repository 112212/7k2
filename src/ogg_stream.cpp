/*
 * Seven Kingdoms 2: The Fryhtan War
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
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include <all.h>
#include <dbglog.h>
#include <file_input_stream.h>
#include <file_util.h>
#include <ogg_stream.h>

#include <sndfile.h>

#include <algorithm>

DBGLOG_DEFAULT_CHANNEL(Audio);


#define BYTES_PER_FRAME 2

OggStream::OggStream() {
	file = 0;
	cursor = 0;
	frames = 0;
	info = 0;
}

OggStream::~OggStream() {
	this->close();
	if(info) delete info;
	if(frames) delete[] frames;
}

bool OggStream::open(const char *file_name) {
	info = new SF_INFO;
	file = sf_open(file_name, SFM_READ, info);
	// printf("reading %d frames\n", info->frames);
	// read all frames
	if(!file) {
		printf("FAILED TO OPEN %s\n", file_name);
		return false;
	}
	
	frames = new short[info->frames * info->channels];
	sf_readf_short(file, frames, info->frames);
	cursor = 0;
	
	sf_close(file);
	return true;
}


bool OggStream::seek(size_t frame_no) {
	// return sf_seek(file, frame_no, SEEK_SET);
	cursor = frame_no;
	return true;
}

void OggStream::close() {
	// if(file) { sf_close(file); delete info; }
	file = 0;
}

long OggStream::read(void *buffer, size_t frame_count) {
	// return sf_readf_short(file, (short*)buffer, frame_count);
	long num_frames = std::min<long>(frame_count, info->frames - cursor);
	if(num_frames <= 0) return 0;
	
	memcpy( buffer, frames + cursor * info->channels, num_frames * sizeof(short) * info->channels);
	cursor += num_frames;
	return num_frames;
}

int32_t OggStream::frame_rate() const {
	return info->samplerate;
}

int OggStream::channels() const {
	return info->channels;
}

int OggStream::sample_size() const {
	return BYTES_PER_FRAME;
}
