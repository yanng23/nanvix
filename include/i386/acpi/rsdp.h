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

#ifndef RSDP_H_
#define RSDP_H_

#define RSDP_SIG_SIZE 0x8          /* Signature size (8 bytes). */
#define RSDP_CHECKSUM_SIZE 0x14    /* Num of bytes included in checksum. */
#define RSDP_REVISION_OFFSET 0xF   /* RSDP revision that tell us the ACPI version. */
#define RSDT_OFFSET 0x10           /* RSDT address offset. */

#ifndef _ASM_FILE_
#include <stdint.h>

/*
 * RSDP structure.
 */
struct rsdp
{
	/* Original RSDP structure. */
	char signature[8];     /**< 8-byte string (not null terminated). */
	uint8_t checksum;      /**< Includes the first 20 bytes. */
	char oem_id[6];        /**< 6-byte string that identifies the OEM. */
	uint8_t revision;      /**< 0 for ACPI 1.0, 2 for ACPI 2.0. */
	uint32_t rsdt_addr;    /**< 32 bit physical address of the RSDT. */

	/* Extended fields - present if revision >= 2. */
	uint32_t length;       /**< The length of the table, in bytes. */
	uint64 xsdt_addr;      /**< 64 bit physical address of the XSDT. */
	uint8_t ext_checksum;  /**< Checksum for the extended fields. */
	uint8_t reserved[8];   /**< Reserved field. */
} __attribute__((packed));

#endif
#endif
