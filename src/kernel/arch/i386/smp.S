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

/* Exported symbols */
.globl search_rsdp
	
/*==================================================*
 *=============== Bootstrap section ================*
 *==================================================*/
.section bootstrap

/*--------------------------------------------------*
 *------------------ RSDP Signature ----------------* 
 *--------------------------------------------------*/
RSDP_SIG:	.string "RSDP PTR "


/*--------------------------------------------------*
 *------------------- Search RSDP ------------------* 
 *--------------------------------------------------*/
search_rsdp:
	/* Save the old base pointer value. */
	push %ebp

	/* set the new base pointer value. */
	movl %esp,%ebp
	
	/* Clear flag D, to compare in correct order. */
	cld

	/* First lookup into main bios memory region */
	/* Push start and end address into stack, then */
	/* call <private> _lookup_memory function. */
	push $0xFFFFF
	push $0xE0000
	call _lookup_memory

	/* Clean up the stack. We have 8 bytes */
	/* (2 params * 4 bytes each) on the stack, */
	/* and the stack grows down. Thus, to get */
	/* rid of the parameters, we can simply add */
	/* 8 to the stack pointer. */
	addl $0x8, %esp

	/* Restore the caller's base pointer. */
	/* No need to deallocate local variables */
	/* neither restore any register. */
	pop %ebp

	/* Return to caller. */
	ret
	

/*--------------------------------------------------*
 *------------------ Lookup Memory -----------------* 
 *--------------------------------------------------*/
_lookup_memory:
	/* Save the old base pointer value. */
	push %ebp

	/* set the new base pointer value. */
	movl %esp,%ebp

	/* Save the values of registers that the */
	/* function will modify. This function uses */
	/* EBX, EDI and ESI. */
	push %ebx
	push %edi
	push %esi

	/* Put value of param 1 into EAX (start address). */
	/* Param 1 can be found at [ebp + 8]. */
	movl 0x8(%ebp), %eax

	/* Put value of param 2 into EAX (end address). */
	/* Param 2 can be found at [ebp + 12]. */
	movl 0xC(%ebp), %eax

_lookup_memory_start_loop:
	cmpl %eax, %ebx
	je   _lookup_memory_end_loop

	/* RSDP Signature */
	movl $RSDP_SIG, %esi

	/* Signature size */
	movl $0x8, %ecx
	
	/* Start byte to compare */
	movl %eax, %edi

	/* Compare bytes */
	rep  cmpsb
	je   _lookup_memory_found

	addl $0x10, %eax
	jmp  _lookup_memory_start_loop
	
_lookup_memory_end_loop:
	/* Return <false> if structure wasn't found. */
	movl $0x0, %eax
	jmp  _lookup_memory_return

_lookup_memory_found:
	/* Return <true> if structure was found. */
	movl $0x1, %eax
	
_lookup_memory_return:
	/* Recover register values. */
	pop %esi
	pop %edi
	pop %ebx

	/* Restore the caller's base pointer. */
	/* No need to deallocate local variable. */
	/* since we didn't use any. */
	pop %ebp

	/* Return to caller. */
	ret	
	







