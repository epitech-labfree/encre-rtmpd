/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifdef HAS_PROTOCOL_TS
#include "protocols/ts/outnettsstream.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "streaming/streamstypes.h"
#include "streaming/nalutypes.h"
#include "protocols/ts/tspacket.h"
#include "protocols/ts/tspacketpat.h"
#include "protocols/ts/tspacketpmt.h"
#include "protocols/ts/tsvideopacket.h"

OutNetTsStream::OutNetTsStream(BaseProtocol *pProtocol,
			       StreamsManager *pStreamsManager, string name)
: BaseOutNetStream(pProtocol, pStreamsManager, ST_OUT_NET_TS, name), _timeForPat(0), _firstVideoFrame(true), _firstAudioFrame(true), _AudioPID(69), _PAT(pProtocol), _PMT(pProtocol, 4242), _Video(pProtocol, 68, _PMT.GetProgramStreamType()), _Audio(pProtocol, 69, _PMT.GetProgramStreamType()) {
}

OutNetTsStream::~OutNetTsStream() {
}

bool OutNetTsStream::IsCompatibleWithType(uint64_t type) {
	return TAG_KIND_OF(type, ST_IN_NET_TS);
}

bool OutNetTsStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	--_timeForPat;
	if (_timeForPat < 0) {
		_timeForPat = 50;
		//TODO : a real pmt
		map<uint16_t, uint16_t> pmt;
		if (_PAT.CreatePAT(pmt) == false) {
			return false;
		}

		if (_PMT.CreatePMT() == false) {
			return false;
		}
	}
	if (isAudio)
		return FeedAudioData(pData, dataLength, absoluteTimestamp);
	else
		return FeedVideoData(pData, dataLength, absoluteTimestamp, processedLength);
}

bool OutNetTsStream::FeedAudioData(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp) {
	return _Audio.CreatePacket(pData, dataLength);
	return true;
}

bool OutNetTsStream::FeedVideoData(uint8_t *pData, uint32_t dataLength,
		   double absoluteTimestamp, uint32_t processedLength) {
	return _Video.CreatePacket(pData, dataLength);
}

void OutNetTsStream::SignalAttachedToInStream() {
	FINEST("Link done :) ");
}

void OutNetTsStream::SignalDetachedFromInStream() {
}

// Called when the feeder finished the work
void OutNetTsStream::SignalStreamCompleted() {
}

bool OutNetTsStream::Play(double absoluteTimestamp, double length) {
	return true;
}

// brief This will pause the feeding process
bool OutNetTsStream::Pause() {
	return true;
}

// brief This will resume the feeding process
bool OutNetTsStream::Resume() {
	return true;
}

// brief  will seek to the specified point in time.
// param absoluteTimestamp
bool OutNetTsStream::Seek(double absoluteTimestamp) {
	return true;
}

// brief This will stop the feeding process
bool OutNetTsStream::Stop() {
	return true;
}

// brief Called when a play command was issued
// param absoluteTimestamp - the timestamp where we want to seek before start the feeding process
// param length
bool OutNetTsStream::SignalPlay(double &absoluteTimestamp, double &length) {
	return true;
}

bool OutNetTsStream::SignalPause() {
	return true;
}

// brief Called when a resume command was issued
bool OutNetTsStream::SignalResume() {
	return true;
}

bool OutNetTsStream::SignalSeek(double &absoluteTimestamp) {
	return true;
}

bool OutNetTsStream::SignalStop() {
	return true;
}

#endif /* HAS_PROTOCOL_TS */
