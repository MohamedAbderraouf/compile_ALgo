#include "ast.h"


pile_parsing_t* init_pile_parsing(){

    pile_parsing_t* p = (pile_parsing_t*)malloc(sizeof(pile_parsing_t));
    
    if (p==NULL){
        printf("Erreur allocation pile_parsing\n");
        exit(EXIT_FAILURE);
    }
    p->noeud = nt_S;
    p->prec = NULL;
    strcpy(p->id,"");

    return p;
}

pile_parsing_t* push_pile_parsing(pile_parsing_t* p ,const char *id , noeud_t noeud){

    pile_parsing_t* new_p = (pile_parsing_t*)malloc(sizeof(pile_parsing_t));
    if (p==NULL){
        printf("Erreur allocation pile_parsing\n");
        exit(EXIT_FAILURE);
    }
    p->prec = new_p;
    new_p->noeud = noeud;
    strcpy(new_p->id,id);
    return new_p;
}

// pile_parsing_t* pop_pile_parsing(pile_parsing_t* p){
    
//     pile_parsing_t* new_p = p->prec;
//     if (new_p==NULL){
//         return p;
//     }
//     free(p);
//     return new_p;
// }
pile_parsing_t* pop_pile_parsing(pile_parsing_t* p) {
    if (p == NULL) {
        return NULL; // Handle case where p is NULL
    }

    pile_parsing_t* new_p = p->prec; // Store the previous node

    free(p); // Free the memory of the current node

    return new_p; // Return the previous node
}

void parcourir_pile(pile_parsing_t* p) {
    if (p == NULL) {
        printf("La pile est vide.\n");
        return;
    }

    while (p != NULL) {
        printf("Element : id = %s, noeud = %s\n", p->id, get_node_name(p->noeud));
        p = pop_pile_parsing(p);
    }
}


const char* get_node_name(noeud_t noeud) {
    switch (noeud) {
        case nt_S:
            return "nt_S";
        case nt_ALGO:
            return "nt_ALGO";
        case nt_ALGOLIST:
            return "nt_ALGOLIST";
        case nt_ALL_IDF_PARAM_FUNCTION:
            return "nt_ALL_IDF_PARAM_FUNCTION";
        case nt_ALL_COMMANDS:
            return "nt_ALL_COMMANDS";
        case nt_COMMAND:
            return "nt_COMMAND";
        case nt_SET_COMMAND:
            return "nt_SET_COMMAND";
        case nt_INCR_COMMAND:
            return "nt_INCR_COMMAND";
        case nt_DECR_COMMAND:
            return "nt_DECR_COMMAND";
        case nt_OUT_COMMAND:
            return "nt_OUT_COMMAND";
        case nt_DOFORI_COMMAND:
            return "nt_DOFORI_COMMAND";
        case nt_DOWHILE_COMMAND:
            return "nt_DOWHILE_COMMAND";
        case nt_RETURN_COMMAND:
            return "nt_RETURN_COMMAND";
        case nt_IF_COMMAND:
            return "nt_IF_COMMAND";
        case nt_IF_ELSE_COMMAND:
            return "nt_IF_ELSE_COMMAND";
        case nt_ALL_CALLS:
            return "nt_ALL_CALLS";
        case nt_CALL_FUNC:
            return "nt_CALL_FUNC";
        case nt_ALL_PARAMS:
            return "nt_ALL_PARAMS";
        case nt_PARAMS:
            return "nt_PARAMS";
        case nt_EXPR:
            return "nt_EXPR";
        case t_IDF_PARAM_FUNCTION:
            return "t_IDF_PARAM_FUNCTION";
        case t_IDF:
            return "t_IDF";
        case t_ADD:
            return "t_ADD";
        case t_SUB:
            return "t_SUB";
        case t_MULT:
            return "t_MULT";
        case t_DIV:
            return "t_DIV";
        case t_AND:
            return "t_AND";
        case t_OR:
            return "t_OR";
        case t_DIF:
            return "t_DIF";
        case t_EGAL:
            return "t_EGAL";
        case t_NOT:
            return "t_NOT";
        case t_NUM:
            return "t_NUM";
        case t_FALSE:
            return "t_FALSE";
        case t_TRUE:
            return "t_TRUE";
        default:
            return "Unknown node";
    }
}


