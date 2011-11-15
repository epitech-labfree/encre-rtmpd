/*
** room.h
** Login : <elthariel@hydre>
** Started on  Thu May 26 13:28:13 2011 Julien 'Lta' BALLET
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
**
*
* - A meeting map holds a list of the different existing meetings
* - A meeting is a hash of user, since users logs on a meeting
* - A user will probably records the list of stream he's reading/publishing
*
*/

#ifndef   	ROOM_H_
# define   	ROOM_H_

# include <string>
# include <vector>
# include <map>

#include "protocols/rtmp/basertmpappprotocolhandler.h"

namespace app_encre
{

typedef std::map<std::string, std::string> properties_map;

class has_properties
{
 public:
  properties_map                        &properties();

 protected:
  properties_map                        m_props;
};

/* class meeting : public has_properties */
/* { */
/*  public: */
/*   meeting() {} */
/*   meeting(std::string name); */
/* }; */

class stream : public has_properties
{
 public:
  typedef std::vector<std::string>      list;

  stream() {}
  stream(std::string meeting, std::string owner_uid, std::string name);
  stream(const stream &);
  ~stream();

  stream                                &operator=(const stream &);
  std::string                           name();
  std::string                           owner();
  std::string                           meeting();
};

class user : public has_properties
{
 public:

  user() {}
  user(std::string uid, std::string token, std::string meeting);
  user(const user &);
  ~user();

  user                                  &operator=(const user &);
  stream::list                          &streams();
  std::string                           meeting();
  std::string                           uid();
  std::string                           token();

 protected:
  stream::list                          m_streams;
};


class meeting : public map<std::string, user>, public has_properties
{
 public:
  meeting()
  {
    properties()["name"] = std::string("meeting_default_constructor");
  }
  meeting(std::string name)
  {
    properties()["name"] = name;
  }
  meeting(const meeting &other)
    : map<std::string, user>(other), has_properties(other)
  {
  }
  meeting       &operator=(const meeting &other)
  {
    m_props = other.m_props;
    return *this;
  }

  bool          exists(std::string user)
  {
    return find(user) != end();
  }
 private:
  //meeting() {}
};

class meeting_map : public map<std::string, meeting>
{
};

class stream_map : public map<std::string, stream>
{
};


}

#endif
