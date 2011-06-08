#ifdef HAS_PROTOCOL_TS
#ifndef _OUTBOUNDTSPROTOCOL_H
#define	_OUTBOUNDTSPROTOCOL_H

#include "protocols/ts/basetsprotocol.h"

class DLLEXP OutboundTsProtocol
: public BaseTsProtocol {
public:
	OutboundTsProtocol();
	virtual ~OutboundTsProtocol();

	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	static bool Connect(string ip, uint16_t port, Variant customParameters);
	static bool SignalProtocolCreated(BaseProtocol *pProtocol, Variant customParameters);

protected:
	virtual bool PerformHandshake(IOBuffer &buffer);
};

#endif	/* _OUTBOUNDTSPROTOCOL_H */

#endif /* HAS_PROTOCOL_TS */

