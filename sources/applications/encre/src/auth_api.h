/*
** auth_api.h
** Login : <elthariel@rincevent>
** Started on  Tue Sep 20 14:46:56 2011 elthariel
** $Id$
**
** Author(s):
**  - elthariel <elthariel@gmail.com>
**
** Copyright (C) 2011 elthariel
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

#ifndef   	AUTH_API_H_
# define   	AUTH_API_H_

namespace app_encre
{
class auth
{
 public:
  enum stage
  {
    UNKOWN,
    CONNECT,
    PUBLISH,
    PLAY
  };

  virtual bool  authorize(stage id, Variant &request) = 0;
};

class encre_auth : public auth
{
 public:
  encre_auth(

  virtual bool  authorize(stage id, Variant &request);
};
}

#endif 	    /* !AUTH_API_H_ */
