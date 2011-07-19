/*
** encre_helpers.h
** Login : <elthariel@hydre>
** Started on  Tue Jul 12 16:57:23 2011 Julien 'Lta' BALLET
** $Id$
**
** Author(s):
**  - Julien 'Lta' BALLET <elthariel@gmail.com>
**
** Copyright (C) 2011 Julien 'Lta' BALLET
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef   	ENCRE_HELPERS_H_
# define   	ENCRE_HELPERS_H_

# include "common.h"

# define AT(x, y) (x[(const uint32_t)y])
# define HAS_IDX(x, y) (x.HasKey(format(VAR_INDEX_VALUE"%u", y)))

#endif 	    /* !ENCRE_HELPERS_H_ */
