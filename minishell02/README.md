# minishell

A limited recreation of **bash**, built from scratch in C as a project for [42 Firenze](https://42firenze.it/).

## About

Minishell is one of the core projects in the 42 curriculum. The goal is to implement a functional shell that reproduces a subset of bash behavior, gaining hands-on experience with process creation, file descriptors, pipes, signal handling, and environment management.

The reference behavior for edge cases is always `bash`.

## Features

- Interactive prompt with command history (via `readline`)
- Execution of commands using `PATH` resolution or absolute/relative paths
- Quoting: single quotes `'...'` (literal) and double quotes `"..."` (allows `$` expansion)
- Environment variable expansion (`$VAR`, `$?`)
- Redirections: `<`, `>`, `>>`, `<<` (here-document)
- Pipelines (`cmd1 | cmd2 | ...`)
- Signal handling (`ctrl-C`, `ctrl-D`, `ctrl-\`) matching bash behavior

### Built-in commands

| Command  | Notes                          |
|----------|--------------------------------|
| `echo`   | supports `-n`                  |
| `cd`     | relative or absolute path      |
| `pwd`    | no options                     |
| `export` | no options                     |
| `unset`  | no options                     |
| `env`    | no options or arguments        |
| `exit`   | no options                     |

## Build

```bash
make        # compile
make clean  # remove object files
make fclean # remove object files and binary
make re     # fclean + recompile
```

The binary is named `minishell`. Run it with:

```bash
./minishell
```

### Debug targets

```bash
make debug     # recompile with -g -O0
make asan      # compile with AddressSanitizer + UBSan
make valgrind  # run valgrind on the debug binary
```

## Requirements

- A C compiler (`cc`)
- GNU `readline` library
- `make`
