#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "scheduler.h"

int MAX_ARGS_SIZE = 7; //7 for set command 2 (Command + Var) + 5 (maximum number of arguments)

int help();
int quit();
int badcommand();
int FCFS();
int RR();
int AGING();
int SJF();
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int badcommandDigitVariable();
int badcommandFileDoesNotExist();
int badcommandNoSuchPolicy();
int scheduler(char *policy);
int badcommandTooManyTokens();
int PCB_clear(struct PCB* pcb);
int echo(char* var);
int my_ls();
int exec(char* script[], char* policy, int len);
int PID_temp = 0;

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;
	
	//switch from batch mode to interactive mode when the file reaches the end
	if ( args_size < 1){
		char userInput[MAX_USER_INPUT];		// user's input stored here
		int errorCode = 0;					// zero means no error, default

		for (int i=0; i<MAX_USER_INPUT; i++) //init user input
		userInput[i] = '\0';

		freopen("/dev/tty", "r", stdin); //change to interactive mode

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
		
		//test
		if (args_size > 7) return badcommandTooManyTokens(); //2 (Command + Var) + 5 (maximum number of arguments)
		if (args_size < 3) return badcommand(); //if the variable is set to nothing
		char tmp[16];
		sscanf(command_args[1],"%s", tmp);
		//check if variable is an integer
		int isDigit = 1;
		int j=0;

		while(j<strlen(tmp) && isDigit == 1){
			if(tmp[j] <= '0' || tmp[j] >='9'){
				isDigit = 0;
			}
			j++;
		}

		if(isDigit == 1) return badcommandDigitVariable();			
		
		//concat all the tokens together
		char concatArgs[700]; 
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
	
	} else if (strcmp(command_args[0], "echo")==0) {
		
		if (args_size != 2) return badcommand();
		return echo(command_args[1]);

	} else if (strcmp(command_args[0], "my_ls")==0) {
		
		if (args_size != 1) return badcommand();
		return my_ls();

	} else if (strcmp(command_args[0], "exec")==0) {
	
		if (args_size > 5) return badcommand();
		
		char *programs[args_size-1];

		for(int i = 1 ; i < args_size - 1 ; i++){
			programs[i-1] = strdup(command_args[i]) ;
		}
		return exec(programs, command_args[args_size-1],args_size-2);
	}
	else return badcommand();
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

int badcommandDigitVariable(){
	printf("%s\n", "Connot have a integer as a variable");
	return 1;
}

int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many Tokens");
	return 2;
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int badcommandNoSuchPolicy(){
	printf("%s\n", "Bad command: No such policy. Chose betweem FCFS, RR, SJF and AGING");
	return 1;
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

int echo(char* var){
	//checking if first char is $
	if (var[0] == '$'){
		//if yes remove it
		char* v = &var[1];
		//check if the variable exist
		if (strcmp (v , "Variable does not exist")==0){
			printf("\n");
		}
		//if it does display value
		else print(v);
	}
	//if no $ just print/echo variable
	else printf("%s\n", var);

	return 0;
}

int my_ls(){
	//calling system to output ls in a vertical way
	system("ls -1");
   return(0);
}

int run(char* script){
	int errCode = 0;

	char line[1000]; //buffer for line
	int var = 0; //line number
	int size = 0; //size of program
	FILE *p = fopen(script,"rt");  // open file and p points to it

	struct PCB *pcb = (struct PCB*) malloc(sizeof(struct PCB)); //create pcb for the file
	head = pcb; //set head
	tail = pcb; //set tail

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	pcb->base = var;
	pcb->PC = var;
	pcb->next = NULL;
	pcb->PID = PID_temp;
	PID_temp++;
	pcb->back = NULL;

	fgets(line,999,p);

	while(1){

		char buffer[4]; //string buffer for integer conversion
		sprintf(buffer,"%d",var); //copy integer as a string ex: 1 -> "1"

		set(buffer, line); //set line to line number as variable
	
		var++; //increment line number
		size++; //increment size of program

		//if end of file break
		if(feof(p)){
			break;
		}

		//get next line
		fgets(line,999,p);
	}

	//set length of program to size
	pcb->length = size;
	pcb->score =size;

	//close file
	fclose(p);

	errCode = scheduler("FCFS");

	return errCode;
}
int exec(char* script[], char* policy, int nbr){
	int errCode = 0;
	int var = 0; //line number
	struct PCB *prev = NULL;

	for (int i = 0 ; i < nbr; i++){

		FILE *p = fopen(script[i],"rt");  // open file and p points to it

		if(p == NULL){
				return badcommandFileDoesNotExist();
		}

		int size = 0; //size of program
		char line[1000]; //buffer for line
		struct PCB *pcb = (struct PCB*) malloc(sizeof(struct PCB)); //create pcb for the file
		

		if ( prev == NULL){
			head = pcb; //set head
			tail = pcb; //set tail

			pcb->base = var;
			pcb->PC = var;
			pcb->next = NULL;
			pcb->PID = PID_temp;
			PID_temp++;
			pcb->back = NULL;
			prev = pcb ;
		}
		else{ 
			head = pcb ;

			pcb->base = var;
			pcb->PC = var;
			pcb->next = prev;
			pcb->PID = PID_temp;
			PID_temp++;
			pcb->back = NULL;
			prev->back = pcb;
			prev = pcb;
		}

		fgets(line,999,p);

		while(1){

			char buffer[4]; //string buffer for integer conversion
			sprintf(buffer,"%d",var); //copy integer as a string ex: 1 -> "1"

			set(buffer, line); //set line to line number as variable
		
			var++; //increment line number
			size++; //increment size of program

			//if end of file break
			if(feof(p)){
				break;
			}

			//get next line
			fgets(line,999,p);
		}

		//set length of program to size
		pcb->length = size;
		pcb->score = size;

		//printf("pcb: %d and size: %d \n", pcb->PID, size);

		//close file
		fclose(p);
	}

	errCode = scheduler(policy);
	return errCode;
}

int PCB_clear(struct PCB* pcb){
 //clear the PCB in the shell memory
 //remove PCB from QUEUE
 char index[4];	
 for(int i = pcb->base; i < pcb->length; i++){
	sprintf(index,"%d",i);
	mem_clear(index);
 }

 if(pcb->back != NULL){
	 if(pcb->next != NULL){
	 	pcb->back->next = pcb->next;
	 }else{
		pcb->back->next = NULL;
	 }
 }

 if(pcb->next != NULL){
	 if(pcb->back != NULL){
		 pcb->next->back = pcb->back;
	 }else{
		 pcb->next->back = NULL;
	 }
 }

 if(head == pcb){
	 head = pcb->next;
 }

 if(tail == pcb){
	 tail = pcb->back;
 }
}
