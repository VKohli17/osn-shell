#include "functions.h"
#include "headers.h"

extern int mugglesafe = 0;
extern int bgprocess;
extern double timeforfg;

int main() {

    const char fakehome[150];
    const char realhome[150];
    signal(SIGCHLD, eternalchecker);
    signal(SIGINT, handle_ctrlc);
    signal(SIGTSTP, handle_ctrlz);
    signal(SIGQUIT, handle_ctrld);
    strcpy(fakehome, get_cwd());
    strcpy(realhome, "/home/");
    strcat(realhome, get_user());
    while (1){

        if (mugglesafe) prompt(fakehome, mugglesafe, timeforfg);
        else prompt(realhome, mugglesafe, timeforfg);
        char pipedtextpath[150];
        strcpy(pipedtextpath, start_dir);
        strcat(pipedtextpath, ".pipe.txt");
        char input[max_input];
        int i = 0;
        char c;
        while ((c = getchar()) != '\n' && i < max_input - 1){
            // if ctrl + d is pressed
            if (c == EOF){
                printf("\n");
                exit(0);
            }
            if (c == '\t') c = ' ';
            if (!(c == ' ' && (i ==0 || input[i-1] == ' '))){
                input[i] = c;
                i++;
            }
        }
        input[strcspn(input, "\n")] = '\0';

        // remove trailing spaces
        while (input[i-1] == ' ' || input[i-1] == '\t'){
            input[i-1] = '\0';
            i--;
        }
        input[i] = '\0';

        // open .history.txt file in append mode
        char lastline[max_input];
        FILE* history = fopen(".history.txt", "a+");
        if (history == NULL){
            perror("Can't open .history.txt");
            // exit(1);
        }
        // save last line of .history.txt to lastline
        while (fgets(lastline, max_input, history) != NULL);
        if (strcmp(input,lastline)!=0){
            fprintf(history, "%s\n", input);
        }
        // close the file
        fclose(history);

        // tokenize input
        char** commands = (char**)malloc(sizeof(char*) * max_input); // storing all commands
        int num_commands = 0;

        char* command = strtok(input, ";");
        while (command != NULL){
            commands[num_commands] = command;
            num_commands++;
            command = strtok(NULL, ";");
        }

        int isbg[num_commands];
        for (int i = 0;i < num_commands; i++){
            isbg[i] = 0;
        }

        for (int i = 0; i < num_commands; i++){
            trimleadingandTrailing(commands[i]);
        }

        // execute commands
        for (int j=0; j<num_commands;j++){
            // if there are '|' in the command, tokenize into piped_commands
            char** piped_commands = (char**)malloc(sizeof(char*) * 50);
            int num_piped_commands = 0;
            char* piped_command = strtok(commands[j], "|");
            while (piped_command != NULL){
                piped_commands[num_piped_commands] = piped_command;
                num_piped_commands++;
                piped_command = strtok(NULL, "|");
            }
            // if number of piped commands is 0, pipes haven't been used
            int is_piped = 0;
            if (num_piped_commands) is_piped = 1;
            for (int i = 0; i < num_piped_commands; i++){
                trimleadingandTrailing(piped_commands[i]);
            }
            for (int i = 0; i < num_piped_commands; i++){
                char actual_command[max_input];
                strcpy(actual_command, piped_commands[i]);
                char input_file[150];
                char output_file[150];
                int append = 0;
                int is_input = 0;
                int is_output = 0;
                // if there's > but not >>
                if (strchr(piped_commands[i], '>') != NULL && strchr(piped_commands[i], '>') == strrchr(piped_commands[i], '>')){
                    char* token = strtok(commands[j], ">");
                    strcpy(actual_command, token);
                    trimleadingandTrailing(actual_command);
                    token = strtok(NULL, ">");
                    trimleadingandTrailing(token);
                    strcpy(output_file, token);
                    is_output = 1;
                }
                // if there's >>
                else if (strchr(piped_commands[i], '>') != NULL && strchr(piped_commands[i], '>') != strrchr(piped_commands[i], '>')){
                    char* token = strtok(commands[j], ">");
                    strcpy(actual_command, token);
                    trimleadingandTrailing(actual_command);
                    token = strtok(NULL, ">");
                    trimleadingandTrailing(token);
                    strcpy(output_file, token);
                    append = 1;
                    is_output = 1;
                }
                // if there's <
                if (strchr(piped_commands[i], '<') != NULL){
                    char* token = strtok(commands[j], "<");
                    strcpy(actual_command, token);
                    trimleadingandTrailing(actual_command);
                    token = strtok(NULL, "<");
                    trimleadingandTrailing(token);
                    strcpy(input_file, token);
                    is_input = 1;
                }
                // if is_input, open input_file in read mode
                FILE* input_file_pointer;
                if (is_input){
                    input_file_pointer = fopen(input_file, "r");
                    if (input_file_pointer == NULL){
                        perror("Can't open input file");
                        // exit(1);
                    }
                }
                char first_line_input[max_input];
                // read first line of input_file
                if (is_input){
                    fgets(first_line_input, max_input, input_file_pointer);
                    first_line_input[strcspn(first_line_input, "\n")] = '\0';
                }
                // concatenate first line of input_file to actual_command
                if (is_input){
                    strcat(actual_command, " ");
                    strcat(actual_command, first_line_input);
                }
                // if is_piped and i!=0, open .pipe.txt in read mode
                FILE* pipe_read_pointer;
                if (is_piped && i != 0){
                    pipe_read_pointer = fopen(".pipe.txt", "r");
                    if (pipe_read_pointer == NULL){
                        perror("Can't open .pipe.txt");
                        // exit(1);
                    }
                }
                // read first line of .pipe.txt
                char first_line_pipe[max_input];
                if (is_piped && i != 0){
                    fgets(first_line_pipe, max_input, pipe_read_pointer);
                    first_line_pipe[strcspn(first_line_pipe, "\n")] = '\0';
                }
                // concatenate first line of .pipe.txt to actual_command
                if (is_piped && i != 0){
                    strcat(actual_command, " ");
                    strcat(actual_command, first_line_pipe);
                }
                // if is_output, open output_file in write mode
                FILE* output_file_pointer;
                if (is_output){
                    // printf("reached\n");
                    // fclose(pipe_pointer);
                    // freopen("/dev/tty", "w", stdout);
                    if (append){
                        output_file_pointer = freopen(output_file, "a+", stdout);
                    }
                    else{
                        output_file_pointer = freopen(output_file, "w", stdout);
                    }
                    if (output_file_pointer == NULL){
                        perror("Can't open output file");
                        // exit(1);
                    }
                }
                else if (is_piped && i != num_piped_commands-1){
                    output_file_pointer = freopen(pipedtextpath, "w", stdout);
                }
                choose_command(actual_command);
                // close output_file_pointer
                if (is_output || (is_piped && i != num_piped_commands-1)){
                    fclose(output_file_pointer);
                }
                if (is_output || (is_piped && i != num_piped_commands-1 && !is_output)) freopen("/dev/tty", "w", stdout);
                // close input_file_pointer
                if (is_input){
                    fclose(input_file_pointer);
                }
                // close pipe_read_pointer
                if (is_piped && i != 0){
                    fclose(pipe_read_pointer);
                }
            }
        }
    }
    return 0;
}