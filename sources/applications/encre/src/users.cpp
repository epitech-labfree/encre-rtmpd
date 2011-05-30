/*
** users.cpp
** Login : <elthariel@hydre>
** Started on  Thu May 26 13:56:17 2011 Julien 'Lta' BALLET
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
#include "users.h"

using namespace app_encre;

properties_map                  &has_properties::properties()
{
  return m_props;
}

user::user(std::string uid, std::string sid)
{
  properties()["uid"] = uid;
  properties()["sid"] = sid;
}

user::user(const user &o)
  : m_streams(o.m_streams)
{
}

user::~user()
{
}

user                                  &user::operator=(const user &o)
{
  m_streams = o.m_streams;
  m_props = o.m_props;

  return *this;
}

stream::list                          &user::streams()
{
  return m_streams;
}








/****************
 * stream class
 */

stream::stream(std::string uid, std::string name)
{
  properties()["owner"] = uid;
  properties()["name"] = name;
}

stream::stream(const stream &o)
{
}

stream::~stream()
{
}

stream                                  &stream::operator=(const stream &o)
{
  m_props = o.m_props;

  return *this;
}

std::string                             stream::name()
{
  return properties()["name"];
}

std::string                             stream::owner()
{
  return properties()["owner"];
}

