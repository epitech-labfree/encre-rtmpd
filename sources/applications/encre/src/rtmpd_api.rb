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

require 'socket'
require 'json'

class Rtmpd
  [:server, :port, :logger].each do |e|
    eval "def #{e}()
            nil
            @conf[:#{e}] if @conf.has_key? :#{e}
          end"
  end

  def initialize(opts = {})
    @conf = { :server => '127.0.0.1', :port => 1234 }.merge opts

    @socket = Socket.tcp(server, port)
  end

  def cmd(h = {})
    @socket << h.to_json << "\n"
    logger.info @socket.readline
  end

  def user_new(uid, sid)
    cmd(:command => "user.new", :uid => uid, :sid => sid)
  end

  def user_del(uid)
    cmd(:command => "user.del", :uid => uid)
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

