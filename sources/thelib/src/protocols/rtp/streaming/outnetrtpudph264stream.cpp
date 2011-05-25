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


#ifdef HAS_PROTOCOL_RTP
#include "protocols/rtp/streaming/outnetrtpudph264stream.h"
#include "streaming/nalutypes.h"
#include "streaming/streamstypes.h"
#include "streaming/baseinstream.h"
#include "protocols/rtp/connectivity/outboundconnectivity.h"

#define MAX_RTP_PACKET_SIZE 1350

OutNetRTPUDPH264Stream::OutNetRTPUDPH264Stream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseOutNetRTPUDPStream(pProtocol, pStreamsManager, name) {
	memset(&_videoData, 0, sizeof (_videoData));
	_videoData.msg_iov = new iovec[2];
	_videoData.msg_iovlen = 2;
	_videoData.msg_namelen = sizeof (sockaddr_in);
	_videoData.msg_iov[0].iov_base = new uint8_t[14];
	((uint8_t *) _videoData.msg_iov[0].iov_base)[0] = 0x80;
	EHTONLP(((uint8_t *) _videoData.msg_iov[0].iov_base) + 8, _ssrc);
	_pSPS = NULL;
	_SPSLen = 0;
	_pPPS = NULL;
	_PPSLen = 0;

	memset(&_audioData, 0, sizeof (_audioData));
	_audioData.msg_iov = new iovec[3];
	_audioData.msg_iovlen = 3;
	_audioData.msg_namelen = sizeof (sockaddr_in);
	_audioData.msg_iov[0].iov_len = 14;
	_audioData.msg_iov[0].iov_base = new uint8_t[14];
	((uint8_t *) _audioData.msg_iov[0].iov_base)[0] = 0x80;
	((uint8_t *) _audioData.msg_iov[0].iov_base)[1] = 0xe0;
	EHTONLP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 8, _ssrc);
	_audioData.msg_iov[1].iov_len = 0;
	_audioData.msg_iov[1].iov_base = new uint8_t[16];
	_audioPacketsCount = 0;
}

OutNetRTPUDPH264Stream::~OutNetRTPUDPH264Stream() {
	delete[] (uint8_t *) _videoData.msg_iov[0].iov_base;
	delete[] _videoData.msg_iov;
	memset(&_videoData, 0, sizeof (_videoData));
	if (_pSPS != NULL)
		delete[] _pSPS;
	if (_pPPS != NULL)
		delete[] _pPPS;

	delete[] (uint8_t *) _audioData.msg_iov[0].iov_base;
	delete[] (uint8_t *) _audioData.msg_iov[1].iov_base;
	delete[] _audioData.msg_iov;
	memset(&_audioData, 0, sizeof (_audioData));
}

bool OutNetRTPUDPH264Stream::FeedDataVideo(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	if (_pInStream->GetType() == ST_IN_NET_RTMP) {
		if (processedLength == 0) {
			if (pData[1] != 1)
				return true;
			_videoBuffer.IgnoreAll();
		}
		_videoBuffer.ReadFromBuffer(pData, dataLength);
		if (dataLength + processedLength == totalLength) {
			//Last chunk. Do the damage
			pData = GETIBPOINTER(_videoBuffer);
			uint32_t dataLength = GETAVAILABLEBYTESCOUNT(_videoBuffer);
			if (dataLength < 9) {
				WARN("Bogus packet");
				return true;
			}
			pData += 5;
			dataLength -= 5;
			uint32_t chunkSize = 0;
			uint32_t tsIncrement = 0;
			while (dataLength >= 4) {
				chunkSize = ENTOHLP(pData);
				if (chunkSize > (dataLength - 4)) {
					WARN("Bogus packet");
					return true;
				}
				pData += 4;
				dataLength -= 4;
				if (chunkSize == 0)
					continue;
				if (!FeedDataVideoFUA(pData, chunkSize, 0, chunkSize,
						absoluteTimestamp + (double) tsIncrement / 90000.00)) {
					FATAL("Unable to feed data");
					return false;
				}
				pData += chunkSize;
				dataLength -= chunkSize;
			}
		}
		return true;
	} else {
		return FeedDataVideoFUA(pData, dataLength, processedLength, totalLength,
				absoluteTimestamp);
	}
}

bool OutNetRTPUDPH264Stream::FeedDataAudio(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	return FeedDataAudioMPEG4Generic(pData, dataLength, processedLength, totalLength,
			absoluteTimestamp);
}

void OutNetRTPUDPH264Stream::SignalAttachedToInStream() {
	StreamCapabilities *pCapabilities = GetCapabilities();

	_SPSLen = pCapabilities->avc._spsLength + 12;
	_pSPS = new uint8_t[_SPSLen];
	_pSPS[0] = 0x80;
	_pSPS[1] = 0xE1;
	EHTONLP(_pSPS + 8, _ssrc);
	memcpy(_pSPS + 12, pCapabilities->avc._pSPS,
			pCapabilities->avc._spsLength);

	_PPSLen = pCapabilities->avc._ppsLength + 12;
	_pPPS = new uint8_t[_PPSLen];
	_pPPS[0] = 0x80;
	_pPPS[1] = 0xE1;
	EHTONLP(_pPPS + 8, _ssrc);
	memcpy(_pPPS + 12, pCapabilities->avc._pPPS,
			pCapabilities->avc._ppsLength);
}

bool OutNetRTPUDPH264Stream::FeedDataVideoFUA(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp) {
	if (
			(NALU_TYPE(pData[0]) != NALU_TYPE_SLICE)
			&& (NALU_TYPE(pData[0]) != NALU_TYPE_IDR)) {
		return true;
	}

	uint32_t sentAmount = 0;
	uint32_t chunkSize = 0;
	while (sentAmount != dataLength) {
		chunkSize = dataLength - sentAmount;
		chunkSize = chunkSize < MAX_RTP_PACKET_SIZE ? chunkSize : MAX_RTP_PACKET_SIZE;

		//1. Flags
		if (processedLength + sentAmount + chunkSize == totalLength) {
			((uint8_t *) _videoData.msg_iov[0].iov_base)[1] = 0xe1;
		} else {
			((uint8_t *) _videoData.msg_iov[0].iov_base)[1] = 0x61;
		}

		//2. counter
		EHTONSP(((uint8_t *) _videoData.msg_iov[0].iov_base) + 2, _videoCounter);
		_videoCounter++;

		//3. Timestamp
		EHTONLP(((uint8_t *) _videoData.msg_iov[0].iov_base) + 4,
				BaseConnectivity::ToRTPTS(absoluteTimestamp, 90000));

		if (chunkSize == totalLength) {
			//4. No chunking
			_videoData.msg_iov[0].iov_len = 12;
			_videoData.msg_iov[1].iov_base = pData;
			_videoData.msg_iov[1].iov_len = chunkSize;
		} else {
			//5. Chunking
			_videoData.msg_iov[0].iov_len = 14;

			if (processedLength + sentAmount == 0) {
				//6. First chunk
				((uint8_t *) _videoData.msg_iov[0].iov_base)[12] = (pData[0]&0xe0) | NALU_TYPE_FUA;
				((uint8_t *) _videoData.msg_iov[0].iov_base)[13] = (pData[0]&0x1f) | 0x80;
				_videoData.msg_iov[1].iov_base = pData + 1;
				_videoData.msg_iov[1].iov_len = chunkSize - 1;
			} else {
				if (processedLength + sentAmount + chunkSize == totalLength) {
					//7. Last chunk
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] &= 0x1f;
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] |= 0x40;
				} else {
					//8. Middle chunk
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] &= 0x1f;
				}
				_videoData.msg_iov[1].iov_base = pData;
				_videoData.msg_iov[1].iov_len = chunkSize;
			}
		}

		_pConnectivity->FeedVideoData(_videoData);
		sentAmount += chunkSize;
		pData += chunkSize;
	}

	return true;
}

bool OutNetRTPUDPH264Stream::FeedDataAudioMPEG4Generic_aggregate(uint8_t *pData,
		uint32_t dataLength, uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp) {
	//1. We only support frame-by-frame approach
	if (dataLength != totalLength) {
		WARN("Chunked mode not yet supported");
		return true;
	}

	//2. Test if we need to send what we have so far
	if (((14 + _audioData.msg_iov[1].iov_len + GETAVAILABLEBYTESCOUNT(_audioBuffer) + 2 + dataLength - 7) > MAX_RTP_PACKET_SIZE)
			|| (_audioData.msg_iov[1].iov_len == 16)) {
		//3. counter
		EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 2, _audioCounter);
		_audioCounter++;

		//4. Timestamp
		EHTONLP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 4,
				BaseConnectivity::ToRTPTS(absoluteTimestamp,
				GetCapabilities()->aac._sampleRate));

		//6. put the actual buffer
		_audioData.msg_iov[2].iov_len = GETAVAILABLEBYTESCOUNT(_audioBuffer);
		_audioData.msg_iov[2].iov_base = GETIBPOINTER(_audioBuffer);

		EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 12,
				_audioData.msg_iov[1].iov_len * 8);

		_pConnectivity->FeedAudioData(_audioData);

		_audioBuffer.IgnoreAll();
		_audioData.msg_iov[1].iov_len = 0;
	}

	//3. AU-Header
	uint16_t auHeader = (uint16_t) ((dataLength - 7) << 3);
	auHeader = auHeader | ((uint8_t) (_audioData.msg_iov[1].iov_len / 2));
	EHTONSP(((uint8_t *) _audioData.msg_iov[1].iov_base) + _audioData.msg_iov[1].iov_len, auHeader);
	_audioData.msg_iov[1].iov_len += 2;


	//4. Save the buffer
	_audioBuffer.ReadFromBuffer(pData + 7, dataLength - 7);

	return true;
}

bool OutNetRTPUDPH264Stream::FeedDataAudioMPEG4Generic_one_by_one(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength, double absoluteTimestamp) {

	//1. Take care of chunked content first
	//this will update pData and dataLength if necessary
	if (dataLength != totalLength) {
		//chunked
		if (processedLength == 0) {
			//beginning of the packet
			_audioBuffer.IgnoreAll();
			_audioBuffer.ReadFromBuffer(pData, dataLength);
			return true;
		} else {
			if (totalLength < processedLength + dataLength) {
				//middle
				if (GETAVAILABLEBYTESCOUNT(_audioBuffer) == 0) {
					//we don't have the beginning of the packet. We started
					//directly in the middle
					return true;
				}
				_audioBuffer.ReadFromBuffer(pData, dataLength);
				return true;
			} else {
				//last
				if (GETAVAILABLEBYTESCOUNT(_audioBuffer) == 0) {
					//we don't have the beginning of the packet. We started
					//directly in the middle
					return true;
				}
				_audioBuffer.ReadFromBuffer(pData, dataLength);
				pData = GETIBPOINTER(_audioBuffer);
				dataLength = GETAVAILABLEBYTESCOUNT(_audioBuffer);
				if (dataLength != totalLength) {
					ASSERT("We should not be here!!!!");
					return false;
				}
			}
		}
	}


	//2. Do we have enough data?
	if (dataLength <= 7) {
		WARN("Bogus AAC packet");
		_audioBuffer.IgnoreAll();
		return true;
	}

	//3. Take care of the RTMP headers if necessary
	if (_pInStream->GetType() == ST_IN_NET_RTMP) {
		if (pData[1] != 1) {
			WARN("This is a RTMP audio config packet");
			_audioBuffer.IgnoreAll();
			return true;
		}
		if (dataLength <= 9) {
			WARN("Bogus AAC packet");
			_audioBuffer.IgnoreAll();
			return true;
		}

		dataLength -= 2;
		pData += 2;
	}

	//4. The packet might start with an ADTS header. Remove it if necessary
	uint32_t adtsHeaderLength = 0;
	if ((ENTOHSP(pData) >> 3) == 0x1fff) {
		adtsHeaderLength = 7;
	}

	/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           timestamp                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           synchronization source (SSRC) identifier            |
   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
   |            contributing source (CSRC) identifiers             |
   |                             ....                              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- .. -+-+-+-+-+-+-+-+-+-+
   |AU-headers-length|AU-header|AU-header|      |AU-header|padding|
   |                 |   (1)   |   (2)   |      |   (n)   | bits  |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- .. -+-+-+-+-+-+-+-+-+-+
	 */

	//5. counter
	EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 2, _audioCounter);
	_audioCounter++;

	//6. Timestamp
	EHTONLP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 4,
			(uint32_t) (absoluteTimestamp
			* (double) GetCapabilities()->aac._sampleRate / 1000.000));

	EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 12, 16);

	uint16_t auHeader = (uint16_t) ((dataLength - adtsHeaderLength) << 3);
	EHTONSP(((uint8_t *) _audioData.msg_iov[1].iov_base), auHeader);
	_audioData.msg_iov[1].iov_len = 2;

	//7. put the actual buffer
	_audioData.msg_iov[2].iov_len = dataLength - adtsHeaderLength;
	_audioData.msg_iov[2].iov_base = pData + adtsHeaderLength;

	if (!_pConnectivity->FeedAudioData(_audioData)) {
		FATAL("Unable to feed data");
		_audioBuffer.IgnoreAll();
		return false;
	}

	_audioBuffer.IgnoreAll();
	return true;
}
#endif /* HAS_PROTOCOL_RTP */

