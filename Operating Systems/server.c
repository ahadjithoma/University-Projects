#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFERSIZE 100

int main()
{	
	int fileDescriptor;
	pid_t pid;
	char buffer[BUFFERSIZE];
	char *filePath = "/tmp/fifoFile";
	
	/* create a fifo file (number 1) with read, write, execute permisions to owner, group and others (numbers 777) */
	mkfifo(filePath, 0010777);
	
	/* create Parent and Child processes */
	pid = fork(); 


	/* check if fork operation has failed and if so, close the program */		
	if (pid<0){	
		printf("Fork Failed. Server will now close.\n");
		return -1;
	}

	/* Check if it's the child process and call Client */
	if (pid==0)
	{
		main2();	/* call client */
	}

	/* check if it's parent process and do some work */
	else if (pid>0) 
	{	
	 	/* Get the client's message - open file, and then read the file */
	 	fileDescriptor = open(filePath, O_RDONLY);
	  	read(fileDescriptor, buffer, BUFFERSIZE); 
	
	  	printf("(Server) Message received: %s\n", buffer);
	  
	  	/* close file and delete it */
	  	close(fileDescriptor);
		unlink(filePath);
		
	}
	 
	wait(0); /* ensure that a child process won't become a zombie */

	return 1;
}

