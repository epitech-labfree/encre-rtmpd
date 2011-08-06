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
#include "protocols/ts/tspacketpat.h"
#include "protocols/ts/tspacket.h"
#include "protocols/ts/tsboundscheck.h"

uint8_t TSPacketPAT::_versionNumber = 31;

TSPacketPAT::TSPacketPAT() {
	//fields
	_tableId = 0;
	_sectionSyntaxIndicator = true;
	_reserved1 = false;
	_reserved2 = 0;
	_sectionLength = 0;
	_transportStreamId = 0;
	_reserved3 = 0;
	_currentNextIndicator = false;
	_sectionNumber = 0;
	_lastSectionNumber = 0;
	_crc = 0xffffffff;

	//internal variables
	_patStart = 0;
	_patLength = 0;
	_entriesCount = 0;
}

TSPacketPAT::~TSPacketPAT() {
}

TSPacketPAT::operator string() {
	string result = "";
	result += format("tableId:                %hhu\n", _tableId);
	result += format("sectionSyntaxIndicator: %hhu\n", _sectionSyntaxIndicator);
	result += format("reserved1:              %hhu\n", _reserved1);
	result += format("reserved2:              %hhu\n", _reserved2);
	result += format("sectionLength:          %hu\n", _sectionLength);
	result += format("transportStreamId:      %hu\n", _transportStreamId);
	result += format("reserved3:              %hhu\n", _reserved3);
	result += format("versionNumber:          %hhu\n", _versionNumber);
	result += format("currentNextIndicator:   %hhu\n", _currentNextIndicator);
	result += format("sectionNumber:          %hhu\n", _sectionNumber);
	result += format("lastSectionNumber:      %hhu\n", _lastSectionNumber);
	result += format("crc:                    %x\n", _crc);
	result += format("entriesCount:           %u\n", _entriesCount);
	result += format("NIT count:              %"PRIz"u\n", _networkPids.size());
	if (_networkPids.size() > 0) {

		FOR_MAP(_networkPids, uint16_t, uint16_t, i) {
			result += format("\tNIT %hu: %hu\n", MAP_KEY(i), MAP_VAL(i));
		}
	}
	result += format("PMT count:              %"PRIz"u\n", _programPids.size());
	if (_programPids.size() > 0) {

		FOR_MAP(_programPids, uint16_t, uint16_t, i) {
			result += format("\tPMT %hu: %hu\n", MAP_KEY(i), MAP_VAL(i));
		}
	}
	return result;
}

bool TSPacketPAT::Read(uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor) {
	//1. Read table id
	CHECK_BOUNDS(1);
	_tableId = pBuffer[cursor++];
	if (_tableId != 0) {
		FATAL("Invalid table id");
		return false;
	}

	//2. read section length and syntax indicator
	CHECK_BOUNDS(2);
	_sectionSyntaxIndicator = (pBuffer[cursor]&0x80) != 0;
	_reserved1 = (pBuffer[cursor]&0x40) != 0;
	_reserved2 = (pBuffer[cursor] >> 4)&0x03;
	_sectionLength = ENTOHSP((pBuffer + cursor))&0x0fff;
	cursor += 2;

	//4. Compute entries count
	_entriesCount = (_sectionLength - 9) / 4;

	//5. Read transport stream id
	CHECK_BOUNDS(2);
	_transportStreamId = ENTOHSP((pBuffer + cursor));
	cursor += 2;

	//6. read current next indicator and version
	CHECK_BOUNDS(1);
	_reserved3 = pBuffer[cursor] >> 6;
	_versionNumber = (pBuffer[cursor] >> 1)&0x1f;
	_currentNextIndicator = (pBuffer[cursor]&0x01) != 0;
	cursor++;

	//7. read section number
	CHECK_BOUNDS(1);
	_sectionNumber = pBuffer[cursor++];

	//8. read last section number
	CHECK_BOUNDS(1);
	_lastSectionNumber = pBuffer[cursor++];

	//9. read the table
	for (uint32_t i = 0; i < _entriesCount; i++) {
		//9.1. read the program number
		CHECK_BOUNDS(2);
		uint16_t programNumber = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
		cursor += 2;

		//9.2. read the network or program map pid
		CHECK_BOUNDS(2);
		uint16_t temp = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
		cursor += 2;
		temp &= 0x1fff;

		if (programNumber == 0)
			_networkPids[programNumber] = temp;
		else
			_programPids[programNumber] = temp;
	}

	//10. read the CRC
	CHECK_BOUNDS(4);
	_crc = ENTOHLP((pBuffer + cursor));
	cursor += 4;

	//12. done
	return true;
}

map<uint16_t, uint16_t> &TSPacketPAT::GetPMTs() {

	return _programPids;
}

map<uint16_t, uint16_t> &TSPacketPAT::GetNITs() {

	return _networkPids;
}

uint32_t TSPacketPAT::GetCRC() {

	return _crc;
}

uint32_t TSPacketPAT::PeekCRC(uint8_t *pBuffer, uint32_t cursor, uint32_t maxCursor) {
	//1. ignore table id
	CHECK_BOUNDS(1);
	cursor++;

	//3. read section length
	CHECK_BOUNDS(2);
	uint16_t length = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
	length &= 0x0fff;
	cursor += 2;

	//4. Move to the crc position
	CHECK_BOUNDS(length - 4);
	cursor += (length - 4);

	//5. return the crc
	CHECK_BOUNDS(4);
	return ENTOHLP((pBuffer + cursor)); //----MARKED-LONG---
}

uint8_t* TSPacketPAT::CreatePAT(IOBuffer& pBuffer, uint32_t& cursor, uint32_t maxCursor, map<uint16_t, uint16_t>& pmt) {
	uint8_t numberOfSection = pmt.size(); // only one program with audio and video
	uint8_t CurrentNextIndicator = 1;
	uint8_t lastSectionNumber = 0;

	_crc = 0xffffffff;

	uint8_t tmp8 = 0x00;
	tmp8 = 188 - (cursor + 1 + 8 + numberOfSection * 4 + 4); //PointerFiled
	pBuffer.ReadFromBuffer(&tmp8, 1);
	for (uint32_t i=0; i < tmp8; ++i) {
		pBuffer.ReadFromBuffer((uint8_t*)"\0", 1);
	}

	tmp8 = 0x00; //tableId
	pBuffer.ReadFromBuffer(&tmp8, 1);
	TSPacket::calCrc(_crc, &tmp8, 1);

	tmp8 = 0b10110000; // see http://en.wikipedia.org/wiki/Program_Specific_Information#PAT_.28Program_Association_Table.29

	uint16_t tmp16 = 2 + 1 + 1 + 1 + 4 * numberOfSection + 4; // IdStream (2) + (reserved + versionNumber + Current/Next Indicator)(1) + sectionNumber(1) + LastSectionNumber(1) + ((ProgramNumber + reserved + ProgramId)(4) * sectionNumber) + crc(4)
	tmp16 += tmp8 * 0x100;

	uint16_t tmp16_2;
	TSPacket::cpyUgly((uint8_t*)&tmp16_2, (uint8_t*)&tmp16, 2);
	pBuffer.ReadFromBuffer((uint8_t*)&tmp16_2, 2);
	TSPacket::calCrc(_crc, (uint8_t*)&tmp16_2, 2);

	tmp16 = 0;
	TSPacket::cpyUgly((uint8_t*)&tmp16_2, (uint8_t*)&tmp16, 2);
	pBuffer.ReadFromBuffer((uint8_t*)&tmp16_2, 2);
	TSPacket::calCrc(_crc, (uint8_t*)&tmp16_2, 2);

	tmp8 = (_versionNumber % 32) << 1;
	tmp8 |= (CurrentNextIndicator & 0x1);
	pBuffer.ReadFromBuffer(&tmp8, 1);
	TSPacket::calCrc(_crc, &tmp8, 1);

	pBuffer.ReadFromBuffer(&_sectionNumber, 1);
	TSPacket::calCrc(_crc, &_sectionNumber, 1);
	pBuffer.ReadFromBuffer(&lastSectionNumber, 1);
	TSPacket::calCrc(_crc, &lastSectionNumber, 1);

	uint32_t tmp32;
	uint32_t tmp32_2;
	map<uint16_t, uint16_t>::iterator it = pmt.begin();
	map<uint16_t, uint16_t>::iterator ite = pmt.end();
	while (it != ite) {
		tmp32 = it->second << 16;
		tmp32 |= (0xE000);
		tmp32 |= (it->first & 0x1fff);

		TSPacket::cpyUgly((uint8_t*)&tmp32_2, (uint8_t*)&tmp32, 4);
		TSPacket::calCrc(_crc, (uint8_t*)&tmp32_2, 4);
		pBuffer.ReadFromBuffer((uint8_t*)&tmp32_2, 4);
		++it;
	}

	TSPacket::cpyUgly((uint8_t*)&tmp32_2, (uint8_t*)&_crc, 4);
	pBuffer.ReadFromBuffer((uint8_t*)&tmp32_2, 4);
	return GETIBPOINTER(pBuffer);
}

#endif	/* HAS_PROTOCOL_TS */
