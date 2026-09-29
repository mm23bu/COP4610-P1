# COP4610-ShellProject1 (group 38)
This project is a custom command-line shell written in C that provides many of the basic features of a Unix shell. The shell reads and processes user commands, searches for executable programs using `$PATH`, and runs external commands using `fork()` and `execv()`.
It also supports environment variable and tilde expansion, input and output redirection, piping between commands, background processing, and built-in commands such as `cd`, `jobs`, and `exit`. 

## Table of Contents
- [Group Members](#group-members)
- [Division of Labor](#division-of-labor)
- [File Listing](#file-listing)
- [Source File](#source-files)
- [How to Compile & Execute](#how-to-compile--execute)
- [Development Log](#development-log)
- [Meetings](#meetings)
- [Extra Credit](#extra-credit-1)
- [Bugs and Considerations](#bugs-and-considerations)
  

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
- **Responsibilities**: Implement optional shell features beyond the required functionality, including Shell-ception, unlimited piping, and combined piping with I/O redirection.
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
  - These files are generated by the Makefile

- `bin/`
  - Contains the compiled `shell` executable.
  - The executable is generated by the Makefile

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
```bash
./bin/shell
```


## Development Log

### Rodney Carey

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-19 | Implemented tilde expansion to replace tildes (' ~ ') in tokens as HOME path  |
| 2026-09-19 | Create interface for path resolving  |
| 2026-09-28 | Implement internal command execution and testing of project  |

### Dhruv Patel

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-14 | Implemented prompt to display current username, machine, and pwd in required format. Added fallback if NULL.|
| 2026-09-19 | Worked on `$PATH` search to locate executable commands using the directories in the PATH environment variable.|
| 2026-09-22 | Implemented external command using fork() and execv(), used waitpid() to wait for foreground processes to finish. Connected command execution to the `$PATH` search and main shell  |
| 2026-09-22 | Tested external command execution on linprog to ensure commands and their arguments run correctly, and verifies that the shell continues working after commands finish|
| 2026-09-24 | Added input/output redirection with file permissions, expanded file and PATH error checking, improved pipe cleanup, and verified testing on Mac. |
| 2026-09-27 | Added a two-pipe limit check, expanded error handling and testing for multi-pipe commands and freeze-prevention, and re-verified input/output redirection compatibility. |
| 2026-09-25 | Resolved background job tracking and completion status bugs, fixed exit history formatting, and verified core shell built-ins, pipelines, and redirection. |


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


### Meeting 3: September 26, 2026   
We focused on finishing the remaining work and testing the project together. We checked the shell's features and looked for bugs or unexpected behavior that needed attention. This meeting led to additional testing and bug fixing to help make sure the project worked as expected before submission.


## Extra Credit
| Extra Credit | Status | 
|--------------|--------|
| Shell-ception | Completed |
| Unlimited Piping | Not Completed |
| Piping with I/O Redirection | Not Completed |

**Shell-ception**:
- ***Implementation***: Shell-ception was supported by allowing the shell executable to be launched as a normal external command from inside an already running instance of the shell. Since external commands are executed using `fork()` and `execv()`, running `./bin/shell` starts a new child shell while the original shell remains running.
- ***Testing***: Shell-ception was tested by starting multiple nested shell instances using `./bin/shell`. Commands such as `ls`, `pwd`, and `cd` were executed inside the nested shells to confirm that each shell worked normally. Each nested shell was then exited individually to verify that control returned to the previous shell.
- Example Test:
```text
./bin/shell
./bin/shell
./bin/shell
ls
echo $USER
exit
pwd
exit
exit
```



## Bugs and Considerations
- No major known bugs are currently present based on the testing completed so far.
- One consideration is that some error messages may not exactly match the wording used by Bash. However, the messages still indicate the cause of the error, such as an invalid command, missing file, invalid directory, or incorrect number of arguments.
- If the `USER`, `PWD`, or `MACHINE` environment variable is not set, the prompt displays `unknown` for that value. This prevents the prompt from using a null value and allows the shell to continue running normally.


