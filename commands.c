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
char last_dir[150];
char temp_dir[150];
int last_dir_set = 0;

// pwd
void pwd(){
    char pwd[150];
    if (getcwd(pwd, sizeof(pwd))){
        printf("%s\n", pwd);
    }
    else{
        perror("Can't get current working directory");
    }
}

// echo
void echo(char* input){
    printf("%s\n", input);
}

// cd
void cd (char* input){
    // printf("Vansh Chutiya hai\n");
    char home_dir[150];
    strcpy(home_dir, "/home/");
    strcat(home_dir, get_user());
    // if input has any spaces, return error
    if (strchr(input, ' ')){
        printf("cd: invalid input, only one argument possible\n");
        return;
    }
    if (strcmp(input,".")==0){
        chdir(get_cwd());
    }
    else if (strcmp(input,"-")==0){
        if (last_dir_set){
            strcpy(temp_dir, last_dir);
            strcpy(last_dir, get_cwd());
            if (chdir(temp_dir)){
                perror("Can't change directory");
            }
            pwd();
        }
    }
    else if (input[0]=='~'){
        char new_dir[150];
        strcpy(last_dir, get_cwd());
        last_dir_set = 1;
        strcpy(new_dir, home_dir);
        strcat(new_dir, input+1);
        if (chdir(new_dir)){
            perror("Can't change directory");
        }
    }
    else{
        strcpy(last_dir, get_cwd());
        last_dir_set = 1;
        if (chdir(input)){
            perror("Can't change directory");
        }
    }
}

// clear
void clear(){
    system("clear");
}

// ls
void ls(char** input){
    const char cur_dir[150];
    strcpy(cur_dir, get_cwd());
    struct dirent *entry = NULL;
    DIR *dir = NULL;
    // iterate through all arguments, look for -l, -a, -la, -al
    int isl = 0;
    int isa = 0;
    int i = 0;
    int j = 0;
    char** arguments = (char**)malloc(sizeof(char*) * (max_input-3));    
    while (input[i] != NULL){
        if (strcmp(input[i], "-l") == 0){
            isl = 1;
        }
        else if (strcmp(input[i], "-a") == 0){
            isa = 1;
        }
        else if (strcmp(input[i], "-la") == 0 || strcmp(input[i], "-al") == 0){
            isl = 1;
            isa = 1;
        }
        else{
            if (strcmp(input[i],".")==0){
                arguments[j] = cur_dir;
            }
            else if (input[i][0] == '~'){
                char home_dir[150];
                strcpy(home_dir, "/home/");
                strcat(home_dir, get_user());
                char new_dir[150];
                strcpy(new_dir, home_dir);
                strcat(new_dir, input[i]+1);
                arguments[j] = new_dir;
            }
            else{
                arguments[j] = input[i];
            }
            j++;
        }
        i++;
    }

    // if no arguments, use current directory
    if (arguments[0] == NULL){
        arguments[0] = cur_dir;
    }
    printf("\n");
    // iterate through all arguments
    i = 0;
    if (!isl){
        while (arguments[i] != NULL){
            // check if the argument is the name of a file or a directory
            if (filetype(arguments[i])==1){
                chdir(arguments[i]);
                dir = opendir(get_cwd());
                // dir = opendir(arguments[i]);
                if (dir == NULL){
                    perror("Can't open directory");
                }
                // print the directory name in blue and bold
                if (j){
                    printf("%s%s%s%s%s:\n", ice, bold, arguments[i], endcol, endbold);
                } 
                // arrange the files in the directory in alphabetical order
                int n = 0;
                while ((entry = readdir(dir)) != NULL){
                    n++;
                }
                char** files = (char**)malloc(sizeof(char*) * n);
                rewinddir(dir);
                int j = 0;
                while ((entry = readdir(dir)) != NULL){
                    files[j] = entry->d_name;
                    j++;
                }
                for (int i = 0; i < n-1; i++){
                    for (int j = 0; j < n-i-1; j++){
                        if (strcmp(files[j], files[j+1]) > 0){
                            char* temp = files[j];
                            files[j] = files[j+1];
                            files[j+1] = temp;
                        }
                    }
                }
                // print the files in the directory, with blue for directories and pink for executables
                for (int i = 0; i < n; i++){
                    if (isa || files[i][0] != '.'){
                        char* isbold = NULL;
                        char* color = NULL;
                        if (filetype(files[i])==1){
                            isbold = bold;
                            color = blue;
                        }
                        else if (filetype(files[i])==2){
                            isbold = bold;
                            color = pink;
                        }
                        else{
                            isbold = endbold;
                            color = endcol;
                        }
                        printf("%s%s%s%s%s\n", isbold, color, files[i], endbold, endcol);
                    }
                }
                printf("\n");
                closedir(dir);
                chdir(cur_dir);
                i++;
            }
            else if (filetype(arguments[i])==2){
                printf("%s%s%s%s%s\n", pink, bold, arguments[i], endcol, endbold);
                printf("\n");
                i++;
            }
            else if (filetype(arguments[i])==3 || filetype(arguments[i])==0){
                printf("%s\n", arguments[i]);
                printf("\n");
                i++;
            }
            else{
                perror("This is not a file or directory");
                i++;
            }
        }
    }
    else{
        while (arguments[i] != NULL){
            // check if the argument is the name of a file or a directory
            if (filetype(arguments[i])==1){
                chdir(arguments[i]);
                dir = opendir(get_cwd());
                // dir = opendir(arguments[i]);
                if (dir == NULL){
                    perror("Can't open directory");
                }
                // print the directory name in blue and bold
                if (j){
                    printf("%s%s%s%s%s:\n", ice, bold, arguments[i], endcol, endbold);
                } 
                // arrange the files in the directory in alphabetical order
                int n = 0;
                while ((entry = readdir(dir)) != NULL){
                    n++;
                }
                char** files = (char**)malloc(sizeof(char*) * n);
                rewinddir(dir);
                int j = 0;
                while ((entry = readdir(dir)) != NULL){
                    files[j] = entry->d_name;
                    j++;
                }
                for (int i = 0; i < n-1; i++){
                    for (int j = 0; j < n-i-1; j++){
                        if (strcmp(files[j], files[j+1]) > 0){
                            char* temp = files[j];
                            files[j] = files[j+1];
                            files[j+1] = temp;
                        }
                    }
                }
                // print long ls 
                for (int i = 0; i < n; i++){
                    if (isa || files[i][0] != '.'){
                        print_long_ls(files[i]);
                        // printf("%s\n", files[i]);
                    }
                }
                printf("\n");
                closedir(dir);
                chdir(cur_dir);
                i++;
            }
            else if (filetype(arguments[i])==2 || filetype(arguments[i])==3 || filetype(arguments[i])==0){
                print_long_ls(arguments[i]);
                printf("\n");
                i++;
            }
            else{
                perror("This is not a file or directory");
                i++;
            }
        }
    }
}

// pinfo
void pinfo(int pid){
    char path[150];
    char status[150];
    char getline[150];
    char mem[150];
    char name[150];
    char pathofexe[150];
    char* exe = (char*)malloc(sizeof(char) * 150);
    FILE *fd;
    char pid_string[150];
    char pid_name[150];
    sprintf(pid_string, "%d", pid);
    strcpy(path, "/proc/");
    strcat(path, pid_string);
    strcat(path, "/status");
    fd = fopen(path, "r");
    if (fd == NULL){
        perror("Can't open file");
    }
    else{
        // read line by line
        while (fgets(getline, 150, fd) != NULL){
            // get the name
            if (strstr(getline, "Name:") != NULL){
                strcpy(name, getline);
                for (int i = 0; i < strlen(name); i++){
                    if (name[i] == '\t'){
                        name[i] = ' ';
                    }
                }
                printf("%s", name);
            }
            // Pid
            else if (strncmp(getline, "Pid:", 4) == 0){
                strcpy(pid_name, getline);
                for (int i = 0; i < strlen(pid_name); i++){
                    if (pid_name[i] == '\t'){
                        pid_name[i] = ' ';
                    }
                }
                printf("%s", pid_name);
            }
            // state
            else if (strstr(getline, "State:") != NULL){
                strcpy(status, getline);
                for (int i = 0; i < strlen(status); i++){
                    if (status[i] == '\t'){
                        status[i] = ' ';
                    }
                }
                printf("%s", status);
            }
            // memory
            else if (strstr(getline, "VmSize:") != NULL){
                strcpy(mem, getline);
                mem[0] = 'M';
                mem[1] = 'e';
                mem[2] = 'm';
                mem[3] = 'o';
                mem[4] = 'r';
                mem[5] = 'y';
                for (int i = 0; i < strlen(mem); i++){
                    if (mem[i] == '\t'){
                        mem[i] = ' ';
                    }
                }
                // remove consecutive spaces
                int i = 0;
                int j = 0;
                while (mem[i]){
                    if (!(mem[i] == ' ' && mem[i+1] == ' ')){
                        mem[j++] = mem[i];
                    }
                    i++;
                }
                mem[j] = '\0';
                printf("%s", mem);
            }            
        }
        strcpy(pathofexe, "/proc/");
        strcat(pathofexe, pid_string);
        strcat(pathofexe, "/exe");
        long long len = readlink(pathofexe, exe, 150);
        exe[len] = '\0';
        if (len == -1){
            perror("Can't read link");
        }
        else{
            printf("Executable Path: %s\n", exe);
        }
        fclose(fd);
    }
}

// discover
void discover(char** input){
    char cur_dir[150];
    strcpy(cur_dir, get_cwd());
    struct dirent *entry = NULL;
    DIR *dir = NULL;
    // iterate through all arguments, look for -d, -f.
    int isf = 1;
    int isd = 1;
    int i = 0;
    char inputfile[150];
    char inputfolder[150];
    // set both to null
    inputfile[0] = '\0';
    inputfolder[0] = '\0';
    while (input[i] != NULL){
        if (strcmp(input[i], "-f") == 0){
            isd = 0;
        }
        else if (strcmp(input[i], "-d") == 0){
            isf = 0;
        }
        else{
            // if it is enclosed in quotes, set inputfile to the string between the quotes
            if (input[i][0] == '\"'){
                int j = 1;
                while (input[i][j] != '\"'){
                    inputfile[j-1] = input[i][j];
                    j++;
                }
                inputfile[j-1] = '\0';
            }
            // if no quotes, set inputfolder to the string
            else{
                strcpy(inputfolder, input[i]);
            }
        }
        i++;
    }
    if (isd == isf) {
        isd = 1;
        isf = 1;
    }
    if (inputfolder[0] == '\0'){
        strcpy(inputfolder, cur_dir);
    }
    if (chdir(inputfolder) == -1){
        perror("Can't change directory");
    }
    // if (isd*isf == 1){
        // print both files and directories
        dir = opendir(get_cwd());
        if (dir == NULL){
            perror("Can't open directory");
        }
        else{
            while ((entry = readdir(dir)) != NULL){
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
                    if (filetype(entry->d_name) == 1){
                        // if inputline is null or if inputline is not null and the name of the file matches
                        if (inputfile[0] == '\0' || (strcmp(entry->d_name, inputfile) == 0)){
                            if (isd) printf("%s%s%s%s%s\n", blue, bold, entry->d_name, endcol, endbold);
                        }
                        chdir(entry->d_name);
                        // print all files in the directory
                        DIR *dir2 = opendir(get_cwd());
                        if (dir2 == NULL){
                            perror("Can't open directory");
                        }
                        else{
                            struct dirent *entry2 = NULL;
                            while ((entry2 = readdir(dir2)) != NULL){
                                if (strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0){
                                    if (filetype(entry2->d_name) == 1){
                                        // if inputline is null or if inputline is not null and the name of the file matches
                                        if (inputfile[0] == '\0' || (strcmp(entry2->d_name, inputfile) == 0)){
                                            if (isd) printf("%s%s%s%s%s\n", blue, bold, entry2->d_name, endcol, endbold);
                                        }
                                        chdir(entry2->d_name);
                                        // print all files in the directory
                                        DIR *dir3 = opendir(get_cwd());
                                        if (dir3 == NULL){
                                            perror("Can't open directory");
                                        }
                                        else{
                                            struct dirent *entry3 = NULL;
                                            while ((entry3 = readdir(dir3)) != NULL){
                                                if (strcmp(entry3->d_name, ".") != 0 && strcmp(entry3->d_name, "..") != 0){
                                                    if (filetype(entry3->d_name) == 1){
                                                        // if inputline is null or if inputline is not null and the name of the file matches
                                                        if (inputfile[0] == '\0' || (strcmp(entry3->d_name, inputfile) == 0)){
                                                            if (isd) printf("%s%s%s%s%s\n", blue, bold, entry3->d_name, endcol, endbold);
                                                        }
                                                    }
                                                    else{
                                                        // if inputline is null or if inputline is not null and the name of the file matches
                                                        if (inputfile[0] == '\0' || (strcmp(entry3->d_name, inputfile) == 0)){
                                                            if (isf) printf("%s\n" , entry3->d_name);
                                                        }
                                                    }
                                                }
                                            }
                                            closedir(dir3);
                                        }
                                        chdir("..");
                                        printf("\n");
                                    }
                                    else {
                                        if (inputfile[0] == '\0' || (strcmp(entry2->d_name, inputfile) == 0)){
                                            if (isf) printf("%s\n", entry2->d_name);
                                        }           
                                    }
                                }
                            }
                            closedir(dir2);
                        }
                        chdir("..");
                        printf("\n");
                    }
                    else{
                        if (inputfile[0] == '\0' || (strcmp(entry->d_name, inputfile) == 0)){
                            if (isf) printf("%s\n", entry->d_name);
                        }
                    }
                }
            }
            closedir(dir);
        }
    chdir(cur_dir);
}

// history
void history(){
    // read and print the last 10 lines of history
    char historypath[150];
    strcpy(historypath, start_dir);
    strcat(historypath, ".history.txt");
    // printf("%s", historypath);
    FILE *fp = fopen(historypath, "r");
    if (fp == NULL){
        perror("Can't open file");
    }
    else{
        // array of 40 strings
        char *lines[40];
        char line[max_input];
        // print last 10 lines unless they're the same as the previous line
        int i = 0;
        while (fgets(line, max_input, fp) != NULL){
            i++;
        }
        i = i - 40;
        if (i < 0) i = 0;
        fseek(fp, 0, SEEK_SET);
        int line_index = 0;
        while (fgets(line, max_input, fp) != NULL){
            if (i > 0){
                i--;
            }
            else{
                // push into lines array
                lines[line_index] = malloc(max_input);
                strcpy(lines[line_index], line);
                line_index++;
            }
        }
        char* last_ten_unique_lines [11];
        int new_index = 0;
        int old_index = 39;
        // copy last line into first of new
        last_ten_unique_lines[new_index] = malloc(max_input);
        strcpy(last_ten_unique_lines[new_index], lines[old_index]);
        new_index++;
        old_index--;
        while (new_index < 11 && old_index >=0){
            if (strcmp(last_ten_unique_lines[new_index-1], lines[old_index]) != 0){
                last_ten_unique_lines[new_index] = malloc(max_input);
                strcpy(last_ten_unique_lines[new_index], lines[old_index]);
                new_index++;
            }
            old_index--;
        }
        printf("\n");
        for (int i = 10; i > 0; i--){
            printf("%s", last_ten_unique_lines[i]);
        }
        printf("\n");
        fclose(fp);
    }
}