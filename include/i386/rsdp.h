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

#ifndef I386_RSDP_H_
#define I386_RSDP_H_

/* BIOS attributes. */
#define EBDA_POINTER    0x40E   /* EBDA pointer address. */
#define EBDA_SIZE       0x400   /* Size of 1kB. */
#define MAIN_BIOS_START 0xE0000 /* Main bios start address. */
#define MAIN_BIOS_END   0xFFFFF /* Main bios end address. */

/* Stack offsets. */
#define ARG0_OFFSET     0x8     /* 1st argument passed to a function. */
#define ARG1_OFFSET     0xC     /* 2nd argument passed to a function. */

/* RSDP attributes. */
#define RSDP_SIG_SIZE   0x8     /* Signature size (8 bytes). */
#define CHECKSUM_SIZE   0x14    /* Num of bytes included in checksum. */

#endif
