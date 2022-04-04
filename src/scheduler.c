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
void sortQueueL();
void sortQueueS();
void decrement();
struct PCB* findSmaller();
void switchPlace(struct PCB* pcb);

int scheduler(char *policy){
	//choosing different policies
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

void sortQueueL(){
	int end = 0;
	struct PCB* pcb = head;

	while(end != 1){
		end = 1;
		//traversing through pcbs
		while (pcb != NULL){
			struct PCB* prev = pcb->back;
			//checking if current pcb has greater length then the previous one
			if (prev != NULL && pcb->length > prev->length) {
				pcb->back = prev->back;
				//making sure not NULL and then updating pointers
				if(prev->back != NULL){
					prev->back->next = pcb;
				}
				prev->next = pcb->next;
				if(pcb->next != NULL){
					pcb->next->back = prev;
				}
				pcb->next = prev;
				prev->back = pcb;

				if(pcb == head){
					head = prev;
				}

				if(prev == tail){
					tail = pcb;
				}
				end = 0;
			}

			pcb = prev;
		}
	}
}

void sortQueueS(){
	int end = 0;
	struct PCB* pcb = head;

	while(end != 1){
		end = 1;
		//traversing through pcbs
		while (pcb != NULL){
			struct PCB* prev = pcb->back;
			//checking if current pcb has greater score then the previous one
			if (prev != NULL && pcb->score > prev->score) {
				pcb->back = prev->back;
				//making sure not NULL and then updating pointers
				if(prev->back != NULL){
					prev->back->next = pcb;
				}
				prev->next = pcb->next;
				if(pcb->next != NULL){
					pcb->next->back = prev;
				}
				pcb->next = prev;
				prev->back = pcb;

				if(pcb == head){
					head = prev;
				}

				if(prev == tail){
					tail = pcb;
				}
				end = 0;
			}

			pcb = prev;
		}
	}
}

int SJF(){
	int errCode = 0;
	//calling sort to get shortest job 
	sortQueueL();

	struct PCB* tmp = head;
	
	while (tmp != NULL){
		
		int index = tmp->PC;
		int PID = tmp->PID;
		int currpage = tmp->currpage;
		//looping through each program
		char prog[4];	
		sprintf(prog,"%d",PID);

		for (int i = index; i < (tmp->length); i++){

			if((i % FRAME_L == 0) && (i != 0)){
				currpage++;
				tmp->currpage = currpage;
			}
			
			char* userInput = mem_get_page_value(prog, currpage, i % FRAME_L);


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
			tmp->PC += 1;
		}
		//clear pcb when pcb reaches the end
		PCB_clear(tmp);
		tmp = tmp->back;
	}
	return errCode;
}

int RR(){
	int errCode = 0;
	struct PCB* tmp = head;

	while(head != NULL){

		int index = tmp->PC;
		int PID = tmp->PID;
		int currpage = tmp->currpage;
		//looping through each program and running two instructions per program
		for (int i = index; (i < index + 2) && (i < (tmp->length)); i++){
			
			char prog[4];	
			sprintf(prog,"%d",PID);

			if((i % FRAME_L == 0) && (i != 0)){
				currpage++;
				tmp->currpage = currpage;
			}
			
			char* userInput = mem_get_page_value(prog, currpage, i%FRAME_L);

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

			tmp->PC += 1;
		}
		//clear tmp when tmp reaches the end
		if(tmp->PC == (tmp->length)){
			PCB_clear(tmp);
		}

		tmp = tmp->back;

		if(tmp == NULL){
			tmp = head;
		}
	}
	return errCode;
}

void decrement(){
	struct PCB* pcb = head;
	//checking which pcb needs to decrement the score
	while(pcb != NULL){
		if(pcb != head){
			if(pcb->score != 0){
				pcb->score -= 1;
			}
		}
		pcb = pcb->back;
	}
}


int AGING(){
	int errCode = 0;
	//sorting queue 
	sortQueueS();
	
	while(head != NULL){

		struct PCB* tmp = head;
		int index = tmp->PC;
		int PID = tmp->PID;
		int currpage = tmp->currpage;
		

		char prog[4];	
		sprintf(prog,"%d",PID);

		if((index % FRAME_L == 0) && (index != 0)){
			currpage++;
			tmp->currpage = currpage;
		}

		char* userInput = mem_get_page_value(prog,currpage,index);

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
		head->PC += 1;

		if(head->PC == (head->length)){
			PCB_clear(head);
		}
		decrement();
		sortQueueS();

	}
	return errCode;
}

int FCFS(){
	int errCode = 0;
	struct PCB* tmp = head;

	while(tmp != NULL){
		int index = tmp->PC;
		int PID = tmp->PID;
		int currpage = tmp->currpage;
		//looping through each program
		for (int i = index; i < (tmp->length); i++){
			
			char prog[4];	
			sprintf(prog,"%d",PID);

			if((i % FRAME_L == 0) && (i != 0)){
				currpage++;
				tmp->currpage = currpage;
			}
			
			char* userInput = mem_get_page_value(prog, currpage, i%FRAME_L);

			
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
			tmp->PC += 1;
		}
		//clear tmp when tmp reaches the end
		PCB_clear(tmp);
		tmp = tmp->back;
	}
	return errCode;
}