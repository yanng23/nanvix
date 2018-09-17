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
#include <i386/acpi/madt.h>	
#include <i386/acpi/rsdt.h>

/* Exported symbols */
.globl get_madt

/*============================================================================*
 *                              bootstrap section                             *
 *============================================================================*/
.section bootstrap

/* MADT Signature (1st field of MADT table) */
MADT_SIG:	.string "APIC"
	
/**
 * @brief Search for MADT.
 *
 * @param RSDT number of entries
 * @param RSDT entries start address
 *
 * @returns MADT address if found, 0 otherwise.
 */
get_madt:
	/* Save the old base pointer value. */
	push %ebp

	/* Set the new base pointer value. */
	movl %esp, %ebp

	/*
	 * Put value of param 1 into EAX (start of
	* the array of entries).
	 * Param 2 can be found at [ebp + 8].
	 */
	movl ARG0_OFFSET(%ebp), %eax
	
	/*
	 * Put value of param 2 into EAX (the size of
	 * the array of entries).
	 * Param 1 can be found at [ebp + 12].
	 */
	movl ARG1_OFFSET(%ebp), %edx

	/* Calculate the end of the table. */
	addl %eax, %edx
	
	/* Clear flag D, to compare in correct order. */
	cld

get_madt_start_loop:	
	cmpl %eax, %edx
	je get_madt_end_loop

	/* MADT signature. */
	movl $MADT_SIG, %esi

	/* MADT signature size. */
	movl $MADT_SIG_SIZE, %ecx

	/* Initial byte that we're going to compare. */
	movl (%eax), %edi

	/* Comparing bytes. */
	rep cmpsb
	je get_madt_found

	addl $RSDT_ENTRIE_SIZE, %eax
	jmp get_madt_start_loop
	
get_madt_end_loop:
	/* Return $INVALID if structure wasn't found. */
	movl $INVALID, %eax
	jmp get_madt_return

get_madt_found:
	/* Return the MADT address is table was found. */
	movl %edi, %eax
	subl $MADT_SIG_SIZE, %eax
	
get_madt_return:
	/*
	 * Restore the caller's base pointer.
	 * No need to deallocate local variables
	 * neither restore any register.
	 */
	pop %ebp
	
	ret
