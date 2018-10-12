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
	uint8_t reserved[2]; /**< Must be set to zero. */
	uint64_t local_apic_addr; /**< Physical address of Local APIC. */
} __attribute__((packed));

/**
 * IO SAPIC (type 6).
 */
struct madt_io_sapic{
	madt_ics_header header;
	uint8_t io_apic_id; /**< IO SAPIC ID. */
	uint8_t reserved; /**< Must be zero. */
	uint32_t gs_interrupt_base; /**< The global system interrupt number where
								   this I/O SAPIC’s interrupt inputs start. */
	uint64_t io_sapic_addr; /**< The 64-bit physical address to access this I/O
							   SAPIC. Each I/O SAPIC resides at a unique address. */
} __attribute__((packed));

/**
 * Local SAPIC (type 7).
 */
struct madt_local_sapic {
	madt_ics_header header;
	uint8_t proc_id;
	uint8_t local_sapic_id; /**< The processor’s local SAPIC ID. */
	uint8_t local_sapic_eid; /**< The processor’s local SAPIC EID. */
	uint8_t reserved[3]; /**< Must be zero. */
	uint32_t flags; /**< Local SAPIC flags. */
	uint32_t proc_uid;
	char proc_uid_str[];
} __attribute__((packed));

/**
 * Platform Interrupt Sources (type 8).
 */
struct madt_interrupt_src {
	madt_ics_header header;
	uint16_t flags; /**< MPS INTI flags. */
	uint8_t interrupt_type; /**< 1 PMI
							   2 INIT
							   3 Corrected Platform Error Interrupt
							   All other values are reserved. */
	uint8_t proc_id; /**< Processor ID of destination. */
	uint8_t proc_eid; /**< Processor EID of destination. */
	uint8_t io_sapic_vec; /**< Value that OSPM must use to program the vector
							 field of the I/O SAPIC redirection table entry for
							 entries with the PMI interrupttype. */
	uint32_t gs_interrupt; /**< The Global System Interrupt that this platform
							  interrupt will signal. */
	uint32_t platform_int_src_flags; /**< Platform Interrupt Source Flags. */
} __attribute__((packed));

/**
 * Processor Local x2APIC (type 9).
 */
struct madt_local_x2apic {
	madt_ics_header header;
	uint8_t reserved[2]; /**< Must be zero. */
	uint32_t x2_apic_id; /**< The processor’s local x2APIC ID. */
	uint32_t flags; /**< Same as local APIC flags. */
	uint32_t proc_uid;
} __attribute__((packed));

/**
 * Local x2APIC NMI (type A).
 */
struct madt_local_x2nmi {
	madt_ics_header header;
	uint16_t flags; /**< Same as MPS INTI flags. */
	uint32_t proc_uid; /**< UID corresponding to the ID listed in the processor
						  Device object. A value of 0xFFFFFFFF signifies that
						  this applies to all processors in the machine. */
	uint8_t x2_apic_lint; /**< Local x2APIC interrupt input LINTn to which NMI
							 is connected. */
	uint8_t reserved[3]; /**< Must be zero. */
} __attribute__((packed));

/**
 * GIC CPU Interface (GICC) (type B).
 */
struct madt_gicc {
	madt_ics_header header;
	uint8_t reserved[2]; /**< Must be zero. */
	uint32_t id; /**< GIC's CPU Interface Number. In GICv1/v2 implementations,
					this value matches the bit index of the associated processor
					in the GIC distributor's GICD_ITARGETSR register. For GICv3/4
					implementations this field must be provided by the platform,
					if compatibility mode is supported. If it is not supported by
					the implementation, then this field must be zero. */
	
	uint32_t proc_uid; /**< The OS associates this GICC Structure with a processor
						  device object in the namespace when the _UID child object
						  of the processor device evaluates to a numeric value that
						  matches the numeric value in this field. */
	
	uint32_t flags;
	uint32_t parking_prot_version; /**< Version of the ARM-Processor Parking Protocol
									  implemented. */
	
	uint32_t perf_interrupt_gsiv; /**< The GSIV used for Performance Monitoring
									 Interrupts. */
	
	uint64_t parked_address; /**< The 64-bit physical address of the processor’s
								Parking Protocol mailbox. */
	
	uint64_t phys_address; /**< On GICv1/v2 systems and GICv3/4 systems in GICv2
							  compatibility mode, this field holds the 64-bit physical
							  address at which the processor can access this GIC CPU
							  Interface. If provided here, the "Local Interrupt
							  Controller Address" field in the MADT must be ignored by
							  the OSPM. */

	uint64_t gicv; /**< Address of the GIC virtual CPU interface registers. If the
					  platform is not presenting a GICv2 with virtualization extensions
					  this field can be 0. */

	uint64_t gich; /**< Address of the GIC virtual interface control block registers.
					  If the platform is not presenting a GICv2 with virtualization
					  extensions this field can be 0. */

	uint32_t vgic_interrupt; /**< GSIV for Virtual GIC maintenance interrupt. */
	uint64_t gicr_addr;
	uint64_t mpidr;
	uint8_t proc_power_efficiency;
	uint8_t reserved[3]; /**< Must be zero. */
} __attribute__((packed));

/**
 * GIC Distributor (type C).
 */
struct madt_gic_dist {
	madt_ics_header header;
	uint8_t reserved_1[2]; /**< Must be zero. */
	uint32_t id; /**< This GIC Distributor’s hardware ID. */
	uint64_t phys_addr; /**< The 64-bit physical address for this Distributor. */
	uint32_t sys_vector_base; /**< Reserved - Must be zero. */
	uint8_t gic_version; /**< 0x00: No GIC version is specified, fall back to
							hardware discovery for GIC version
							0x01: GICv1
							0x02: GICv2
							0x03: GICv3
							0x04: GICv4
							0x05-0xFF, Reserved for future use. */
	uint8_t reserved_2[3]; /**< Must be zero. */
} __attribute__((packed));

/**
 * GIC MSI Frame (type D).
 */
struct madt_gic_msi {
	madt_ics_header header;
	uint8_t reserved[2]; /**< Must be zero. */
	uint32_t frame_id; /**< GIC MSI Frame ID. In a system with multiple GIC MSI
						  frames, this value must be unique to each one. */
	uint64_t phys_addr; /**< The 64-bit physical address for this MSI Frame. */
	uint32_t flags; /**< GIC MSI Frame Flags. */
	uint16_t spi_count; /**< SPI Count used by this frame. Unless the SPI Count
						   Select flag is set to 1 this value should match the lower
						   16 bits of the MSI_TYPER register in the frame. */
	uint16_t spi_base; /**< SPI Base used by this frame. Unless the SPI Base Select flag
						  is set to 1 this value should match the upper 16 bits of the
						  MSI_TYPER register in the frame. */
} __attribute__((packed));

/**
 * GIC Redistributor (type E).
 */
struct madt_gic_red {
	madt_ics_header header;
	uint8_t reserved[2]; /**< Must be zero. */
	uint64_t phys_range_addr; /**< The 64-bit physical address of a page range
								 containing all GIC Redistributors. */
	uint32_t range_length; /**< Length of the GIC Redistributor Discovery
							  page range. */
} __attribute__((packed));

/**
 * GIC Interrupt Translation Service (ITS, type F).
 */
struct madt_its {
	madt_ics_header header;
	uint8_t reserved[2]; /**< Must be zero. */
	uint32_t id; /**< GIC ITS ID. In a system with multiple GIC ITS units,
					this value must be unique to each one. */
	
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
	uint64_t phys_addr; /**< The 64-bit physical address for the Interrupt
						   Translation Service. */
	uint8_t reserved[4]; /**< Must be zero. */
} __attribute__((packed));

#endif
#endif
