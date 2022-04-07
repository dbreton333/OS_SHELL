int interpreter(char* command_args[], int args_size);
int help();

struct PCB {
	int PID;
	int PC;
	int page;
	int currpage;
	int length;
	int score;
	struct PCB *next;
	struct PCB *back;
};

struct PCB *head;
struct PCB *tail;

struct PCB* PCB_clear(struct PCB* pcb);
