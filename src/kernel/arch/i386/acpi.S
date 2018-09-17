/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *              2017-2017 Clement Rouquier <clementrouquier@gmail.com>
 *              2018-2018 Davidson Francis <davidsondfgl@gmail.com>
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

/* Must come first */
#define _ASM_FILE_

#include <i386/asm_call.h>
#include <i386/acpi/acpi.h>

/* Exported symbols */
.globl acpi_checksum

/*============================================================================*
 *                              bootstrap section                             *
 *============================================================================*/
.section bootstrap

/**
 * @brief Assert that RSDP structure is valid.
 *
 * @param RSDP base address
 *
 * @returns VALID if checksum is ok, INVALID otherwise.
 */
acpi_checksum:
	/* Save the old base pointer value. */
	push %ebp

	/* set the new base pointer value. */
	movl %esp, %ebp

	/*
	 * Save the values of registers that the
	 * function will modify. This function uses
	 * only EBX.
	 */
	push %ebx

	/* Get RSDP base address. */
	movl ARG0_OFFSET(%ebp), %edx
	
	/* Get number of bytes to check. */
	movl ARG1_OFFSET(%ebp), %ecx
	
	/* Sum starts with 0. */
	andl $0x0, %eax
	
acpi_checksum_loop:
	/* Sum RSDP bytes (including checksum). */
	addl (%edx), %eax
	inc %edx
	
	/*
	 * Uses ECX register as loop counter.
	 * ECX starts with 20, being decremented
	 * at each iteration. When ECX equals zero,
	 * loop is over.
	 */
	loop acpi_checksum_loop

	/* If the lowest byte is equal to zero, the structure is valid. */
	cmp $0x0, %al
	je acpi_checksum_valid

	/* Else, we got an error. */
	movl $INVALID, %eax
	jmp acpi_checksum_return
	
acpi_checksum_valid:
	movl $VALID, %eax
	
acpi_checksum_return:
	/* Recover register values. */
	pop %ebx

	/*
	 * Restore the caller's base pointer.
	 * No need to deallocate local variable.
	 * since we didn't use any.
	 */
	pop %ebp

	/* Return to caller. */
	ret	
