#ifdef HAS_PROTOCOL_ENCRE
#include "encreappprotocolhandler.h"
#include "protocols/encre/baseencreprotocol.h"
#include "application/clientapplicationmanager.h"
#include "protocols/protocolmanager.h"
#include "netio/netio.h"
using namespace app_encre2;

EncreAppProtocolHandler::EncreAppProtocolHandler(Variant &configuration)
: BaseEncreAppProtocolHandler(configuration) {
}

EncreAppProtocolHandler::~EncreAppProtocolHandler() {
}

void EncreAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	//1. Get the TS protocol ID from the parameters
	uint32_t tsId = pProtocol->GetCustomParameters()["tsId"];

	//2. Get the TS protocol
	BaseProtocol *pTSProtocol = ProtocolManager::GetProtocol(tsId);
	if (pTSProtocol == NULL) {
		FATAL("Unable to get TS protocol by id: %u", tsId);
		pProtocol->EnqueueForDelete();
		return;
	}

	//3. Link them
	pProtocol->SetNearProtocol(pTSProtocol);
	pTSProtocol->SetFarProtocol(pProtocol);
}

#endif /* HAS_PROTOCOL_ENCRE */
