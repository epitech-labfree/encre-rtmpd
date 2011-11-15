/*
** cli_handler.cpp
** Login : <elthariel@hydre.freelab.lab.epitech.eu>
** Started on  Wed Apr 13 14:39:45 2011 Julien 'Lta' BALLET
** $Id$
**
** Author(s):
**  - Julien 'Lta' BALLET <elthariel@gmail.com>
**
** Copyright (C) 2011 Julien 'Lta' BALLET
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <iostream>

#ifdef HAS_PROTOCOL_CLI
# include "cli_handler.h"

using namespace app_encre;

CLIAppProtocolHandler::CLIAppProtocolHandler(Variant &configuration)
  : BaseCLIAppProtocolHandler(configuration),
    m_client(0)
{
}

EncreApplication &CLIAppProtocolHandler::encre()
{
  return *dynamic_cast<EncreApplication *>(GetApplication());
}

CLIAppProtocolHandler::~CLIAppProtocolHandler()
{
}

/*
 * Only one Encre controller is allowed to connect, remember the
 * current controller and disconnects any other which tries to connect.
 */
void CLIAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol)
{
  if (!m_client)
  {
    WARN("Encre: A CLI controller is connecting");
  }
  else
  {
    WARN("Encre: A CLI controller is already connected");
    //pProtocol->GracefullyEnqueueForDelete();
  }
  m_client = pProtocol;
}

void CLIAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol)
{
  if (pProtocol == m_client)
    m_client = 0;
  WARN("Encre: The CLI controller is disconnecting");
}

bool CLIAppProtocolHandler::ProcessMessage(BaseProtocol *pFrom, Variant &message)
{
  Variant cmd;
  std::string json = message["command"];
  uint32_t wtf = 0;

  Variant::DeserializeFromJSON(json, cmd, wtf);

  FINEST("Encre: Received JsonCLI message %s\n%s", STR(json), STR(cmd.ToString()));

  if (ProcessInvokeCommand(pFrom, cmd))
  {
    SendSuccess(pFrom, "{\"result\":\"ok\"}", cmd);
    return true;
  }
  else
  {
    SendFail(pFrom, "{\"result\":\"nok\"}");
    return false;
  }
}

bool CLIAppProtocolHandler::SendControllerMessage(Variant &data)
{
  FINEST("Trying to send a message");

  if (m_client)
    return Send(m_client, "EVENT", "none", data);
  else
    return false;
}


bool CLIAppProtocolHandler::ProcessInvokeCommand(BaseProtocol *pFrom, Variant &command)
{
  string cmd = STR(command["command"]);

  FINEST("ProcessInvokeCommand %s:\n%s", STR(cmd), STR(command.ToString("", 1)));

  if (cmd == "user.new")
    return ProcessInvokeUserNew(pFrom, command);
  else if (cmd == "user.del")
    return ProcessInvokeUserDel(pFrom, command);
  else if (cmd == "meeting.new")
    return ProcessInvokeMeetingNew(pFrom, command);
  else if (cmd == "meeting.del")
    return ProcessInvokeMeetingDel(pFrom, command);
  else if (cmd == "stream.new")
    return ProcessInvokeStreamNew(pFrom, command);
  else if (cmd == "stream.del")
    return ProcessInvokeStreamDel(pFrom, command);
  else if (cmd == "stream.mute")
    return ProcessInvokeStreamMute(pFrom, command);
  else if (cmd == "stream.record")
    return ProcessInvokeStreamRecord(pFrom, command);
  else if (cmd == "stream.watcher.new")
    return ProcessInvokeStreamWatcherNew(pFrom, command);
  else if (cmd == "stream.watcher.del")
    return ProcessInvokeStreamWatcherDel(pFrom, command);
  else
    return false;
}

bool CLIAppProtocolHandler::ProcessInvokeMeetingNew(BaseProtocol *pFrom,
                                                    Variant &cmd)
{
  // Are all the required parameters here ?
  if (!cmd.HasKey("name"))
  {
    FINEST("Missing parameter");
    return false;
  }

  if (encre().meetings().find(cmd["name"]) == encre().meetings().end())
  {
    meeting m(cmd["name"]);
    encre().meetings()[cmd["name"]] = m;
  }
  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeMeetingDel(BaseProtocol *pFrom,
                                                    Variant &cmd)
{
  // Are all the required parameters here ?
  if (!cmd.HasKey("name"))
  {
    FINEST("Missing parameter");
    return false;
  }

  if (encre().meetings().find(cmd["name"]) != encre().meetings().end())
  {
    encre().meetings().erase(cmd["name"]);
    return true;
  }
  return false;
}

bool CLIAppProtocolHandler::ProcessInvokeUserNew(BaseProtocol *pFrom, Variant &cmd)
{
  // Are all the required parameters here ?
  if (!(cmd.HasKey("uid") && cmd.HasKey("token") && cmd.HasKey("room")))
  {
    FINEST("Missing parameter");
    return false;
  }

  // Meeting exists ?
  if (encre().meetings().find(cmd["meeting"]) == encre().meetings().end())
  {
    FINEST("Meeting %s doesn't exists", STR(cmd["meeting"]));
    return false;
  }

  if (encre().meetings()[cmd["meeting"]].exists(cmd["uid"]))
  {
    FINEST("User already exists.");
    return true;
  }

  user new_user(cmd["uid"], cmd["token"], cmd["meeting"]);
  encre().meetings()[cmd["meeting"]][cmd["uid"]] = new_user;

  FINEST("Creating new user (%s, %s) in meeting %s",
         STR(cmd["uid"]), STR(cmd["token"]), STR(cmd["room"]));

  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeUserDel(BaseProtocol *pFrom, Variant &cmd)
{
  // Are all the required parameters here ?
  if (!(cmd.HasKey("uid") && cmd.HasKey("room")))
  {
    FINEST("Missing parameter");
    return false;
  }

  // Meeting exists ?
  if (encre().meetings().find(cmd["meeting"]) == encre().meetings().end())
  {
    FINEST("Meeting %s doesn't exists", STR(cmd["meeting"]));
    return false;
  }

  if (encre().meetings()[cmd["room"]].exists(cmd["uid"]))
  {
    FINEST("User doesn't exist.");
    return false;
  }

  encre().meetings()[cmd["room"]].erase(cmd["uid"]);
  FINEST("Deleting a user");

  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeStreamNew(BaseProtocol *pFrom, Variant &command)
{
  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeStreamDel(BaseProtocol *pFrom, Variant &command)
{
  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeStreamMute(BaseProtocol *pFrom, Variant &command)
{
  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeStreamRecord(BaseProtocol *pFrom, Variant &command)
{
  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeStreamWatcherNew(BaseProtocol *pFrom, Variant &command)
{
  return true;
}

bool CLIAppProtocolHandler::ProcessInvokeStreamWatcherDel(BaseProtocol *pFrom, Variant &command)
{
  return true;
}

#endif  /* HAS_PROTOCOL_CLI */

