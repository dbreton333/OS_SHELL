#include<stdlib.h>
#include<string.h>
#include<stdio.h>


#define SHELL_MEM_LENGTH 1000
static const int FRAME_S = 1000;  
static const int FRAME_L = 3;
static const int VAR_S = 1000;
static const int TABLE_S = 1000;

struct memory_struct{
	char *var;
	char *value;

};

struct frame_struct{
	  char *prog; 
	  char **values;
	  
};

struct page_table{
	  char *prog; 
	  int frame;
	  int memory; 
};

struct frame_struct f_store[1000];
struct memory_struct shellmemory[1000];
struct page_table pageTable[1000];

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
		f_store[i].prog = "none";
		char *frame[FRAME_L];

		int j;
		for(j = 0; j < FRAME_L; j++){
			frame[j] = strdup("none");
		}

		f_store[i].values = frame;
		
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

int mem_set_frame_value(int var_in, char *prog, char* value_in) {


	if (strcmp(f_store[var_in].prog, prog) == 0){
		int j;
		for(j = 0; j < FRAME_L; j++){
			if(strcmp(f_store[var_in].values[j], "none") == 0){
				f_store[var_in].values[j] = strdup(value_in);
				return var_in;
			}
		}
	} 

	int i;

	//frame does not exist, need to find a free spot.
	for (i=0; i<FRAME_S; i++){
		if (strcmp(f_store[i].prog, "none") == 0){
			f_store[i].prog = prog;
			int j;
			for(j = 0; j < FRAME_L; j++){
				if(strcmp(f_store[i].values[j], "none") == 0){
					f_store[i].values[j] = strdup(value_in);
					return i;
				}
			}
			return i;
		} 
	}

	return i;
}

void mem_clear(char *var_in){
	for (int i=0; i<VAR_S; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].var = strdup("none");
			break;
		} 
	}
}

void mem_clear_prog(char *var_in){
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

char *mem_get_frame_value(char *prog, int frame, int index) {
	int i;
	int line;
	for (i=0; i<TABLE_S; i++){
		if(strcmp(pageTable[i].prog,prog)){
			if(pageTable[i].frame == frame){
				line = pageTable[i].memory;
			}
		}
	}

	return f_store[line].values[index];

}