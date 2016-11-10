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
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stropts.h>
#include <dev/tty.h>

#define LINE_SIZE 80

int readline(int fd, char *buf, size_t length)
{
	for (size_t i = 0; i < (length - 1); i++)
	{
		char ch;
		
		if (read(fd, &ch, 1) < 1)
			return (0);
		
		if (ch == '\n')
		{
			buf[i] = '\0';
			break;
		}
		
		buf[i] = ch;
	}
	
	return (1);
}

int writeline(int fd, char *buf)
{
	int ret;

	for (size_t i = 0; *(buf+i) != '\0'; i++)
		ret = write(fd, &buf[i], 1);

	ret = write(fd, "\n", 1);

	return ret;
}

int main(int argc, char *argv[])
{
	if (argc > 1 && strcmp(argv[1],"clear") == 0)
	{
		ioctl(fileno(stdout), TTY_CLEAR);

		fprintf(stderr," _   _   ___   _   _ _   _ _______   __\n");
		fprintf(stderr,"| \\ | | / _ \\ | \\ | | | | |_   _\\ \\ / /\n");
		fprintf(stderr,"|  \\| |/ /_\\ \\|  \\| | | | | | |  \\   /\n");
		fprintf(stderr,"| . ` ||  _  || . ` | | | | | |  /   \\ \n");
		fprintf(stderr,"| |\\  || | | || |\\  \\ \\_/ /_| |_/ /^\\ \\\n");
		fprintf(stderr,"\\_| \\_/\\_| |_/\\_| \\_/\\___/ \\___/\\/   \\/\n");

		fprintf(stderr,"______ _____  _____ _____   ___________ _____ _____ _____ _____ _   _ \n");
		fprintf(stderr,"| ___ \\  _  ||  _  |_   _| |  ___|  _  \\_   _|_   _|_   _|  _  | \\ | |\n");
		fprintf(stderr,"| |_/ / | | || | | | | |   | |__ | | | | | |   | |   | | | | | |  \\| |\n");
		fprintf(stderr,"|    /| | | || | | | | |   |  __|| | | | | |   | |   | | | | | | . ` |\n");
		fprintf(stderr,"| |\\ \\\\ \\_/ /\\ \\_/ / | |   | |___| |/ / _| |_  | |  _| |_\\ \\_/ / |\\  |\n");
		fprintf(stderr,"\\_| \\_|\\___/  \\___/  \\_/   \\____/|___/  \\___/  \\_/  \\___/ \\___/\\_| \\_/\n\n\n");

		char *arg[2];
		arg[0] = "-";
		arg[1] = NULL;
		execve("/bin/tsh", (char *const*)arg, (char *const*)environ);
		return 0;
	}

	printf("----------------------------------------------------\n");
	printf("                Nanvix InitTab Tool\n");
	printf("----------------------------------------------------\n");

	char inittab[LINE_SIZE];

	int fd;
	if ((fd = open("/etc/inittab", O_RDONLY)) < 0)
		return (-1);

	if (!readline(fd, inittab, LINE_SIZE))
		return (-1);

	/* Checks if this hack has been run before. */
	if( strcmp(inittab, "y /bin/itt itt clear") == 0 )
	{
		printf("> Nothing to do here, your system is already hacked!\n");
		return 0;
	}
	close(fd);

	if ((fd = open("/etc/inittab", O_TRUNC | O_WRONLY)) < 0)
		return (-1);

	/* Set the TSH and clear the file. */
	printf("> Hacking system...\n");

	if ( writeline(fd, "y /bin/itt itt clear") < 0 )
		return (-1);

	printf("> Your system is hacked, please reboot to apply the changes!\n");

	close(fd);
	sync();
}
