#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "shellmemory.h"

static const int TABLE_S = 100;

struct memory_struct{ //var memory
	char *var;
	char *value;
};

struct frame_struct{ //physical page memory
	  char *frame; 
	  char **values;	  
};

struct page_table_struct{ //page table -> maps virtual page to physical page
	  char *PID; 
	  int pageno;
	  int frameno; 
};

struct frame_struct f_store[FRAMESIZE/3];
struct memory_struct shellmemory[VARMEMSIZE];
struct page_table_struct pagetable[100];


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

//inits the memory for the three arrays
void mem_init(){ 
	int i;
	for (i=0; i<VAR_S; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}

	for (i=0; i<FRAME_S; i++){		
		f_store[i].frame = "none";

		char **frame = malloc(FRAME_L * sizeof(char*));

		int j;
		for(j = 0; j < FRAME_L; j++){
			frame[j] = strdup("none");
		}

		f_store[i].values = frame;
	}

	for (i=0; i<TABLE_S; i++){		
		pagetable[i].PID = "none";
		pagetable[i].frameno = -1;
		pagetable[i].pageno = -1;

	}
}

/*******************************
 

     Var Shell memory memory functions


*******************************/


void resetmem(){ // reset var memory
	for(int i=0 ; i < VAR_S ; i++ ){
		shellmemory[i].var = strdup("none");
		shellmemory[i].value = strdup("none");
	}
}

// Set key value pair for var
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

//clear var based on variable name
void mem_clear(char *var_in){
	for (int i=0; i<VAR_S; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].var = strdup("none");
			break;
		} 
	}
}

//get value based on variable name
char *mem_get_value(char *var_in) {
	int i;

	for (i=0; i<VAR_S; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";
}

/*******************************
 

     FRame Shell memory memory functions


*******************************/

//reset physical page memory // GOOD
void resetmemframe(){
	int i;

	for (i=0; i<FRAME_S; i++){		
		f_store[i].frame = strdup("none");

		char **frame = malloc(FRAME_L * sizeof(char*));

		int j;
		for(j = 0; j < FRAME_L; j++){
			frame[j] = strdup("none");
		}

		f_store[i].values = frame;
		
	}

	for (i=0; i<TABLE_S; i++){		
		pagetable[i].PID = strdup("none");
		pagetable[i].frameno = -1;
		pagetable[i].pageno = -1;
	}

}

//clear frame based on frame index //GOOD
void mem_clear_frame(int frameno){
		f_store[frameno].frame = strdup("none");
		char **frame = malloc(FRAME_L * sizeof(char*));
		int j;
		for(j = 0; j < FRAME_L; j++){
			frame[j] = strdup("none");
		}
		f_store[frameno].values = frame;
}

//set frame value base on frame index //GOOD
void mem_set_frame_value(int frameno, char* value_in) {
	int j;
	for(j = 0; j < FRAME_L; j++){
		if(strcmp(f_store[frameno].values[j], "none") == 0){
			f_store[frameno].values[j] = strdup(value_in);
			break;
		}
	}
}

//get available frame index and set page table, if no more space returns -1 //GOOD
int mem_get_frame_number_setup(char *prog, int page) {

	int frame = mem_get_table_value(prog,page);

	if(frame == -1){ //get new space since frame not in table

		char pagestr[2];
		sprintf(pagestr, "%d", page);

	 	frame = mem_get_new_frame();
		if(frame != -1){ //no more space in f_store
	 		mem_set_page_table(prog, page, frame);
		}	 
	}

	return frame;
}

//get table value with page number and program number //GOOD
int mem_get_table_value(char *prog, int page){
	int i;
	int frame = -1;
	for (i=0; i<TABLE_S; i++){
		if(strcmp(pagetable[i].PID,prog) == 0){
		 	if(pagetable[i].pageno == page){
				frame = pagetable[i].frameno;
				return frame;
			}
		}
	}
	return frame;
}

//set frame value when loading programs //GOOD
int mem_init_page_value(char *prog, int page, char *value_in){ 
	int frame = mem_get_frame_number_setup(prog, page);
	if(frame != -1){ //if frame == -1 mean no more space in f_Store
		mem_set_frame_value(frame, value_in);
	}
	return frame; //return -1 in no more space in f_store
}

//get empty frame, return -1 if none //GOOD
int mem_get_new_frame(){

	int frameno = -1;
	for (int frametmp = 0; frametmp<FRAME_S; frametmp++){
		if (strcmp(f_store[frametmp].frame, "none") == 0){
			f_store[frametmp].frame = strdup("full");
			frameno = frametmp;
			break; 
		} 
	}
	return frameno;
}

//get the value at a specific line in a frame //GOOD
char *mem_get_frame_value(int frameno,int line) {
	return strdup(f_store[frameno].values[line]);
}

//get a page value by getting the frame index and then frame value
char *mem_get_page_value(char* prog, int page, int line){

	
	int frame = mem_get_table_value(prog,page);

	if(frame == -1){
		frame = mem_page_fault(prog, page, line);

		if(!((strcmp(MRU->prog,prog)==0)&&(MRU->page == page))){
			struct frames *p = (struct frames*) malloc(sizeof(struct frames));
			p->frameno = frame;
			p->page = page;
			p->prog = prog;
			MRU->back = p;
			MRU = p;		
		}
		return "break";

	}else{

		struct frames *p = (struct frames*) malloc(sizeof(struct frames));
		if(LRU == NULL){
			p->frameno = frame;
			p->page = page;
			p->back = NULL;
			p->prog = prog;
			LRU = p;
			MRU = p;
		}else if(!((strcmp(MRU->prog,prog)==0)&&(MRU->page == page))){
			
			p->frameno = frame;
			p->page = page;
			p->prog = prog;
			MRU->back = p;
			MRU = p;		
		}
	
		return mem_get_frame_value(frame,line);
	}

}


//print line of frame and evict
int mem_evict_frame(){

	int frame = LRU->frameno;
	char* prog = LRU->prog;
	int page = LRU->page;

	if(LRU->back != NULL){
		LRU = LRU->back;
	}

	int i;
	for(i = 0; i<FRAME_L; i++){
		char* str = mem_get_frame_value(frame,i);
		if(strcmp(str,"none") != 0){
			printf("%s", str);
		}
	}

	for (i=0; i<TABLE_S; i++){
		if(strcmp(pagetable[i].PID,prog) == 0){
		 	if(pagetable[i].pageno == page){
				pagetable[i].PID = strdup("none");
				pagetable[i].frameno = -1;
				pagetable[i].pageno = -1;
				break;
			}
		}
	}

	mem_clear_frame(frame);

	return frame;
	
}

int mem_page_fault(char* prog, int page, int line){

	int frame = mem_get_new_frame();
	if(frame == -1){
		printf("Page fault! Victim page contents:\n");
		frame = mem_evict_frame();
		printf("End of victim page contents.\n");
	}
	mem_get_and_save_page_from_backstore(frame, prog, page);
	mem_set_page_table(prog,page,frame);
	return frame;
}

void mem_get_and_save_page_from_backstore(int frame,char* prog,int page){
	FILE *p = NULL;
	char line[1000]; //buffer for line
	char w[100] = "backstore/prog";
	strcat(w, prog);
	strcat(w, ".txt");
	p = fopen(w, "rt");
	int i;

	for(i=0; i<(page)*FRAME_L; i++){
		fgets(line,999,p);
	}

		for(i=0; i<FRAME_L; i++){
			fgets(line,999,p);

			mem_set_frame_value(frame, line);

			if(feof(p)){
				break;
			}
		}
	
	
}

//set table page number, frame number  and program number
void mem_set_page_table(char *prog, int page, int frame) {
	int i;
	for (i=0; i<TABLE_S; i++){
		if(strcmp(pagetable[i].PID,"none") == 0){
			pagetable[i].PID = strdup(prog);
			pagetable[i].pageno = page;
			pagetable[i].frameno = frame;
			break;
		}
	}
}