
#ifdef HAS_PROTOCOL_TS
#ifndef _TSAPPPROTOCOLHANDLER_H
#define	_TSAPPPROTOCOLHANDLER_H

#include "protocols/ts/basetsappprotocolhandler.h"

namespace app_encre2 {

	class TSAppProtocolHandler
	: public BaseTSAppProtocolHandler {
	public:
		TSAppProtocolHandler(Variant &configuration);
		virtual ~TSAppProtocolHandler();
	};
}

#endif	/* _TSAPPPROTOCOLHANDLER_H */
#endif	/* HAS_PROTOCOL_TS */
