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
    res = {}
    h.each do |k, v|
      c = context.dup << k
      if v.is_a? Hash
        res.merge!(__url_map(v, c))
      elsif v.respond_to? :to_s
        res[__context_to_string c] = v
      end
    end
    res
  end

  def flatten_for_query
    __url_map(self)
  end

  def url_encode
    __url_map(self).join("&")
  end
end

class UceEvent
  include Singleton

  def initialize
    @request = nil
    @cred = {}
  end

  def self.i
    self.instance
  end

  def on_login(uid, sid)
    @cred[:uid] = uid
    @cred[:sid] = sid
    #test_event 'test' => 1, 'test2' => {'subtest1' => 1, 'subtest2' => 2}
    #event 'test_event', { 'test' => 1, 'test2' => 2 }
  end

  def on_error(type, metadata)
    puts "UceEvent.send_event error"
    # send_event(type, metadata)
  end

  def on_response(pipe, type, metadata)
    puts "UceEvent.send_event success (#{pipe.response_header.status}))"
    # send_event(type, metadata)
  end

  def event(type, data, room = nil)
    meta = data.flatten_for_query
    query = @cred.merge({'type' => type}).merge(meta)
    @request = EM::HttpRequest.new(Conf.i.uce_url + '/event/' + room.to_s)
    pipe = @request.post :query => query
    pipe.errback { on_error type, data }
    pipe.callback { on_response pipe, type, data }
  end

   def method_missing(sym, *args)
     room = nil
     room = args[1] if args.length > 1
     self.event(sym.to_s, args[0], room)
   end
end

