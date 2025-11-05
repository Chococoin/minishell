# Gestión de Memoria

## Funciones de Limpieza Implementadas

### 1. Tokens (`token_clear`)
**Ubicación**: `src/parsing/token_list.c`

Libera toda la lista enlazada de tokens:
- Libera cada `token->value` (string duplicado)
- Libera cada nodo `token`
- Recorre toda la lista enlazada

```c
void token_clear(t_token **tokens)
{
    t_token *tmp;

    while (*tokens)
    {
        tmp = (*tokens)->next;
        free((*tokens)->value);
        free(*tokens);
        *tokens = tmp;
    }
}
```

### 2. Comandos (`cmd_clear`)
**Ubicación**: `src/parsing/command_utils.c`

Libera toda la lista enlazada de comandos:
- Libera `argv` (array de strings)
- Libera cada redirección en la lista
- Libera cada nodo de comando
- Recorre toda la lista enlazada

```c
void cmd_clear(t_cmd **cmds)
{
    t_cmd *next;

    while (*cmds)
    {
        next = (*cmds)->next;
        free_argv((*cmds)->argv);
        free_redirs((*cmds)->redirs);
        free(*cmds);
        *cmds = next;
    }
}
```

### 3. Entorno (`free_env`)
**Ubicación**: `src/env/copy_env.c`

Libera la copia del entorno:
- Libera cada string del array
- Libera el array mismo

```c
void free_env(char **env)
{
    int i = 0;

    while (env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}
```

### 4. Recursos del Loop (`free_resources`)
**Ubicación**: `src/utils/utils.c`

Libera los recursos de cada iteración del loop principal:
- Libera array `parts` (resultado de split_input)
- Libera cada string individual de parts
- Libera `original` (copia del input)
- Libera `input` (readline)

```c
void free_resources(char *input, char **parts, char *original)
{
    int i;

    if (parts)
    {
        i = 0;
        while (parts[i])
        {
            free(parts[i]);
            i++;
        }
        free(parts);
    }
    if (original)
        free(original);
    if (input)
        free(input);
}
```

## Flujo de Memoria en el Loop Principal

### Ciclo de Vida de los Datos

```
readline() → input
    ↓
ft_strdup(input) → original
    ↓
split_input(input) → parts (array de strings)
    ↓
tokenize(parts) → tokens (lista enlazada)
    ↓
expand_tokens(tokens) → modifica tokens in-place
    ↓
commands_from_tokens(tokens) → cmds (lista enlazada)
    ↓
execute_cmd(cmds) → ejecución
    ↓
LIMPIEZA:
    cmd_clear(&cmds)
    token_clear(&tokens)
    free_resources(input, parts, original)
```

### Limpieza al Salir

Al terminar el programa (comando `exit` o Ctrl-D):
```c
free_env(my_env);
return (0);
```

## Memory Leaks Corregidos

### Leak #1: Array de `parts`
**Problema**: Solo se liberaba el array, no los strings individuales
**Solución**: Iterar sobre parts y liberar cada string antes de liberar el array

### Leak #2: Entorno `my_env`
**Problema**: El entorno copiado nunca se liberaba al salir
**Solución**: Agregar `free_env(my_env)` antes de retornar de main

### Leak #3: Redirecciones
**Problema**: No se liberaba `redir->target`
**Solución**: En `free_redirs`, liberar cada `target` antes del nodo

## Verificación de Memory Leaks

### Herramientas Disponibles

1. **macOS**: `leaks` (herramienta nativa)
2. **Linux**: `valgrind` (más detallado)

### Ejemplo de Uso (macOS)

```bash
# Ejecutar con MallocStackLogging para mejor diagnóstico
MallocStackLogging=1 ./minishell

# En otra terminal:
leaks minishell
```

### Ejemplo de Uso (Linux)

```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
```

## Checklist de Memoria

✅ **Tokens**: Liberados con `token_clear`
✅ **Comandos**: Liberados con `cmd_clear`
✅ **Array argv**: Liberado dentro de `cmd_clear`
✅ **Redirecciones**: Liberadas dentro de `cmd_clear`
✅ **Input readline**: Liberado en `free_resources`
✅ **Original (strdup)**: Liberado en `free_resources`
✅ **Parts (split)**: Liberado completamente en `free_resources`
✅ **Entorno**: Liberado con `free_env` al salir

## Notas de Implementación

### Readline y History
- `readline()` asigna memoria que debe ser liberada con `free()`
- `add_history()` hace su propia copia interna, no retiene el puntero
- Por lo tanto, es seguro liberar el input después de `add_history()`

### Structs con Listas Enlazadas
- Siempre recorrer toda la lista
- Guardar `next` antes de liberar el nodo actual
- Usar funciones de limpieza recursivas o iterativas

### Arrays de Strings
- Liberar primero cada string individual
- Luego liberar el array mismo
- Verificar NULL antes de liberar

## Próximos Pasos

1. ✅ Implementar funciones de limpieza
2. ✅ Corregir leaks identificados
3. ⏳ Verificar con valgrind en Linux (si disponible)
4. ⏳ Probar con casos edge (inputs muy largos, muchos pipes)
5. ⏳ Documentar para la defensa
