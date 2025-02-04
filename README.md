# Custom Shell

## Overview

This project is a simple command-line shell implemented in C++. The shell can execute basic commands, handle built-in commands like cd, help, and exit, and correctly process
input with single or double quotes. It also provides error handling for various cases, such as mismatched quotes, command failures, and incorrect usage of built-in commands.

## Features

```
Basic Command Execution: Executes commands available on the system by forking a child process and using execvp.

Built-in Commands:
cd <directory>: Changes the current directory.
help: Displays a help message.
exit: Exits the shell.

Error Handling:
Detects and reports mismatched quotes in input.
Reports non-zero exit codes from commands.
Handles incorrect usage of the cd command.

Quoting: Supports single (') and double (") quotes for arguments containing spaces.
```
## Getting Started

## Building the Shell

1.Compile the shell:
g++ -o custom_shell custom_shell.cpp

1.Run the shell:
./custom_shell

## Usage

Basic Commands: Type any command available on your system and press Enter to execute it.
$ ls -l

Change Directory:
$ cd /path/to/directory

If the directory does not exist or if the command is misused, the shell will print an error message.
Help:
$ help

Exit:
$ exit

```
Quoting:
Single quotes:
$ echo 'Hello World'
```
```
Double quotes:
$ echo "The cat's name is Felix"
```
```
Mismatched quotes will result in an error:
$ echo "This will cause an error
error: mismatched quotes
```

