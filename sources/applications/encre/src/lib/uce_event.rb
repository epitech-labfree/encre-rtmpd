##
## uce_event.rb
## Login : <elthariel@rincevent>
## Started on  Thu Jul 21 14:04:16 2011 elthariel
## $Id$
##
## Author(s):
##  - elthariel <elthariel@gmail.com>
##
## Copyright (C) 2011 elthariel
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
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
require 'singleton'
require 'em-http'

class Hash
  def __context_to_string(c)
    res = c.shift.to_s
    c.each { |e| res += "[#{e}]" }
    res
  end

  def __url_map(h, context = [])
    res = []
    h.each do |k, v|
      c = context.dup << k
      if v.is_a? Hash
        res << __url_map(v, c)
      elsif v.respond_to? :to_s
        res << "#{__context_to_string c}=#{v}"
      end
    end
    res
  end

  def url_encode
    __url_map(self).join("&")
  end
end

class UceEvent
  include Singleton

  def initialize
    @request = EM::HttpRequest.new(Conf.i.uce_url + '/event')
    @cred = {}
  end

  def self.i
    self.instance
  end

  def on_login(uid, sid)
    @cred[:uid] = uid
    @cred[:sid] = sid
    #test_event 'test' => 1, 'test2' => {'subtest1' => 1, 'subtest2' => 2}
    test_event 'test' => 1, 'test2' => 2
  end

  def on_error(type, metadata)
    puts "UceEvent.send_event error"
    # send_event(type, metadata)
  end

  def on_response(pipe, type, metadata)
    puts "UceEvent.send_event success (#{pipe.response_header.status}))"
    # send_event(type, metadata)
  end

  def send_event(type, metadata)
    query = @cred.merge({'type' => type, 'metadata' => metadata})
    pipe = @request.post :keepalive => true, :query => query
    pipe.errback { on_error type, metadata }
    pipe.callback { on_response pipe, type, metadata }
  end

  def method_missing(sym, *args)
    send_event(sym.to_s, args[0])
  end
end

