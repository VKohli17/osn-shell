## Basic Linux Shell

*Made for Assignment 3 for Operating Systems and Networks (Fall 2022), IIIT Hyderabad* 

### Usage
- Use `make` (or `make -s` if you want to suppress the compilation messages) to compile the shell.
- `./ash` runs the shell, Ctrl+D/"quit"/"exit" exits the shell.

### Implemented features
- Input prompt
- Built-in commands - `cd`, `pwd`, `echo`, `quit`/`exit`
- `ls` command (with flags, and file/directory arguments)
- System commands with and without arguments, in foreground and background (with messages indicating completion)
- `pinfo` command (with and without PID argument)
- `discover` (Stripped down version of `find` command)
- `history` (shows last 10 unique commands)
- i/o redirection, piping
- Commands pertaining to foreground, background, stopped processes:
  - `jobs` to list all processes spawned by shell
  - `sig` to send a signal to a process
  - `fg` to bring a process to foreground
  - `bg` to resume a stopped background process
- Signal handling:
  - Ctrl+C to kill foreground process
  - Ctrl+Z to send foreground process to background
  - Ctrl+D to exit shell

Since system commands are supported, most features not explicitly implemented ought to work as well. Some exceptions to this:
- The i/o is all high level, so escape sequences, tab completion, no echo mode etc. are not supported.
- alias has not been implemented for this shell, running it will only affect parent shell.
- Similarly, user profile and environment variables are not supported.

Assumptions:
- Username, nodename, path names are all less than 150 characters.
- The maximum supported input size to the terminal is 4096 characters.
- Tabs are treated as spaces
- I'm truncating user, group name to 5 characters (and size to 10, 0-padded) in `ls -l` output to maintain a nicer formatting
- recursion depth in discover is 3
- alphabetical sorting (like for `ls` output): uppercase takes precedence
- `&` is treated as an argument for a command, and does not tokenize the command like `;` does.

Work in progress/for later:
- Making the readme more user-friendly for people unfamiliar with the assignment
- Cleaning up the code
- Add more Harry Potter references/easter eggs
- fix crash when using output redirection along with piping
- implement the abovementioned skipped features