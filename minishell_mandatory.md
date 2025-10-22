# 🐚 Minishell — Mandatory Part

### Program name  
`minishell`

### Turn-in files  
`Makefile`, `*.h`, `*.c`

### Makefile rules  
`NAME`, `all`, `clean`, `fclean`, `re`

### Arguments  
*(no arguments required)*

---

## 🧩 External Functions Allowed
```
readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs
```

### Libft authorized  
✅ Yes

---

## 🧠 Description  
Write your own simple shell — **a mini Bash**.

Your shell must:

1. **Display a prompt** when waiting for a new command.  
2. Have a **working history** (via `readline` and `add_history`).  
3. **Search and launch executables** (based on `$PATH` or using relative/absolute paths).  
4. Use **at most one global variable** — only to store the signal number received.  

   ⚠️ This variable must *only* hold the signal number.  
   It must **not** give access to other data or structures.

---

## 🚫 Forbidden Behavior
- Do **not** interpret:
  - Unclosed quotes  
  - Special characters not required (e.g. `\`, `;`)  
- No structures or data in global scope (except the signal number).

---

## ✴️ Quotes Handling
- **Single quotes `'`** → prevent interpretation of all metacharacters inside.  
- **Double quotes `"`** → prevent interpretation of metacharacters *except* `$`.

---

## 🔁 Redirections
Implement:

| Operator | Behavior |
|-----------|-----------|
| `<` | Redirect input |
| `>` | Redirect output |
| `<<` | Heredoc — read until a delimiter line (no history update required) |
| `>>` | Append output (append mode) |

---

## 🔗 Pipes
Implement **pipes (`|`)**:  
> The output of each command must be connected to the input of the next one.

---

## 🌿 Environment Variables
Your minishell must handle:

- `$VAR` → expand to the variable value.  
- `$?` → expand to the **exit status** of the most recent foreground pipeline.

---

## ⚙️ Signal Handling
Handle:
- **Ctrl-C**, **Ctrl-D**, **Ctrl-\\** → same behavior as Bash.

### In interactive mode:
| Key | Behavior |
|------|-----------|
| `Ctrl-C` | Display a new prompt on a new line |
| `Ctrl-D` | Exit the shell |
| `Ctrl-\` | Do nothing |

---

## 🧩 Built-in Commands
Your shell must implement the following built-ins:

| Command | Behavior |
|----------|-----------|
| `echo` | with option `-n` |
| `cd` | with only a relative or absolute path |
| `pwd` | with no options |
| `export` | with no options |
| `unset` | with no options |
| `env` | with no options or arguments |
| `exit` | with no options |

---

## 💡 Notes
- `readline()` may cause memory leaks — **you don’t need to fix leaks inside `readline`**,  
  but **your own code must be leak-free**.
- Limit yourself strictly to what the subject requires.  
  Anything not explicitly required is **not expected**.
- If in doubt, **use Bash as reference behavior**.

---

## 🔗 Reference
👉 [GNU Bash Manual](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/)
