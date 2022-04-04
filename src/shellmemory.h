void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
void mem_clear(char *var_in);
int mem_set_frame_value(int var_in, char *prog, char* value_in);
char *mem_get_frame_value(char *prog, int frame, int index);
void resetmem();