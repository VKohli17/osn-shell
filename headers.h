#include <stdio.h>    
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <termios.h>
#include <ctype.h>

#define max_input 4096
#define _DEFAULT_SOURCE
#define __USE_GNU
#define __USE_LARGEFILE64
#define _GNU_SOURCE
#define start_dir "/home/albus/Documents/OSN/A2/"
#define realhome_dir "/home/albus/"