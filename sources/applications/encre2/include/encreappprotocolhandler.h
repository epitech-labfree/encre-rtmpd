
#ifdef HAS_PROTOCOL_ENCRE
#ifndef _ENCREAPPPROTOCOLHANDLER_H
#define	_ENCREAPPPROTOCOLHANDLER_H

#include "protocols/encre/baseencreappprotocolhandler.h"
namespace app_encre2 {

	class EncreAppProtocolHandler
	: public BaseEncreAppProtocolHandler {
	public:
		EncreAppProtocolHandler(Variant &configuration);
		virtual ~EncreAppProtocolHandler();
		virtual void RegisterProtocol(BaseProtocol *pProtocol);
	};
}
#endif	/* _ENCREAPPPROTOCOLHANDLER_H */
#endif /* HAS_PROTOCOL_ENCRE */

