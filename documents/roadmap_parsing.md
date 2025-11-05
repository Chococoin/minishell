# 🗺️ Roadmap Parsing Minishell

Este roadmap está enfocado solo en la parte de **Parsing**, dividido en tareas diarias
para completarlo en 7-10 días. Se basa en el subject de Minishell y en la checklist extendida.

---

## 📅 Día 1–2: Tokenización básica
- [x] Separar la línea de entrada en tokens por espacios.
- [x] Ignorar espacios múltiples consecutivos.
- [x] Manejar comillas simples y dobles para no romper tokens.
- [x] Probar con inputs simples: `echo hola`, `echo "hola mundo"`.

## 📅 Día 3: Clasificación de tokens
- [x] Identificar tipos de token: WORD, PIPE, REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC.
- [x] Construir una lista de tokens con su tipo asociado.
- [x] Detectar errores básicos: `| |`, `>`, `>>` o `<` sin archivo después.

## 📅 Día 4–5: Construcción de comandos
- [x] Agrupar tokens en estructuras de comando.
- [x] Guardar nombre y argumentos en cada comando.
- [x] Asociar redirecciones con cada comando.
- [x] Conectar comandos con una lista enlazada para manejar pipes.
- [x] Probar con inputs: `ls -l | grep txt`, `cat < file.txt > out.txt`.

## 📅 Día 6–7: Expansiones
- [x] Implementar expansión de `$VAR` usando el entorno.
- [x] Implementar expansión de `$?` para el último exit status.
- [x] Respetar las reglas de comillas: `' '` no expande, `" "` sí expande.
- [x] Probar con: `echo $USER`, `echo "$USER"`, `echo '$USER'`.

## 📅 Día 8: Validaciones
- [x] Detectar comillas no cerradas.
- [x] Validar pipes al inicio o al final de la línea.
- [x] Validar redirecciones sin archivo.
- [x] Preparar mensajes de error (similares a Bash).
- [x] Detectar caracteres especiales no soportados (;, \, &).

## 📅 Día 9: Integración con ejecución (simulada)
- [x] Asegurarse de que el parser devuelva una lista enlazada limpia.
- [x] Simular el recorrido de la lista enlazada sin ejecutar nada real.
- [x] Probar con múltiples pipes y redirecciones combinadas.

## 📅 Día 10: Refactor y pruebas finales
- [x] Revisar fugas de memoria (valgrind).
- [x] Limpiar y dividir el código en funciones pequeñas.
- [x] Documentar cada etapa del parser para la defensa.
- [x] Preparar ejemplos explicativos para mostrar al evaluador.

---

## ✅ Resultado esperado
Al final del roadmap deberías tener un **parser funcional** que:
- Convierte cualquier input válido en una lista enlazada de comandos.
- Maneja correctamente comillas, pipes, redirecciones y expansiones.
- Detecta y reporta errores de sintaxis.
- Devuelve estructuras limpias listas para la fase de ejecución.
