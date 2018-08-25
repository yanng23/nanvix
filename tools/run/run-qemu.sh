# 
# Copyright(C) 2011-2018 Pedro H. Penna   <pedrohenriquepenna@gmail.com> 
#              2016-2018 Davidson Francis <davidsondfgl@gmail.com>
#
# This file is part of Nanvix.
#
# Nanvix is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Nanvix is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Nanvix.  If not, see <http://www.gnu.org/licenses/>.
#

# NOTES:
#   - This script should work in any Linux distribution.
#   - You should run this script with superuser privileges.
#

export CURDIR=`pwd`

if [ "$TARGET" = "i386" ]; then
	qemu_command="qemu-system-i386 -drive
		file=nanvix.iso,format=raw,if=ide,media=cdrom
		-m 256M -mem-prealloc"
	dbg_ddd=false
	
	while [ $# -gt 0 ]; do
		case "$1" in
			--dbg)
				qemu_command+=" -s -S"
				shift
				;;
			--ddd)
				dbg_ddd=true
				shift
				;;
			--perf)
				qemu_command+=" -cpu host --enable-kvm"
				shift
				;;
			--serial)
				qemu_command+=" -nographic -display none"
				shift
				;;
			--smp)
				qemu_command+=" -smp 2"
				shift
				;;
		esac
	done

	if [ $dbg_ddd = true ]; then
		qemu_command+=" & ddd --debugger
			$CURDIR/tools/dev/toolchain/i386/bin/i386-elf-gdb"
	else
		qemu_command+=" & $CURDIR/tools/dev/toolchain/i386/bin/i386-elf-gdb"
	fi

	eval $qemu_command
else
	qemu-system-or1k       \
		-kernel bin/kernel \
		-serial stdio      \
		-display none      \
		-m 256M            \
		-mem-prealloc
fi
