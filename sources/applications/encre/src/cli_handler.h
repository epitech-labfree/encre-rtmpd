/*
** cli_handler.h
** Login : <elthariel@hydre.freelab.lab.epitech.eu>
** Started on  Wed Apr 13 14:38:37 2011 Julien 'Lta' BALLET
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

#ifndef   	CLI_HANDLER_H_
# define   	CLI_HANDLER_H_

#include "protocols/cli/basecliappprotocolhandler.h"
#include "encreapplication.h"

namespace app_encre
{

class CLIAppProtocolHandler : public BaseCLIAppProtocolHandler
{
 public:
  CLIAppProtocolHandler(Variant &configuration);
  virtual ~CLIAppProtocolHandler();

  virtual bool          ProcessMessage(BaseProtocol *pFrom, Variant &message);
  // Sends a message to the client connected to the CLI interface.
  bool                  SendMessage(BaseProtocol *pFrom, Variant &message);

  EncreApplication &encre();

 protected:
  bool ProcessInvokeCommand(BaseProtocol *pFrom, Variant &message);
  bool ProcessInvokeUserNew(BaseProtocol *pFrom, Variant &command);
  bool ProcessInvokeUserDel(BaseProtocol *pFrom, Variant &command);
  bool ProcessInvokeStreamNew(BaseProtocol *pFrom, Variant &command);
  bool ProcessInvokeStreamDel(BaseProtocol *pFrom, Variant &command);
  bool ProcessInvokeStreamMute(BaseProtocol *pFrom, Variant &command);
  bool ProcessInvokeStreamRecord(BaseProtocol *pFrom, Variant &command);
  bool ProcessInvokeStreamWatcherNew(BaseProtocol *pFrom, Variant &command);
  bool ProcessInvokeStreamWatcherDel(BaseProtocol *pFrom, Variant &command);

};
}

#endif 	    /* !CLI_HANDLER_H_ */
