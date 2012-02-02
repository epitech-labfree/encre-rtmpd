##
## encre_settings.rb
## Login : <elthariel@rincevent>
## Started on  Tue Jul 19 18:34:01 2011 elthariel
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

require 'singleton'
require 'logger'

class Conf
  include Singleton

  # edit the next line to change the default path
  @@conf_path = "config.yml"

  def initialize()
    @options = {
      :rtmpd_server => "localhost",
      :rtmpd_port => 2342,
      :uce_server => "localhost",
      :uce_port => 5280,
      :uce_method => "http",
      :uce_prefix => "/api/0.6",
      :uce_uid => "encre-video",
      :uce_token => "123456",
      :logfile => "STDOUT",
      :loglevel => "DEBUG",
      :token_secret => "givemeagoodrandomsecret"
    }

    begin
      if File.exists? path
        yaml = YAML::load_file(path)
        @options.merge! yaml
      end

      yaml = YAML::dump(@options)

      if ((File.exists? path and File.writable? path) or not File.exists?(path))
        File.open(path, "w") do |f|
          f << yaml
        end
      end
    rescue => e
      $stderr.write "Error with settings parsing: #{e}\n"
      $stderr.write "#{e.backtrace}\n"
    end

    init_logger
    validates

    logger.info "Configuration hash is: #{@options}"
  end

  def path
    @@conf_path
  end
  def self.i
    self.instance
  end

  def init_logger
    if logfile == "STDOUT" or logfile == "STDERR"
      @options[:logger] = Logger.new Kernel.const_get logfile.to_sym
    else
      @options[:logger] = Logger.new logfile
    end
    logger.level = Logger.const_get loglevel.to_sym
    logger.progname = "encrev15_uconnector"
    logger.warn "Started Encre v1(.5) UCEngine connector"
  end

  def uce_url
    "#{uce_method}://#{uce_server}:#{uce_port}#{uce_prefix}"
  end

  def validates
  end

  def method_missing(sym)
    if @options.has_key? sym
      @options[sym]
    else
      raise NoMethodError
    end
  end
end
