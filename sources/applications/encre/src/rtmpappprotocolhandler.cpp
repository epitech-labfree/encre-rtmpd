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
  rtmp_connect  r(request["invoke"]["parameters"]);

  FINEST("InvokeConnect params : %s", STR(request.ToString()));

  if (!(BaseRTMPAppProtocolHandler::ProcessInvokeConnect(pFrom, request)
        && r.is_valid()))
  {
    WARN("ProcessInvokeConnect: Wrong number of parameters");
    // FIXME The nice way ??
    pFrom->GracefullyEnqueueForDelete();
    return false;
  }

  FINEST("ProcessInvokeConnect (uid, sid) in room = (%s, %s) in [%s]",
         STR(r.p("uid")), STR(r.p("sid")), STR(r.p("room")));

  //if (encre().users().exists(uid) && encre().users()[uid].properties()["sid"] == sid)
  {
    FINEST("User authenticated");
    // FIXME I'm working on this. LEAK
    encre().cli().SendControllerMessage(*(new Variant("Client is connected, test message")));

    pFrom->GetCustomParameters()["uid"] = r.p("uid");
    pFrom->GetCustomParameters()["sid"] = r.p("sid");
    pFrom->GetCustomParameters()["room"] = r.p("room");
    return true;
  }
  // else
  // {
  //   FINEST("User isn't allowed to connect (yet)");
  //   pFrom->GracefullyEnqueueForDelete();
  //   return false;
  // }
}

bool        RTMPAppProtocolHandler::ProcessInvokePublish(BaseRTMPProtocol *pFrom,
                                                         Variant &request)
{
  rtmp_publish  r(request["invoke"]["parameters"]);

  if (!(BaseRTMPAppProtocolHandler::ProcessInvokePublish(pFrom, request)
        && r.is_valid()))
  {
    WARN("ProcessInvokePublish: Wrong number of parameters");
    // FIXME The nice way ??
    pFrom->GracefullyEnqueueForDelete();
    return false;
  }

  FINEST("ProcessInvokePublish: from %s in room '%s' (%s, %s)\n%s",
         STR(pFrom->GetCustomParameters()["uid"]),
         STR(pFrom->GetCustomParameters()["room"]),
         STR(r.p("stream_name")), STR(r.p("stream_mode")),
         STR(request.ToString("", 1)));

  return true;
}

bool        RTMPAppProtocolHandler::ProcessInvokePlay(BaseRTMPProtocol *pFrom,
                                                      Variant &request)
{
  rtmp_play r(request["invoke"]["parameters"]);

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

