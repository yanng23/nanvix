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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MADT_H_
#define MADT_H_

#define MADT_SIG_SIZE 0x4	   /* Number of bytes that signature has. */
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
struct madt_local_apic {
	madt_ics_header header;
	uint8_t proc_uid;
	uint8_t apic_id; /**< The processor's local APIC ID. */
	uint32_t flags; /**< Bit 0: Enabled (if zero, this processor is unusable).
					   Bits 1..31: Reserved (must be zero). */
} __attribute__((packed));


/**
 * IO APIC (type 1).
 */
struct madt_io_apic {
	madt_ics_hdr_t header;
	uint8_t io_apic_id;
	uint8_t reserved; /**< Value is always 0. */
	uint32_t io_apic_addr;
	uint32_t gs_interrupt_base; /**< Global System Interrupt Base. */
} __attribute__((packed));


/**
 * Interrupt Source Override (type 2).
 */
struct madt_isrc_override {
	madt_ics_header header;
	uint8_t bus; /**< Value is always 0. Constant, meaning ISA. */
	uint8_t src; /**< Bus-relative interrupt source (IRQ). */
	uint32_t gs_interrupt; /**< The Global System Interrupt that this bus-relative
							  interrupt source will signal. */
	uint16_t flags; /**< MPS INTI flags. */
} __attribute__((packed));

/**
 * Non-maskable Interrupt (NMI) Source (type 3).
 */
struct madt_nmi {
	madt_ics_header header;
	uint16_t flags; /**< MPS INTI flags. */
	uint32_t gs_interrupt; /**< The Global System Interrupt that this NMI
							  will signal. */
} __attribute__((packed));

/**
 * Local APIC NMI (type 4).
 */
struct madt_local_apic_nmi {
	madt_ics_header header;
	uint8_t proc_uid;
	uint16_t flags; /**< MPS INTI flags. */
	uint8_t local_apic_lint; /**< Local APIC interrupt input LINTn to which
								NMI is connected. */
} __attribute__((packed));

/**
 * Local APIC Address Override (type 5).
 */
struct madt_local_apic_override {
	madt_ics_header header;
	uint16_t reserved; /**< Must be set to zero. */
	uint64_t local_apic_addr; /**< Physical address of Local APIC. */
} __attribute__((packed));

/**
 * IO SAPIC (type 6).
 */
struct madt_io_sapic{
	madt_ics_header header;
} __attribute__((packed));

/**
 * Local SAPIC (type 7).
 */
struct madt_local_sapic {
	madt_ics_header header;
} __attribute__((packed));

/**
 * Platform Interrupt Sources (type 8).
 */
struct madt_interrupt_src {
	madt_ics_header header;
} __attribute__((packed));

/**
 * Processor Local x2APIC (type 9).
 */
struct madt_local_x2apic {
	madt_ics_header header;
} __attribute__((packed));

/**
 * Local x2APIC NMI (type A).
 */
struct madt_local_x2nmi {
	madt_ics_header header;
} __attribute__((packed));

/**
 * GIC CPU Interface (GICC) (type B).
 */
struct madt_gicc {
	madt_ics_header header;
} __attribute__((packed));

/**
 * GIC Distributor (type C).
 */
struct madt_gicd {
	madt_ics_header header;
} __attribute__((packed));

/**
 * GIC MSI Frame (type D).
 */
struct madt_gic_msi {
	madt_ics_header header;
} __attribute__((packed));

/**
 * GIC Redistributor (type E).
 */
struct madt_gicred {
	madt_ics_header header;
} __attribute__((packed));

/**
 * GIC Interrupt Translation Service (ITS, type F).
 */
struct madt_its {
	madt_ics_header header;
} __attribute__((packed));

/**
 * Multiple APIC Descriptor Table.
 */
struct madt
{
	struct acpi_sdt_header header; /**< Header that is common to all the SDTs. */
	uint32_t local_ic_addr; /**< Local Interrupt Controller Address. The 32-bit
							   physical address at which each processor can access
							   its local interrupt controller. */
	uint32_t flags; /**< Multiple APIC flags. Bit 0: PCAT_COMPAT (a one indicates
					   that the system also has a PC-AT-compatible dual-8259 setup.
					   The 8259 vectors must be disabled (that is, masked) when
					   enabling the ACPI APIC operation). Bits 1..31: Reserved
					   (this value is zero). */
} __attribute__((packed));
#endif
