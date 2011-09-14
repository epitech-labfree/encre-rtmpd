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

#ifdef HAS_PROTOCOL_ENCRE
#ifndef _BASEENCREPROTOCOL_H
#define	_BASEENCREPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/encre/baseencreappprotocolhandler.h"

class DLLEXP BaseEncreProtocol
: public BaseProtocol {
protected:
	IOBuffer		_outputBuffer;
public:
	BaseEncreProtocol(uint64_t protocolType);
	virtual ~BaseEncreProtocol();

	virtual bool			AllowFarProtocol(uint64_t type);
	virtual bool			AllowNearProtocol(uint64_t type);
	virtual bool			SignalInputData(int32_t recvAmount);
	virtual bool			SignalInputData(IOBuffer &buffer);
	virtual bool			Initialize(Variant &parameters);
	virtual IOBuffer*		GetOutputBuffer();
	virtual void			SetApplication(BaseClientApplication *pApplication);
	bool				SendRawData(uint8_t *buffer, uint32_t length);

private:
	bool				DataToSend(IOBuffer &buffer);
	bool				TypeStream(std::string&);

	bool				_bTypeStream;
	bool				_bSender;
	bool				_bReceiver;
	std::string			_bStreamName;

	BaseEncreAppProtocolHandler*	_pProtocolHandler;
};

#endif	/* _BASEENCREPROTOCOL_H */
#endif /* HAS_PROTOCOL_ENCRE */
