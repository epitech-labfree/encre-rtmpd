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
#ifndef _TSPACKET_H
#define	_TSPACKET_H

#include "common.h"
#include "protocols/baseprotocol.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "protocols/ts/tspacketpmt.h"

class TSPacket {
private:
	IOBuffer _packet;

	uint8_t _syncByte;
	bool _transportErrorIndicator;
	bool _payloadUnitStartIndicator;
	bool _transportPriority;
	uint16_t _pid;
	uint8_t _scramblingControl;
	uint8_t _adaptationFieldExist;
	uint64_t _dts;
	vector<TSStreamInfo*> _programStreamType;
	map<uint16_t, uint16_t> _pmt;
	BaseProtocol *_pProtocol;

	vector<int> _adaptationFiled; // TODO change type
	vector<int> _payloadData; // TODO change type

public:
	TSPacket(BaseProtocol *_pProtocol, uint16_t pid, double timestamp);
	virtual ~TSPacket();

	operator string();
	bool CreatePacket(uint8_t* pData, uint32_t dataLength, bool isAudio, bool pat=false, bool pmt=false);
	void CreateAdaptationField(uint32_t maxData, uint8_t currentDataToCopy, uint8_t dataLength, uint32_t& cursor, bool pcr);
	bool sendData();
	static void cpyUgly(uint8_t* dest, uint8_t* src, uint32_t nb);
	static map<uint16_t, uint8_t> _continu;
	static void calCrc(uint32_t& crc, uint8_t* buff, uint32_t length);
};

#endif	/* _TSPACKET_H */
#endif	/* HAS_PROTOCOL_TS */
