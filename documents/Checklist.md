# Checklist Minishell

## Prompt & Cronologia
- [ ] Mostrare un prompt fisso `minishell$ `
- [ ] Usare `readline()` per l’input
- [ ] Salvare la cronologia con `add_history()`

## Esecuzione
- [ ] Risolvere i percorsi tramite `$PATH`
- [ ] Eseguire i binari con `fork` + `execve`
- [ ] Gestire percorsi assoluti/relativi

## Parsing
- [ ] Gestire `'single quotes'` (nessuna espansione)
- [ ] Gestire `"double quotes"` (espansione di `$VAR`, `$?`)
- [ ] Riconoscere redirezioni `<`, `>`, `>>`, `<<`
- [ ] Implementare le pipe `|`
- [ ] Implementare espansione di `$VAR`
- [ ] Implementare espansione di `$?`
- [ ] Gestire i segnali nel parser (Ctrl-C, Ctrl-D, Ctrl-\)

## Built-in
- [ ] `echo -n`
- [ ] `cd` (percorso assoluto o relativo)
- [ ] `pwd`
- [ ] `export`
- [ ] `unset`
- [ ] `env`
- [ ] `exit`

## Segnali
- [ ] Ctrl-C → nuova linea e nuovo prompt
- [ ] Ctrl-D → uscita
- [ ] Ctrl-\ → non fare nulla

## Redirezioni
- [ ] `<` redirige stdin
- [ ] `>` redirige stdout (truncate)
- [ ] `>>` redirige stdout (append)
- [ ] `<<` heredoc fino al delimitatore

## Flusso Minishel

+----------------+
|   Utente       |
| (scrive input) |
+----------------+
        |
        v
+----------------+
|   Prompt       |
| (readline)     |
+----------------+
        |
        v
+----------------+
|   Parser       |   <-- 🔹 Germán
| - Tokenizza    |
| - Gestisce     |
|   quotes       |
| - Espande $VAR |
| - Riconosce    |
|   redirezioni  |
| - Gestisce |   |
+----------------+
        |
        | comandi strutturati
        v
+----------------+
|   Esecutore    |   <-- 🔹 Sihtmi
| - Controlla se |
|   è built-in   |
| - Se built-in: |
|   esegue       |
| - Se esterno:  |
|   fork+execve  |
| - Gestisce $?  |
+----------------+
        |
        v
+----------------+
|   Output       |
| (stdout/stderr)|
+----------------+
        |
        v
+----------------+
|   Prompt di    |
|   nuovo ciclo  |
+----------------+
