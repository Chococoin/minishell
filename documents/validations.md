# Validaciones Implementadas

## Validaciones de Comillas
- Detecta comillas sin cerrar (` ' " `)
- Mensaje de error muestra el tipo específico de comilla
- Ejemplo: `unexpected EOF while looking for matching `"`

## Validaciones de Pipes
- Pipe al inicio de línea: `| echo` → error
- Pipe al final de línea: `echo |` → error
- Pipes consecutivos: `echo | |` → error
- Pipe después de redirección: `echo > | grep` → error

## Validaciones de Redirecciones
- Redirección sin archivo destino: `echo >` → error
- Redirección seguida de pipe: `echo > | grep` → error

## Caracteres Especiales No Soportados
- `;` (semicolon) - no soportado por el subject
- `\` (backslash) - no soportado por el subject
- `&` (ampersand) - no soportado por el subject
- Solo se validan fuera de comillas (dentro de comillas son literales)

## Mensajes de Error
Todos los mensajes siguen el formato de bash:
- `minishell: syntax error near unexpected token 'X'`
- `minishell: unexpected EOF while looking for matching 'X'`
