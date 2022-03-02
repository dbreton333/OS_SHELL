struct PCB {
	int PID;
	int base;
	int PC;
	int length;
	struct PCB *next;
	struct PCB *back;
};
int scheduler(char *policy);
int SJF();
int RR();
int AGING();
int FCFS();
int PCB_clear(struct PCB* pcb);