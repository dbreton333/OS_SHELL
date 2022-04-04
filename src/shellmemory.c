#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "shellmemory.h"



#define SHELL_MEM_LENGTH 1000
static const int FRAME_S = 1000;  
static const int VAR_S = 1000;
static const int TABLE_S = 1000;

struct memory_struct{
	char *var;
	char *value;

};

struct frame_struct{
	  char *frame; 
	  char **values;
	  
};

struct page_table_struct{
	  char *PID; 
	  int pageno;
	  int frameno; 
};

struct frame_struct f_store[1000];
struct memory_struct shellmemory[1000];
struct page_table_struct pagetable[1000];

void resetmem(){
	for(int i=0 ; i < VAR_S ; i++ ){
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}

// Helper functions
int match(char *model, char *var) {
	int i, len=strlen(var), matchCount=0;
	for(i=0;i<len;i++)
		if (*(model+i) == *(var+i)) matchCount++;
	if (matchCount == len)
		return 1;
	else
		return 0;
}

char *extract(char *model) {
	char token='=';    // look for this to find value
	char value[1000];  // stores the extract value
	int i,j, len=strlen(model);
	for(i=0;i<len && *(model+i)!=token;i++); // loop till we get there
	// extract the value
	for(i=i+1,j=0;i<len;i++,j++) value[j]=*(model+i);
	value[j]='\0';
	return strdup(value);
}


// Shell memory functions

void mem_init(){

	int i;
	for (i=0; i<VAR_S; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}

	for (i=0; i<FRAME_S; i++){		
		f_store[i].frame = "none";

		char **frame = mallloc(FRAME_L * sizeof(char*));

		int j;
		for(j = 0; j < FRAME_L; j++){
			frame[j] = strdup("none");
		}

		f_store[i].values = frame;
	}

	for (i=0; i<TABLE_S; i++){		
		pagetable[i].PID = "none";
		pagetable[i].frameno = 0;
		pagetable[i].pageno = 0;
	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
	
	int i;

	for (i=0; i<VAR_S; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<VAR_S; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	return;
}

void mem_clear(char *var_in){
	for (int i=0; i<VAR_S; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].var = strdup("none");
			break;
		} 
	}
}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;

	for (i=0; i<VAR_S; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";

}

void resetframemem(){
	for (i=0; i<FRAME_S; i++){		
		f_store[i].frame = "none";

		char **frame = mallloc(FRAME_L * sizeof(char*));

		int j;
		for(j = 0; j < FRAME_L; j++){
			frame[j] = strdup("none");
		}

		f_store[i].values = frame;
		
	}

	for (i=0; i<TABLE_S; i++){		
		pagetable[i].PID = strdup("none");
		pagetable[i].frameno = 0;
		pagetable[i].pageno = 0;
	}
}

void mem_clear_frame(char *var_in){
	for (int i=0; i<FRAME_S; i++){
		if (strcmp(f_store[i].frame, var_in) == 0){
			f_store[i].frame = strdup("none");
			break;
		} 
	}
}

void mem_set_frame_value(int frameno, char* value_in) {
	int j;
	for(j = 0; j < FRAME_L; j++){
		if(strcmp(f_store[frameno].values[j], "none") == 0){
			f_store[frameno].values[j] = strdup(value_in);
		}
	}
}

int mem_get_frame_number(char *prog, int page) {

	int i;
	int frame;
	int hit = 0;
	for (i=0; i<TABLE_S; i++){
		if(strcmp(pagetable[i].PID,prog) == 0){
			if(pagetable[i].pageno == page){
				frame = pagetable[i].frameno;
				hit = 1;
			}
		}
	}
	
	if(hit = 0){
		frame = mem_get_new_frame();
		mem_set_page_table(prog,page,frame);
	}
	//if NO Hit find page in memory

	return frame;
}

void mem_set_page_value(char *prog, int page, char *value_in){ //could return success or not
	int frame = mem_get_frame_number(prog, page);
	mem_set_frame_value(frame, value_in);
}

int mem_get_new_frame(){ //Gets first empty frame
	int frameno;
	for (frameno=0; frameno<FRAME_S; frameno++){
		if (strcmp(f_store[frameno].frame, "none") == 0){
			return frameno; //returm frame index
		} 
	}
}

char *mem_get_frame_value(int frameno,int line) {
	return f_store[frameno].values[line];
}

char *mem_get_page_value(char* prog, int page, int line){
	int frame = mem_get_frame_number(prog,page);
	return mem_get_frame_value(frame,line);
}


void mem_set_page_table(char *prog, int page, int frame) {
	for (i=0; i<TABLE_S; i++){
		if(strcmp(pagetable[i].PID,"none") == 0){
			pagetable[i].PID = strdup(prog);
			pagetable[i].pageno = page;
			pagetable[i].frameno = frame;
		}
	}
}