
#include "encre2application.h"
#include "protocols/baseprotocol.h"
#include "encreappprotocolhandler.h"
#include "tsappprotocolhandler.h"
#include "streaming/basestream.h"
#include "streaming/streamstypes.h"

using namespace app_encre2;

Encre2Application::Encre2Application(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_ENCRE
  	_pEncreHandler = NULL;
#endif /* HAS_PROTOCOL_ENCRE */
#ifdef HAS_PROTOCOL_TS
  	_pTSHandler = NULL;
#endif /* HAS_PROTOCOL_TS */
}

Encre2Application::~Encre2Application() {
#ifdef HAS_PROTOCOL_ENCRE
	UnRegisterAppProtocolHandler(PT_INBOUND_ENCRE);
	if (_pEncreHandler != NULL) {
		delete _pEncreHandler;
		_pEncreHandler = NULL;
	}
#endif /* HAS_PROTOCOL_ENCRE */
#ifdef HAS_PROTOCOL_TS
	UnRegisterAppProtocolHandler(PT_INBOUND_TS);
	if (_pTSHandler != NULL) {
		delete _pTSHandler;
		_pTSHandler = NULL;
	}
#endif /* HAS_PROTOCOL_TS */
}

bool Encre2Application::Initialize() {
#ifdef HAS_PROTOCOL_ENCRE
	_pEncreHandler = new EncreAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_TCP, _pEncreHandler);
	RegisterAppProtocolHandler(PT_INBOUND_ENCRE, _pEncreHandler);
#endif /* HAS_PROTOCOL_ENCRE */
#ifdef HAS_PROTOCOL_ENCRE
	_pTSHandler = new TSAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_TS, _pTSHandler);
#endif /* HAS_PROTOCOL_TS */
	return true;
}

// void EncreApplication::SignalStreamRegistered(BaseStream *pStream) {
// 	if (pStream->GetType() != ST_IN_NET_TS)
// 		return;

// 	BaseProtocol *pProtocol = pStream->GetProtocol();
// 	if (pProtocol == NULL) {
// 		ASSERT("Protocol is NULL!!!");
// 	}
// 	//uint32_t contextId = pProtocol->GetCustomParameters()["contextId"];
// 	// ClientContext *pContext = ClientContext::GetContext(contextId, 0, 0);
// 	// if (pContext == NULL) {
// 	// 	WARN("Context not available anymore");
// 	// 	pProtocol->EnqueueForDelete();
// 	// 	return;
// 	// }

// 	// pContext->SignalStreamRegistered(pStream);
// }

// void EncreApplication::SignalStreamUnRegistered(BaseStream *pStream) {
// 	if (pStream->GetType() != ST_IN_NET_TS)
// 		return;

// 	BaseProtocol *pProtocol = pStream->GetProtocol();
// 	if (pProtocol == NULL) {
// 		ASSERT("Protocol is NULL!!!");
// 	}
// 	// uint32_t contextId = pProtocol->GetCustomParameters()["contextId"];
// 	// ClientContext *pContext = ClientContext::GetContext(contextId, 0, 0);
// 	// if (pContext == NULL) {
// 	// 	WARN("Context not available anymore");
// 	// 	pProtocol->EnqueueForDelete();
// 	// 	return;
// 	// }

// 	// pContext->SignalStreamUnRegistered(pStream);
// }
