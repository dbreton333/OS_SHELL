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
struct PCB* decrement();
void switchPlace(struct PCB* pcb);

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

void sortQueueL(){
	int end = 0;
	struct PCB* pcb = head;

	while(end != 1){
		end = 1;

		while (pcb != NULL){
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
	sortQueueL();

	struct PCB* tmp = head;
	
	while (tmp != NULL){
		
		int index = tmp->PC;
		for (int i = index; i < (tmp->length + tmp->base); i++){
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
	struct PCB* pcb = head;

	while(head != NULL){
		//printf("pcb back : %d\n",pcb->PC);
		int index = pcb->PC;
		for (int i = index; (i < index + 2) && (i < (pcb->length + pcb->base)); i++){
			
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

			pcb->PC += 1;
		}
		//clear pcb when pcb reaches the end
		if(pcb->PC == (pcb->length + pcb->base)){
			PCB_clear(pcb);
		}
		pcb = pcb->back;

		if(pcb == NULL){
			pcb = head;
		}
	}
	return errCode;
}

struct PCB* decrement(){
	struct PCB* pcb = head;
	struct PCB* smaller = NULL;
	while(pcb != NULL){
		if(pcb != head){
			if(pcb->score != 0){
				pcb->score -= 1;
			}
			if(pcb->score < head->score){
				if(smaller == NULL){
					smaller = pcb;
				}else{
					if(pcb->score < smaller->score){
						smaller = pcb;
					}	
				}
			}
		}
		pcb = pcb->back;
	}

	return smaller;
}

void switchPlace(struct PCB* pcb) {
	struct PCB* hd = head;

	if(hd->back = pcb){
		hd->back = pcb->back;

		if(pcb->back != NULL){
			pcb->back->next = hd;
		}

		pcb->next = NULL;

		hd->next = pcb;
		pcb->back = hd;

	}else{
		struct PCB* tmp1 = pcb->back;
		struct PCB* tmp2 = pcb->next;

		pcb->back = hd->back;
		pcb->next = NULL;

		hd->next = tmp2;
		hd->back = tmp1;

	}
}


int AGING(){
	int errCode = 0;
	int end = 0;

	sortQueueL();

	struct PCB* tmp = head;
	
	while (head != NULL){
		
		int index = tmp->PC;
		for (int i = index; i < (tmp->length + tmp->base); i++){
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

			tmp->PC += 1;

			struct PCB* pcbs = decrement();

			if(pcbs != NULL){
				switchPlace(pcbs);
				break;
			}

		}

		//clear pcb when pcb reaches the end
		if(tmp->PC == (tmp->length + tmp->base)){
			PCB_clear(tmp);
		}

		tmp = head;

	}

	return errCode;
}

int FCFS(){
	int errCode = 0;
	struct PCB* pcb = head;

	while(pcb != NULL){
		//printf("pcb back : %d\n",pcb->PC);
		int index = pcb->PC;
		for (int i = index; i < (pcb->length + pcb->base); i++){
			
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
			pcb->PC += 1;
		}
		//clear pcb when pcb reaches the end
		PCB_clear(pcb);
		pcb = pcb->back;
	}
	return errCode;
}