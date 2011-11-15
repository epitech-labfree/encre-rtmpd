##
## rtmpd_api.rb
## Login : <elthariel@hydre>
## Started on  Wed May 25 16:27:22 2011 Julien 'Lta' BALLET
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

require 'eventmachine'
require 'singleton'
require 'json'

class RtmpdConnection < EM::Connection
  def initialize(*args)
    super
  end

  def post_init
    @buffer = String.new
    @message_callback = nil
    Rtmpd.i.send = Proc.new { |data| send_data data }
    #send_data({:command => "nothinh", :test => 23}.to_json.to_s + "\n")
  end

  def receive_data(data)
    data.split("\r\n\r\n").each do |line|
      msg = JSON.parse line
      _on_message msg
    end
  end

  def on_message(&block)
    @message_callback = block
  end

  def _on_message(message)
    @message_callback.call(message) if @message_callback
  end

  def unbind
    Conf.i.logger.warn "Connection to Rtmpd closed :/"
    EM::Timer.new(1) do
      Conf.i.logger.warn "Trying to reconnect to Rtmpd"
      EM.connect(Conf.i.rtmpd_server, Conf.i.rtmpd_port, RtmpdConnection)
    end
  end
end

class Rtmpd
  include Singleton

  attr_accessor :send

  def initialize()
  end

  def self.i
    self.instance
  end

  def cmd(h = {})
    @send.call h.to_json + "\n" if @send
  end

  def user_new(uid, sid)
    cmd(:command => "user.new", :uid => uid, :sid => sid)
  end

  def user_del(uid)
    cmd(:command => "user.del", :uid => uid)
  end

  def meeting_new(meeting)
    cmd(:command => "meeting.new", :name => meeting)
  end

  def meeting_del(meeting)
    cmd(:command => "meeting.del", :name => meeting)
  end

  def stream_new(uid, stream)
    cmd(:command => "stream.new", :uid => uid, :stream => stream)
  end

  def stream_del(uid, stream)
    cmd(:command => "stream.del", :uid => uid, :stream => stream)
  end

  def stream_mute(uid, stream, audio, video)
    logger.warn "FIXME: stream.mute is currently not supported by rtmpd"
    cmd(:command => "stream.mute", :uid => uid, :stream => stream, :audio => audio, :video => video)
  end

  def stream_record(uid, stream)
    cmd(:command => "stream.record", :uid => uid, :stream => stream)
  end

  def stream_watcher_new(uid, stream, watcher_uid)
    cmd(:command => "stream.watcher.new", :uid => uid, :stream => stream, :watcher_uid => watcher_uid)
  end

  def stream_watcher_del(uid, stream, watcher_uid)
    cmd(:command => "stream.watcher.new", :uid => uid, :stream => stream, :watcher_uid => watcher_uid)
  end
end

