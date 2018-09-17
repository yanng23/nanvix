/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RSDT_H_
#define RSDT_H_

#define RSDT_HEADER_SIZE 0x24    /* Header size, excluding entries. */
#define RSDT_LENGTH_OFFSET 0x4   /* Length of the entire table. */
#define RSDT_ENTRIES_OFFSET 0x24 /* Array of other SDTs. */
#define RSDT_ENTRIE_SIZE 0x4     /* Num of bytes of each entry. */

#endif
