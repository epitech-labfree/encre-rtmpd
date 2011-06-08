#ifndef _ENCRE2APPLICATION_H
#define _ENCRE2APPLICATION_H

#include "application/baseclientapplication.h"

namespace app_encre2 {

#ifdef HAS_PROTOCOL_ENCRE
	class EncreAppProtocolHandler;
#endif /* HAS_PROTOCOL_ENCRE */
#ifdef HAS_PROTOCOL_TS
	class TSAppProtocolHandler;
#endif /* HAS_PROTOCOL_TS */

	class Encre2Application
	: public BaseClientApplication {
	private:
#ifdef HAS_PROTOCOL_ENCRE
		EncreAppProtocolHandler *_pEncreHandler;
#endif /* HAS_PROTOCOL_ENCRE */
#ifdef HAS_PROTOCOL_TS
		TSAppProtocolHandler *_pTSHandler;
#endif /* HAS_PROTOCOL_TS */

	public:
		Encre2Application(Variant &configuration);
		virtual ~Encre2Application();

		virtual bool Initialize();
		// void	SignalStreamRegistered(BaseStream *pStream);
		// void	SignalStreamUnRegistered(BaseStream *pStream);
	};
}

#endif	/* _ENCRE2APPLICATION_H */
