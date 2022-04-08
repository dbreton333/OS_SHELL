int match(char *model, char *var);
char *extract(char *model);
void mem_init();
void resetmem();
void mem_set_value(char *var_in, char *value_in);
void mem_clear(char *var_in);
char *mem_get_value(char *var_in);
void resetmemframe();
void mem_clear_frame(int frameno);
void mem_set_frame_value(int frameno, char* value_in);
int mem_get_frame_number(char *prog, int page);
int mem_init_page_value(char *prog, int page, char *value_in);
char *mem_get_frame_value(int frameno,int line);
int mem_get_table_value(char *prog, int page);
char *mem_get_page_value(char* prog, int page, int line);
void mem_set_page_table(char *prog, int page, int frame);
int mem_get_new_frame();
int mem_evict_frame();
int mem_page_fault(char* prog, int page, int line);
void mem_get_and_save_page_from_backstore(int frame,char* prog,int page);

static const int FRAME_L = 3;
static const int FRAME_SIZE = FRAMESIZE;
static const int FRAME_S = FRAMESIZE/3;
static const int VAR_S = VARMEMSIZE;

struct frames {
	int frameno;
	char *prog;
	int page;
	struct frames *back;
};

struct frames *MRU;
struct frames *LRU;