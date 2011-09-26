/*
** rtmp_request.cpp
** Login : <elthariel@rincevent>
** Started on  Tue Sep 20 15:27:59 2011 elthariel
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

#include <iostream>
#include "rtmp_request.h"
#include "encre_helpers.h"

namespace app_encre
{
  rtmp_request::rtmp_request(Variant &request, BaseRTMPProtocol *conn)
    : m_request(request), m_conn(conn)
  {
  }

  rtmp_request::rtmp_request(const rtmp_request &req)
    : m_request(req.m_request), m_conn(req.m_conn)
  {
  }

/*******************************
 * Rtmp InvokeConnect Request
 */

  rtmp_connect::rtmp_connect(Variant &request, BaseRTMPProtocol *conn)
    : rtmp_request(request, conn)
  {
  }

  rtmp_connect::rtmp_connect(const rtmp_connect &req)
    : rtmp_request(req)
  {
  }

  bool          rtmp_connect::is_valid()
  {
    return (HAS_IDX(m_request, 1)
            && HAS_IDX(AT(m_request, 1), 0)
            && HAS_IDX(AT(m_request, 1), 1));
  }

  std::string   rtmp_connect::parameter(std::string param)
  {
    std::string result;

    if (param == "uid")
    {
      result = STR(AT(AT(m_request, 1), 0));
    }
    else if (param == "sid")
    {
      result = STR(AT(AT(m_request, 1), 1));
    }
    else if (param == "room")
    {
      result = STR(AT(m_request, 0)["app"]);
      result = result.substr(result.find('/') + 1);
    }

    return result;
  }

/*******************************
 * Rtmp InvokePublish Request
 */

/* Example request
    <MAP name="" isArray="false">
        <MAP name="header" isArray="false">
            <UINT32 name="channelId">8</UINT32>
            <UINT8 name="headerType">0</UINT8>
            <BOOL name="isAbsolute">true</BOOL>
            <UINT32 name="messageLength">41</UINT32>
            <UINT8 name="messageType">20</UINT8>
            <UINT32 name="streamId">1</UINT32>
            <UINT32 name="timestamp">55</UINT32>
        </MAP>
        <MAP name="invoke" isArray="false">
            <STR name="functionName">publish</STR>
            <DOUBLE name="id">0.000</DOUBLE>
            <BOOL name="isFlex">false</BOOL>
            <MAP name="parameters" isArray="false">
                <NULL name="__index__value__0"></NULL>
                <STR name="__index__value__1">test_stream</STR>
                <STR name="__index__value__2">live</STR>
            </MAP>
        </MAP>
    </MAP>
 */

  rtmp_publish::rtmp_publish(Variant &request, BaseRTMPProtocol *conn)
    : rtmp_request(request, conn)
  {
  }

  rtmp_publish::rtmp_publish(const rtmp_publish &req)
    : rtmp_request(req)
  {
  }

  bool          rtmp_publish::is_valid()
  {
    return (HAS_IDX(m_request, 2));
  }

  std::string   rtmp_publish::parameter(std::string param)
  {
    std::string result;

    if (param == "stream_name")
    {
      result = STR(AT(m_request, 1));
    }
    else if (param == "stream_mode")
    {
      result = STR(AT(m_request, 2));
    }

    return result;
  }


/*******************************
 * Rtmp InvokePlay Request
 */

  rtmp_play::rtmp_play(Variant &request, BaseRTMPProtocol *conn)
    : rtmp_request(request, conn)
  {
  }

  rtmp_play::rtmp_play(const rtmp_play &req)
    : rtmp_request(req)
  {
  }

  bool          rtmp_play::is_valid()
  {
    return (HAS_IDX(m_request, 1));
  }

  std::string   rtmp_play::parameter(std::string param)
  {
    std::string result;

    if (param == "name")
    {
      result = STR(AT(m_request, 1));
    }

    return result;
  }


}

