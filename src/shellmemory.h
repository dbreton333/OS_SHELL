int match(char *model, char *var);
char *extract(char *model);
void mem_init();
void resetmem();
void mem_set_value(char *var_in, char *value_in);
void mem_clear(char *var_in);
char *mem_get_value(char *var_in);
void resetmemframe();
void mem_clear_frame(char *var_in);
void mem_set_frame_value(int frameno, char* value_in);
int mem_get_frame_number(char *prog, int page);
void mem_set_page_value(char *prog, int page, char *value_in);
int mem_get_new_frame(char* page);
char *mem_get_frame_value(int frameno,int line);
char *mem_get_page_value(char* prog, int page, int line);
void mem_set_page_table(char *prog, int page, int frame);

static const int FRAME_S = 1000;//frl;
static const int FRAME_L = 1000;//frl;
