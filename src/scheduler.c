#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"
#include "scheduler.h"

int FCFS();
int RR();
int AGING();
int SJF();
int badcommandNoSuchPolicy();

struct PCB *head;
struct PCB *tail;
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
	struct PCB* sjf= pcb;
	
	while (pcb != NULL){
		if (sjf->length > pcb->length){
			sjf->next = pcb->next;
			pcb->back = sjf->back;
			pcb->next->back = sjf;
			sjf->back = pcb;
			pcb->next = sjf;
			sjf = pcb;

		}
		pcb = pcb->back;
	}

	
	while (pcb != NULL){
		
		for (int i = pcb->PC; i < pcb->length ; i++){
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
			}else{

				errCode = parseInput(userInput);
				if (errCode == -1) exit(99);	// ignore all other errors
				memset(userInput, 0, sizeof(userInput));	
			}
			free(liToken);
		}
		//clear pcb when pcb reaches the end
		PCB_clear(pcb);
		pcb = pcb->back;
	}
	return errCode;
	 //for compilation
}

int RR(){

	return 0; //for compilation
}

int AGING(){

	return 0; //for compilation
}

int FCFS(){
	int errCode = 0;
	struct PCB* pcb = tail;

	while(pcb != NULL){
		for (int i = pcb->PC; i < pcb->length ; i++){
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
			}else{

				errCode = parseInput(userInput);
				if (errCode == -1) exit(99);	// ignore all other errors
				memset(userInput, 0, sizeof(userInput));	
			}
			free(liToken);
		}
		//clear pcb when pcb reaches the end
		PCB_clear(pcb);
		pcb = pcb->back;
	}
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
	 tail == pcb->back;
 }
}