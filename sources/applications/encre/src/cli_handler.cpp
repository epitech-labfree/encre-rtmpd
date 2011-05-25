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

namespace app_encre
{

  CLIAppProtocolHandler::CLIAppProtocolHandler(Variant &configuration)
    : BaseCLIAppProtocolHandler(configuration)
  {

  }

  CLIAppProtocolHandler::~CLIAppProtocolHandler()
  {
  }

  bool CLIAppProtocolHandler::ProcessMessage(BaseProtocol *pFrom, Variant &message)
  {
    FINEST("Encre: Received JsonCLI message %s", message.ToString().c_str());
    return SendFail(pFrom, "Not yet implemented");
  }
}

#endif  /* HAS_PROTOCOL_CLI */

