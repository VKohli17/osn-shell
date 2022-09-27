#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

void prompt(char*, int, double);
void trimleadingandTrailing(char*);
char* get_user();
char* get_machine();
char* get_cwd();
char* get_cwd_no_path(char*);
int filetype(char*);
void print_long_ls (char*);
void eternalchecker (int);
void clear();
void handle_ctrlz(int);
void handle_ctrlc(int);
void handle_ctrld(int);

#endif
