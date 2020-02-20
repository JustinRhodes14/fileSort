#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
//TODO: LINKED LIST FUNCTIONALITY
//STORE STUFF IN LINKED LIST TYPE AGNOSTICALLY
//PRINT OUT STUFF TYPE AGNOSTICALLY
//COMPARATOR
//TYPE AGNOSTIC SORTS
enum boolean {true = 1, false = 0};//boolean var, do enum boolean to declare a new one (see function prototype below)

typedef struct _node {
	void *data;
	struct _node *next;
	struct _node *prev;
}Node;

void LLIterator(Node*);
void freeLL(Node*);
enum boolean commaDetect(char);
enum boolean badChar(char);

//lols

int main(int argc, char** argv) {

	if (argc < 3) {
		printf("Fatal Error: expected 3 arguments, got less\n");
		exit(0);
	}
	
	char type = argv[2][1];
	
	int fd = open(argv[1],O_RDONLY);	
	
	Node *head = (Node*)malloc(sizeof(Node));//This is just for testing, need to officially work on LL's
	head->data = "first";
	head->next = NULL; 
	head->prev = NULL;
	int bytesRead = 1;
	int count = 0;
	int curr = 0;
	char* last = ",";
	char word[257]; 
	//TOKENIZER, can chamge the Linked list stuff, don't touch the tokenizer unless you think theres an issue,
	//functionality is right but it doesn't store it into the linked list properly
	//DESCRIPTION: Tokenizer removes bad characters (spaces, tabs, new lines, and underscores) and only
	//stores ascii character data, once a comma is reached, the tokenizer will print out the word, after we get
	//further into the project instead of printing we will store it in the linked list to sort
	do {
		char buffer;
		bytesRead = read(fd,&buffer,1);
		if (commaDetect(buffer) == true){//Need to fix these LL's yo
			Node *temp = (Node*)malloc(sizeof(Node));
			temp->data = word;
			temp->next = head->next;
			temp->prev = head;
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
	printf("val of word: %s\n", word); //This prints the last word of the tokenizer since it is stored
					   //after the file is finished being browsed
	//END OF TOKENIZER FUNCTION -----------------------------------------------------------
	//LLIterator(head);
	close(fd);
	return 0;


}
//Feel free to change the iterator functionality, as well as adding insert method
void LLIterator (Node *ptr) {
	while(ptr != NULL) {
		printf(" LNode: %s->",ptr->data);
		ptr = ptr->next;
	}
	printf("\n");
}
//This should be good but maybe change it as you work on linked lists more
void freeLL(Node *ptr) {

	while(ptr != NULL) {
		Node* temp = ptr->next;
		Node* temp2 = ptr;
		ptr = temp;
		free(temp2);
	}

}
//Makes sure we don't read in bad characters from strings
enum boolean badChar(char input) {
	if(input == '\t' || input == '_' || input == '\n' || input == ' ') {
		return true;
	}	

	return false;
}
//Detects commas so we know when to store the token
enum boolean commaDetect(char input) {
	if (input == ',') {
		return true;
	}

	return false;

}
