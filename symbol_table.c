#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"


sym_tab* function_get_var(func_tab *fonction, char *nom_idf) {
    // Vérifier si la fonction est valide
    if (fonction == NULL) {
        fprintf(stderr, "La fonction n'est pas valide\n");
        return NULL;
    }

    // Parcourir la table de symboles de la fonction
    sym_tab *ptr = fonction->table;
    while (ptr != NULL) {
        // Comparer le nom_idf avec celui du symbole actuel
        if (strcmp(ptr->nom_idf, nom_idf) == 0) {
            return ptr; // Retourner le symbole trouvé
        }
        ptr = ptr->ptr;
    }

    return NULL;
}


int function_add_var(func_tab **fonction, char *nom_idf, TYPE_VAR type, int num_var) {

    // Allouer de l'espace pour le nouveau symbole
    sym_tab *new_var = malloc(sizeof(sym_tab));
    if (new_var == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }


    sym_tab *ptr = (*fonction)->table;
    if (ptr != NULL)
    {
        while (ptr->ptr != NULL){
            if (strcmp(ptr->nom_idf , nom_idf) == 0)
                return -1;
            ptr = ptr->ptr;
        }
        
    }
    
    // Allouer de l'espace pour le nom_idf
    new_var->nom_idf = malloc(strlen(nom_idf) + 1);
    if (new_var->nom_idf == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(new_var->nom_idf, nom_idf);

    // Remplir les détails du symbole
    new_var->type = type;
    new_var->num_var = num_var;
    new_var->ptr = NULL; // Le nouveau symbole est ajouté à la fin de la liste

    // Si la table des symboles de la fonction est vide, ajouter le nouveau 
    // symbole comme premier élément
    if ((*fonction)->table == NULL) {
        (*fonction)->table = new_var;
    } else {
        // Sinon, parcourir la liste jusqu'à la fin et ajouter le nouveau symbole
        ptr->ptr = new_var;
    }
}








void hlist_init(functions_hash_list **ts){

    // Allocation memoire
    *ts = (functions_hash_list*)malloc(sizeof(functions_hash_list));
    
    // verif memory allocation
    if (ts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialisation de la ts a NULL
    for (int i = 0; i < HASH_SIZE; i++) {
        (*ts)->hash_list[i] = NULL;
    }
}


unsigned int hash_function(char* key) {
    unsigned int hash = 1;
    int len = strlen(key);

    for (int i = 0; i < len; i++) {
        hash = ((hash+1)*key[i]) % HASH_SIZE ;
    }

    return hash ;
}

int hlist_add_function(functions_hash_list **ts, char* nom_func, int nbr_params, int nbr_locals) {
    // Calculate position
    int position = hash_function(nom_func);
    
    // Pointer to the head of the list at the calculated position
    func_tab **add_ptr = &((*ts)->hash_list[position]); 

    // Traverse the list until the end
    // Check for duplicates with the same number of parameters
    while (*add_ptr != NULL) {
        // If this condition is true, then there is a duplicate
        if ((*add_ptr)->nbr_params == nbr_params && strcmp((*add_ptr)->nom_func, nom_func) == 0) {
            return -1; // Duplicate found
        }    
        add_ptr = &((*add_ptr)->ptr);
    }

    // Allocate memory for the new function 
    func_tab *fonction = malloc(sizeof(func_tab));
    if (fonction == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for nom_func
    fonction->nom_func = malloc(strlen(nom_func) + 1);
    if (fonction->nom_func == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(fonction); // Free previously allocated memory
        exit(EXIT_FAILURE);
    }
    strcpy(fonction->nom_func, nom_func);

    fonction->nbr_params = nbr_params;
    fonction->nbr_locals = nbr_locals;
    fonction->table = NULL;
    fonction->ptr = NULL;

    // Add the new function to the end of the list
    *add_ptr = fonction;

    return 0; // Success
}


func_tab* hlist_get_function(functions_hash_list *ts , char* nom_func , int nbr_params){
    // found || not found
    int status = -1;

    // calculate position
    int position = hash_function(nom_func);
    func_tab *add_ptr = ts->hash_list[position]; 
    
    // parcour jusque a la fin
    // verification d'existance

    while (add_ptr != NULL){
        // if cette condition est vrai alors il exist un duplicata
        if(add_ptr->nbr_params == nbr_params && strcmp(add_ptr->nom_func , nom_func) == 0){
            status = 0;
            break;
        }    
        add_ptr = add_ptr->ptr;
    }
    return status==0?add_ptr:NULL;
}



// Function to free a single symbol table
void free_sym_table(sym_tab* table) {
    while (table != NULL) {
        sym_tab* temp = table;
        table = table->ptr;
        free(temp->nom_idf);
        free(temp);
    }
}

// Function to free a single function table
void free_func_table(func_tab* table) {
    while (table != NULL) {
        func_tab* temp = table;
        table = table->ptr;
        free_sym_table(temp->table); // Free the symbol table associated with the function
        free(temp->nom_func);
        free(temp);
    }
}

// Function to free all hash lists in functions_hash_list
void free_hash_lists(functions_hash_list* hash_lists) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        if (hash_lists->hash_list[i] != NULL) {
            free_func_table(hash_lists->hash_list[i]);
        }
    }
}
