#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Fatal Error: expected 2 arguments, got less\n");
	}

	int number = 2;
	
	int sum = number + 2;
	
	int fd = open("test.txt",O_RDONLY);	
		
	char buffer[101];
	int t = memset(buffer,'\0',101);	
	int bytesRead = 1;
	int curr = 0;
	do {
		bytesRead = read(fd,buffer,100-curr);
		curr += bytesRead;		
	}while(bytesRead > 0 && curr < 100);
	
	close(fd);	
	
	printf("buffer bytes: %d\n",bytesRead);
	printf("buffer: %s\n",buffer);
	//printf("val of errno: %d\n", errno);
	//printf("error is: %s\n", strerror(errno));
	//printf("come back to me\n");
	//printf("more tests\n");
	//printf("work pls\n");
	return 0;


}
