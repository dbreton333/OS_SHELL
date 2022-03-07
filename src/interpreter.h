int interpreter(char* command_args[], int args_size);
int help();

struct PCB {
	int PID;
	int base;
	int PC;
	int length;
	struct PCB *next;
	struct PCB *back;
};

struct PCB *head;
struct PCB *tail;

int PCB_clear(struct PCB* pcb);