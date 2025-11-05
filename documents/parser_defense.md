# Documentación del Parser - Defensa

## Introducción

El parser de minishell es responsable de convertir la entrada del usuario en una estructura de datos ejecutable. Está diseñado siguiendo el principio de separación de responsabilidades, dividido en 4 fases principales.

## Arquitectura del Parser

### Vista General del Flujo

```
Usuario escribe: echo "Hello $USER" | grep Hello > out.txt
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ FASE 1: Split Input                                         │
│ Divide por espacios respetando comillas                     │
│ Output: ["echo", "Hello $USER", "|", "grep", "Hello",      │
│          ">", "out.txt"]                                    │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ FASE 2: Tokenize                                            │
│ Clasifica cada elemento como WORD, PIPE, REDIR, etc.       │
│ Output: [WORD:"echo", WORD:"Hello $USER", PIPE,            │
│          WORD:"grep", WORD:"Hello", REDIR_OUT,             │
│          WORD:"out.txt"]                                    │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ FASE 3: Expand                                              │
│ Expande variables ($VAR, $?) respetando comillas           │
│ Output: [WORD:"echo", WORD:"Hello german", PIPE,           │
│          WORD:"grep", WORD:"Hello", REDIR_OUT,             │
│          WORD:"out.txt"]                                    │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ FASE 4: Build Commands                                      │
│ Agrupa tokens en comandos con redirecciones                │
│ Output: cmd1{argv:["echo","Hello german"],                 │
│              redirs:NULL, next:cmd2}                        │
│         cmd2{argv:["grep","Hello"],                         │
│              redirs:[OUT:"out.txt"], next:NULL}             │
└─────────────────────────────────────────────────────────────┘
                            ↓
                Lista de comandos lista para ejecutar
```

## Fase 1: Split Input

### Archivo
`src/parsing/split_input.c`

### Responsabilidad
Dividir la entrada en tokens básicos respetando comillas.

### Características
- Maneja comillas simples `'...'` y dobles `"..."`
- Ignora espacios múltiples
- Detecta comillas sin cerrar
- Preserva espacios dentro de comillas

### Ejemplo
```
Input:  echo    "hello world"   'test'
Output: ["echo", "hello world", "test"]
```

### Estructura de Datos
```c
typedef struct s_split_ctx {
    char   **tokens;   // Array de strings
    size_t count;      // Número de tokens
    size_t capacity;   // Capacidad del array
    char   quote_error; // Tipo de comilla sin cerrar (0 si OK)
} t_split_ctx;
```

### Validaciones
- ✅ Comillas sin cerrar → error con tipo específico
- ✅ Preserva contenido literal dentro de comillas
- ✅ Memoria dinámica con crecimiento automático

## Fase 2: Tokenize

### Archivo
`src/parsing/tokenizer.c`

### Responsabilidad
Clasificar cada token según su tipo.

### Tipos de Token
```c
typedef enum e_token_type {
    TOKEN_WORD,          // Palabra normal o argumento
    TOKEN_PIPE,          // |
    TOKEN_REDIR_IN,      // <
    TOKEN_REDIR_OUT,     // >
    TOKEN_REDIR_APPEND,  // >>
    TOKEN_HEREDOC        // <<
} t_token_type;
```

### Ejemplo
```
Input:  ["cat", "<", "input.txt", "|", "grep", "test"]
Output: [WORD:"cat", REDIR_IN, WORD:"input.txt",
         PIPE, WORD:"grep", WORD:"test"]
```

### Validaciones
- ✅ Pipe al inicio → error
- ✅ Pipe al final → error
- ✅ Pipes consecutivos → error
- ✅ Redirección sin archivo → error

## Fase 3: Expand

### Archivos
- `src/parsing/expand.c`
- `src/parsing/expand_utils.c`

### Responsabilidad
Expandir variables de entorno respetando el contexto de comillas.

### Reglas de Expansión
1. **Comillas simples**: NO expande
   - `'$USER'` → `"$USER"` (literal)

2. **Comillas dobles**: SÍ expande
   - `"$USER"` → `"german"` (expandido)

3. **Sin comillas**: SÍ expande
   - `$USER` → `german` (expandido)

4. **Exit status**: `$?` expande al último código de salida

### Ejemplo Completo
```
Entorno: USER=german
Input:  echo '$USER' "$USER" $USER $?
                ↓ (no expande)
                ↓       ↓ (expande)
                ↓       ↓     ↓ (expande)
                ↓       ↓     ↓   ↓ (expande a 0)
Output: echo $USER german german 0
```

### Implementación
```c
void expand_tokens(t_token *tokens, char **envp, int exit_status)
{
    // Recorre cada token
    // Si es WORD, procesa carácter por carácter
    // Detecta $ y expande según el contexto de comillas
    // Reemplaza el valor del token con el resultado
}
```

## Fase 4: Build Commands

### Archivo
`src/parsing/command_builder.c`

### Responsabilidad
Agrupar tokens en comandos con sus argumentos y redirecciones.

### Estructura de Comando
```c
typedef struct s_cmd {
    char         **argv;    // Argumentos (argv[0] = comando)
    size_t       argc;      // Número de argumentos
    t_redir      *redirs;   // Lista de redirecciones
    struct s_cmd *next;     // Siguiente comando (pipe)
} t_cmd;

typedef struct s_redir {
    t_token_type  type;     // Tipo de redirección
    char          *target;  // Archivo destino
    struct s_redir *next;   // Siguiente redirección
} t_redir;
```

### Ejemplo
```
Input:  [WORD:"cat", REDIR_IN, WORD:"in.txt", PIPE,
         WORD:"grep", WORD:"test", REDIR_OUT, WORD:"out.txt"]

Output:
    cmd1 {
        argv: ["cat"],
        argc: 1,
        redirs: [IN:"in.txt"],
        next: cmd2
    }
    cmd2 {
        argv: ["grep", "test"],
        argc: 2,
        redirs: [OUT:"out.txt"],
        next: NULL
    }
```

## Validaciones Globales

### Archivo
`src/parsing/validate.c`

### Caracteres Especiales No Soportados
- `;` (semicolon)
- `\` (backslash fuera de comillas)
- `&` (ampersand)

Estos caracteres son detectados ANTES del parsing principal y generan errores específicos.

### Mensajes de Error
Todos los mensajes siguen el formato de bash:
```
minishell: syntax error near unexpected token 'X'
minishell: unexpected EOF while looking for matching 'X'
minishell: 'X' is not supported
```

## Gestión de Memoria

### Funciones de Limpieza

1. **`token_clear(t_token **tokens)`**
   - Libera toda la lista de tokens
   - Libera cada `value` (string)
   - Libera cada nodo

2. **`cmd_clear(t_cmd **cmds)`**
   - Libera toda la lista de comandos
   - Libera `argv` (array de strings)
   - Libera todas las redirecciones
   - Libera cada nodo

3. **`free_resources(char *input, char **parts, char *original)`**
   - Libera input de readline
   - Libera array parts y sus strings
   - Libera original (strdup del input)

### Flujo de Limpieza
```
Al terminar cada iteración:
    1. cmd_clear(&cmds)      // Libera comandos
    2. token_clear(&tokens)  // Libera tokens
    3. free_resources(...)   // Libera input, parts, original

Al salir del programa:
    4. free_env(my_env)      // Libera entorno copiado
```

## Testing

### Casos de Prueba Cubiertos

#### 1. Tokenización Básica
- ✅ Comandos simples
- ✅ Múltiples espacios
- ✅ Comillas simples y dobles
- ✅ Comillas anidadas

#### 2. Pipes y Redirecciones
- ✅ Pipe simple
- ✅ Múltiples pipes
- ✅ Todas las redirecciones (<, >, >>, <<)
- ✅ Múltiples redirecciones en un comando

#### 3. Expansiones
- ✅ Variables de entorno
- ✅ Exit status ($?)
- ✅ Comillas simples (no expande)
- ✅ Comillas dobles (expande)
- ✅ Variables sin comillas (expande)

#### 4. Validaciones
- ✅ Comillas sin cerrar
- ✅ Pipes mal colocados
- ✅ Redirecciones sin archivo
- ✅ Caracteres no soportados

#### 5. Casos Complejos
- ✅ `cat < in.txt | grep "test" | sort > out.txt`
- ✅ `echo "User: $USER Status: $?"`
- ✅ `ls -la | grep ".txt" | wc -l`

## Puntos Fuertes del Parser

1. **Modular**: Cada fase es independiente y testeable
2. **Robusto**: Validaciones exhaustivas en cada fase
3. **Eficiente**: Lista enlazada permite pipes ilimitados
4. **Sin Memory Leaks**: Funciones de limpieza completas
5. **Cumple Norminette**: 67 archivos OK
6. **Compatible con Bash**: Mensajes de error idénticos

## Preguntas Frecuentes en la Defensa

### ¿Por qué 4 fases?
Separación de responsabilidades. Cada fase resuelve un problema específico, facilitando debugging y mantenimiento.

### ¿Cómo manejan las comillas?
Split_input mantiene un estado de comilla activa. Dentro de comillas, todos los caracteres se tratan como literales.

### ¿Y las expansiones dentro de comillas dobles?
Expand_tokens recibe el token CON comillas, las procesa para saber el modo (simple/doble/sin), y expande según el modo.

### ¿Cómo saben si un pipe está mal colocado?
Tokenizer verifica que:
- El token anterior no sea NULL (pipe al inicio)
- El token siguiente no sea NULL (pipe al final)
- El token siguiente no sea otro PIPE (pipes consecutivos)

### ¿Qué pasa con los memory leaks?
Cada estructura tiene su función de limpieza (token_clear, cmd_clear, free_env). El main llama a estas funciones después de usar las estructuras.

### ¿Cómo prueban el parser?
- Tests unitarios por fase
- Tests de integración con casos complejos
- Verificación con valgrind/leaks (macOS)
- Comparación de outputs con bash real

## Archivos del Parser

```
src/parsing/
├── split_input.c       # Fase 1: Split por espacios/comillas
├── tokenizer.c         # Fase 2: Clasificar tokens
├── token_list.c        # Operaciones de lista de tokens
├── token_utils.c       # Utilidades de tokens
├── expand.c            # Fase 3: Expandir variables
├── expand_utils.c      # Helpers de expansión
├── command_builder.c   # Fase 4: Construir comandos
├── command_utils.c     # Limpieza de comandos
└── validate.c          # Validar caracteres especiales
```

## Estado Actual

✅ **Parsing Completo**
✅ **Todas las validaciones**
✅ **Expansiones funcionando**
✅ **Sin memory leaks**
✅ **Norminette OK**
✅ **Tests pasando**

**Próximo paso**: Implementar ejecución de comandos
