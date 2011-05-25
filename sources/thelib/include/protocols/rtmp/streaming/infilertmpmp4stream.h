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


#ifdef HAS_PROTOCOL_RTMP
#ifndef _INFILERTMPMP4STREAM_H
#define	_INFILERTMPMP4STREAM_H

#include "protocols/rtmp/streaming/infilertmpstream.h"

class DLLEXP InFileRTMPMP4Stream
: public InFileRTMPStream {
private:
	uint8_t _videoCodecHeaderInit[5];
	uint8_t _videoCodecHeaderKeyFrame[2];
	uint8_t _videoCodecHeader[2];
	uint8_t _audioCodecHeaderInit[2];
	uint8_t _audioCodecHeader[2];
public:
	InFileRTMPMP4Stream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name);
	virtual ~InFileRTMPMP4Stream();
protected:
	virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
			IOBuffer &buffer);
	virtual bool FeedMetaData(FileClass *pFile, MediaFrame &mediaFrame);
};


#endif	/* _INFILERTMPMP4STREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

