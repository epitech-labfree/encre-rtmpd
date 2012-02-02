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

#include "protocols/ts/tsaudiopacket.h"
#include "protocols/ts/tspacketpes.h"

uint64_t getDate();

TSAudioPacket::TSAudioPacket(BaseProtocol* protocol, uint16_t pid, std::vector<TSStreamInfo*>& streamType)
: TSPacket(protocol, pid) {

	if (streamType.size() == 0) {
		_pcr = true;
	} else {
		_pcr = false;
	}
	TSStreamInfo* audio = new TSStreamInfo;
	audio->streamType = TS_STREAMTYPE_AUDIO_AAC;
	audio->elementaryPID = pid;
	audio->esInfoLength = 0;
	ADD_VECTOR_END(streamType, audio);
}

TSAudioPacket::~TSAudioPacket() {
}

void TSAudioPacket::AACCap(uint32_t& cursor, uint8_t* pData, uint32_t& dataLength) {
	uint8_t packetStartLol = 0xff;
	_packet.ReadFromBuffer(&packetStartLol, 1);
	packetStartLol = 0xf0;
	_packet.ReadFromBuffer(&packetStartLol, 1);
	cursor += 2;
	dataLength -= 2;
}

bool TSAudioPacket::CreatePacket(uint8_t* pData, uint32_t dataLength) {
	uint32_t cursor = 0;
	uint32_t maxData = _maxCursor - 4 - 19 - (_pcr ? 8 : 0); // packet size - header packet size - PES packet size - adaptation field

	// increase the size for AAC header
	// if (dataLength > 1) {
	// 	dataLength += 2;
	// }

	// 1. Create the header
	uint8_t currentDataToCopy = (dataLength > maxData) ? maxData : dataLength;
	if (_pcr) {
		_adaptationFieldExist |= 0x2;
	} else if (currentDataToCopy < maxData) {
		maxData -= 8; //adaptation field
		_adaptationFieldExist |= 0x2;
	}

	CreateHeader();
	cursor = 4;
	_dts = getDate() - 2000;

	// 2. Do the adaptation field
	if (_pcr) {
		CreateAdaptationField(maxData, currentDataToCopy, dataLength, cursor, _pcr, _dts);
	}

	uint32_t findPacketNumber = 0;
	findPacketNumber = (dataLength + 19) / 184 + 1;
        findPacketNumber = findPacketNumber; // FIXME Ugly NOP to Disable compilation error

	// 4. Send the PES
	uint64_t pts = getDate() - 2000;
	TSPacketPES packetPES;
	packetPES.CreatePESHeader(_packet, cursor, _maxCursor, false, pts, _dts, dataLength);

	// 5. Do the fucking work for h264
	// AACCap(cursor, pData, dataLength);

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
