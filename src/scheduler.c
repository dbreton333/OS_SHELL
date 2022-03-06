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
	struct PCB* sjf = pcb;
	struct PCB* temp = pcb;

	//int i = 0;
	while (pcb != NULL){
		//printf("in i = %d \n",i);
		if (sjf->length > pcb->length) {
			printf("sjf length : %d\n",sjf->length);
			printf("pcb length : %d\n",pcb->length);
			//printf("in condition\n");
				sjf->back = pcb->back;
				//printf("1\n");
				pcb->next = sjf->next;
				//printf("2\n");
				if(pcb->back != NULL){
					pcb->back->next = sjf;
					//printf("3\n");
				}
				sjf->next = pcb;
				//printf("4\n");
				pcb->back = sjf;
				//printf("5\n");
				temp = sjf;
				sjf = pcb;
				printf("id : %d \n", sjf->PID);
		}
		printf("pcb %d and length %d \n", pcb->PID, pcb->length);
		printf("sjf %d and leng %d \n", sjf->PID, sjf->length);
		pcb = temp->back;
		//i++;
	}
	
	
	while (sjf != NULL){
		
		for (int i = sjf->PC; i < sjf->length ; i++){
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
		for (int i = pcb->PC; i < pcb->length ; i++){
			
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