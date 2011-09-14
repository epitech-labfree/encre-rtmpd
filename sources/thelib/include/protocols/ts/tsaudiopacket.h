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
#ifndef _TSAUDIOPACKET_H
#define	_TSAUDIOPACKET_H

#include "common.h"
#include "protocols/ts/tspacket.h"
#include "protocols/ts/tspacketpmt.h"

class TSAudioPacket : public TSPacket {
private:
	void AACCap(uint32_t& cursor, uint8_t* pData, uint32_t& dataLength);
	uint64_t _dts;
	bool _pcr;

public:
	TSAudioPacket(BaseProtocol *_pProtocol, uint16_t pid, std::vector<TSStreamInfo*>& streamType);
	virtual ~TSAudioPacket();
	bool CreatePacket(uint8_t* pData, uint32_t dataLength);
};

#endif	/* _TSAUDIOPACKET_H */
#endif	/* HAS_PROTOCOL_TS */
