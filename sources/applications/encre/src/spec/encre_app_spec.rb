##
## encre_app_spec.rb
## Login : <elthariel@steel.newbacchus.org>
## Started on  Wed Feb 22 18:35:05 2012 elthariel
## $Id$
##
## Author(s):
##  - elthariel <>
##
## Copyright (C) 2012 elthariel
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

require "rubygems"
require "bundler/setup"
require 'rtmpd_api'
require 'json'

class MySocket
  attr_reader :socket

  def initialize()
    @socket = TCPSocket.new 'localhost', 2342
  end

  def send(data)
    @socket.write data
  end

  def recv
    JSON.parse @socket.gets "\r\n\r\n"
  end

  def close
    @socket.close
  end
end

$s = MySocket.new
Rtmpd.i.send = Proc.new { |d| $s.send d }

def get_res
  $s.recv
end

def closed?
  $s.socket.closed?
end

describe Rtmpd do
  meetings = []
  5.times { meetings.push rand(999999999999).to_s }

  context "meeting.new" do
    it "allows meeting creation" do
      meetings.each do |m|
        Rtmpd.i.meeting_new m
        res = get_res
        res["status"].should == "SUCCESS"
        closed?.should be_false
      end
    end

    it "silently warns if the meeting exists already" do
      meetings.each do |m|
        Rtmpd.i.meeting_new m
        res = get_res
        res["status"].should == "SUCCESS"
        closed?.should be_false
      end
    end
  end

  context "meeting.del" do
    it "allows existant meeting destruction" do
      meetings.each do |m|
        Rtmpd.i.meeting_del m
        res = get_res
        res["status"].should == "SUCCESS"
        closed?.should be_false
      end
    end

    it "should refuse to destroy non-existant meeting" do
      5.times do
        Rtmpd.i.meeting_del rand(999999999).to_s
        res = get_res
        res["status"].should == "FAIL"
        closed?.should be_false
      end
    end
  end



  context "user.new" do
    users = []
    10.times{|i| Rtmpd.i.meeting_new "test_#{i}"; get_res}
    10.times{|i| users.push "test_#{i}"}

    it "refuses to create user in unexistant room" do
      10.times do
        Rtmpd.i.user_new rand(9999999999).to_s, rand(9999999999).to_s, rand(9999999999).to_s
        res = get_res
        res["status"].should == "FAIL"
        closed?.should be_false
      end
    end

    it "creates user when the meeting exists" do
      10.times do |i|
        10.times do |j|
          Rtmpd.i.user_new "test_#{i}", rand(999999).to_s, "test_#{j}"
          get_res['status'].should == "SUCCESS"
          closed?.should be_false
        end
      end
    end

    it "updates user token (and maybe warn) when the user and meeting already exist" do
      10.times do |i|
        10.times do |j|
          Rtmpd.i.user_new "test_#{i}", rand(999999).to_s, "test_#{j}"
          get_res['status'].should == "SUCCESS"
          closed?.should be_false
        end
      end
    end
  end

  context "user.del" do
    it "deletes user when the meeting and the user exist" do
      10.times do |i|
        10.times do |j|
          Rtmpd.i.user_del "test_#{i}", "test_#{j}"
          get_res['status'].should == "SUCCESS"
          closed?.should be_false
        end
      end
    end

    it "refuses to delete user when the user doesn't exists in the room" do
      10.times do |i|
        10.times do |j|
          Rtmpd.i.user_del "test_#{i}", "test_#{j}"
          get_res['status'].should == "FAIL"
          closed?.should be_false
        end
      end
    end

    it "refuses to delete user when the room doesn't exist" do
      10.times do |i|
        10.times do |j|
          Rtmpd.i.user_del "test_#{rand(9999) * 1000}", "test_#{rand(9999) * 1000}"
          get_res['status'].should == "FAIL"
          closed?.should be_false
        end
      end
    end

  end
end
