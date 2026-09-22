# COP4610-P1
# Shell

[Description]

## Group Members
- **Rodney Carey**: @fsu.edu
- **Dhruv Patel**: drp24@fsu.edu
- **Mahir Asef Bin Masud**: mm23bu@fsu.edu
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
├── README.md
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: e.g., `gcc` for C/C++, `rustc` for Rust.
- **Dependencies**: List any libraries or frameworks necessary (rust only)

### Compilation
For a C/C++ example:
```bash
make
```
This will build the executable in ...
### Execution
```bash
make run
```
This will run the program ...

## Development Log
Each member records their contributions here.

### [Rodney Carey]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-19 | Implemented tilde expansion to replace tildes (' ~ ') in tokens as HOME path  |
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |

### [Dhruv Patel]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-14 | Implemented prompt to display current username, machine, and pwd in required format. Added fallback if NULL.|
| 2026-09-19 | Worked on `$PATH` search to locate executable commands using the directories in the PATH environment variable.|
| 2026-09-22 | Implemented external command using fork() and execv(), used waitpid() to wait for foreground processes to finish. Connected command execution to the `$PATH` search and main shell  |
| 2026-09-22 | Tested external command execution on linprog to ensure commands and their arguments run correctly, and verifies that the shell continues working after commands finish|


### [Mahir Masud]

| Date       | Work Completed / Notes |
|------------|------------------------|
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |


## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| YYYY-MM-DD | [Names]              | [Agenda items]   | [Actions/Next steps]  |
| YYYY-MM-DD | [Names]              | [Agenda items]   | [Actions/Next steps]  |



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

