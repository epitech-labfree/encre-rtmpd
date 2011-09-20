##
## uce_longpoller.rb
## Login : <elthariel@rincevent>
## Started on  Wed Jul 20 14:41:15 2011 elthariel
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
require 'singleton'
require 'em-http'

class UceLongPoller
  include Singleton

  attr_accessor :types, :handlers

  def initialize
    @request = EM::HttpRequest.new(Conf.i.uce_url + '/event')
    @connection = nil
    @last_time = nil
    @handlers = {}
    @cred = {}

    @types = [] # 'internal.meeting.add'
            #]
  end

  def self.i
    self.instance
  end

  def start
    if @time
      #puts "Launching poller request"
      query = @cred.merge({'type' => @types.join(','),
        'start' => @time, '_async' => 'lp'})

      @connection = @request.get :query => query, :inactivity_timeout => 0
      @connection.callback {on_events}
      @connection.errback {start}
    else
      get_time
    end
  end

  def on_events
    s = @connection.response_header.status.to_i

    if s < 200 and s >= 300
      EM::Timer.new(1) { start }
    else
      r = JSON.parse @connection.response
      r['result'].each do |event|
        if @handlers.has_key? event["type"]
          h = @handlers[event["type"]]
          if h.respond_to? :each
            h.each { |p| p.call event }
          elsif h.respond_to? :call
            h.call event
          else
            puts "Wrong event handler for type : #{event["type"]}, event is #{event}"
          end
        else
          puts "Received an unhandled event : #{event}"
        end
        @time = event['datetime'].to_i + 1 if event['datetime'] >= @time
      end
    end
    start
  end

  def on_login(uid, sid)
    @cred[:uid] = uid
    @cred[:sid] = sid
    start
  end

  def get_time
    @connection = EM::HttpRequest.new(Conf.i.uce_url + '/time').get :query => @cred
    @connection.callback {on_time}
  end

  def on_time
    r = JSON.parse @connection.response
    @time = r["result"].to_i
    start
  end


end

