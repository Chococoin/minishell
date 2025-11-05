# Tests del Parser

## Estado del Parser

El parser está implementado y funcional con las siguientes características:

### ✅ Implementado
1. **Tokenización**: Separa input por espacios, respeta comillas
2. **Clasificación de tokens**: WORD, PIPE, REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC
3. **Construcción de comandos**: Lista enlazada de comandos con argv y redirecciones
4. **Expansiones**:
   - `$VAR`: expande variables de entorno
   - `$?`: expande exit status
   - Respeta comillas: `'...'` no expande, `"..."` sí expande
5. **Validaciones**:
   - Comillas sin cerrar
   - Pipes mal colocados (inicio/fin/consecutivos)
   - Redirecciones sin archivo
   - Caracteres especiales no soportados (`;`, `\`, `&`)

## Casos de Prueba

### 1. Tokenización Básica

| Input | Tokens Esperados |
|-------|------------------|
| `echo hola` | [WORD:"echo"], [WORD:"hola"] |
| `echo    hola` | [WORD:"echo"], [WORD:"hola"] (múltiples espacios ignorados) |
| `echo 'hola mundo'` | [WORD:"echo"], [WORD:"hola mundo"] |
| `echo "hola mundo"` | [WORD:"echo"], [WORD:"hola mundo"] |

### 2. Clasificación de Tokens

| Input | Tokens con Tipo |
|-------|-----------------|
| `echo hola \| grep h` | [WORD:"echo"], [WORD:"hola"], [PIPE], [WORD:"grep"], [WORD:"h"] |
| `cat < input.txt` | [WORD:"cat"], [REDIR_IN], [WORD:"input.txt"] |
| `echo hola > output.txt` | [WORD:"echo"], [WORD:"hola"], [REDIR_OUT], [WORD:"output.txt"] |
| `echo hola >> output.txt` | [WORD:"echo"], [WORD:"hola"], [REDIR_APPEND], [WORD:"output.txt"] |
| `cat << EOF` | [WORD:"cat"], [HEREDOC], [WORD:"EOF"] |

### 3. Construcción de Comandos

| Input | Estructura Esperada |
|-------|-------------------|
| `echo hola` | cmd{argv:["echo","hola"], redirs:NULL, next:NULL} |
| `echo hola \| grep h` | cmd1{argv:["echo","hola"], next:cmd2} -> cmd2{argv:["grep","h"]} |
| `cat < in.txt > out.txt` | cmd{argv:["cat"], redirs:[IN:"in.txt", OUT:"out.txt"]} |

### 4. Expansiones

| Input | Resultado Esperado (con USER=german) |
|-------|-------------------------------------|
| `echo $USER` | Expande a "echo german" |
| `echo '$USER'` | NO expande, mantiene "$USER" literal |
| `echo "$USER"` | Expande a "echo german" |
| `echo $?` | Expande al último exit status (ej: "0") |
| `echo "$USER:$?"` | Expande a "echo german:0" |

### 5. Validaciones - Casos de Error

| Input | Error Esperado |
|-------|----------------|
| `echo "sin cerrar` | `unexpected EOF while looking for matching '"'` |
| `echo 'sin cerrar` | `unexpected EOF while looking for matching "'"` |
| `\| echo hola` | `syntax error near unexpected token '\|'` |
| `echo hola \|` | `syntax error near unexpected token 'newline'` |
| `echo \| \| grep` | `syntax error near unexpected token '\|'` |
| `echo >` | `syntax error near unexpected token 'newline'` |
| `echo ; echo` | `minishell: ';' is not supported` |
| `echo \\ hola` | `minishell: '\\' is not supported` |
| `echo & hola` | `minishell: '&' is not supported` |

### 6. Casos Complejos

| Input | Descripción |
|-------|-------------|
| `cat < input.txt \| grep hola \| sort > output.txt` | Input + múltiples pipes + output |
| `echo "User: $USER Status: $?"` | Múltiples expansiones en comillas dobles |
| `echo 'no expand $USER' "expand $USER"` | Mix de comillas simples y dobles |
| `ls -la \| grep ".txt" \| wc -l > count.txt` | Pipeline completo con opciones |

## Verificación Manual

Para probar manualmente el parser, ejecutar el minishell y probar cada caso:

```bash
./minishell
minishell> echo hola
minishell> echo $USER
minishell> echo "sin cerrar
minishell> | echo
minishell> exit
```

## Próximos Pasos

1. ✅ Parser implementado
2. ⏳ Verificar memory leaks con valgrind
3. ⏳ Documentar estructuras de datos para la defensa
4. ⏳ Preparar ejemplos visuales del flujo de parsing

## Notas de Implementación

### Archivos del Parser
- `src/parsing/split_input.c`: Primera fase - split por espacios/comillas
- `src/parsing/tokenizer.c`: Segunda fase - clasificar tokens
- `src/parsing/expand.c` + `expand_utils.c`: Tercera fase - expandir variables
- `src/parsing/command_builder.c`: Cuarta fase - construir comandos
- `src/parsing/validate.c`: Validaciones de caracteres especiales

### Estructuras de Datos
```c
typedef struct s_token {
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

typedef struct s_cmd {
    char            **argv;
    size_t          argc;
    t_redir         *redirs;
    struct s_cmd    *next;
} t_cmd;
```

### Flujo de Parsing
```
Input String
    ↓
split_input() → char **parts
    ↓
tokenize() → t_token *tokens
    ↓
expand_tokens() → tokens con expansiones
    ↓
commands_from_tokens() → t_cmd *commands
    ↓
Lista enlazada de comandos lista para ejecutar
```
