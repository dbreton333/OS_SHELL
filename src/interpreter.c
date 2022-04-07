#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <dirent.h>


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
int badcommandSameFileName();
int scheduler(char *policy);
int badcommandTooManyTokens();
int PCB_clear(struct PCB* pcb);
int echo(char* var);
int my_ls();
int exec(char* script[], char* policy, int len);
int PID_temp = 0;
int loadSize = 2;

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

		if(isDigit == 1) return badcommandDigitVariable();	//CAN DELETE THAT EVENTUALLY	
		
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

		printf("Frame Store Size = %d; ", FRAMESIZE);
		printf("Variable Store Size = %d\n", VARMEMSIZE);

		int errCode = 0;
		errCode = run(command_args[1]);
		resetmem();
		resetmemframe();
		PID_temp = 0;
		system("rm -rf ./backstore");
		system("mkdir backstore");
		return errCode;
	
	} else if (strcmp(command_args[0], "echo")==0) {
		
		if (args_size != 2) return badcommand();
		return echo(command_args[1]);

	} else if (strcmp(command_args[0], "my_ls")==0) {
		
		if (args_size != 1) return badcommand();
		return my_ls();

	} else if (strcmp(command_args[0], "exec")==0) {
	
		if (args_size > 5) return badcommand();
		char *programs[args_size-2];


		for(int i = 1 ; i < args_size - 1 ; i++){
			programs[i-1] = strdup(command_args[i]);
		}

		printf("Frame Store Size = %d", FRAMESIZE);
		printf("Variable Store Size = %d", VARMEMSIZE);

		//FILES CAN NOW HAVE THE SAME NAME

		// for(int i = 0 ; i < args_size - 2 ; i++){
		// 	if((i+1<args_size - 2 && (strcmp(programs[i],programs[i+1]) == 0)) || (i+2< args_size - 2 && (strcmp(programs[i],programs[i+2]) == 0 ))){
		// 		return badcommandSameFileName();
		// 	}
		// }

		int errCode = 0;
		errCode = exec(programs, command_args[args_size-1], args_size-2);
		resetmem();
		resetmemframe();
		PID_temp=0;
		system("rm -rf ./backstore");	
		system("mkdir backstore");

		return errCode;
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

	system("rm -rf ./backstore");
	
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}
int badcommandSameFileName(){
	printf("%s\n","Bad command: same file name");
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

	//INITIALISE
	int errCode = 0;
	char line[1000]; //buffer for line
	int size = 0; //size of program
	struct dirent *dir;
	FILE *p = NULL;
	char t[100] = "cp ";
	char w[100] = "backstore/prog";


	//CHECK IF FILE EXIST
	p = fopen(script,"rt");
	if(p == NULL){
		return badcommandFileDoesNotExist();
	}
	fclose(p);


	//COPY FILE IN BACKSORE
	strcat(t, script);
	strcat(t, " ");
	strcat(t, w);
	char prognb[2];
	sprintf(prognb, "%d", PID_temp);
	strcat(t, prognb);
	strcat(t, ".txt");

	system(t);

	//GET FILE FROME BACKSTORE
	strcat(w, prognb);
	strcat(w, ".txt");
	p = fopen(w, "rt");

	//CHECK IF FILE IN BACKSTORE
	if(p == NULL){
		system("rm -rf ./backstore");	
		system("mkdir backstore");
		return badcommandFileDoesNotExist();
	}


	struct PCB *pcb = (struct PCB*) malloc(sizeof(struct PCB)); //create pcb for the file
	head = pcb; //set head
	tail = pcb; //set tail

	pcb->PC = 0;
	pcb->next = NULL;
	pcb->PID = PID_temp;
	PID_temp++;
	pcb->back = NULL;

	int page = 0;

	fgets(line,999,p);


	while(1){

		if(((size % FRAME_L) == 0) && (size != 0)){
			page++;
		}

		if(page > loadSize){
			break;
		}

	

		mem_init_page_value(prognb, page, line);  //set line in corresponding page

	

		size++; //increment size of program

		//if end of file break
		if(feof(p)){
			break;
		}

		//get next line
		fgets(line,999,p);
	}

	//set length of program to size
	pcb->page = page;
	pcb->length = size;
	pcb->score = size;

	//close file
	fclose(p);

	errCode = scheduler("FCFS");

	return errCode;
}

int exec(char* script[], char* policy, int nbr){
	int errCode = 0;

	struct PCB *prev = NULL;
	FILE *p = NULL;
	char *prognbrs[nbr];
	int Iprognbrs[nbr];


	
	for (int i = 0 ; i < nbr; i++){

		//CHECK IF FILE EXIST
		p = fopen(script[i],"rt");
		if(p == NULL){
			return badcommandFileDoesNotExist();
		}
		fclose(p);

		char t[100] = "cp ";
		char w[] = "backstore/prog";

		//COPY FILE IN BACKSTORE
		strcat(t, script[i]);
		strcat(t, " ");
		strcat(t, w);
		char prognb[2];

		sprintf(prognb, "%d", PID_temp);
		prognbrs[i] = strdup(prognb);
		Iprognbrs[i] = PID_temp;
		PID_temp++;
		strcat(t, prognb);
		strcat(t, ".txt");

		system(t);

	}

	for (int i = 0 ; i < nbr; i++){

		//GET FILE FROM BACKSTORE
		char file[100] = "backstore/prog";

		strcat(file, prognbrs[i]);
		strcat(file, ".txt");
		p = fopen(file,"rt");  // open file and p points to it

		if(p == NULL){
			system("rm -rf ./backstore");	
			system("mkdir backstore");
			return badcommandFileDoesNotExist();
		}


		int size = 0; //size of program
		char line[1000]; //buffer for line
		struct PCB *pcb = (struct PCB*) malloc(sizeof(struct PCB)); //create pcb for the file
		

		if ( prev == NULL){
			head = pcb; //set head
			tail = pcb; //set tail

			pcb->PC = 0;
			pcb->currpage = 0;
			pcb->next = NULL;
			pcb->PID = Iprognbrs[i];
			pcb->back = NULL;
			prev = pcb ;
		}
		else{ 
			tail = pcb ;

			pcb->PC = 0;
			pcb->currpage = 0;
			pcb->next = prev;
			pcb->PID = Iprognbrs[i];
			pcb->back = NULL;
			prev->back = pcb;
			prev = pcb;
		}

		int page = 0;

		fgets(line,999,p);

		while(1){

			if(((size % FRAME_L) == 0) && (size != 0)){
				page++;
			}

			if(page > loadSize){
				break;
			}

			mem_init_page_value(prognbrs[i], page, line);  //set line in corresponding page
	
			size++; //increment size of program



			//if end of file break
			if(feof(p)){
				break;
			}

			//get next line
			fgets(line,999,p);
		}

		//set length of program to size
		pcb->page = page;
		pcb->length = size;
		pcb->score = size;

		//close file
		fclose(p);
	}

	errCode = scheduler(policy);
	return errCode;
}

int PCB_clear(struct PCB* pcb){
 //remove PCB from QUEUE

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
	 head = pcb->back;
 }

 if(tail == pcb){
	 tail = pcb->next;
 }
}
