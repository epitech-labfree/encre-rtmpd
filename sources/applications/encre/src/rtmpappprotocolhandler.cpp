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
#include "protocols/rtmp/messagefactories/messagefactories.h"
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


bool        RTMPAppProtocolHandler::AuthenticateInbound(BaseRTMPProtocol *pFrom, Variant &request,
                                                        Variant &authState)
{
  FINEST("AuthenticateInbound");

  return true;
}

bool        RTMPAppProtocolHandler::ProcessInvokeConnect(BaseRTMPProtocol *pFrom, Variant &request)
{
  std::string uid, sid;
  Variant params = request["invoke"]["parameters"];

  if (!(BaseRTMPAppProtocolHandler::ProcessInvokeConnect(pFrom, request)
        &&params.HasKey("__index__value__1")
        && params["__index__value__1"].HasKey("__index__value__0")
        && params["__index__value__1"].HasKey("__index__value__1")))
  {
    WARN("ProcessInvokeConnect: Wrong number of parameters");
    // FIXME The nice way ??
    pFrom->GracefullyEnqueueForDelete();
    return false;
  }

  uid = STR(params["__index__value__1"]["__index__value__0"]);
  sid = STR(params["__index__value__1"]["__index__value__1"]);

  FINEST("ProcessInvokeConnect (uid, sid) = (%s, %s)", STR(uid), STR(sid));

  if (encre().users().exists(uid) && encre().users()[uid].properties()["sid"] == sid)
  {
    FINEST("User authenticated");
    return true;
  }
  else
  {
    FINEST("User isn't allowed to connect (yet)");
    pFrom->GracefullyEnqueueForDelete();
    return false;
  }
}

bool        RTMPAppProtocolHandler::ProcessInvokeCreateStream(BaseRTMPProtocol *pFrom,
                                                              Variant &request)
{
  BaseRTMPAppProtocolHandler::ProcessInvokeCreateStream(pFrom, request);

  FINEST("ProcessInvokeCreateStream");

  return true;
}

bool        RTMPAppProtocolHandler::ProcessInvokePublish(BaseRTMPProtocol *pFrom,
                                                         Variant &request)
{
  BaseRTMPAppProtocolHandler::ProcessInvokePublish(pFrom, request);

  FINEST("ProcessInvokePublish: \n%s", STR(request.ToString("", 1)));

  return true;
}

