/*
 * Copyright(C) 2011-2014 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *              2015-2015 Davidson Francis <davidsondfgl@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <unistd.h>

int main()
{
	fprintf(stderr,"------------------------------------\n");
	fprintf(stderr,"        Nanvix Fork Bomb\n");
	fprintf(stderr,"------------------------------------\n");

	fprintf(stderr,"Eating resources and freezing your system in\n3..2..1...\n");

	/* Some busy waiting. */
	int i=1;
	while(i++ < 39999999);
	
	/* Fork bomb. */
	while(1)
		fork();
}
