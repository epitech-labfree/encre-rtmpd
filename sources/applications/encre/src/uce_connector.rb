#! /usr/bin/ruby1.9.1
## uce_connector.rb
## Login : <elthariel@hydre>
## Started on  Wed May 25 13:52:44 2011 Julien 'Lta' BALLET
## $Id$
##
## Author(s):
##  - Julien 'Lta' BALLET <elthariel@gmail.com>
##
## Copyright (C) 2011 Julien 'Lta' BALLET
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

require 'rubygems'
require 'logger'
require 'yaml'

$:.unshift File.dirname(File.expand_path $0) + '/lib'

require 'encre_settings'
require 'uce_event'
require 'uce_login'
require 'uce_longpoller'
require 'uce_meeting'
require 'rtmpd_api'
require 'shell'
require 'token_generator'

require 'profiler'

$log = Conf.i.logger
$log.warn "Encre rtmpd <-> ucengine connector starting up ..."

# Profiler__::start_profile
# Signal.trap('SIGINT') do
#   Profiler__.print_profile STDOUT
#   exit 0
# end

def rtmpd_event_handler(e)
  puts "##################3"
  puts e
  if e["data"]["type"] == "publish"
    puts "publish"
    UceEvent.i.ev_stream_started({ :metadata => { :user_uid => e["data"]["uid"],
                                     :room => e["data"]["room"],
                                     :name => e["data"]["stream_name"]}},
                                 e["data"]["room"])

  end
end

UceLongPoller.i.handlers["internal.roster.add"] = Proc.new do |event|
  puts "Receiving internal.roster.add (#{event["from"]})"
  UceEvent.i.ev_token({:to => event["from"], :metadata => {:token => TokenGenerator.generate(event["from"])}},
                      event["location"])
end

EM.run do
  EM.connect(Conf.i.rtmpd_server, Conf.i.rtmpd_port, RtmpdConnection).on_message { |e| rtmpd_event_handler e }
  EM.open_keyboard Shell
  UceLogin.new(Proc.new { |uid, sid| puts "Connected with #{uid}, #{sid}"},
               Proc.new { |u, s| UceLongPoller.i.on_login(u, s) },
               Proc.new { |u, s| UceEvent.i.on_login(u, s) },
               Proc.new { |u, s| UceMeeting.list(u, s) })
end

#Profiler__.print_profile STDOUT
