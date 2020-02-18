#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct _node {
	void *data;
	struct _node *next;
}Node;

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Fatal Error: expected 2 arguments, got less\n");
		exit(0);
	}
	
	char type = argv[1][1];
	
	int fd = open("test.txt",O_RDONLY);	
	
	Node *head = (Node*)malloc(sizeof(Node));

	head->data = "hello"; 
	head->next = NULL;
	printf("val of head: %s\n",head->data);	
	
	char buffer[101];
	memset(buffer,'\0',101);	
	int bytesRead = 1;
	int curr = 0;
	do {
		
		if (curr >= 100) {
			curr = 0;
			Node *temp = (Node*)malloc(sizeof(Node));
			temp->data = buffer;
			temp->next = head->next;
			head->next = temp;
			memset(buffer,'\0',101);	
		}
		bytesRead = read(fd,buffer,100-curr);
		curr += bytesRead;		
	}while(bytesRead > 0);
	
	close(fd);	
	
	printf("buffer bytes: %d\n",bytesRead);
	printf("buffer: %s\n",buffer);
	printf("type: %c\n", type);
	return 0;


}

void LLIterator (Node *ptr) {

}

int badChar(char buffer) {
	if(buffer == '\t' || buffer == '_' || buffer == '\n' || buffer == ' ') {
		return 1;
	}	

	return 0;
}
