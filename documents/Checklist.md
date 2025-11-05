# Checklist Minishell

## Prompt & Cronologia
- [x] Mostrare un prompt fisso `minishell$ `
- [x] Usare `readline()` per l'input
- [x] Salvare la cronologia con `add_history()`

## Esecuzione
- [x] Risolvere i percorsi tramite `$PATH`
- [x] Eseguire i binari con `fork` + `execve`
- [x] Gestire percorsi assoluti/relativi

## Parsing
- [x] Gestire `'single quotes'` (nessuna espansione)
- [x] Gestire `"double quotes"` (espansione di `$VAR`, `$?`)
- [x] Riconoscere redirezioni `<`, `>`, `>>`, `<<`
- [x] Implementare le pipe `|` (esecuzione)
- [x] Implementare espansione di `$VAR`
- [x] Implementare espansione di `$?`
- [ ] Gestire i segnali nel parser (Ctrl-C, Ctrl-D, Ctrl-\)

## Built-in
- [x] `echo -n`
- [x] `cd` (percorso assoluto o relativo)
- [x] `pwd`
- [x] `export`
- [x] `unset`
- [x] `env`
- [x] `exit`

## Segnali
- [ ] Ctrl-C → nuova linea e nuovo prompt
- [ ] Ctrl-D → uscita
- [ ] Ctrl-\ → non fare nulla

## Redirezioni
- [x] `<` redirige stdin
- [x] `>` redirige stdout (truncate)
- [x] `>>` redirige stdout (append)
- [x] `<<` heredoc fino al delimitatore

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
