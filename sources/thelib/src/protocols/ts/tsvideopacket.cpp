/*
 *  Copyright (c) 2011,
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

#include "protocols/ts/tsvideopacket.h"
#include "protocols/ts/tspacketpes.h"

TSVideoPacket::TSVideoPacket(BaseProtocol* protocol, uint16_t pid, std::vector<TSStreamInfo*>& streamType)
: TSPacket(protocol, pid) {
	if (streamType.size() == 0) {
		_pcr = true;
	} else {
		_pcr = false;
	}

	TSStreamInfo* video = new TSStreamInfo;
	video->streamType = TS_STREAMTYPE_VIDEO_H264;
	video->elementaryPID = pid;
	video->esInfoLength = 0;
	ADD_VECTOR_END(streamType, video);
}

TSVideoPacket::~TSVideoPacket() {
}

uint64_t getDate() {
	uint64_t res;
	struct timespec ts;

	if( clock_gettime( CLOCK_MONOTONIC, &ts ) == EINVAL ) {
		(void)clock_gettime( CLOCK_REALTIME, &ts );
	}

	res = ((uint64_t)ts.tv_sec * (uint64_t)1000000)
	      + (uint64_t)(ts.tv_nsec / 1000);
	return res;
}


void TSVideoPacket::H264Cap(uint32_t& cursor, uint8_t* pData, uint32_t& dataLength) {
	uint8_t packetStartLol = 0x0;
	_packet.ReadFromBuffer(&packetStartLol, 1);
	_packet.ReadFromBuffer(&packetStartLol, 1);
	cursor += 2;
	if (pData[1] != 0) {
		packetStartLol = 0x00;
		_packet.ReadFromBuffer(&packetStartLol, 1);
		cursor += 1;
		dataLength -= 1;
	}
	packetStartLol = 0x01;
	_packet.ReadFromBuffer(&packetStartLol, 1);
	cursor += 1;
	dataLength -= 3;
}

bool TSVideoPacket::CreatePacket(uint8_t* pData, uint32_t dataLength) {
	uint32_t cursor = 0;
	uint32_t maxData = _maxCursor - 4 - 19 - (_pcr ? 8 : 0); // packet size - header packet size - PES packet size - adaptation field

	// increase the size for h264 header
	if (dataLength > 1) {
		dataLength += 3;
	}
	if (dataLength > 1 && pData[1] != 0) {
		dataLength += 1;
	}

	// 1. Create the header
	uint8_t currentDataToCopy = (dataLength > maxData) ? maxData : dataLength;
	if (_pcr) {
		_adaptationFieldExist |= 0x2;
	} else if (currentDataToCopy < maxData) {
		_adaptationFieldExist |= 0x2;
		maxData -= 8; //adaptation field
	}

	CreateHeader();
	cursor = 4;
	_dts = getDate() - 2000;

	// 2. Do the adaptation field
	if (_adaptationFieldExist & 0x2) {
		CreateAdaptationField(maxData, currentDataToCopy, dataLength, cursor, _pcr, _dts);
	}

	uint32_t findPacketNumber = 0;
	findPacketNumber = (dataLength + 19) / 184 + 1;

	// 4. Send the PES
	uint64_t pts = getDate() - 2000;
	TSPacketPES packetPES;
	packetPES.CreatePESHeader(_packet, cursor, _maxCursor, false, pts, _dts, dataLength);

	// 5. Do the fucking work for h264
	H264Cap(cursor, pData, dataLength);

	uint32_t useDataLength = 0;
	uint32_t length = (_maxCursor - cursor > dataLength - useDataLength) ? dataLength - useDataLength : (_maxCursor - cursor);
	_packet.ReadFromBuffer(pData, length);
	useDataLength += length;
	pData += length;
	cursor += length;
	for (; cursor < _maxCursor; ++cursor) {
		uint8_t tmp8 = 0xFF;
		_packet.ReadFromBuffer(&tmp8, 1);
	}

	// 5. copy the data in the packets
	for (; useDataLength < dataLength; ) {
		_adaptationFieldExist = 0x1;
		_payloadUnitStartIndicator = 0;

		CreateHeader();
		cursor = 4;

		length = (_maxCursor - cursor > dataLength - useDataLength) ? dataLength - useDataLength : (_maxCursor - cursor);
		_packet.ReadFromBuffer(pData, length);
		cursor += length;
		useDataLength += length;
		pData += length;
		for (; cursor < _maxCursor; ++cursor) {
			uint8_t tmp8 = 0x00;
			_packet.ReadFromBuffer(&tmp8, 1);
		}
	}

	if (sendData() == false) {
		return false;
	}
	_packet.IgnoreAll();
	_payloadUnitStartIndicator = true;
	return true;
}

#endif /* HAS_PROTOCOL_TS */
