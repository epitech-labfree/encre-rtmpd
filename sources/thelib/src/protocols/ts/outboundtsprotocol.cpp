
#ifdef HAS_PROTOCOL_TS

#include "protocols/ts/outboundtsprotocol.h"
#include "protocols/protocolfactorymanager.h"
#include "netio/netio.h"
#include "protocols/ts/basetsappprotocolhandler.h"
#include "protocols/ts/basetsprotocol.h"
#include "application/clientapplicationmanager.h"

OutboundTsProtocol::OutboundTsProtocol()
: BaseTsProtocol() {
}

OutboundTsProtocol::~OutboundTsProtocol() {
}

bool OutboundTsProtocol::Connect(string ip, uint16_t port,
		Variant customParameters) {

	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_TS);
	if (chain.size() == 0) {
		FATAL("Unable to obtain protocol chain from settings: %s",
				CONF_PROTOCOL_OUTBOUND_TS);
		return false;
	}
	if (!TCPConnector<OutboundTsProtocol>::Connect(ip, port, chain,
			customParameters)) {
		FATAL("Unable to connect to %s:%hu", STR(ip), port);
		return false;
	}
	return true;
}

bool OutboundTsProtocol::PerformHandshake(IOBuffer &buffer) {
	BaseProtocol *pFarProtocol = GetFarProtocol();
	if (pFarProtocol != NULL)
		pFarProtocol->EnqueueForOutbound();
	return true;
}

bool OutboundTsProtocol::AllowFarProtocol(uint64_t type) {
	return (type == PT_TCP) || (type == PT_INBOUND_TS);
}

bool OutboundTsProtocol::AllowNearProtocol(uint64_t type) {
	ASSERT("Operation not supported");
	return false;
}

bool OutboundTsProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("Operation not supported");
	return false;
}

bool OutboundTsProtocol::SignalInputData(IOBuffer &buffer) {
	return true;
}

bool OutboundTsProtocol::SignalProtocolCreated(BaseProtocol *pProtocol,
		Variant customParameters) {
  //1. Get the application  designated for the newly created connection                                                                                                       
  if (customParameters[CONF_APPLICATION_NAME] != V_STRING) {
    FATAL("connect parameters must have an application name");
    return false;
  }
  BaseClientApplication *pApplication = ClientApplicationManager::FindAppByName(
										customParameters[CONF_APPLICATION_NAME]);
  if (pApplication == NULL) {
    FATAL("Application %s not found", STR(customParameters[CONF_APPLICATION_NAME]));
    return false;
  }

  if (pProtocol == NULL) {
    FATAL("Connection failed:\n%s", STR(customParameters.ToString()));
    return pApplication->OutboundConnectionFailed(customParameters);
  }

  //2. Set the application                                                                                                                                                    
  pProtocol->SetApplication(pApplication);


  //3. Trigger processing, including handshake                                                                                                                                
  OutboundTsProtocol *pOutboundTsProtocol = (OutboundTsProtocol *) pProtocol;
  pOutboundTsProtocol->SetOutboundConnectParameters(customParameters);
  IOBuffer dummy;
  return pOutboundTsProtocol->SignalInputData(dummy);
}

#endif /* HAS_PROTOCOL_TS */

