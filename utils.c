#include "headers.h"
#include "functions.h"

extern int mugglesafe;
extern char endcol[] = "\033[0m";
extern char blue[] = "\033[38;2;29;153;243m";
extern char pink[] = "\033[38;2;239;149;255m";
extern char seafoam[] = "\033[38;2;154;218;209m";
extern char lavender[] = "\033[38;2;128;128;248m";
extern char ice[] = "\033[38;2;191;244;255m";
extern char bold[] = "\e[1m";
extern char endbold[] = "\e[0m";
// extern int bgprocesses[50] = {0};
// extern int indexofbg;
// extern int bgprocess=0;

// removing leading, trailing spaces from commands
void trimleadingandTrailing(char *s){
	int  i,j;
 
	for(i=0;s[i]==' ';i++);
		
	for(j=0;s[i];i++){
		s[j++]=s[i];
	}
	s[j]='\0';
	for(i=0;s[i]!='\0';i++){
		if(s[i]!=' ') j=i;
	}
	s[j+1]='\0';
}

// getting username
char* get_user(){
    static char username[150];
    cuserid(username);
    return username;
}

// getting node name
char* get_machine(){
    static char machine[150];
    struct utsname name;
    if (uname(&name)){
        perror("Can't get machine name");
        exit(1);
    }
    strcpy(machine, name.nodename);
    return machine;
}

// getting current directory (absolute path)
char* get_cwd(){
    static char cwd[150];
    if (getcwd(cwd, sizeof(cwd))){
        return cwd;
    }
    else{
        perror("Can't get current working directory");
        exit(1);
    }
}

char* get_cwd_no_path(char* cwd_with_path){
    static char cwd_no_path[150];
    char* to_paste = strrchr(cwd_with_path, '/') + 1;
    char homedir[150];
    strcpy(homedir, "/home/");
    strcat(homedir, get_user());
    if (strcmp(cwd_with_path, homedir) == 0){
        strcpy(cwd_no_path, "~");
    }
    else{
        strcpy(cwd_no_path, to_paste);
    }
    return cwd_no_path;
}

int filetype(char* filename){
    struct stat s;    
    // return 1 for directory, 2 for executable, 3 for everything else
    if (stat(filename, &s) == 0){
        if (s.st_mode & __S_IFDIR) return 1;
        else if (s.st_mode & S_IXUSR) return 2;
        else return 3;
    }
    else{
        // print the error message
        // perror("stat");
        return 0;
    }

}

void print_long_ls (char* filename){    
    // print the permissions
    struct stat st;
    if (lstat(filename, &st) == 0){
        // print the file type
        if (S_ISDIR(st.st_mode)) printf("d");
        else if (S_ISREG(st.st_mode)) printf("-");
        else if (S_ISLNK(st.st_mode)) printf("l");
        else if (S_ISCHR(st.st_mode)) printf("c");
        else if (S_ISBLK(st.st_mode)) printf("b");
        else if (S_ISFIFO(st.st_mode)) printf("p");
        else if (S_ISSOCK(st.st_mode)) printf("s");
        else printf("?");
        // print the permissions
        printf( (st.st_mode & S_IRUSR) ? "r" : "-");
        printf( (st.st_mode & S_IWUSR) ? "w" : "-");
        printf( (st.st_mode & S_IXUSR) ? "x" : "-");
        printf( (st.st_mode & S_IRGRP) ? "r" : "-");
        printf( (st.st_mode & S_IWGRP) ? "w" : "-");
        printf( (st.st_mode & S_IXGRP) ? "x" : "-");
        printf( (st.st_mode & S_IROTH) ? "r" : "-");
        printf( (st.st_mode & S_IWOTH) ? "w" : "-");
        printf( (st.st_mode & S_IXOTH) ? "x" : "-");
        printf("  ");
        printf("%03d  ", st.st_nlink);
        printf("%-5s  ", getpwuid(st.st_uid)->pw_name);
        printf("%-5s  ", getpwuid(st.st_uid)->pw_name);
        // printf("%-5s  ", getgrgid(st.st_gid)->gr_name);
        printf("%010d  ", st.st_size);
        struct tm* time = localtime(&st.st_mtime);
        char month[3];
        switch (time->tm_mon){
            case 0: strcpy(month, "Jan"); break;
            case 1: strcpy(month, "Feb"); break;
            case 2: strcpy(month, "Mar"); break;
            case 3: strcpy(month, "Apr"); break;
            case 4: strcpy(month, "May"); break;
            case 5: strcpy(month, "Jun"); break;
            case 6: strcpy(month, "Jul"); break;
            case 7: strcpy(month, "Aug"); break;
            case 8: strcpy(month, "Sep"); break;
            case 9: strcpy(month, "Oct"); break;
            case 10: strcpy(month, "Nov"); break;
            case 11: strcpy(month, "Dec"); break;
        }
        printf("%s  %02d  %02d:%02d  ", month, time->tm_mday, time->tm_hour, time->tm_min);
        int type = filetype(filename);
        char *color = NULL, *isbold = NULL;
        if (type==1){
            color = blue;
            isbold = bold;
        }
        else if (type==2){
            color = pink;
            isbold = bold;
        }
        else{
            color = endcol;
            isbold = endbold;
        }
        // print the file name
        printf("%s%s%s%s%s\n", color, isbold, filename, endcol, endbold);
    }
    else {
        // print the error message
        perror("stat");
    }

}

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}