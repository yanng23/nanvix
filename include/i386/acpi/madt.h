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

#ifndef MADT_H_
#define MADT_H_

#define MADT_SIG_SIZE 0x4      /* Number of bytes that signature has. */
#define MADT_LENGTH_OFFSET 0x4 /* The length entry offset. */

#ifndef _ASM_FILE_
#include <i386/acpi/acpi.h>
#include <stdint.h>

/**
 * Interrupt Controller Structure Header.
 */
struct madt_ics_header {
	uint8_t type;
	uint8_t length;
} __attribute__((packed));

/**
 * Processor local APIC (type 0).
 */
typedef struct madt_local_apic {
	madt_ics_header header;
	uint8_t proc_uid;
    uint8_t apic_id; /**< The processor's local APIC ID. */

	/**
	 * Bit 0: Enabled (if zero, this processor is unusable).
	 * Bits 1..31: Reserved (must be zero).
	 */
    uint32_t flags;
} acpi_madt_local_apic_t;


/**
 * Multiple APIC Descriptor Table.
 */
struct madt
{
	struct acpi_sdt_header header; /**< Header that is common to all the SDTs. */

	/**
	 * Local Interrupt Controller Address.
	 * The 32-bit physical address at which each processor can access its local
	 * interrupt controller.
	 */
	uint32_t local_ic_addr;

	/**
	 * Multiple APIC flags.
	 *
	 * Bit 0: PCAT_COMPAT (a one indicates that the system also has a
	 * PC-AT-compatible dual-8259 setup. The 8259 vectors must be disabled
	 * (that is, masked) when enabling the ACPI APIC operation).
	 *
	 * Bits 1..31: Reserved (this value is zero).
	 */
	uint32_t flags;
} __attribute__((packed));
#endif
