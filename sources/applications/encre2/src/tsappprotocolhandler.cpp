#ifdef HAS_PROTOCOL_TS
#include "tsappprotocolhandler.h"
#include "application/baseclientapplication.h"
#include "protocols/ts/tspacketpmt.h"
#include "protocols/rtmp/sharedobjects/so.h"
#include "protocols/rtmp/sharedobjects/somanager.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/protocoltypes.h"
using namespace app_encre2;

TSAppProtocolHandler::TSAppProtocolHandler(Variant &configuration)
: BaseTSAppProtocolHandler(configuration) {

}

TSAppProtocolHandler::~TSAppProtocolHandler() {
}

#endif	/* HAS_PROTOCOL_TS */

