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
require 'logger'
require 'yaml'
require './ucengine_api'
require './rtmpd_api'

$conf = YAML::load_file 'platform.yml'
$conf[:logger] = STDOUT if $conf[:logger] == "STDOUT" || !$conf[:logger]
$log = Logger.new($conf[:logger])
if (Logger::Severity::const_defined? $conf[:loglvl].upcase)
  $log.level = Logger.const_get($conf[:loglvl].upcase.to_sym)
else
  $log.level = Logger::INFO  #default logger level
end

$log.warn "Encre rtmpd <-> ucengine connector starting up ..."
$log.info "Configuration hash is: #{$conf}"

#uce = UCEngine.new(:logger => $log, :uid => 'encre-video', :token => '123456')
rtmpd = Rtmpd.new(:logger => $log)

#rtmpd.cmd({:command => "user.new", :pwet => 98765431})
rtmpd.user_new("user_uid", "user_sid")
#rtmpd.user_del("test_uid")

