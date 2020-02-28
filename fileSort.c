#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
//TODO:
//QUICK SORT
//DEBUGGING/FIXUPS (FIX COMPARATOR SO EMPTY STRING IS 0 OR '')
//DEBUG LONG STRINGS >310, apparently seg fautlts after 345...
enum boolean {true = 1, false = 0};//boolean var, do enum boolean to declare a new one (see function prototype below)


enum boolean type = true; //true represents Strings, false represents ints

int LLSize = 0; //used to store our stuff in an array to perform the sorts
int bufferSize = 257; //used to expand buffer

typedef struct _node {
	char *word;//should just have to change this to a char* and then instead of strcpy 
			//just set this equal to the word (shouldn't have to malloc this)
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
int partition(int* arr, int low, int high, int(*comparator)(void*,void*));
void quickSortRec(int* arr, int low, int high, int(*comparator)(void*,void*));

int main(int argc, char** argv) { //need to fix it for empty strings like ",," and ", ," or ",\t,",
//also need to add a function to free the LL
//apparently ,, in int version counts as a zero
//gonna just read in the entire file all in one buffer, keep track of buffer size, realloc every time accordingly
//parse through the buffer and extract tokens for each one (just gonna use a while loop to go through entire buffer and
//extract everything
	if (argc < 3) {
		printf("Fatal Error: expected 3 arguments, got less\n");
		exit(0);
	}
	
	if (argv[1][0] != '-') {
		printf("Fatal Error: %s is not a valid sort flag\n", argv[1]);
		exit(0);
	} else if (argv[1][1] != 'q' && argv[1][1] != 'i') {
		printf("Fatal Error: %s is not a valid sort flag\n", argv[1]);
		exit(0);
	}
	
	char sort = argv[1][1];
	
	int fd = open(argv[2],O_RDONLY);	
		
	if (fd == -1) {
		printf("Fatal Error: %s is not a valid file name\n",argv[2]);
		exit(0);	
	}

	Node *head;
	
	enum boolean headSet = false;
	int bytesRead = 1;
	int count = 0;
	int curr = 0;
	char *word = (char*)malloc(bufferSize * sizeof(char)); 
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
					head->word = (char*)malloc(curr * sizeof(char));
					word[curr+1] = '\0';
					strcpy(head->word,word); //changed from strcpy
					type = true; //strings 
				}
				head->next = NULL;
				head->prev = NULL;
				//printf("val of wordf: %s\n",head->data);
				bufferSize = 257;
				word = (char*)malloc(bufferSize * sizeof(char));	
				headSet = true;
				curr = 0;
			} else {
				Node *temp = (Node*)malloc(sizeof(Node));
				//temp->data = (char*)malloc(str * sizeof(char) + 1);
				//temp->word = word;
				if (type == false) {//type is ints
					temp->data = atoi(word);
				} else {//type is strings
					temp->word = (char*)malloc(curr * sizeof(char));
					word[curr+1] = '\0';
					strcpy(temp->word,word); //changed from strcpy
				}
				temp->next = head->next;
				temp->prev = head;
				head->prev = NULL;
				head->next = temp;
				//printf("val of wordt: %s\n",head->next->data);
				bufferSize = 257;
				word = (char*)malloc(bufferSize * sizeof(char));
				curr = 0;
			}
			LLSize++;
		}else if(badChar(buffer) == true) {
			continue;
		}  else {
			if (strlen(word) == bufferSize - 1) {
				//word = (char*) realloc(word, bufferSize * 2);
				char* temp = (char*)malloc(bufferSize * 2 * sizeof(char));
				memcpy(temp,word,(bufferSize - 1));
				bufferSize = bufferSize * 2;
				word = temp;
				free(temp);
			}	
			word[curr] = buffer;
			curr++;
		}
		count+= bytesRead;
	}while(bytesRead > 0);
	if (strlen(word) != 0 || badChar(word[0] == false)) {
	LLSize++;
	Node *temp = (Node*)malloc(sizeof(Node));
	//temp->word = word;
		if (type == false) {//type is ints
			temp->data = atoi(word);
		} else {//type is strings
			temp->word = (char*)malloc(curr * sizeof(char));
			bufferSize = 257;
			word[curr+1] = '\0';
			strcpy(temp->word,word);//changed from strcpy
			//printf("b4 list: %s\n",temp->word);
			//printf("size: %d\n",strlen(temp->word));
			//printf("made it: %s\n",temp->word);
		}
		temp->next = head->next;
		temp->prev = head;
		temp->prev = NULL;
		head->next = temp;
	}
	//printf("val of word: %s\n", word); //This prints the last word of the tokenizer since it is stored
						   //after the file is finished being browsed
	//END OF TOKENIZER FUNCTION -----------------------------------------------------------
	//LLIterator(head);, will use this later
	//printf("LLSize: %d\n", LLSize);
	
	
	int  (*fptr)(void*,void*);
	fptr = comparator;	
	if (sort == 'i') {
		int t = insertionSort(head,fptr);	
	} else {
		//printf("quicksort under maintenance lmao\n");
		//printf("\n");
		int t = quickSort(head, fptr);
	}

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
	LLIterator(head);
	close(fd);
	return 0;


}
//Feel free to change the iterator functionality, as well as adding insert method
void LLIterator (Node *ptr) {
	if (type == true) {//strings
		while(ptr != NULL) {
			printf("%s\n",ptr->word);
			ptr = ptr->next;
		}
	} else {//ints
		while (ptr != NULL) {
			printf("%d\n",ptr->data);
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
		int least;
		int len1 = strlen(item1);
		int len2 = strlen(item2);
		char *str1 = (char*)(item1); 
		char *str2 = (char*)(item2);
		if (len1 > len2) {
			if (len2 == 0) {
				return 1;
			}
			least = len2;	
		} else {
			if (len1 == 0) {
				return 0;
			}
			least = len1;
		}
		int i = 0;
		while (i < least) {
			if (str1[i] != str2[i]) {
				return ((int)(str1[i] - str2[i]));
			}
			i++; 
		}
		return 0;
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
int insertionSort(void* toSort, int (*comparator)(void*,void*)) { //String sort isn't working for certain file formats, unsure why but I'll figure it out
//tmrw
	Node* ptr = (Node*)toSort;
	Node* sorted = (Node*)toSort;
	if (type == false) { //int sorting
		int* intSort = (int*)malloc(LLSize * sizeof(int));
		int i = 0;
		while (i < LLSize) {
			intSort[i] = ptr->data;
			ptr = ptr->next;
			//printf("int: %d\n",intSort[i]);
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
		//printf("SORTED VALS\n");
		int g = 0;
		while (g < LLSize) {
			sorted->data = intSort[g];
			//printf("Sorted: %d\n",sorted->data);
			sorted = sorted->next;
			g++;	
		}
	} else { //string sorting
		char** stringSort = (char**)malloc(LLSize * sizeof(char*));
		int i = 0;
		while (i < LLSize) {
			stringSort[i] = ptr->word;
			ptr = ptr->next;
			//printf("string: %s\n",stringSort[i]);
			i++;
		}
		//insertion sort
		int j = 1;
		int k = 0;
		while (j < LLSize) {
			char *comp;
			strcpy(comp,stringSort[j]);
			k = j-1;
			while (k >= 0 && comparator(stringSort[k],comp/*comp*/) > 0) {
				strcpy(stringSort[k+1],stringSort[k]);
				k--;
			}
			strcpy(stringSort[k+1],comp/*comp*/);
			j++;
		}
		//printf("SORTED VALS\n");
		int g = 0;
		while (g < LLSize) {
				
			//strcpy(sorted->word,stringSort[g]);
			//printf("stringSort[g]: %s\n",stringSort[g]);
			sorted->word = stringSort[g];
			//printf("Sorted: %s\n", sorted->word);
			sorted = sorted->next;
			g++;
		}
		
	}// following this, everything is stored, now we can sort using the comparator (o god)
	
	
	return 1; //success
}

int quickSort(void* toSort, int (*comparator)(void*,void*)) {
//quicksort is recursive in nature so we need to make a seperate function for this function to call
//maybe have the quicksort function return an array or make 2 (one for strings, one for ints) and call
//it accordingly
	//printf("size of LL is: %d\n", LLSize);
	//printf("\n");
	Node* ptr = (Node*)toSort;
	Node* sorted = (Node*)toSort;
	int high = LLSize-1;
	if(type == false) { //int
		int* intSort = (int*)malloc(LLSize * sizeof(int));
		int i = 0;
		while (i < LLSize) {
			intSort[i] = ptr->data;
			ptr = ptr->next;
			//printf("int: %d\n",intSort[i]);
			i++;
		}
		
		quickSortRec(intSort, 0, high, comparator);
			
		int g = 0;
		while (g < LLSize) {
			sorted->data = intSort[g];
			//printf("Sorted: %d\n",sorted->data);
			sorted = sorted->next;
			g++;	
		}
	} else { //string
		char** stringSort = (char**)malloc(LLSize * sizeof(char*));
		int i = 0;
		while (i < LLSize) {
			stringSort[i] = ptr->word;
			ptr = ptr->next;
			//printf("string: %s\n",stringSort[i]);
			i++;
		}
		//quickSortRec(stringSort,0,high,comparator);
		int g = 0;
		while (g < LLSize) {
			sorted->word = stringSort[g];
			sorted = sorted->next;
			g++;
		}
		
	}
	
	return 1;
}

int partition(int* arr, int low, int high, int(*comparator)(void*,void*)) {
	int pivot = arr[high];
	int i = (low-1);
	int j = low;
	while(j < high) {
	//	if(arr[j] < pivot) {
		if(comparator(&arr[j], &pivot) == 2) {
			i++;
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
		j++;
	}

	int temp = arr[i+1];
	arr[i+1] = arr[high];
	arr[high] = temp;

	return i+1;
}	

void quickSortRec(int* arr, int low, int high, int(*comparator)(void*,void*)) {
	if (low < high) {
		int pi = partition(arr, low, high, comparator);
		quickSortRec(arr, low, pi-1, comparator);
		quickSortRec(arr, pi+1, high, comparator);
	}
}










