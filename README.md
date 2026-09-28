# COP4610-ShellProject1 (group 38)

## Table of Contents
- [Group Members](#group-members)
- [Division of Labor](#division-of-labor)
- [File Listing](#file-listing)
- [Source File](#source-files)
- [How to Compile & Execute](#how-to-compile--execute)
- [Development Log](#development-log)
- [Meetings](#meetings)
- [Bugs](#bugs)
- [Extra Credit](#extra-credit-1)
- [Considerations](#considerations)

This project is a custom command-line shell written in C that provides many of the basic features of a Unix shell. The shell reads and processes user commands, searches for executable programs using `$PATH`, and runs external commands using `fork()` and `execv()`.
It also supports environment variable and tilde expansion, input and output redirection, piping between commands, background processing, and built-in commands such as `cd`, `jobs`, and `exit`. 

## Group Members
- **Rodney Carey**: rc24n@fsu.edu
  - Github ID: rodneycd
- **Dhruv Patel**: drp24@fsu.edu
  - Github ID: DhruvP16
- **Mahir Asef Bin Masud**: mm23bu@fsu.edu
  - Github ID: mm23bu

## Division of Labor

### Part 1: Prompt
- **Responsibilities**: Display a shell prompt containing the current username, machine name, and working directory.
- **Assigned to**: Dhruv Patel

### Part 2: Environment Variables
- **Responsibilities**: Expand tokens beginning with `$` into their corresponding environment variable values, allowing users to reference environment variables in any shell comman
- **Assigned to**: Mahir Masud, Rodney Carey

### Part 3: Tilde Expansion
- **Responsibilities**: Expand the tilde (`~`) into the user's home directory when used alone or at the beginning of a path followed by `/`.
- **Assigned to**: Rodney Carey

### Part 4: $PATH Search
- **Responsibilities**: Locate executable commands by searching the directories listed in `$PATH`. Display an error message if a command cannot be found.
- **Assigned to**: Dhruv Patel, Rodney Carey

### Part 5: External Command Execution
- **Responsibilities**: Allow the shell to execute external programs, including commands with arguments, while keeping the shell itself running
- **Assigned to**: Mahir Masud, Rodney Carey

### Part 6: I/O Redirection
- **Responsibilities**: Support input and output redirection, allowing commands to read input from files and write output to files. Create or overwrite output files as required and report errors for invalid input files.
- **Assigned to**: Dhruv Patel, Mahir Masud

### Part 7: Piping
- **Responsibilities**: Support up to two pipes, allowing the output of one command to be passed as input to the next. Enable multiple connected commands to execute concurrently.
- **Assigned to**: Rodney Carey, Dhruv Patel, Mahir Masud

### Part 8: Background Processing
- **Responsibilities**: Allow commands to execute in the background without preventing users from entering additional commands. Track background jobs, display their status, and support background processing with piping and I/O redirection.
- **Assigned to**: Rodney Carey, Dhruv Patel, Mahir Masud

### Part 9: Internal Command Execution
- **Responsibilities**: Support the built-in commands `exit`, `cd`, and `jobs`. Allow users to exit the shell, change the current working directory, and view active background processes.
- **Assigned to**: Rodney Carey, Dhruv Patel, Mahir Masud

### Extra Credit
- **Responsibilities**: [Description]
- **Assigned to**: Rodney Carey, Dhruv Patel, Mahir Masud

## File Listing
```
shell/
│
├── src/
│ ├── executor.c
│ ├── lexer.c
│ ├── parser.c
│ ├── jobs.c
│ └── main.c
│
├── include/
│ ├── executor.h
│ ├── lexer.h
│ ├── parser.h
│ ├── jobs.h
│ └── shell.h
│
├── bin/
├── obj/
│
├── README.md
└── Makefile
```
### Source Files
- `src/main.c`
  - Main control loop for the shell.
  - Displays the prompt, reads user input, tokenizes and expands commands, and sends commands to the executor.
  - Coordinates functionality from Parts 1-9.
    
- `src/lexer.c`
  - Handles user input and tokenization.
  - Reads command-line input and separates it into individual tokens.
  - Also contains the prompt display function used for Part 1.

- `src/parser.c`
  - Handles token expansion before commands are executed.
  - Implements environment variable expansion for Part 2.
  - Implements tilde expansion for Part 3.
  - Implements `$PATH` searching for Part 4.

- `src/executor.c`
  - Handles command execution and process creation.
  - Implements external command execution using `fork()` and `execv()` for Part 5.
  - Implements input and output redirection for Part 6.
  - Implements piping for Part 7.
  - Handles background command execution for Part 8.
  - Implements the built-in commands `cd`, `jobs`, and `exit` for Part 9.

- `src/jobs.c`
  - Manages background jobs and command history.
  - Keeps track of job numbers, process IDs, command lines, and job completion for Part 8.
  - Supports the `jobs` command and waiting for background jobs during `exit` for Part 9.
  - Stores the command history used when exiting the shell.

### Header Files

- `include/shell.h`
  - Contains shared constants, system headers, and definitions used throughout the shell.
  - Defines values such as the maximum command-line length, number of commands, and number of background jobs.

- `include/lexer.h`
  - Defines the `tokenlist` structure.
  - Contains function declarations for reading input, tokenizing input, freeing tokens, and displaying the prompt.

- `include/parser.h`
  - Contains declarations for token expansion and `$PATH` searching.
  - Supports Parts 2, 3, and 4.

- `include/executor.h`
  - Contains the declaration for the main command execution function.
  - Supports command execution functionality for Parts 4-9.

- `include/jobs.h`
  - Defines the `Job` structure used to track background processes.
  - Contains declarations for background job management and command history functions used in Parts 8 and 9.

### Build and Documentation Files

- `Makefile`
  - Compiles all source files in `src/` into object files in `obj/`.
  - Links the object files to create the `bin/shell` executable.
  - Provides `make`, `make run`, and `make clean` commands.

- `README.md`
  - Contains the project description, group members, division of labor, file listing, compilation instructions, development logs, meetings, known bugs, and extra credit documentation.

### Generated Directories

- `obj/`
  - Contains `.o` object files created during compilation.
  - These files are generated by the Makefile and should not be committed to the final repository.

- `bin/`
  - Contains the compiled `shell` executable.
  - The executable is generated by the Makefile and should not be committed to the final repository.

## How to Compile & Execute

### Requirements
- **Compiler**: `gcc` for C/C++
- **Build Tool**: `make`

### Compilation
Run the following command from the root project directory:
```bash
make
```
This will compile the source files and build the executable in:
`bin/`

### Execution
```bash
make run
```
This will run the program.
Alternatively, from the root project directory, you can run the executable directly with:
`./bin/shell`

## Development Log
Each member records their contributions here.

### Rodney Carey

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-19 | Implemented tilde expansion to replace tildes (' ~ ') in tokens as HOME path  |
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |

### Dhruv Patel

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-14 | Implemented prompt to display current username, machine, and pwd in required format. Added fallback if NULL.|
| 2026-09-19 | Worked on `$PATH` search to locate executable commands using the directories in the PATH environment variable.|
| 2026-09-22 | Implemented external command using fork() and execv(), used waitpid() to wait for foreground processes to finish. Connected command execution to the `$PATH` search and main shell  |
| 2026-09-22 | Tested external command execution on linprog to ensure commands and their arguments run correctly, and verifies that the shell continues working after commands finish|


### Mahir Masud

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-14 | Initialized Git repository and established project directory layout and basic header/source architecture. |
| 2026-09-18 | Implemented expand_tokens() in parser.c to identify $VAR tokens and perform getenv() lookups. |
| 2026-09-21 | Established base execution architecture in executor.c: defined run_child() with dup2() stream redirection and initialized execute_pipeline() to parse and strip trailing & operators. |
| 2026-09-22 | Implemented pipeline slicing into cmd_argv and extracted < and > filenames by null-terminating operator tokens. |
| 2026-09-23 | Implemented pipe allocation, child process spawning with fork(), and mapped file descriptors for pipeline/redirection I/O. |
| 2026-09-26 | Initialized background job tracking table with init_jobs() and implemented history tracking with record_history() and print_history(). |



## Meetings
We had three major meetings on Discord and all members attended.

### Meeting 1: September 13, 2026  
We discussed the project requirements and the different parts that needed to be completed. We went over who would work on each part and how to divide the responsibilities among the group. By the end of the meeting, we had a division of labor so everyone knew which parts they were responsible for.

### Meeting 2: September 20, 2026  
We checked in on everyone's progress and discussed what had been completed and what still needed work. We also discussed whether the original division of labor needed to change based on everyone's progress and any difficulties they were having. By the end of the meeting, we had reviewed the remaining tasks and clarified everyone's responsibilities for finishing the project.


### Meeting 2: September 26, 2026   
We focused on finishing the remaining work and testing the project together. We checked the shell's features and looked for bugs or unexpected behavior that needed attention. This meeting led to additional testing and bug fixing to help make sure the project worked as expected before submission.

## Bugs
- **Bug 1**: This is bug 1.
- **Bug 2**: This is bug 2.
- **Bug 3**: This is bug 3.

## Extra Credit
- **Extra Credit 1**: [Extra Credit Option]
- **Extra Credit 2**: [Extra Credit Option]
- **Extra Credit 3**: [Extra Credit Option]

## Considerations
[Description]

