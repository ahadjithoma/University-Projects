#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFERSIZE 100


main2()
{
	int file;
	char message[BUFFERSIZE];
	char *filePath = "/tmp/fifoFile";

	/* open fifo file to write something */
	file = open(filePath, O_WRONLY);
	
	/* Get the message from the console (user) */
	printf("(Client) Enter message here: ");
	fgets(message, BUFFERSIZE, stdin);
	
	/* write the message to the file */
	write(file, message, sizeof(message));
    close(file);

   	return 2;
}