#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#ifndef AST_H
#define AST_H



typedef enum {
 nt_S
,nt_ALGO
,nt_ALGOLIST

/* list des parametre dans une declaration de 
funct exmpl : puiss(a,b) ici a et b sont des t_IDF_PARAM_FUNCTION */
,nt_ALL_IDF_PARAM_FUNCTION 

,nt_ALL_COMMANDS
,nt_COMMAND
,nt_SET_COMMAND
,nt_INCR_COMMAND
,nt_DECR_COMMAND
,nt_OUT_COMMAND
,nt_DOFORI_COMMAND
,nt_DOWHILE_COMMAND
,nt_RETURN_COMMAND
,nt_IF_COMMAND
,nt_IF_ELSE_COMMAND
,nt_ALL_CALLS
,nt_CALL_FUNC
,nt_ALL_PARAMS
,nt_PARAMS
,nt_EXPR

,t_IDF_PARAM_FUNCTION
,t_IDF
,t_ADD
,t_SUB
,t_MULT
,t_DIV
,t_AND
,t_OR
,t_DIF
,t_EGAL
,t_NOT
,t_NUM
,t_FALSE
,t_TRUE
}noeud_t;


typedef struct non_terminal {
    char id[50];
    noeud_t noeud;
    struct non_terminal* kids[3];
} non_terminal_t;


//************************************************************************************************
//****************************** PILE DE PARSING *************************************************

typedef struct pile_parsing {
    char id[50];
    noeud_t noeud;
    struct pile_parsing* prec;
} pile_parsing_t;

pile_parsing_t* init_pile_parsing();
pile_parsing_t* push_pile_parsing(pile_parsing_t* p , const char *id , noeud_t noeud);
pile_parsing_t* pop_pile_parsing(pile_parsing_t* p);
void parcourir_pile(pile_parsing_t* p);



// debuging tools 
const char* get_node_name(noeud_t noeud) ;

// non_terminal_t* init_tree(){
//     non_terminal_t* tree = (non_terminal_t*)malloc(sizeof(non_terminal_t));
//     tree->noeud = nt_S;
//     tree->kids[0] = NULL;
//     tree->kids[1] = NULL;
//     tree->kids[2] = NULL;

//     return tree;
// }

// non_terminal_t* add_node(non_terminal_t *nt , char id[50] , int index, noeud_t noeud){
    
//     // Initialize a new non_terminal element
//     non_terminal_t *new_node = (non_terminal_t*)malloc(sizeof(non_terminal_t));
//     if (new_node == NULL) {
//         // Handle memory allocation failure
//         printf("Memory allocation failed.\n");
//         return;
//     }

//     new_node->noeud = noeud;
//     strcpy(new_node->id, id);
    
//     new_node->kids[0] = NULL;
//     new_node->kids[1] = NULL;
//     new_node->kids[2] = NULL;
    
//     nt->kids[index] = new_node;

//     return new_node;
// }




#endif