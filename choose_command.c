#include "headers.h"
#include "functions.h"

extern int mugglesafe;
extern char endcol[];
extern char blue[];
extern char pink[];
extern char seafoam[];
extern char lavender[];
extern char ice[];
extern char bold[];
extern char endbold[];
int bgprocesses[50] = {0};
char* bgprocessesnames[50] = {NULL};
int indexofbg = 0;
extern double timeforfg = 0;
int fgprocess = 0;
char* fgprocessname = NULL;

// checking which command was typed, implementing accordingly
void choose_command(char* input){
    if ((strcmp(input, "quit") == 0) 
            || (strcmp(input, "exit") == 0)
            || (strcmp(input, "mischief managed") == 0)
            || (strcmp(input, "q") == 0)){
            exit(0);
        }

    if (strcmp(input, "pwd") == 0){
        pwd();
    }

    else if (strncmp(input,"echo ",5) == 0){
        echo(input + 5);
    }

    else if (strncmp(input,"sonoros ",8) == 0){
        echo(input + 8);
    }
    
    else if (strcmp(input,"cd")==0){
        cd("~");
    }
    
    else if (strncmp(input,"cd ",3) == 0){
        cd(input + 3);
    }

    else if ((strncmp(input, "ls ", 3)==0)||(strcmp(input,"ls")==0)){
        // tokenizing input by space to get flags and arguments
        char* token = strtok(input, " ");
        char** arguments = (char**)malloc(sizeof(char*) * (max_input-3));
        // adding all words except first ("ls") to arguments
        int i = 0;
        while (token != NULL){
            if (i != 0){
                arguments[i-1] = token;
            }
            token = strtok(NULL, " ");
            i++;
        }
        ls(arguments);
        // cd(".");         
    }

    else if ((strncmp(input, "discover ", 9)==0)||(strcmp(input,"discover")==0)){
        // tokenizing input by space to get flags and arguments
        char* token = strtok(input, " ");
        char** arguments = (char**)malloc(sizeof(char*) * (max_input-3));
        // adding all words except first ("discover") to arguments
        int i = 0;
        while (token != NULL){
            if (i != 0){
                arguments[i-1] = token;
            }
            token = strtok(NULL, " ");
            i++;
        }
        discover(arguments);
        // cd(".");         
    }

    else if (strcmp(input,"history")==0){
        history();
    }

    else if (strncmp(input, "pinfo ", 6) == 0){
        // convert the pid to int
        int pid = atoi(input + 6);
        pinfo(pid);
    }
    else if (strcmp(input, "pinfo") == 0){
        pinfo(getpid());
    }
    
    else if (strcmp(input, "reparo")==0){
        mugglesafe = 0;
    }

    else if (strcmp(input, "riddikulus")==0){
        mugglesafe = 1;
    }

    else if (strcmp(input, "jobs")==0){
        jobs(0);
    }

    else if (strcmp(input, "jobs -r")==0){
        jobs(1);
    }

    else if (strcmp(input, "jobs -s")==0){
        jobs(2);
    }

    else if (strncmp(input, "sig ", 4) == 0){
        int arg1, arg2;
        sscanf(input + 4, "%d %d", &arg1, &arg2);
        send_signal(arg1, arg2);
    }

    else if (strncmp(input, "fg ", 3) == 0){
        int arg;
        sscanf(input + 3, "%d", &arg);
        fg(arg);
    }

    else if (strncmp(input, "bg ", 3) == 0){
        int arg;
        sscanf(input + 3, "%d", &arg);
        bg(arg);
    }

    // clear function
    else if (strcmp(input, "clear")==0 || strcmp(input, "c")==0){
        clear();
    }

    else {
        int bg;
        // check if the last character is &
        if (input[strlen(input) - 1] == '&'){
            input[strlen(input) - 1] = '\0';
            trimleadingandTrailing(input);
            bg = 1;
            bgprocessesnames[indexofbg] = (char*)malloc(sizeof(char) * (strlen(input) + 1));
            strcpy(bgprocessesnames[indexofbg], input);
        }
        else{
            bg = 0;
            fgprocessname = (char*)malloc(sizeof(char) * (strlen(input) + 1));
            strcpy(fgprocessname, input);
        }
        char* argv[max_input];
        int i = 0;
        char* token = strtok(input, " ");
        while (token != NULL){
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;
        if (bg){
            pid_t pid = fork();
            if(pid<0)
            {
                perror("Could not execute command");
                return;
            }
            else if(pid>0)
            {
                setpgid(pid,0);
                // print pid and name 
                printf("\'%s\' with pid [%d] running in background\n", bgprocessesnames[indexofbg], pid);
                bgprocesses[indexofbg] = pid;
                // bgprocessesnames[indexofbg] = argv[0];
                indexofbg = (indexofbg + 1)%50;
                return;
            }
            else
            {
                setpgid(0,0);
                if(execvp(argv[0],argv)<0)
                {
                    perror("Could not execute command");
                    exit(1);
                }
            }
            // when process gets completed, print it
            // printf("[%d] exited normally\n", bgprocesses[indexofbg]);
        }
        else {
            time_t start, end;
            time(&start);
            pid_t pid = fork();
            fgprocess = pid;
            if(pid<0)
            {
                perror("Child not created");
                return;
            }
            else if(pid>0)
            {
                int w_st;
                setpgid(pid,0);
                waitpid(pid, &w_st,WUNTRACED);
            }
            else
            {
                setpgid(0,0);
                int len = strlen(argv[0]);  
                if(execvp(argv[0],argv)<0)
                {
                    perror("Could not execute command");
                    exit(1);
                }

                //wait(NULL);
            }
            time(&end);
            double time_taken = end - start;
            char fakehome[150];
            if (mugglesafe) strcpy(fakehome, start_dir);
            else strcpy(fakehome, realhome_dir);
            if (time_taken > 1) {
                // prompt(fakehome, mugglesafe, time_taken);
                timeforfg = time_taken;
            }
        }
    }
}

void jobs(int flag){
    // print all the bgprocesses' index, name, pid and whether it's running or stopped
    for (int i = 0; i < 50; i++){
        if (bgprocesses[i] > 0){
            char status[10];
            int w_st;
            pid_t pid = waitpid(bgprocesses[i], &w_st, WNOHANG | WUNTRACED);
            if (pid == 0){
                strcpy(status, "Running");
            }
            else if (pid == -1){
                strcpy(status, "Stopped");
            }
            else{
                strcpy(status, "Stopped");
            }
            // if flag = 0, print all. if flag = 1, print all running. if flag = 2, print all stopped.
            if (flag == 0){
                printf("[%d] %s %s [%d]\n", i, status, bgprocessesnames[i], bgprocesses[i]);
            }
            else if (flag == 1){
                if (strcmp(status, "Running") == 0){
                    printf("[%d] %s %s [%d]\n", i, status, bgprocessesnames[i], bgprocesses[i]);
                }
            }
            else if (flag == 2){
                if (strcmp(status, "Stopped") == 0){
                    printf("[%d] %s %s [%d]\n", i, status, bgprocessesnames[i], bgprocesses[i]);
                }
            }
        }
    }
}

void eternalchecker (int p){
    int process = bgprocesses[indexofbg];
    char* processname = bgprocessesnames[indexofbg];
    int status;
    char* username = get_user();
    char* machine = get_machine();
    char* cwd = get_cwd();
    char* cwd_no_path = get_cwd_no_path(cwd);
    char fakehome[150];
    if (mugglesafe) strcpy(fakehome, start_dir);
    else strcpy(fakehome, realhome_dir);
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
    for (int i=0;i<50;i++){
        process = bgprocesses[i];
        processname = bgprocessesnames[i];
        int wpid = waitpid(process, &status, WNOHANG);
        if (wpid == -1){
            // perror("waitpid");
            // exit(EXIT_FAILURE);
        }
        if (wpid == process!= 0){
            if (status == 0){
                char strtoprint[150];
                sprintf(strtoprint, "\'%s\' with pid [%d] exited normally\n%s[%s%s%s@%s%s %s%s]$ %s", processname, process, bold, lavender, username, seafoam, machine, ice, cwd_no_path, endbold, endcol);
                write(1, strtoprint, strlen(strtoprint));
            }
            else{
                char strtoprint[150];
                sprintf(strtoprint, "\'%s\' with pid [%d] exited abnormally\n%s[%s%s%s@%s%s %s%s]$ %s", processname, process, bold, lavender, username, seafoam, machine, ice, cwd_no_path, endbold, endcol);
                write(1, strtoprint, strlen(strtoprint));
            }
        }
    }
}

void send_signal(int processindex, int signal){
    int process = bgprocesses[processindex];
    if (process == 0){
        printf("No such process\n");
        return;
    }
    int ret = 0;
    ret = kill(process, signal);
    if (ret == -1){
        perror("kill");
        return;
    }
}

void fg(int processindex){
    // bring process to foreground
    int process = bgprocesses[processindex];
    char* processname = malloc(sizeof(char)*150);
    strcpy(processname, bgprocessesnames[processindex]);
    fgprocess = process;
    strcpy(fgprocessname, processname);
    if (process == 0){
        printf("No such process\n");
        return;
    }
    int w_st;
    int ret = 0;
    ret = kill(process, SIGCONT);
    if (ret == -1){
        perror("kill");
        return;
    }
    pid_t pid = waitpid(process, &w_st, WUNTRACED);
    if (pid == -1){
        perror("waitpid");
        return;
    }
    if (WIFSTOPPED(w_st)){
        printf("\'%s\' with pid [%d] stopped\n", processname, process);
        return;
    }
    if (WIFEXITED(w_st)){
        printf("\'%s\' with pid [%d] exited normally\n", processname, process);
        return;
    }
    if (WIFSIGNALED(w_st)){
        printf("\'%s\' with pid [%d] exited abnormally\n", processname, process);
        return;
    }
    if (WIFCONTINUED(w_st)){
        printf("\'%s\' with pid [%d] continued\n", processname, process);
        return;
    }
}

void bg(int processindex){
    // if bg process is stopped, continue running it
    int process = bgprocesses[processindex];
    if (process == 0){
        printf("No such process\n");
        return;
    }
    int ret = 0;
    ret = kill(process, SIGCONT);
    if (ret == -1){
        perror("kill");
        return;
    }
}

void handle_ctrlc(int sig){
    // if ctrl+c is pressed, kill whatever process is running
    int pid = fgprocess;
    int ret = 0;
    ret = kill(pid, SIGINT);
    printf("\n");
    if (ret == -1){
        perror("kill");
        return;
    }
}

void handle_ctrld(int sig){
    // if ctrl+d is pressed, quit the shell
    printf("\n");
    exit(0);
}

void handle_ctrlz(int sig){
    // on ctrl+z, send foreground process to background
    int pid = fgprocess;
    int ret = 0;
    ret = kill(pid, SIGTSTP);
    // ret = kill(pid, SIGCONT);
    printf("\n");
    // add process to bgprocesses
    bgprocesses[indexofbg] = pid;
    bgprocessesnames[indexofbg] = fgprocessname;
    indexofbg = (indexofbg + 1) % 50;
    if (ret == -1){
        perror("kill");
        return;
    }
}