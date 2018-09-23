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

#ifndef ACPI_H_
#define ACPI_H_

#define REVISION_OLD_ACPI 0x0 /* ACPI 1.0 has revision value equal to zero. */
#define INVALID 0x0 /* Invalid structure. */
#define VALID 0x1 /* Valid structure. */

#ifndef _ASM_FILE_
#include <stdint.h>

/**
 * A header that is common to all the SDT.
 */
struct acpi_sdt_header
{
	char signature[4];         /**< 4-byte string (not null terminated). */
	uint32_t length;           /**< The length of the table, in bytes. */
	uint8_t revision;          /**< 0 for ACPI 1.0, 2 for ACPI 2.0. */
	uint8_t checksum;          /**< Includes the first 20 bytes. */
	char oem_id[6];            /**< 6-byte string that identifies the OEM. */
	char oem_table_id[8];      /**< Must match the OEM Table ID in the FADT.*/
	uint32_t oem_revision;     /**< OEM revision for supplied OEM table id. */
	uint32_t creator_id;       /**< Vendor ID of utility that created the table. */
	uint32_t creator_revision; /**< Revision of utility that created the table. */
} __attribute__((packed));

#endif
