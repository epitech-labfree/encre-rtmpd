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


#include "rtmpappprotocolhandler.h"
#include "cli_handler.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "rtmp_request.h"

using namespace app_encre;

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {

 }

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

EncreApplication &RTMPAppProtocolHandler::encre()
{
  return *dynamic_cast<EncreApplication *>(GetApplication());
}

bool        RTMPAppProtocolHandler::ProcessInvokeConnect(BaseRTMPProtocol *pFrom, Variant &request)
{
  rtmp_connect  r(request["invoke"]["parameters"], pFrom);

  FINEST("InvokeConnect params : %s", STR(request.ToString()));

  if (!(BaseRTMPAppProtocolHandler::ProcessInvokeConnect(pFrom, request)
        && r.is_valid()))
  {
    WARN("ProcessInvokeConnect: Wrong number of parameters");
    // FIXME The nice way ??
    pFrom->GracefullyEnqueueForDelete();
    return false;
  }

  FINEST("ProcessInvokeConnect (uid, token) in room = (%s, %s) in [%s]",
         STR(r.p("uid")), STR(r.p("token")), STR(r.p("room")));

  if (encre().meetings().exists(r.p("room"))
      && encre().meetings()[r.p("room")].exists(r.p("uid"))
      && encre().meetings()[r.p("room")][r.p("uid")].properties()["token"] == r.p("token"))
  {
//    Variant msg("Client connection has been authorized");

    FINEST("User authenticated");
//    encre().cli().SendControllerMessage(msg);

    pFrom->GetCustomParameters()["uid"] = r.p("uid");
    pFrom->GetCustomParameters()["token"] = r.p("token");
    pFrom->GetCustomParameters()["room"] = r.p("room");
    return true;
  }
  else
  {
    FINEST("User isn't allowed to connect (yet)");
    pFrom->EnqueueForDelete();
    return false;
  }
}

bool        RTMPAppProtocolHandler::ProcessInvokePublish(BaseRTMPProtocol *pFrom,
                                                         Variant &request)
{
  rtmp_publish  r(request["invoke"]["parameters"], pFrom);

  if (!(BaseRTMPAppProtocolHandler::ProcessInvokePublish(pFrom, request)
        && r.is_valid()))
  {
    WARN("ProcessInvokePublish: Wrong number of parameters");
    // FIXME The nice way ??
    pFrom->GracefullyEnqueueForDelete();
    return false;
  }

  pFrom->GetCustomParameters()["stream_name"] = r.p("stream_name");
  FINEST("ProcessInvokePublish: from %s in room '%s' (%s, %s)\n%s",
         STR(pFrom->GetCustomParameters()["uid"]),
         STR(pFrom->GetCustomParameters()["room"]),
         STR(r.p("stream_name")), STR(r.p("stream_mode")),
         STR(request.ToString("", 1)));

  Variant cli_msg;
  cli_msg["type"] = "stream.publish";
  cli_msg["uid"] = pFrom->GetCustomParameters()["uid"];
  cli_msg["room"] = pFrom->GetCustomParameters()["room"];
  cli_msg["stream_name"] = r.p("stream_name");
  encre().cli().SendControllerMessage(cli_msg);

  return true;
}

bool        RTMPAppProtocolHandler::ProcessInvokePlay(BaseRTMPProtocol *pFrom,
                                                      Variant &request)
{
  rtmp_play r(request["invoke"]["parameters"], pFrom);

  if (!(BaseRTMPAppProtocolHandler::ProcessInvokePlay(pFrom, request)
        && r.is_valid()))
  {
    WARN("ProcessInvokePlay: Wrong number of parameters");
    pFrom->GracefullyEnqueueForDelete();
    return false;
  }

  FINEST("ProcessInvokePlay: \n%s", STR(request.ToString("", 1)));

  return true;
}

bool        RTMPAppProtocolHandler::ProcessInvokeCloseStream(BaseRTMPProtocol *pFrom,
                                                             Variant &request)
{
  FINEST("### ProcessInvokeCloseStream");
  return BaseRTMPAppProtocolHandler::ProcessInvokeCloseStream(pFrom, request);
}

bool        RTMPAppProtocolHandler::ProcessInvokeReleaseStream(BaseRTMPProtocol *pFrom,
                                                               Variant &request)
{
  FINEST("### ProcessInvokeReleaseStream");
  return BaseRTMPAppProtocolHandler::ProcessInvokeReleaseStream(pFrom, request);
}

bool        RTMPAppProtocolHandler::ProcessInvokeDeleteStream(BaseRTMPProtocol *pFrom,
                                                              Variant &request)
{
  FINEST("### ProcessInvokeDeleteStream");
  Variant cli_msg;
  cli_msg["type"] = "stream.delete";
  cli_msg["uid"] = pFrom->GetCustomParameters()["uid"];
  cli_msg["room"] = pFrom->GetCustomParameters()["room"];
  cli_msg["stream_name"] = pFrom->GetCustomParameters()["stream_name"];
  encre().cli().SendControllerMessage(cli_msg);

  return BaseRTMPAppProtocolHandler::ProcessInvokeDeleteStream(pFrom, request);
}
