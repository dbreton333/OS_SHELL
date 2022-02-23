
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "interpreter.h"
#include "shellmemory.h"


int MAX_USER_INPUT = 1000;
int parseInput(char ui[]);

// Start of everything
int main(int argc, char *argv[]) {

	printf("%s\n", "Shell version 1.1 Created January 2022");
	help();

	char prompt = '$';  				// Shell prompt
	char userInput[MAX_USER_INPUT];		// user's input stored here
	int errorCode = 0;					// zero means no error, default

	//init user input
	for (int i=0; i<MAX_USER_INPUT; i++)
		userInput[i] = '\0';
	
	//init shell memory
	mem_init();

	while(1) {							
		printf("%c ",prompt);
		fgets(userInput, MAX_USER_INPUT-1, stdin);
		char* token;
		char** liToken =  malloc(10 * sizeof(char*));;
		int i = 0;

		//online mode -> checks if there is the symbole ;
		if(strchr(userInput, ';') != NULL){
	
			token = strtok(userInput, ";");

			while( token != NULL ) {
				liToken[i] = malloc(200);
				strcpy(liToken[i], token);
				token = strtok(NULL, ";");
				i++;
			}

			int j = 0;
			
			while( liToken[j] != NULL){
				//parseInput for every instruction
				errorCode = parseInput(liToken[j]);
				if (errorCode == -1) exit(99);	// ignore all other errors
				memset(liToken[j], 0, sizeof(liToken[j]));
				free(liToken[j]); 
				j++;
			}
		}else{

			errorCode = parseInput(userInput);
			if (errorCode == -1) exit(99);	// ignore all other errors
			memset(userInput, 0, sizeof(userInput));	
		}
		free(liToken);
	}


	return 0;

}

// Extract words from the input then call interpreter
int parseInput(char ui[]) {
 
	char tmp[200];
	char *words[100];							
	int a,b;							
	int w=0; // wordID

	
	// skip white spaces
	for(a=0; ui[a]==' ' && a<1000; a++);

	while(ui[a] != '\0' && a<1000) {
		
		//get rid of spaces
		for(a; ui[a]==' '; a++);

		for(b=0; ui[a]!='\0' && ui[a]!=' ' && ui[a] && a<1000; a++, b++)
			tmp[b] = ui[a];						// extract a word
	 
		tmp[b] = '\0';

		words[w] = strdup(tmp);

		w++;

		if(ui[a] == '\0'){
    		break;
		}

		a++; 
	}


	return interpreter(words, w);
}
