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
#ifndef _OUTNETTSSTREAM_H
#define	_OUTNETTSSTREAM_H

#include "streaming/baseoutnetstream.h"
#include "protocols/ts/tspacketpat.h"
#include "protocols/ts/tspacketpmt.h"
#include "protocols/ts/tsvideopacket.h"

class BaseEncreProtocol;

class DLLEXP OutNetTsStream
: public BaseOutNetStream {
protected:
	int32_t _timeForPat;
	bool _firstVideoFrame;
	bool _firstAudioFrame;
	uint32_t _AudioPID;
	TSPacketPAT _PAT;
	TSPacketPMT _PMT;
	TSVideoPacket _Video;
	
public:
	OutNetTsStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager, string name);
	virtual ~OutNetTsStream();

	virtual bool IsCompatibleWithType(uint64_t type);

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);

	virtual void SignalAttachedToInStream();
	// Called after the link is broken
	virtual void SignalDetachedFromInStream();
	// Called when the feeder finished the work
	virtual void SignalStreamCompleted();
	virtual bool Play(double absoluteTimestamp, double length);
	// brief This will pause the feeding process
	virtual bool Pause();
	// brief This will resume the feeding process
	virtual bool Resume();
	// brief  will seek to the specified point in time.
	// param absoluteTimestamp
	virtual bool Seek(double absoluteTimestamp);
	// brief This will stop the feeding process
	virtual bool Stop();
	// brief Called when a play command was issued
	// param absoluteTimestamp - the timestamp where we want to seek before start the feeding process
	// param length
	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	// brief Called when a pasue command was issued
	virtual bool SignalPause();
	// brief Called when a resume command was issued
	virtual bool SignalResume();

	// brief Called when a seek command was issued
	// @param absoluteTimestamp
	virtual bool SignalSeek(double &absoluteTimestamp);
	// brief Called when a stop command was issued
	virtual bool SignalStop();


private:
	bool FeedAudioData(uint8_t *pData, uint32_t dataLength, double absoluteTimestamp);
	bool FeedVideoData(uint8_t *pData, uint32_t dataLength, double absoluteTimestamp, uint32_t processedLength);
};


#endif	/* _OUTNETTSSTREAM_H */

#endif /* HAS_PROTOCOL_TS */
