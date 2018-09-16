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

#include <i386/acpi.h>
#include <i386/asm_call.h>
#include <i386/bios.h>
#include <i386/rsdp.h>

/* Exported symbols */
.globl search_rsdp

/*============================================================================*
 *                              bootstrap section                             *
 *============================================================================*/
.section bootstrap

/* RSDP Signature (1st field of RSDP structure) */
RSDP_SIG:	.string "RSD PTR "

/**
 * @brief Search for RSDP structure.
 *
 * @returns RSDP address if found, 0 otherwise.
 */
search_rsdp:
	/* Save the old base pointer value. */
	push %ebp

	/* set the new base pointer value. */
	movl %esp, %ebp
	
	/*
	 * First lookup into main bios memory region that
	 * is between 0xE0000 and 0xFFFFF.
	 *
	 * Push start and end address into stack, then
	 * call <private> _lookup_memory function.
	 */
	push $MAIN_BIOS_END
	push $MAIN_BIOS_START
	call _lookup_memory

	/*
	 * Clean up the stack. We have 8 bytes
	 * (2 params * 4 bytes each) on the stack,
	 * and the stack grows down. Thus, to get
	 * rid of the parameters, we can simply add
	 * 8 to the stack pointer.
	 */
	addl $0x8, %esp

	/*
	 * If RSDP wasn't found, lookup into EBDA.
	 * If %eax value isn't zero, we already have
	 * the address.
	 */
	cmpl $0x0, %eax
	jne search_rsdp_checksum

	/*
	 * Get EBDA initial address by looking
	 * for a 2 byte real mode segment pointer
	 * that is located at 40:0Eh (segment:offset).
	 */
	movl $EBDA_POINTER, %eax
	movl (%eax), %eax

	/*
	 * We're looking in the first 1kB of the EBDA.
	 * We use 0x401 (and not 0x400) as the final address
	 * for the same reason that we used 0x10000 while
	 * looking into main bios area, i.e. we're running
	 * the loop while address < 0x401.
	 */
	movl %eax,       %ebx
	addl $EBDA_SIZE, %ebx

	push %ebx
	push %eax
	call _lookup_memory

	/*
	 * Clean up the stack. We have 8 bytes
	 * (2 params * 4 bytes each) on the stack,
	 * and the stack grows down. Thus, to get
	 * rid of the parameters, we can simply add
	 * 8 to the stack pointer.
	 */
	addl $0x8, %esp

	/*
	 * If RSDP wasn't found at all, just return
	 * and maybe run with just one core. 
	 */
	cmpl $0x0, %eax
	je   search_rsdp_return
	
search_rsdp_checksum:
	/* Pass number of bytes to check. */
	push $CHECKSUM_SIZE
	
	/* Save EAX content and pass RSDP base address as param. */
	push %eax
	
	/* Call checksum function. */
	call acpi_checksum

	/* Move return value to EDX. */
	movl %eax, %edx

	/* Restore EAX content and clean up stack. */
	pop  %eax
	addl $0x4, %esp

	/* If checksum failed, just return 0. */
	cmpl $INVALID_CHECKSUM, %edx
	je search_rsdp_checkum_failed

	/*
	 * Check whether the ACPI version is 1.0 or 2.0.
	 * If it is 2.0, call checksum to the extended part
	 * of RSDP.
	 */
	movl %eax, %edx
	addl $REVISION_OFFSET, %edx
	movl (%edx), %edx
	cmpb $REVISION_OLD_ACPI, %dl
	je search_rsdp_return

	/*** TODO: Checksum for the extended area on ACPI 2.0. ***/
	nop

search_rsdp_checkum_failed:
	movl $INVALID_CHECKSUM, %eax
	
search_rsdp_return:
	/*
	 * Restore the caller's base pointer.
	 * No need to deallocate local variables
	 * neither restore any register.
	 */
	pop %ebp
	
	/* Return to caller. */
	ret

/**
 * @brief Lookup for RSDP in memory.
 *
 * @param Initial address.
 * @param Final address.
 *
 * @returns RSDP address if found, 0 otherwise.
 */
_lookup_memory:
	/* Save the old base pointer value. */
	push %ebp

	/* set the new base pointer value. */
	movl %esp, %ebp

	/*
	 * Save the values of registers that the
	 * function will modify. This function uses
	 * EBX, EDI and ESI.
	 */
	push %ebx
	push %edi
	push %esi

	/*
	 * Put value of param 1 into EAX (start address).
	 * Param 1 can be found at [ebp + 8].
	 */
	movl ARG0_OFFSET(%ebp), %eax

	/*
	 * Put value of param 2 into EBX (end address).
	 * Param 2 can be found at [ebp + 12].
	 */
	movl ARG1_OFFSET(%ebp), %ebx

	/* Clear flag D, to compare in correct order. */
	cld

_lookup_memory_start_loop:
	cmpl %ebx, %eax
	jg   _lookup_memory_end_loop

	/* RSDP Signature */
	movl $RSDP_SIG, %esi

	/* Signature size */
	movl $RSDP_SIG_SIZE, %ecx
	
	/* Initial byte that we're going to compare */
	movl %eax, %edi

	/* Comparing bytes */
	rep cmpsb
	je _lookup_memory_found
	
	addl $0x10, %eax
	jmp _lookup_memory_start_loop
	
_lookup_memory_end_loop:
	/* Return 0 (false) if structure wasn't found. */
	movl $0x0, %eax
	jmp _lookup_memory_return

_lookup_memory_found:
	/* Return the RSDP address if structure was found. */
	movl %edi, %eax
	subl $RSDP_SIG_SIZE, %eax
	
_lookup_memory_return:
	/* Recover register values. */
	pop %esi
	pop %edi
	pop %ebx

	/*
	 * Restore the caller's base pointer.
	 * No need to deallocate local variable.
	 * since we didn't use any.
	 */
	pop %ebp

	/* Return to caller. */
	ret	
