## Links to utils resources

https://www.gnu.org/savannah-checkouts/gnu/bash/manual/
https://www.gnu.org/savannah-checkouts/gnu/bash/manual/html_node/Bourne-Shell-Builtins.html

# PROMPT_SIGNALS

# PROMPT & SIGNALS

**Tecnico:**

- Prompt gestito con `readline`.

- Segnali principali:

- Ctrl-C → SIGINT: nuova linea, nuovo prompt.

- Ctrl-D → EOF: chiude shell.

- Ctrl-\ → SIGQUIT: ignorato.

**📝 Spicciole:**

Il prompt è come il **campanello** della shell che ti dice “sono pronta”. I segnali sono come gesti che fai col telecomando: STOP (Ctrl-C), SPEGNI (Ctrl-D), o URLO IGNORATO (Ctrl-\).

**👉 Esempio pratico (C):**

```c
void	sigint_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);         // cancella l’input corrente
	write(1, "\n", 1);              // stampa newline
	rl_on_new_line();               // aggiorna posizione cursore
	rl_redisplay();                 // stampa nuovo prompt
}

void	setup_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);   // gestisce Ctrl-C
	signal(SIGQUIT, SIG_IGN);       /* // ignora Ctrl-\\ */
}
```
# ENV_MANAGER

<aside>
<img src="/icons/folder_gray.svg" alt="/icons/folder_gray.svg" width="40px" />

## 1) Cos’è l’“environment” (e come vive tra processi)

- **`environ`**: puntatore globale al tuo zainetto (`char **`v).
- **`envp`**: lo **zaino che consegni** al nuovo programma in `execve`.
- **Exported**: variabile marcata per **entrare nello zaino** dei figli.
- **Identificatore valido**: `[A-Za-z_][A-Za-z0-9_]*` (mai `=` nel nome, non iniziare con cifra).
- **Vuota vs Unset**: `X=` (esiste, valore **vuoto**) **≠** `unset X` (non esiste).
- **`PWD` / `OLDPWD`**: indirizzo corrente e precedente; `cd` li aggiorna.

**📝 Spicciole:**

- `export` = **metto un biglietto in prima fila** (esportato ai figli).

- `unset` = **strappo il biglietto**.

- `X=` = biglietto **vuoto** ma c’è.

- `unset X` = biglietto proprio **assente**.

- L’**environment** di un processo è un **array** di stringhe `KEY=VALUE` terminato da `NULL`. In C è esposto come `extern char **environ`.

- **Ambiente = elenco di foglietti** `NOME=VALORE` che ogni **processo** riceve quando nasce.
- Esempi: `HOME=/home/vale`, `PATH=/usr/bin:/bin`, `PWD=/home/vale/proj`.
- Quando lanci un nuovo programma (con `execve`), gli passi anche **l’array** `envp` (tutti i foglietti). Se non glieli passi, nasce “senza post‑it”.
</aside>

<aside>
<img src="/icons/folder_gray.svg" alt="/icons/folder_gray.svg" width="40px" />

## 2) Perché ti serve nella tua parte (parser/expander)

- **Expander**: quando vede `$HOME`, `$PATH`, `$?` → deve sostituire col valore preso dal tuo **env manager** (più lo `last_status` per `$?`).
- **Built‑ins** come `export/unset` modificano il tuo env; Antonio per `execve` ti chiederà `char **envp` aggiornato.
</aside>

<aside>
💡

## 3) Come lo rappresentiamo (struttura dati semplice)

```c
typedef struct s_env {
char *key; // es. "HOME"
char *val; // es. "/home/vale" (può essere "", ma non NULL)
struct s_env *next; // lista collegata
} t_env;
```

**Perché lista?** Facile da inserire/rimuovere, niente realloc continui. Per `envp` creerai **al volo** un array `char **` quando serve.

**Invarianti**:

- `key` mai NULL, non vuota e **valida** (vedi regole nome più sotto).
- `val` mai NULL (se non definita, la variabile **non esiste**; se definita a vuoto, `val=""`).
</aside>

<aside>
💡

## 4) Regole dei **nomi** (identificatori) — importantissime

- Primo carattere: **lettera** `A..Z a..z` o **underscore** `_`.
- Seguono: lettere, cifre, underscore. **Niente spazi, niente , niente `?`, niente `.`**.
- Esempi validi: `USER`, `_X`, `JAVA_HOME`, `var2`.
- Esempi **invalidi**: `2X`, `MY-NAME`, `A?B`, `PATH:`, `a b`.
- (Extra opzionale stile bash) `VAR+=value` significa **append** al valore esistente. Non è richiesto nel mandatory, puoi ignorarlo.

Se il nome è invalido:

- `export not+valid` → stampa errore: `minishell: export: \'not+valid\': not a valid identifier` e ritorna **1** (non crea nulla).
- `unset not+valid` → stesso tipo di errore/exit code.
</aside>

<aside>
💡

## 5) Operazioni core (API che devi offrire)

```c
// Carica l’ambiente iniziale copiando 'environ'
t_env *env_from_environ(char **environ);

// Lettura: cerca per chiave (O(n))
char *env_get(t_env *env, const char *key); // NULL se assente

// Scrittura: crea o aggiorna\ nint env_set(t_env **env, const char *key, const char *val); // duplica key/val

// Rimozione: se esiste, elimina la variabile
int env_unset(t_env **env, const char *key);

// Converte a char ** pronto per execve (KEY=VALUE, ... , NULL)
char **env_to_envp(t_env *env); // alloca nuovo array

// Libera tutto
void env_free(t_env *env);
```

**Politica memoria**:

- `env_from_environ` duplica **sempre** stringhe (mai puntare al buffer di `environ`).
- `env_to_envp` crea un **nuovo** array che Antonio libererà dopo l’`execve`.
- `env_set`/`env_unset` mantengono la lista sempre **consistente**.
</aside>

<aside>
💡

## 6) Mini‑workflow tipico

1. All’avvio: `env = env_from_environ(environ);`
2. Prompt → utente digita comandi.
3. Il tuo **expander** fa domande a `env_get` quando vede `$FOO`.
4. `export KEY=VAL` → chiama `env_set(&env, "KEY", "VAL");`
5. `unset KEY` → `env_unset(&env, "KEY");`
6. Prima di eseguire, Antonio chiama `env_to_envp(env)` per avere l’array.
</aside>

<aside>
<img src="/icons/folder_gray.svg" alt="/icons/folder_gray.svg" width="40px" />

## 7) Variabili importanti (che vedrai spesso)

- `PATH`: lista di cartelle separate da `:` dove cercare i programmi (`/usr/bin:/bin:...`).
- `HOME`: cartella home dell’utente (usata da `cd` senza argomenti).
- `PWD` / `OLDPWD`: directory corrente e precedente (aggiornate da `cd`).
- `SHLVL`: livello della shell (opzionale incrementarlo all’avvio: se `SHLVL=2` → metti `3`).
- `_`: in bash è "ultimo argomento o comando eseguito"; **fuori scope** mandatory.
- **Speciale** `$?`: **non** è una vera variabile d’ambiente. È un numero gestito dalla shell (mantieni `last_status`).
</aside>

<aside>
<img src="/icons/folder_gray.svg" alt="/icons/folder_gray.svg" width="40px" />

## 8) `export`, `unset`, `env`: comportamento da replicare

### `export`

- **Senza argomenti**: stampa la lista **ordinata** per nome, formato stile bash:
    - `declare -x KEY="value"`
    - Se la variabile esiste **senza valore** (raro): `declare -x KEY`
- **Con argomenti** `KEY=VAL` o `KEY`:
    - `KEY=VAL` → crea/aggiorna e **esporta**.
    - `KEY` (senza `=`): marca come exported; se non esiste, in bash crea la variabile con **valore vuoto**. Puoi allinearti a questo.
- **Nome invalido** → errore e **exit code 1** (non interrompere l’intera shell).

### `unset`

- Rimuove la variabile dall’ambiente.
- Nome invalido → errore, **exit code 1**.
- Rimuovere `PATH`, `PWD`, `OLDPWD` è **permesso** (poi certi comandi/`cd -` si comporteranno di conseguenza).

### `env`

- **Senza argomenti**: stampa `KEY=VALUE` per ogni variabile **che ha un `=`** (cioè con un valore, anche vuoto `""` → si stampa `KEY=`).
- **Con argomenti/opzioni**: nel mandatory in genere **non richiesto**; puoi stampare errore del tipo `minishell: env: options not supported` con **exit 1**.
</aside>

<aside>
💼

## Modello dati consigliato (con invarianti)

```c
/* tipi per redirezioni */
typedef enum e_redir_type {
    REDIR_IN,      /* <  */
    REDIR_OUT,     /* >  */
    REDIR_APPEND,  /* >> */
    REDIR_HEREDOC  /* << */
}   t_redir_type;

/* nodo redirezione */
typedef struct s_redir {
    t_redir_type      type;
    char             *filename;  /* nome file o delimiter heredoc */
    int               expand;    /* heredoc: 1 espandi $, 0 no */
    struct s_redir   *next;
}   t_redir;

/* nodo comando */
typedef struct s_cmd {
    char          **argv;        /* ["cmd", "arg", NULL] */
    int             is_builtin;  /* 1 = builtin, 0 = altro */
    t_redir        *redirs;      /* lista redirezioni */
    struct s_cmd   *next;        /* prossimo comando della pipeline */
}   t_cmd;

```

**Invarianti importanti:**

- `key` unica (case-sensitive), **mai** con `=`.

- `value` sempre **non-NULL** (usa `""` per “vuota”).

- `exported` è `0/1`. Solo `1` va in `env` e in `envp`.

- Nessun **leak**: ogni `malloc` ha un `free` prevedibile.

**📝 Spicciole:**

Tieni una **rubrica** (lista) dei bigliettini. Ogni riga ha *nome*, *valore* e un **segno di spunta** (“questo va nello zaino dei figli?”).

## Creazione nuovo comando

```c
t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->is_builtin = 0;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}
```

## Riempire struct t_cmd

- **Sostituzione:** `execve(path, argv, envp)` **sostituisce** il processo corrente con il nuovo programma e gli **passa** l’ambiente tramite `envp` (array `KEY=VALUE`, `NULL`-terminato).

```c
static char	*ms_strdup(const char *s)
{
	size_t	n;
	char	*p;
	size_t	i;

	n = ft_strlen(s);
	p = (char *)malloc(n + 1);
	i = 0;
	if (!p)
		return (NULL);
	while (i < n)
	{
		p[i] = s[i];
		i++;
	}
	p[n] = '\0';
	return (p);
}

static size_t	ms_strv_len(char **v)
{
	size_t	n;

	n = 0;
	if (!v)
		return (0);
	while (v[n])
		n++;
	return (n);
}

int		cmd_set_argv(t_cmd *cmd, char **argv)
{
	size_t	n;
	size_t	i;
	size_t	j;
	char	**copy;

	if (!cmd)
		return (0);
	n = ms_strv_len(argv);
	copy = (char **)malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (0);
	i = 0;
	while (i < n)
	{
		copy[i] = ms_strdup(argv[i]);
		if (!copy[i])
		{
			while (i > 0)
				free(copy[--i]);
			free(copy);
			return (0);
		}
		i++;
	}
	copy[n] = NULL;
	if (cmd->argv)
	{
		j = 0;
		while (cmd->argv[j])
			free (cmd->argv[j++]);
		free (cmd->argv);
	}
	cmd->argv = copy;
	return (1);
}
```

</aside>

## 4) API del modulo e semantica precisa

```c
// init / destroyt_env *env_from_environ(char **environ);
// clona environ -> lista con exported=1void   env_free_all(t_env **head);
// querychar  *env_get(t_env *env, const char *key);
// ritorna value (stringa) o NULLint
env_is_exported(t_env *env, const char *key);
// mutate// exported: -1 = lascia com'è;
0/1 = forzaint
env_set(t_env **env, const char *key, const char *value, int exported);
int    env_unset(t_env **env, const char *key);
// stampaint    env_print_all(t_env *env);
// solo exported -> "KEY=VALUE\n"
int    export_print_sorted(t_env *env);
// stile bash: declare -x KEY="VAL"//
execvechar **env_to_envp(t_env *env);
// array `KEY=VALUE`, NULL-terminato//
utilsint    is_valid_identifier(const char *s);
// [A-Za-z_][A-Za-z0-9_]*
int    parse_kv(const char *s, char **k, char **v);
// split "KEY=VAL"
```

**Return convention suggerita:** `0` ok; `1` errore “logico” (es. identificatore invalido); `-1` errore di sistema (alloc fallita, `errno` settato).

**📝 Spicciole:**

Hai funzioni per **cercare**, **scrivere**, **strappare**, **stampare** i bigliettini e per **impacchettare lo zaino** dei figli.

---

## 5) Inizializzazione: clonare `environ`

```c
t_env *env_from_environ(char **environ) 
{    
		t_env *head = NULL;    
		for (size_t i = 0; environ && environ[i]; ++i) 
		{        
				char *eq = strchr(environ[i], '=');        
				if (!eq) 
						continue; 
				// robustezza: salta forme non standard        
				size_t klen = (size_t)(eq - environ[i]);        
				char *key = strndup(environ[i], klen);        
				char *val = strdup(eq + 1);        
				if (!is_valid_identifier(key)) 
				{ 
						free(key); 
						free(val); 
						continue; 
				}        
				env_set(&head, key, val, 1);        
				free(key); free(val);    
		}    
		// opzionale: normalizza SHLVL, inizializza PWD/OLDPWD se mancano    return head;
}
```

**📝 Spicciole:**

All’avvio, **copi lo zaino del tuo processo** nella tua rubrica interna.

---

## 6) Regole per i nomi (identificatori)

- Devono seguire `[A-Za-z_][A-Za-z0-9_]*`.
- **Mai** `=` nel nome.
- Se nome invalido: **non modificare** nulla, stampa errore in stile bash, status `1`.
Esempi invalidi: `2BAD`, `A-B`, `FOO.BAR`, `=X`.

**📝 Spicciole:**

Il **nome** del biglietto deve essere “pulito”: lettere/numeri/underscore e **non iniziare con numero**.

---

## 7) Builtins: comportamento atteso

### 7.1 `env`

- Stampa **solo** le variabili **exported** come `KEY=VALUE`, una per riga.
- Nel mandatory, con argomenti puoi restituire errore (status `1`).

### 7.2 `export`

- **Senza argomenti**: stampa le variabili **exported** in **ordine lessicografico**, formato bash‑like:
    
    ```
    declare -x KEY="VAL"
    declare -x EMPTY=""
    ```
    
- **Con `KEY=VAL`**: crea/aggiorna e **marca exported**.
- **Con solo `KEY`**: se non esiste → crea `KEY=""` e **marca exported**; se esiste → solo **marca exported**.
- Identificatore invalido → errore, **status `1`**, nessun cambiamento.

### 7.3 `unset`

- Rimuove la chiave (sia shell-var sia exported).
- Identificatore invalido → errore, status `1`.
- Chiave inesistente → ok (status `0`).

### 7.4 `cd` → `PWD` / `OLDPWD`

- Dopo `chdir(path)`:
    - `OLDPWD = vecchio PWD` (se esisteva),
    - `PWD = getcwd(...)` (valore fisico).
- **Consiglio**: mantienili **exported** per compatibilità.

**📝 Spicciole:**

- `env` = **leggi solo i biglietti spuntati**.

- `export` = **spuntare** (e creare/aggiornare il biglietto).

- `unset` = **strappare**.

- `cd` aggiorna il biglietto **indirizzo attuale** (PWD) e **precedente** (OLDPWD).

---

## 8) Conversione per `execve`: costruire `envp`

**Memoria:** libera **tutte** le stringhe `envp[i]` e l’array `envp` nel **padre** se `execve` fallisce.

```c
static size_t count_exported(t_env *env) 
{    
		size_t n=0; 
		for (; env; env=env->next) 
		if (env->exported) 
				n++; 
		return n;
}
char **env_to_envp(t_env *env) 
{    
		size_t n = count_exported(env);    
		char **envp = malloc(sizeof(char*) * (n + 1));    
		if (!envp) 
				return NULL;    
		size_t i = 0;    
		for (t_env *e = env; e; e = e->next) 
		{        
				if (!e->exported) 
						continue;        
				size_t len = strlen(e->key) + 1 + strlen(e->value); 
				// "K=V"        
				char *kv = malloc(len + 1);        
				if (!kv) 
				{ 
						/* free parziale... */ 
						return NULL; 
				}        
				sprintf(kv, "%s=%s", e->key, e->value);        
				envp[i++] = kv;
		}   
		envp[i] = NULL;    
		return envp;
}
```

**Ordine:** irrilevante (qualsiasi).

**📝 Spicciole:**

Per consegnare lo zaino al figlio: **impacchetta** solo i biglietti con la spunta in un **array** e dai quello a `execve`.

---

## 9) Stampa `export` ordinata + escaping dei valori

```c
static char *escape_export_value(const char *s) {    // produce una versione con \" e \\ per stampa: "declare -x KEY=\"VAL\""    size_t extra = 0;    for (const char *p=s; *p; ++p) if (*p=='"'||*p=='\\') extra++;    char *out = malloc(strlen(s)+extra+1);    if (!out) return NULL;    char *w=out;    for (const char *p=s; *p; ++p) {        if (*p=='"'||*p=='\\') *w++='\\';        *w++ = *p;    }    *w='\0'; return out;}int export_print_sorted(t_env *env) {    // 1) conta, 2) copia puntatori in array, 3) qsort per key, 4) stampa    // formato: declare -x KEY="VALUE"    return 0;}
```

**📝 Spicciole:**

Quando mostri i biglietti, **metti le virgolette** attorno al valore e **scappa** i caratteri “fastidiosi” (doppi apici e backslash) così l’output somiglia a bash.

---

## 10) Edge cases importanti

- **`KEY=` vs `unset KEY`**: esiste ma vuota **≠** non esiste.
- **Duplicati**: tieni **una sola** entry per `key`; `env_set` aggiorna.
- **Valori con spazi/virgolette**: ok; cura la **stampa** (`export`) con escaping.
- **Identificatori strani** (, `.`, `:`…) → rifiuta (status `1`).
- **Pipeline & builtins**: se `export`/`unset` stanno in **pipeline**, in molte shell l’effetto **non** torna nel padre (perché eseguiti in sottoprocesso). In Minishell: esegui nel **padre** solo quando **non** in pipeline; altrimenti nel figlio.
- **Locale/encoding**: i nomi mantienili “classici” ASCII; i valori possono contenere qualsiasi byte.
- **Sicurezza**: sappi che variabili come `LD_PRELOAD`, `PATH`, `IFS` influenzano il comportamento dei figli (non serve sanificarle nel mandatory, ma è bene esserne consapevoli).

**📝 Spicciole:**

Attento a non confondere **biglietto vuoto** con **biglietto mancante**; se metti un biglietto con spazi, **mostralo tra virgolette** quando lo stampi.

---

## 11) Implementazioni suggerite (frammenti pronti)

### 11.1 `is_valid_identifier`

```c
int is_valid_identifier(const char *s) {    if (!s || !*s) return 0;    if (!( (s[0]>='A'&&s[0]<='Z') || (s[0]>='a'&&s[0]<='z') || s[0]=='_' )) return 0;    for (const char *p=s+1; *p; ++p) {        if (!( ( *p>='A'&&*p<='Z') || ( *p>='a'&&*p<='z') || ( *p>='0'&&*p<='9') || *p=='_' ))            return 0;    }    return 1;}
```

### 11.2 `env_set` (crea o aggiorna; valore sempre non-NULL)

```c
int env_set(t_env **env, const char *key, const char *value, int exported) {    if (!is_valid_identifier(key)) return 1;    for (t_env *e=*env; e; e=e->next) {        if (strcmp(e->key, key)==0) {            char *nv = strdup(value ? value : "");            if (!nv) return -1;            free(e->value); e->value = nv;            if (exported != -1) e->exported = exported ? 1 : 0;            return 0;        }    }    t_env *node = malloc(sizeof(*node));    if (!node) return -1;    node->key = strdup(key);    node->value = strdup(value ? value : "");    if (!node->key || !node->value) { free(node->key); free(node->value); free(node); return -1; }    node->exported = (exported == -1) ? 0 : !!exported;    node->next = *env; *env = node;    return 0;}
```

### 11.3 `env_unset`

```c
int env_unset(t_env **env, const char *key) {    if (!is_valid_identifier(key)) return 1;    t_env *prev=NULL, *cur=*env;    while (cur) {        if (strcmp(cur->key, key)==0) {            if (prev) prev->next = cur->next; else *env = cur->next;            free(cur->key); free(cur->value); free(cur);            return 0;        }        prev = cur; cur = cur->next;    }    return 0;}
```

### 11.4 `cd` → aggiornare `PWD`/`OLDPWD`

```c
int bi_cd(char **argv, t_shell *sh) {    const char *path = argv[1] ? argv[1] : env_get(sh->env, "HOME");    if (!path) { fprintf(stderr, "minishell: cd: HOME not set\n"); return 1; }    char *old = NULL;
    char *pwd = env_get(sh->env, "PWD");    if (pwd) old = strdup(pwd);    if (chdir(path) < 0) { perror("cd"); free(old); return 1; }    char buf[4096];    if (!getcwd(buf, sizeof(buf))) { perror("getcwd"); free(old); return 1; }    if (old) env_set(&sh->env, "OLDPWD", old, 1);    env_set(&sh->env, "PWD", buf, 1);    free(old);    return 0;}
```

---

## 12) Esempi da tastiera (da provare in Bash, per vedere il comportamento)

```bash
export A=1 B= "" Cenv | grep -E '^(A|B|C)='# A=1# B=# C=unset A; env | grep '^A=' || echo "A non esiste"# A non esistecd /; echo "PWD=$PWD OLDPWD=$OLDPWD"
```

**📝 Spicciole:**

Guarda come `env` mostra solo i biglietti **spuntati**. `unset` li toglie. `cd` aggiorna gli indirizzi PWD/OLDPWD.

---

## 13) Performance & sicurezza

- **Complessità:** lista collegata → lookup O(n), stampa O(n log n) se ordini alla stampa (copia in array + `qsort`).
- **Threading:** Minishell è single‑thread → semplice.
- **Sicurezza:** non fidarti dell’input di `export/unset` (valida sempre). Evita `putenv` (lifetime pericoloso).

**📝 Spicciole:**

La rubrica è semplice e veloce **quanto basta** per una minishell; non complicarla con strutture complesse.

---

## 14) Micro‑roadmap “Definition of Done”

- [ ]  `env_from_environ` clona tutto con `exported=1`.
- [ ]  `env_get`, `env_set`, `env_unset` robusti (niente leak).
- [ ]  `export`: stampa ordinata stile bash con escaping corretto.
- [ ]  `env`: solo `KEY=VALUE` esportati.
- [ ]  `cd`: aggiorna `PWD/OLDPWD` coerentemente.
- [ ]  `env_to_envp`: array `NULL`‑terminato, gestione memoria solida.
- [ ]  Edge cases: `KEY=` vs `unset`, nomi invalidi, valori con spazi/quote.
- [ ]  Builtins in pipeline: mutazioni **solo** nel padre quando **non** in pipeline.

---

## 15) Mini‑demo C (leggere/settare/propagare env)

```c
#include <stdio.h>#include <stdlib.h>#include <unistd.h>#include <sys/wait.h>int main(void) {    printf("Prima: FOO=%s\n", getenv("FOO"));    setenv("FOO", "padre", 1);    pid_t pid = fork();    if (pid == 0) {        // FIGLIO: eredita FOO=padre        printf("[figlio] FOO=%s\n", getenv("FOO"));        // Cambia solo nel figlio        setenv("FOO", "figlio", 1);        printf("[figlio] FOO=%s (dopo set)\n", getenv("FOO"));        // Esegui un programma esterno con env propagato        char *argv[] = {"env", NULL};        execve("/usr/bin/env", argv, environ); // qui usiamo direttamente environ per demo        perror("execve"); _exit(127);    }    int st; waitpid(pid, &st, 0);    printf("[padre] FOO=%s (resta 'padre')\n", getenv("FOO"));    return 0;}
```

**📝 Spicciole:**

Il figlio **parte** con lo stesso biglietto (`FOO=padre`), ma se lo modifica **non** cambia quello del padre. E quando esegue `env`, passa il suo zaino al nuovo programma.

# EXPANDER

**Tecnico:**

Gestisce sostituzione `$VAR`, `$?`, quote. Regole:

- `'...'` → no espansioni

- `\"...\"` → espande `$`, niente splitting

- non quotato → espande + splitting

**📝 Spicciole:**

È come **sciogliere variabili**: al posto di $HOME scrivi il vero indirizzo di casa. Con le virgolette decidi se sciogliere o no.

**👉 Esempio pratico (bash):**

```bash
echo $HOMEecho "$HOME"echo '$HOME'
```

# TOKENIZZAZIONE (Lexer)

**Tecnico:**

Il lexer spezza la riga in token: parole, simboli (`|`, `<`, `>`, `>>`, `<<`), quote. Segnala errori (quote non chiuse).

**📝 Spicciole:**

È come **tagliare una frase in parole e segni di punteggiatura**. Ti prepara i mattoncini da cui poi costruisci la frase vera.

**👉 Esempio pratico (C semplificato):**

```c
#include <stdio.h>
#include <string.h>
int main(void)
{
    char line[] = "echo \"ciao mondo\" | wc -c";
    char *tok = strtok(line, " ");    while (tok) {        printf("Token: %s\n", tok);        tok = strtok(NULL, " ");    }}
```

# PARSING

**Tecnico:**

Trasforma i token in strutture dati (`t_cmd`, pipeline, redirections). Riconosce errori sintattici (pipe all’inizio, redirection senza argomento).

**📝 Spicciole:**

Il parsing è come **montare un mobile IKEA**: i pezzi (token) vanno assemblati secondo regole precise, altrimenti non regge.

**👉 Esempio pratico (pseudo):**

```
Input: "ls -l | grep .c > out.txt"

Lexer → [ls] [-l] [|] [grep] [.c] [>] [out.txt]
Parser → Pipeline: Cmd1(ls -l) → pipe → Cmd2(grep .c, redir > out.txt)
```
# Builtins

## Cosa sono?

Comandi interni alla shell.

## Terminologia

- **Builtin**: comando interno (es. `cd`).
- **Exit Status**: numero che indica l’esito.

## Implementazione

- echo -n
- pwd
- cd
- env
- export
- unset
- exit

## Checklist

- [ ]  Builtins implementati
- [ ]  Errori coerenti con Bash
- [ ]  Exit status ok

## Cos’è l’Executor?

Esegue i comandi del parser con pipe, redirections e builtins.

```c

```

## Terminologia

- **fork**: crea nuovo processo.
- **execve**: lancia nuovo programma.
- **dup2**: duplica fd.
- **waitpid**: attende un figlio.

## Implementazione

- PATH resolution.
- Redirections con `dup2`.
- Pipe con `pipe`.
- Attendere figli con `waitpid`.
- Aggiornare `$?`.

## Checklist

- [ ]  PATH risolto
- [ ]  Redirections ok
- [ ]  Pipeline ok
- [ ]  Exit status corretto

# Gestione Memoria

## Cos’è?

Gestione risorse per evitare leaks e fd aperti.

## Terminologia

- **Memory Leak**: memoria non liberata.
- **File Descriptor (fd)**: identificatore numerico di file/pipe.

## Implementazione

- Free tokens, AST, env ogni ciclo.
- Chiudere fd non usati.
- Test con Valgrind.

## Checklist

- [ ]  Nessun leak
- [ ]  Nessun fd fantasma