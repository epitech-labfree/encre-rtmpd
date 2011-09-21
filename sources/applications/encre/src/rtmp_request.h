/*
** rtmp_request.h
** Login : <elthariel@rincevent>
** Started on  Tue Sep 20 15:20:10 2011 elthariel
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

#ifndef   	RTMP_REQUEST_H_
# define   	RTMP_REQUEST_H_

# include <string>
# include "encreapplication.h"

namespace app_encre
{
  class rtmp_request
  {
  public:
    rtmp_request(Variant &request);
    rtmp_request(const rtmp_request &);
    virtual ~rtmp_request(){};

    virtual bool        is_valid() = 0;
    virtual std::string parameter(std::string param) = 0;
    virtual std::string p(std::string param) {return parameter(param);}

  private:
    rtmp_request(){};
  protected:
    Variant             m_request;
  };

  class rtmp_connect : public rtmp_request
  {
  public:
    rtmp_connect(Variant &request);
    rtmp_connect(const rtmp_connect &);

    virtual bool        is_valid();
    virtual std::string parameter(std::string param);

  private:
  };

  class rtmp_publish : public rtmp_request
  {
  public:
    rtmp_publish(Variant &request);
    rtmp_publish(const rtmp_publish &);

    virtual bool        is_valid();
    virtual std::string parameter(std::string param);

  private:
  };

  class rtmp_play : public rtmp_request
  {
  public:
    rtmp_play(Variant &request);
    rtmp_play(const rtmp_play &);

    virtual bool        is_valid();
    virtual std::string parameter(std::string param);

  private:
  };
}

#endif 	    /* !RTMP_REQUEST_H_ */
