#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define SHELL_MEM_LENGTH 1000
static const int FRAME_S = fVal;  
static const int VAR_S = vVal;

struct memory_struct{
	char *var;
	char *value;

};struct memory_struct shellmemory[vVal];

struct frame_struct{
	  int var; 
	  char *val;
	  
};struct frame_struct f_store[fVal];

//reset memory variable store
int resetmem(){
	for(int i=0 ; i < vVal ; i++ ){
		if ((shellmemory[i].var != NULL) && shellmemory[i].value != NULL)
		free(shellmemory[i].var);
		free(shellmemory[i].value);
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
	for (i=0; i<1000; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
	
	int i;

	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

void mem_clear(char *var_in){
	for (int i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].var = strdup("none");
			shellmemory[i].value = strdup("none");
			return;
		} 
	}
}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;

	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){

			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";

}
// int add_file_to_mem(FILE* fp, int* pStart, int* pEnd, char* fileID)
// {
//     char line[SHELL_MEM_LENGTH];
//     size_t i;
//     int error_code = 0;
// 	bool hasSpaceLeft = false;

//     for (i = 100; i < SHELL_MEM_LENGTH; i++){
//         if(strcmp(shellmemory[i].var,"none") == 0){
//             *pStart = (int)i;
// 			hasSpaceLeft = true;
//             break;
//         }
//     }

// 	//shell memory is full
// 	if(hasSpaceLeft == 0){
// 		error_code = 21;
// 		return error_code;
// 	}
    
//     for (size_t j = i; j < SHELL_MEM_LENGTH; j++){
//         if(feof(fp))
//         {
//             *pEnd = (int)j-1;
//             break;
//         }else{
//             fgets(line, 999, fp);
// 			shellmemory[j].var = strdup(fileID);
//             shellmemory[j].value = strdup(line);
//         }
//     }

// 	//no space left to load the entire file into shell memory
// 	if(!feof(fp)){
// 		error_code = 21;
// 		//clean up the file in memory
// 		for(int j = 1; i <= SHELL_MEM_LENGTH; i ++){
// 			shellmemory[j].var = "none";
// 			shellmemory[j].value = "none";
//     	}
// 		return error_code;
// 	}

//     return error_code;
// }