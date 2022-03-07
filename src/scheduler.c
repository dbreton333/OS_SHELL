#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"
#include "interpreter.h"


int FCFS();
int RR();
int AGING();
int SJF();
int badcommandNoSuchPolicy();

int scheduler(char *policy){
	if(strcmp(policy,"FCFS") == 0){
		return FCFS();
	}else if(strcmp(policy,"SJF") == 0){
		return SJF();
	}else if(strcmp(policy,"RR") == 0){
		return RR();
	}else if(strcmp(policy,"AGING") == 0){
		return AGING();
	}else{
		return badcommandNoSuchPolicy();
	}
}

int SJF(){
	int errCode = 0;
	struct PCB* pcb = tail;

	// 
	while (1){
		struct PCB* prev = pcb->back;
		if (prev != NULL && pcb->length > prev->length) {
			pcb->back = prev->back;
			if(prev->back != NULL){
				prev->back->next = pcb;
			}
			prev->next = pcb->next;
			if(pcb->next != NULL){
				pcb->next->back = prev;
			}
			pcb->next = prev;
			tail = prev;
		}else{
		
		}
	}
	
	
	while (sjf != NULL){
		
		for (int i = sjf->PC; i < (sjf->length + sjf->base) ; i++){
			char index[4];	
			sprintf(index,"%d",i);
			char* userInput = mem_get_value(index);
			char* token;
			char** liToken =  malloc(10 * sizeof(char*));;
			int k = 0;

			//online mode -> checks if there is the symbole ;
			if(strchr(userInput, ';') != NULL){
		
				token = strtok(userInput, ";");

				while( token != NULL ) {
					liToken[k] = malloc(200);
					strcpy(liToken[k], token);
					token = strtok(NULL, ";");
					k++;
				}

				int j = 0;
				
				while( liToken[j] != NULL){
					//parseInput for every instruction
					errCode = parseInput(liToken[j]);
					if (errCode == -1) exit(99);	// ignore all other errors
					memset(liToken[j], 0, sizeof(liToken[j]));
					free(liToken[j]); 
					j++;
				}
			}
			else{

				errCode = parseInput(userInput);
				if (errCode == -1) exit(99);	// ignore all other errors
				memset(userInput, 0, sizeof(userInput));	
			}
			free(liToken);
		}
		//clear pcb when pcb reaches the end
		PCB_clear(sjf);
		sjf = sjf->back;
	}
	return errCode;
}

int RR(){
	
	return 0; 
}

int AGING(){

	return 0; //for compilation
}

int FCFS(){
	int errCode = 0;
	struct PCB* pcb = tail;

	while(pcb != NULL){
		printf("pcb len : %d\n",pcb->length);
		printf("pcb# : %d\n",pcb->PID);
		//printf("pcb back : %d\n",pcb->PC);
		for (int i = pcb->PC; i < (pcb->length + pcb->base); i++){
			
			//printf("loop pcb# : %d\n",pcb->PID);
			char index[4];	
			sprintf(index,"%d",i);
			
			char* userInput = mem_get_value(index);
			//printf("first %s\n",userInput);
			char* token;
			char** liToken =  malloc(10 * sizeof(char*));;
			int k = 0;
			
			//online mode -> checks if there is the symbole ;
			if(strchr(userInput, ';') != NULL){
				token = strtok(userInput, ";");

				while( token != NULL ) {
					liToken[k] = malloc(200);
					strcpy(liToken[k], token);
					token = strtok(NULL, ";");
					k++;
				}

				int j = 0;
				
				while( liToken[j] != NULL){
					//parseInput for every instruction
					errCode = parseInput(liToken[j]);
					
					if (errCode == -1) exit(99);	// ignore all other errors
					memset(liToken[j], 0, sizeof(liToken[j]));
					free(liToken[j]); 
					j++;
				}
			}else{
				//printf("else pid %d\n",pcb->PID);
				errCode = parseInput(userInput);
				//printf("str %s\n",userInput);
			
				if (errCode == -1) exit(99);	// ignore all other errors
				memset(userInput, 0, sizeof(userInput));	
			}
			free(liToken);
		}
		//clear pcb when pcb reaches the end
		PCB_clear(pcb);
		printf("before ending\n");
		pcb = pcb->back;
		printf("finish\n");
	}
	return errCode;
}