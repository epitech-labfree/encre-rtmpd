#! /usr/bin/ruby1.9.1
##
## uce-utils.rb
## Login : <elthariel@rincevent>
## Started on  Tue Sep 20 14:12:21 2011 elthariel
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

require 'rubygems'
require 'restclient'
require 'json'

$conf = { :server => 'localhost', :port => 5280,
  :users => { 'root' => 'root'} }
$user = 'root'
$uid = nil

def uce_url(append)
  if $uid
    "http://#{$conf[:server]}:#{$conf[:port]}/api/0.5#{append}&uid=#{$uid}&sid=#{$sid}"
  else
    "http://#{$conf[:server]}:#{$conf[:port]}/api/0.5#{append}"
  end
end

url = uce_url "/presence?name=#{$user}&credential=#{$conf[:users][$user]}"
puts url
res = RestClient.post url, ""
res = JSON.parse(res)["result"]

$uid = res["uid"]
$sid = res["sid"]

puts "Logged with uid,sid = #{$uid},#{$sid}"

url = uce_url "/meeting/all?name=test_uce_connector_#{rand 999999}"
#url = uce_url "/event?type=test_uce_connector_&metadata[rand]=#{rand 999999}"
puts url
res = JSON.parse(RestClient.post url, "")
puts res




