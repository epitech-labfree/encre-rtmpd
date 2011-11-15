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


#include "encreapplication.h"
#include "rtmpappprotocolhandler.h"
#include "cli_handler.h"
#include "protocols/baseprotocol.h"
#include "streaming/streamstypes.h"
using namespace app_encre;

EncreApplication::EncreApplication(Variant &configuration)
  : BaseClientApplication(configuration)
{
  FINEST("TEST");
#ifdef HAS_PROTOCOL_RTMP
  _pRTMPHandler=NULL;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_CLI
  _pCLIHandler=NULL;
#endif /* HAS_PROTOCOL_CLI */
}

EncreApplication::~EncreApplication()
{
#ifdef HAS_PROTOCOL_RTMP
  UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
  UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
  if(_pRTMPHandler!=NULL) {
    delete _pRTMPHandler;
    _pRTMPHandler=NULL;
  }
#endif /* HAS_PROTOCOL_RTMP */

#ifdef HAS_PROTOCOL_CLI
  UnRegisterAppProtocolHandler(PT_INBOUND_JSONCLI);
  if(_pCLIHandler!=NULL) {
    delete _pCLIHandler;
    _pCLIHandler=NULL;
  }
#endif /* HAS_PROTOCOL_RTMP */
}



bool EncreApplication::Initialize()
{
  //TODO: Add your app init code here
  //Things like parsing custom sections inside _configuration for example,
  //initialize the protocol handler(s)

  //1. Initialize the protocol handler(s)
#ifdef HAS_PROTOCOL_RTMP
  _pRTMPHandler = new RTMPAppProtocolHandler(_configuration);
  RegisterAppProtocolHandler(PT_INBOUND_RTMP, _pRTMPHandler);
  RegisterAppProtocolHandler(PT_OUTBOUND_RTMP, _pRTMPHandler);
#endif /* HAS_PROTOCOL_RTMP */

#ifdef HAS_PROTOCOL_CLI
  _pCLIHandler = new CLIAppProtocolHandler(_configuration);
  RegisterAppProtocolHandler(PT_INBOUND_JSONCLI, _pCLIHandler);
#endif /* HAS_PROTOCOL_CLI */

  //2. Use your custom values inside your app config node
  //I'll just print the config for now... Watch the logs
  FINEST("%s app config node:\n%s",
         STR(GetName()), STR(_configuration.ToString()));
  return true;
}

// user_map &EncreApplication::users()
// {
//   return m_users;
// }

meeting_map                 &EncreApplication::meetings()
{
  return m_meetings;
}

stream_map                  &EncreApplication::streams()
{
  return m_streams;
}

#ifdef HAS_PROTOCOL_RTMP
RTMPAppProtocolHandler      &EncreApplication::rtmp()
{
  return *_pRTMPHandler;
}
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_CLI
CLIAppProtocolHandler       &EncreApplication::cli()
{
  return *_pCLIHandler;
}
#endif /* HAS_PROTOCOL_CLI */

void                        EncreApplication::SignalStreamUnRegistered(BaseStream *pStream)
{
  BaseClientApplication::SignalStreamUnRegistered(pStream);

  if (pStream->GetType() == ST_OUT_FILE_RTMP_FLV)
  {
    Variant msg;
    INFO("Encre: End of recording: '%s', requesting upload to uce",
         STR(pStream->GetName()));
    msg["type"] = "new_record";
    msg["path"] = STR(pStream->GetName());
    cli().SendControllerMessage(msg);
  }

  // INFO("Stream %u of type %u with name `%s` unregistered from application",
  //     pStream->GetUniqueId(),
  //      (unsigned)pStream->GetType(),
  //     STR(pStream->GetName()));
}

