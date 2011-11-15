##
## uce_login.rb
## Login : <elthariel@rincevent>
## Started on  Wed Jul 20 15:00:41 2011 elthariel
## $Id$
##
## Author(s):
##  - elthariel <elthariel@gmail.com>
##
## Copyright (C) 2011 elthariel
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##

require 'eventmachine'
require 'em-http'
require 'json'

class UceLogin
  def initialize(*args)
    @callbacks = args

    @request = EM::HttpRequest.new(Conf.i.uce_url + '/presence')
    @request = @request.post :query => {:name => Conf.i.uce_uid, :credential => Conf.i.uce_token}
    @request.callback {on_login}
    @request.errback {on_error}
  end

  def on_login
    # puts @request.response_header.status
    # puts @request.response

    status = @request.response_header.status.to_i

    if status >= 200 and status < 300
      r = JSON.parse @request.response
      @callbacks.each { |c| c.call r['result']['uid'], r['result']['sid'] }
    else
      on_error
    end
  end

  def on_error
    Conf.i.logger.error "Unable to login to UCEngine"
    exit 42
  end
end




