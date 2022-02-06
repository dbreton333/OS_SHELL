#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 7; //7 for set command 2 (Command + Var) + 5 (maximum number of arguments)

int help();
int quit();
int badcommand();
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int badcommandFileDoesNotExist();
int badcommandTooManyTokens();

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;
	
	if ( args_size < 1){
		char userInput[MAX_USER_INPUT];		// user's input stored here
		int errorCode = 0;					// zero means no error, default

		for (int i=0; i<MAX_USER_INPUT; i++) //init user input
		userInput[i] = '\0';

		freopen("/dev/tty", "r", stdin);

		fgets(userInput, MAX_USER_INPUT-1, stdin);
		errorCode = parseInput(userInput);
		if (errorCode == -1) exit(99);	// ignore all other errors
		memset(userInput, 0, sizeof(userInput));

		return errorCode;
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size > 7) return badcommandTooManyTokens(); //2 (Command + Var) + 5 (maximum number of arguments)
		if (args_size < 3) return badcommand();
		char concatArgs[700]; //all tokens concat together
		char *space = " ";
		strcpy(concatArgs, command_args[2]);
		for (int j = 3; j<args_size; j++){
			strcat(concatArgs, space); // add spaces between tokens
			strcat(concatArgs, command_args[j]); // add token
		}
		return set(command_args[1], concatArgs);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many Tokens");
	return 2;
}

int set(char* var, char* value){

	char *link = "=";
	char buffer[1000];
	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);

	mem_set_value(var, value);

	return 0;

}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	fgets(line,999,p);
	while(1){
		errCode = parseInput(line);	// which calls interpreter()
		memset(line, 0, sizeof(line));

		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

    fclose(p);

	return errCode;
}
