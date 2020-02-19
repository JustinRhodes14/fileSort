#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

enum boolean {true = 1, false = 0};

typedef struct _node {
	void *data;
	struct _node *next;
}Node;

void LLIterator(Node*);
void freeLL(Node*);
enum boolean commaDetect(char);
enum boolean badChar(char);

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Fatal Error: expected 2 arguments, got less\n");
		exit(0);
	}
	
	char type = argv[1][1];
	
	int fd = open("test.txt",O_RDONLY);	
	
	Node *head = (Node*)malloc(sizeof(Node));
	head->data = "first";
	head->next = NULL; 
	int bytesRead = 1;
	int count = 0;
	int curr = 0;
	char* last = ",";
	char word[257]; 
	do {
		char buffer;
		bytesRead = read(fd,&buffer,1);
		if (commaDetect(buffer) == true){
			Node *temp = (Node*)malloc(sizeof(Node));
			temp->data = word;
			temp->next = head->next;
			head->next = temp;
			printf("val of word: %s\n",word);
			memset(word,'\0',257);
			curr = 0;
		}else if(badChar(buffer) == true) {
			continue;
		}  else {
			word[curr] = buffer;
			curr++;
		}
		count+= bytesRead;
	}while(bytesRead > 0);
	printf("val of word: %s\n", word);
	//LLIterator(head);
	close(fd);
	return 0;


}

void LLIterator (Node *ptr) {
	while(ptr != NULL) {
		printf(" LNode: %s->",ptr->data);
		ptr = ptr->next;
	}
	printf("\n");
}

void freeLL(Node *ptr) {

	while(ptr != NULL) {
		Node* temp = ptr->next;
		Node* temp2 = ptr;
		ptr = temp;
		free(temp2);
	}

}

enum boolean badChar(char input) {
	if(input == '\t' || input == '_' || input == '\n' || input == ' ') {
		return true;
	}	

	return false;
}

enum boolean commaDetect(char input) {
	if (input == ',') {
		return true;
	}

	return false;

}
