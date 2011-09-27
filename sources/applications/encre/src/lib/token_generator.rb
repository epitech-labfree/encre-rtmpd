##
## token_generator.rb
## Login : <elthariel@hydre>
## Started on  Tue Sep 27 18:12:54 2011 Julien 'Lta' BALLET
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

require 'digest/sha1'

module TokenGenerator
  def self.generate(uid)
    Digest::SHA1.hexdigest(uid + Conf.i.token_secret)
  end

  def self.validate(uid, token)
    real_token = Digest::SHA1.hexdigest(uid + Conf.i.token_secret)
    token == real_token
  end
end

