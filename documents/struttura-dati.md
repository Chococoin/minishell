# Struttura dati per Minishell

## Obiettivo
Dividere il lavoro in due parti:
- **Parsing (Germán)** → leggere la riga di comando e trasformarla in una struttura di dati organizzata.
- **Esecuzione (Sithmi)** → prendere quella struttura e lanciare i processi corretti, applicando pipe, redirezioni e built-in.

---

## Struttura concettuale

Il parser riceve una riga come:

cat input.txt | grep “hola” | wc -l > result.txt

e la trasforma in una **lista di comandi collegati** (lista collegata).

Ogni nodo della lista contiene:
- `nome` → il nome del comando (`cat`, `grep`, `wc`…)
- `args` → lista degli argomenti del comando
- `redirezioni` → elenco delle redirezioni di input/output
- `siguiente` → puntatore al comando successivo (se c’è un pipe)

---

## Esempio in forma simulata (C-like)

```c
// Nodo 1
Comando 1:
  nome          = "cat"
  args          = ["cat", "input.txt", NULL]
  redirezioni   = []
  siguiente     -> Comando 2

// Nodo 2
Comando 2:
  nome          = "grep"
  args          = ["grep", "hola", NULL]
  redirezioni   = []
  siguiente     -> Comando 3

// Nodo 3
Comando 3:
  nome          = "wc"
  args          = ["wc", "-l", NULL]
  redirezioni   = [ {tipo = OUTPUT_TRUNC, file = "result.txt"} ]
  siguiente     = NULL