# Orden de Expansiones en una Shell

Cuando una shell (Bash o minishell) procesa una línea, aplica una serie de **expansiones** antes de ejecutar el comando. El orden es importante porque cada fase transforma el texto, y el resultado de una etapa se usa como entrada a la siguiente.

## 1. Eliminación de backslashes (en contextos sin comillas)
Se procesan los caracteres de escape `\` que preceden a metacaracteres permitidos en el contexto actual (p. ej. un literal escapado).

## 2. Expansión de tildes
`~`, `~user`, `~+`, `~-` se expanden a directorios (`$HOME`, home de `user`, `PWD`, `OLDPWD`). Ocurre únicamente al inicio de un token.

## 3. Expansión de parámetros y variables
`$VAR`, `${VAR}`, `${VAR:-default}` y similares se sustituyen por sus valores. Minishell debe manejar especialmente `$VAR` y `"$VAR"` junto con la variante especial `$?`.

## 4. Expansión de sustitución de comandos (si está permitida)
```
$(comando) → salida del comando
`comando`  → modo heredado (backticks)
```
Esta fase no es obligatoria en minishell, pero forma parte del flujo en Bash.

## 5. Expansión aritmética
`$(( expresion ))` evalúa operaciones numéricas (suma, resta, etc.) y se sustituye por el resultado. Tampoco es requerida en minishell según el subject.

## 6. Eliminación de comillas
Se quitan las comillas que quedaron tras las expansiones previas. Las comillas determinan qué expansiones se aplicaron:
- `'single quotes'` preservan el texto literal (sin expansiones).
- `"double quotes"` permiten `$VAR`, `$?`, `$(...)`, pero protegen espacios y la mayoría de metacaracteres.

## 7. Expansión de patrones (globbing)
Los patrones con `*`, `?`, `[]` se expanden a nombres de archivo que coincidan. Si el patrón no coincide con nada, Bash lo deja literal (dependiendo de opciones); minishell puede optar por no implementar globbing o replicar el comportamiento según el enunciado.

## 8. División en campos (field splitting)
Después de las expansiones anteriores (excepto cuando las comillas lo impiden), la shell divide los resultados en argumentos usando el separador definido por `IFS` (por defecto espacios, tabuladores y saltos de línea).

## 9. Eliminación de nombres nulos
Se descartan los campos que quedaron vacíos tras las expansiones (por ejemplo, si una variable sin valor no estaba entre comillas).

**Resumen clave para minishell:** implementar al menos la expansión de variables (`$VAR`), el código de salida (`$?`), respetar el efecto de comillas y decidir qué hacer con tildes y globbing según el alcance del proyecto. El orden anterior asegura que los tokens finales coincidan con el comportamiento de Bash.
