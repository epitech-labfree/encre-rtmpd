/* 
 *  Copyright (c) ,
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
#include "protocols/ts/tspacketpes.h"
#include "protocols/ts/tspacket.h"

TSPacketPES::TSPacketPES() {
}

TSPacketPES::~TSPacketPES() {
}

void TSPacketPES::CreatePESHeader(IOBuffer& buffer, uint32_t& cursor, uint32_t maxCursor, bool isAudio, uint64_t& pts, uint64_t& dts, uint32_t size) {
	uint32_t header;
	uint32_t tmp32;
	uint64_t ipts;
	uint64_t idts;

	header = 0x00000100;
	if (!isAudio) {
		header |= 0xE0;
	} else {
		header |= 0xC0;
	}
	TSPacket::cpyUgly((uint8_t*)&tmp32, (uint8_t*)&header, 4);
	buffer.ReadFromBuffer((uint8_t*)&tmp32, 4);
	cursor += 4;

	uint16_t length16 = 0xa + 3 + size;
	uint16_t tmp16;
	TSPacket::cpyUgly((uint8_t*)&tmp16, (uint8_t*)&length16, 2);
	buffer.ReadFromBuffer((uint8_t*)&tmp16, 2);
	cursor += 2;

	uint8_t tmp8 = 0x80;
	buffer.ReadFromBuffer(&tmp8, 1);
	cursor +=1;

	tmp8 = 0xC0;
	buffer.ReadFromBuffer(&tmp8, 1);
	cursor +=1;

	tmp8 = 0xa;
	buffer.ReadFromBuffer(&tmp8, 1);
	cursor +=1;

	ipts = pts <= 0 ? 0 : pts * 9 / 100;
	idts = dts <= 0 ? 0 : dts * 9 / 100;

	// pts
	tmp8 = 0x31;
	tmp8 |= ((ipts >> 30) & 0x3) * 0x2;
	buffer.ReadFromBuffer(&tmp8, 1);
	cursor +=1;

	tmp16 = 0x1;
	tmp16 |= ((ipts >> 15) & 0x7FFF) * 0x2;
	uint16_t tmp16_2;
	TSPacket::cpyUgly((uint8_t*)&tmp16_2, (uint8_t*)&tmp16, 2);
	buffer.ReadFromBuffer((uint8_t*)&tmp16_2, 2);
	cursor +=2;

	tmp16 = 0x1;
	tmp16 |= (ipts & 0x7FFF) * 0x2;
	TSPacket::cpyUgly((uint8_t*)&tmp16_2, (uint8_t*)&tmp16, 2);
	buffer.ReadFromBuffer((uint8_t*)&tmp16_2, 2);
	cursor +=2;

	// dts
	tmp8 = 0x31;
	tmp8 |= ((idts >> 30) & 0x3) * 0x2;
	buffer.ReadFromBuffer(&tmp8, 1);
	cursor +=1;

	tmp16 = 0x1;
	tmp16 |= ((idts >> 15) & 0x7FFF) * 0x2;
	TSPacket::cpyUgly((uint8_t*)&tmp16_2, (uint8_t*)&tmp16, 2);
	buffer.ReadFromBuffer((uint8_t*)&tmp16_2, 2);
	cursor +=2;

	tmp16 = 0x1;
	tmp16 |= (idts & 0x7FFF) * 0x2;
	TSPacket::cpyUgly((uint8_t*)&tmp16_2, (uint8_t*)&tmp16, 2);
	buffer.ReadFromBuffer((uint8_t*)&tmp16_2, 2);
	cursor +=2;

}

#endif	/* HAS_PROTOCOL_TS */
