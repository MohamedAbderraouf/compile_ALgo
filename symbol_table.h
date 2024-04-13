#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


#define HASH_SIZE 50

typedef enum {
    LOCAL_VAR,
    PARAM_VAR
} TYPE_VAR;


typedef enum {
    NUM_T,
    BOOL_T,
    NOT_INITIALIZED,
    ERR_T
} TYPE_SYNTH;

typedef enum {
    FALSE_E,
    TRUE_E
} boolean_t;


typedef struct func_sym
{
    char* nom_func;
    int nbr_params;
    int nbr_locals;
    struct sym *table;
    struct func_sym *ptr;
} func_tab;

typedef struct sym
{
    char *nom_idf;
    TYPE_VAR type;
    TYPE_SYNTH type_synth;
    int num_var;
    struct sym *ptr;
} sym_tab;

typedef struct f_hlist {
    
  func_tab* hash_list[HASH_SIZE];

} functions_hash_list;



/**
 * @brief Initialise la structure functions_hash_list.
 * 
 * Cette fonction initialise la structure functions_hash_list en allouant de la mémoire et en initialisant le tableau hash_list à NULL.
 * 
 * @param ts Un pointeur vers un pointeur de la structure functions_hash_list à initialiser.
 */
void hlist_init(functions_hash_list **ts);


/**
 * @brief Ajoute une fonction à la liste de hachage des fonctions.
 * 
 * Cette fonction ajoute une fonction à la liste de hachage des fonctions, en utilisant le nom de la fonction, le nombre de paramètres et le nombre de variables locales fournis.
 * 
 * @param ts Un pointeur vers un pointeur de la structure functions_hash_list où la fonction doit être ajoutée.
 * @param nom_func Le nom de la fonction à ajouter.
 * @param nbr_params Le nombre de paramètres de la fonction.
 * @param nbr_locals Le nombre de variables locales de la fonction.
 * 
 * @return 0 si la fonction est ajoutée avec succès, -1 en cas d'échec.
 */
int hlist_add_function(functions_hash_list **ts , char* nom_func , int nbr_params ,int nbr_locals);


/**
 * @brief Récupère une fonction à partir de la liste de hachage des fonctions.
 * 
 * Cette fonction recherche et renvoie un pointeur vers la fonction correspondant au nom de la fonction et au nombre de paramètres fournis.
 * 
 * @param ts Un pointeur vers la structure functions_hash_list à partir de laquelle la fonction doit être récupérée.
 * @param nom_func Le nom de la fonction à rechercher.
 * @param nbr_params Le nombre de paramètres de la fonction à rechercher.
 * 
 * @return Un pointeur vers la fonction correspondant au nom et au nombre de paramètres spécifiés, ou NULL si la fonction n'est pas trouvée.
 */
func_tab* hlist_get_function(functions_hash_list *ts ,const char* nom_func);

/**
 * @brief Ajoute une variable à la table des symboles d'une fonction.
 * 
 * Cette fonction ajoute une variable à la table des symboles d'une fonction spécifiée, en utilisant le nom de la variable, le type de la variable et le numéro de la variable fournis.
 * 
 * @param fonction Un pointeur vers un pointeur de la structure func_tab représentant la fonction à laquelle la variable doit être ajoutée.
 * @param nom_idf Le nom de la variable à ajouter.
 * @param type Le type de la variable à ajouter (LOCAL_VAR ou PARAM_VAR).
 * @param num_var Le numéro de la variable à ajouter.
 * 
 * @return 0 si la variable est ajoutée avec succès, -1 en cas d'échec.
 */
int function_add_var(func_tab **fonction, char *nom_idf, TYPE_VAR type, int num_var);


/**
 * @brief Récupère une variable à partir de la table des symboles d'une fonction.
 * 
 * Cette fonction recherche et renvoie un pointeur vers la variable correspondant au nom de la variable fourni dans la table des symboles de la fonction spécifiée.
 * 
 * @param fonction Un pointeur vers la structure func_tab représentant la fonction à partir de laquelle la variable doit être récupérée.
 * @param nom_idf Le nom de la variable à rechercher.
 * 
 * @return Un pointeur vers la variable correspondant au nom spécifié, ou NULL si la variable n'est pas trouvée.
 */
sym_tab* function_get_var(func_tab *fonction, char *nom_idf);


/**
 * @brief Libère la mémoire allouée pour une table de symboles.
 * 
 * Cette fonction libère la mémoire allouée pour une table de symboles, y compris la mémoire allouée pour chaque élément de la table.
 * 
 * @param table Un pointeur vers la structure sym_tab représentant la table de symboles à libérer.
 */
void free_sym_table(sym_tab* table);


/**
 * @brief Libère la mémoire allouée pour une table de symboles d'une fonction.
 * 
 * Cette fonction libère la mémoire allouée pour une table de symboles d'une fonction, y compris la mémoire allouée pour chaque élément de la table.
 * 
 * @param table Un pointeur vers la structure func_tab représentant la table de symboles de la fonction à libérer.
 */
void free_func_table(func_tab* table);

/**
 * @brief Libère la mémoire allouée pour toutes les listes de hachage de fonctions.
 * 
 * Cette fonction libère la mémoire allouée pour toutes les listes de hachage de fonctions, y compris la mémoire allouée pour chaque élément de chaque liste.
 * 
 * @param hash_lists Un pointeur vers la structure functions_hash_list représentant l'ensemble des listes de hachage de fonctions à libérer.
 */
void free_hash_lists(functions_hash_list* hash_lists);



int element_exists(sym_tab *head, char *nom_idf) ;



//////////////////////////////////// stack for intermediate code generation

#define MAX_SIZE 50

typedef struct {
    int items[MAX_SIZE];
    int top;
} Stack;

void initialize(Stack *stack);
int isFull(Stack *stack);
int isEmpty(Stack *stack);
void push(Stack *stack, int item);
int pop(Stack *stack);
int peek(Stack *stack);

void print_functions_hash_list(functions_hash_list *hash_list);



#endif