#include "functions.h"
#include "headers.h"

extern char endcol[];
extern char blue[];
extern char pink[];
extern char seafoam[];
extern char lavender[];
extern char ice[];
extern char bold[];
extern char endbold[];
extern double timeforfg;

// printing the prompt
void prompt(char* fakehome, int mugglesafe, double time) {

    char* username = get_user();
    char* machine = get_machine();
    char* cwd = get_cwd();
    char* cwd_no_path = get_cwd_no_path(cwd);
    int fakehome_len = strlen(fakehome);
    if (mugglesafe){
        if (strlen(cwd) < fakehome_len || strncmp(cwd, fakehome, fakehome_len) != 0){
            strcpy(cwd_no_path, cwd);
        }
        else if (strcmp(fakehome, cwd) == 0){
            strcpy(cwd_no_path, "~");
        }
        else{
            strcpy(cwd_no_path, cwd + fakehome_len);
        }
    }
    char time_taken[20];
    if (time > 1){
        // set time_taken to the time taken to execute the command, with one decimal place
        sprintf(time_taken, " took %.0fs", time);
    }
    else{
        time_taken[0] = '\0';
    }
    printf("%s[%s%s%s@%s%s %s%s%s]$ %s", bold, lavender, username, seafoam, machine, ice, cwd_no_path, endcol, time_taken, endbold);
    timeforfg = 0;

}
