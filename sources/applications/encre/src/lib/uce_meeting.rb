##
## uce_meeting.rb
## Login : <elthariel@hydre>
## Started on  Tue Nov 15 14:38:54 2011 Julien 'Lta' BALLET
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

require 'eventmachine'
require 'em-http'
require 'json'

module UceMeeting
  def self.list(u, s)
    @@request = EM::HttpRequest.new(Conf.i.uce_url + '/meeting')
    @@request = @@request.get :query => {:uid => u, :sid => s}
    @@request.errback { self.on_error }
    @@request.callback {self.on_list}

    UceLongPoller.i.handlers["internal.meeting.add"] = Proc.new { |e| self.on_meeting_add e }
    UceLongPoller.i.handlers["internal.meeting.delete"] = Proc.new { |e| self.on_meeting_delete e }
  end

  def self.on_list
    status = @@request.response_header.status.to_i

    if status >= 200 and status < 300
      r = JSON.parse @@request.response
      r["result"].each do |meeting|
        Rtmpd.i.meeting_new(meeting["name"])
      end
    else
      self.on_error
    end
  end

  def self.on_meeting_add(event)
    Rtmpd.i.meeting_new(event["location"])
  end

  def self.on_meeting_delete(event)
    ## FIXME Crashes the server
    #Rtmpd.i.meeting_del(event["location"])
  end

  def self.on_error
    Conf.i.logger.error "Unable to list meetings to UCEngine"
    exit 42
  end
end
