# Correcciones de Norminette

## Resumen
Se corrigieron todos los errores de norminette en el proyecto (67 archivos OK, 0 errores).

## Cambios por Categoría

### 1. Headers (2 archivos)
- **minishell.h**: Correcciones de alineación de structs
- **minishell2.h**: Correcciones de indentación de defines y prototipos

### 2. Parsing (9 archivos)
- **split_input.c**: Refactorización de funciones largas
- **command_builder.c**: Reducción de líneas y variables
- **expand.c**: División en expand.c y expand_utils.c (TOO_MANY_FUNCS)
- Todos los demás archivos de parsing pasaban norminette

### 3. Builtins (13 archivos)
- **BRACE_SHOULD_EOL**: Agregado newline después de closing brace en 10 archivos:
  - builtin.c, cd.c, createdup.c, echo.c, env.c
  - envutils.c, envutils2.c, export.c, pwd.c, unset.c

- **exit.c**:
  - Eliminada línea vacía extra (CONSECUTIVE_NEWLINES)
  - Agregado newline después de closing brace

- **utilits.c**:
  - Dividido en utilits.c y utilits2.c (TOO_MANY_FUNCS)
  - Movida función `straddchar` a utilits2.c

### 4. Main y Utilities (3 archivos)
- **main.c**: Refactorización completa
  - Reducción de 7 variables a 2 (máximo: 5)
  - Reducción de 120 líneas a <25 por función
  - Funciones extraídas: init_env, process_input, execute_cmd, process_loop
  - Funciones movidas a utils.c: print_env, free_resources

- **utils.c**:
  - Corregido espacio antes de nombre de función (SPACE_BEFORE_FUNC)
  - Agregadas funciones: print_env, free_resources

- **ft_strmapi.c**:
  - Eliminados comentarios de código
  - Corregido VAR_DECL_START_FUNC

## Archivos Nuevos
- `src/builtins/utilits2.c`: Contiene función straddchar movida desde utilits.c

## Funciones Agregadas a Headers
- `minishell.h`:
  - `void print_env(char **my_env);`
  - `void free_resources(char *input, char **parts, char *original);`

## Estado Final
- **Antes**: 20 archivos con errores de norminette
- **Después**: 0 archivos con errores
- **Compilación**: ✅ Exitosa
- **Norminette**: ✅ 67 archivos OK

## Notas
- La variable global `g_core` en minishell2.h genera un notice (esperado y aceptable)
- Los builtins tienen funciones faltantes (`str_compare`, `compare_metachars`) y nombres inconsistentes que necesitan corrección futura
- Los builtins no están incluidos en el Makefile por ahora debido a las funciones faltantes
