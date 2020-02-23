#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
//TODO:
//TYPE AGNOSTIC SORTS
enum boolean {true = 1, false = 0};//boolean var, do enum boolean to declare a new one (see function prototype below)


enum boolean type = true; //true represents Strings, false represents ints

int LLSize = 0; //used to store our stuff in an array to perform the sorts

typedef struct _node {
	char word[257];
	int data;
	struct _node *next;
	struct _node *prev;
}Node;

int comparator(void*,void*);
void LLIterator(Node*);
void freeLL(Node*);
enum boolean commaDetect(char);
enum boolean badChar(char);
int insertionSort(void* toSort, int (*comparator)(void*,void*));
int quickSort(void* toSort, int (*comparator)(void*,void*));

int main(int argc, char** argv) {

	if (argc < 3) {
		printf("Fatal Error: expected 3 arguments, got less\n");
		exit(0);
	}
	
	if (argv[1][0] != '-') {
		printf("Fatal Error: %s is not a valid sort flag\n", argv[1]);
		exit(0);
	} else if (argv[1][1] == 'q' || argv[1][1] == 'i') {
	} else {
		printf("Fatal Error: %s is not a valid sort flag\n", argv[1]);
		exit(0);
	}
	
	char sort = argv[1][1];
	
	int fd = open(argv[2],O_RDONLY);	
	
	Node *head;
	
	enum boolean headSet = false;
	int bytesRead = 1;
	int count = 0;
	int curr = 0;
	char word[257]; 
	//TOKENIZER, can chamge the Linked list stuff, don't touch the tokenizer unless you think theres an issue,
	//functionality is right but it doesn't store it into the linked list properly
	//DESCRIPTION: Tokenizer removes bad characters (spaces, tabs, new lines, and underscores) and only
	//stores ascii character data, once a comma is reached, the tokenizer stores it in the respective linked list data type
	//and then we can print it out through the iterator 
	do {
		char buffer;
		bytesRead = read(fd,&buffer,1);
		if (commaDetect(buffer) == true){//Need to fix these LL's yo
			if (headSet == false) {
				head = (Node*)malloc(sizeof(Node));
				//head->data = (char*)malloc(str * sizeof(char) + 1);
				//head->word = word;
				if (isdigit(word[0]) != 0 || word[0] == '-') {
					head->data = atoi(word);
					type = false; //ints
				} else {
					strcpy(head->word,word);
					type = true; //strings 
				}
				head->next = NULL;
				head->prev = NULL;
				//printf("val of wordf: %s\n",head->data);
				memset(word,'\0',257);
				headSet = true;
				curr = 0;
			} else {
				Node *temp = (Node*)malloc(sizeof(Node));
				//temp->data = (char*)malloc(str * sizeof(char) + 1);
				//temp->word = word;
				if (type == false) {//type is ints
					temp->data = atoi(word);
				} else {//type is strings
					strcpy(temp->word,word);
				}
				temp->next = head->next;
				temp->prev = head;
				head->prev = NULL;
				head->next = temp;
				//printf("val of wordt: %s\n",head->next->data);
				memset(word,'\0',257);
				curr = 0;
			}
			LLSize++;
		}else if(badChar(buffer) == true) {
			continue;
		}  else {
			word[curr] = buffer;
			curr++;
		}
		count+= bytesRead;
	}while(bytesRead > 0);
	LLSize++;
	Node *temp = (Node*)malloc(sizeof(Node));
	//temp->word = word;
	if (type == false) {//type is ints
		temp->data = atoi(word);
	} else {//type is strings
		strcpy(temp->word,word);
	}
	temp->next = head->next;
	temp->prev = head;
	temp->prev = NULL;
	head->next = temp;
	//printf("val of word: %s\n", word); //This prints the last word of the tokenizer since it is stored
						   //after the file is finished being browsed
	//END OF TOKENIZER FUNCTION -----------------------------------------------------------
	//LLIterator(head);, will use this later
	printf("LLSize: %d\n", LLSize);
	
	
	int  (*fptr)(void*,void*);
	fptr = comparator;	
	int t = insertionSort(head,fptr);	
	/*
	int i = 0;
	
	char** stringSort;
	int* intSort;
	
	if (type == false) {//type is ints
		intSort = (int*)malloc(LLSize * sizeof(int));
		while (i < LLSize) {
			intSort[i] = ptr->data;
			ptr = ptr->next;
			printf("toSort: %d\n",intSort[i]);
			i++;
		}
	}  else { //type is strings
		stringSort = (char**)malloc(LLSize * sizeof(char*));
		while (i < LLSize) {
			stringSort[i] = ptr->word;
			ptr = ptr->next;
			printf("toSort: %s\n",stringSort[i]);
			i++;
		}	
	}
	*/	
	close(fd);
	return 0;


}
//Feel free to change the iterator functionality, as well as adding insert method
void LLIterator (Node *ptr) {
	if (type == true) {//strings
		while(ptr != NULL) {
			printf("LNode: %s\n",ptr->word);
			ptr = ptr->next;
		}
	} else {//ints
		while (ptr != NULL) {
			printf("LNode: %d\n",ptr->data);
			ptr = ptr->next;
		}
	}
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

int comparator (void* item1, void* item2) { //using global variable type in this case to determine whether we are working with strings or ints
	if (type == true) { //String comparison 
		int comp = strcmp(item1,item2);//works on void* as well, tested this and it properly compares the string
		if (comp > 0) {
			return 1; //returns item 1 since it is greater
		} else if (comp == 0) {
			return 1; //returns item 1 since they are equal
		} else if (comp < 0) {
			return 2; //returns item 2 since item 1 is less
		}
	} else { //int comparison
		if (*(int*)item1 == *(int*)item2) { //casts them as ints then dereferences them to avoid errors i think
			return 1; //returns 1 if we are to insurt item1
		} else if (*(int*)item1 > *(int*)item2) {
			return 1; //returns 1 if we are to insert item1
		} else { //returns 2 if we are to insert item 2
			return 2;
		}	
	
	}	
	return -1; //indicates error, should never get here tho
}
//Insertion sort, the integer sort is working fine, but the string sort isn't working, I think its an isssue with the comparator
//or the insertion sort I wrote for strings itself
int insertionSort(void* toSort, int (*comparator)(void*,void*)) {
	Node* ptr = (Node*)toSort;
	Node* sorted = (Node*)toSort;
	if (type == false) { //int sorting
		int* intSort = (int*)malloc(LLSize * sizeof(int));
		int i = 0;
		while (i < LLSize) {
			intSort[i] = ptr->data;
			ptr = ptr->next;
			printf("int: %d\n",intSort[i]);
			i++;
		}
		//insertion soort
		int j = 1;
		int k = 0;
		while (j < LLSize) {
			int comp = intSort[j];
			k = j-1;
			while (k >= 0 && comparator(&intSort[k],&comp) == 1) {
				intSort[k+1] = intSort[k];
				k--;
			}
			intSort[k+1] = comp;
			j++;
		}
		printf("SORTED VALS\n");
		int g = 0;
		while (g < LLSize) {
			sorted->data = intSort[g];
			printf("Sorted: %d\n",sorted->data);
			sorted = sorted->next;
			g++;	
		}
	} else { //string sorting
		char** stringSort = (char**)malloc(LLSize * sizeof(char*));
		int i = 0;
		while (i < LLSize) {
			stringSort[i] = ptr->word;
			ptr = ptr->next;
			printf("string: %s\n",stringSort[i]);
			i++;
		}
		//insertion sort
		int j = 1;
		int k = 0;
		while (j < LLSize) {
			char* comp = stringSort[j];
			k = j-1;
			while (k >= 0 && comparator(stringSort[j],comp) == 1) {
				stringSort[k+1] = stringSort[k];
				k--;
			}
			stringSort[k+1] = comp;
			j++;
		}
		printf("SORTED VALS\n");
		int g = 0;
		while (g < LLSize) {
			strcpy(sorted->word,stringSort[g]);
			printf("Sorted: %s\n", stringSort[g]);
			sorted = sorted->next;
			g++;
		}
		
	}// following this, everything is stored, now we can sort using the comparator (o god)
	
	
	return 1; //success
}

int quickSort(void* toSort, int (*comparator)(void*,void*)) {

}













