#include "ast.h"

#include <fcntl.h>
#include <unistd.h>

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

int make_asm(pile_parsing_t** p,functions_hash_list *ts){

//    int comparison_counter = 0; // pour le etiquete de == <= < > >= !=
    
    char function_name_current[50];
    // function trace
    Stack stack;
    initializeStack(&stack); 
    /////////////////////////
    pile_parsing_t* current_p = *p;


                // push(&stack,p->id);

   start();

    while (current_p != NULL) {
        
        switch (current_p->noeud) {

            case nt_ALL_IDF_PARAM_FUNCTION:
                printf(":%s\n",current_p->id);
                strcpy(function_name_current,current_p->id);
                //init tmp func 
                break;
            case t_ADD:
                add();break;
            case t_MULT:
                mul();break;
            case t_SUB:
                sub();break;
            case t_DIV:
                division();break;
            case t_AND:
                and();break;
            case t_OR:
                or();break;
            case t_FALSE:
                false_key_word();break;
            case t_TRUE:
                true_key_word();break;
            case t_NUM:
                num(atoi(current_p->id));break;
            // here is the problem
            case t_IDF:
                idf(ts , current_p->id , function_name_current);
                 break;
            case t_INF:
                break;
            case t_INF_EGAL:
                break;
            case t_SUP:
                break;
            case t_SUP_EGAL:
                break;
            case t_EGAL:
                break;
            case t_DIF:
                break;

            case nt_CALL_FUNC:
                functioncall(current_p->id,ts);
                break;
            case nt_RETURN_COMMAND:
                return_function(ts ,function_name_current); 
                break;
            case nt_SET_COMMAND:
                set_command(ts , current_p->id , function_name_current);
                break;
            case nt_END_FUNCTIONS:
                main_function();
                break;
            case nt_S:
                end();

            // ignore command 
            default:
                break;
        }
        current_p = pop_pile_parsing(current_p);
    }


    return 0;

}



int get_place_param_in_stack(int nbr_in_function , int count_locals){
    return 2*count_locals + 6*2 + nbr_in_function * 2 - 2;
}

int get_place_local_in_stack(int nrb_local_in_function){
    return nrb_local_in_function*2  ;
}

void return_function(functions_hash_list *ts  , char *function_name ){

    func_tab *tmp = hlist_get_function(ts,function_name);
    if (tmp == NULL){
        printf("Erreur : function %s not found\n",function_name);
        exit(EXIT_FAILURE);
    }
    
    printf("\tpop ax\n");
    printf("\tcp bx,bp\n");
    printf("\tconst cx,%d\n",get_place_param_in_stack(0 , tmp->nbr_locals));
    printf("\tsub bx,cx\n");
    printf("\tstorew ax,bx\n");
    printf("\tret\n");

}


void set_command(functions_hash_list *ts  , char *varname , char *function_name){

    int position;
    func_tab *tmp = hlist_get_function(ts,function_name);
    if (tmp == NULL){
        printf("Erreur : function %s not found\n",function_name);
        exit(EXIT_FAILURE);
    }
    
    sym_tab *tmp_sym = function_get_var(tmp,varname);
    
    if (tmp_sym->type == LOCAL_VAR){
        position = get_place_local_in_stack(tmp_sym->num_var);
    }
    else if (tmp_sym->type == PARAM_VAR){
        position = get_place_param_in_stack(tmp_sym->num_var , tmp->nbr_locals);
    }
    
    
    printf("\tpop ax\n");
    printf("\tcp bx,bp\n");
    printf("\tconst cx,%d\n",position);
    printf("\tsub bx,cx\n");
    printf("\tstorew ax,bx\n");
}

void idf(functions_hash_list *ts  , char *varname , char *function_name ){
    int position;
    func_tab *tmp = hlist_get_function(ts,function_name);
    if (tmp == NULL){
        printf("Erreur : function %s not found\n",function_name);
        exit(EXIT_FAILURE);
    }
    
    sym_tab *tmp_sym = function_get_var(tmp,varname);
    
    if (tmp_sym->type == LOCAL_VAR){
        position = get_place_local_in_stack(tmp_sym->num_var);
    }
    else if (tmp_sym->type == PARAM_VAR){
        position = get_place_param_in_stack(tmp_sym->num_var , tmp->nbr_locals);
    }

 
    printf("\tcp bx,bp\n");
    printf("\tconst cx,%d\n",position);
    printf("\tsub bx,cx\n");
    printf("\tloadw ax,bx\n");
    printf("\tpush ax\n");


}


void start(){
    // Générer le code de début
    printf("\tconst ax,debut\n");
    printf("\tjmp ax\n");
    printf(":nl\n");
    printf("@string \"\\n\"\n");
    //------------------ Message erreur division par 0 ------------------
    printf(":msgErrDiv0\n");
    printf("@string \"Erreur division par 0\\n\"\n");
    printf(":errDiv0\n");
    printf("\tconst ax,msgErrDiv0\n");
    printf("\tcallprintfs ax\n");
    printf("\tend\n");
    //------------------ Forme Indéterminée ------------------
    printf(":msgErr\n");
    printf("@string \"Forme indeterminée\\n\"\n");
    printf(":errDiv\n");
    printf("\tconst ax,msgErr\n");
    printf("\tcallprintfs ax\n");
    printf("\tend\n");

    printf(":msgErrOverflow\n");
    printf("@string \"Erreur dépassement de capacité\\n\"\n");
    printf(":errOverflow\n");
    printf("\tconst ax,msgErrOverflow\n");
    printf("\tcallprintfs ax\n");
    printf("\tend\n");
 
}

// between start() and main_function() i should put the code for 
// all the functions

void main_function(){
        //------------------
    printf(":debut\n");
    printf("\tconst bp,pile\n");
    printf("\tconst sp,pile\n");
    printf("\tconst ax,2\n");
    printf("\tsub sp,ax\n");
}

void end(){
    // Générer le code de fin
    printf("\tcp ax,sp\n");
    printf("\tcallprintfd ax\n");
    printf("\tconst ax,nl\n");
    printf("\tcallprintfs ax\n");
    printf("\tpop ax\n");
    printf("\tend\n");
    printf(":pile\n");
    printf("@int 0\n");

}


void functioncall(const char *name , functions_hash_list *ts ){
    printf(";calling %s function\n",name);
    
    printf(";return value\n");
    printf("\tpush ax\n");
    printf(";context comutation\n");
    printf("\tpush ax\n");
    printf("\tpush bx\n");
    printf("\tpush cx\n");
    printf("\tpush dx\n");

    func_tab *tmp = hlist_get_function(ts,name);

    printf(";local and global vars space\n");
    for (int i = 0; i < (tmp->nbr_locals); i++)
    {
        printf("\tpush ax\n");
    }
    
    printf("\tpush bp\n");

    printf("\tcp bp,sp\n");
    printf("\tconst dx,%s\n",name);
    printf("\tcall dx\n");
    

    printf("\tpop bp\n");
    printf(";free local and global vars space\n");
    for (int i = 0; i < (tmp->nbr_locals); i++)
    {
        printf("\tpop ax\n");
    }
    printf(";free comutation\n");
    printf("\tpop dx\n");
    printf("\tpop cx\n");
    printf("\tpop bx\n");
    printf("\tpop ax\n");
    

    printf("\tpop ax\n");
    
    for (int i = 0; i < (tmp->nbr_params); i++)
    {
        printf("\tpop bx\n");
    }

    printf("\tpush ax\n");


}




void add(){
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,errOverflow\n");
    printf("\tadd ax,bx\n");
    printf("\tjmpe cx\n");
    printf("\tpush ax\n");
}

void sub(){
    printf("\tpop bx\n");
    printf("\tpop ax\n");
    printf("\tsub ax,bx\n");
    printf("\tpush ax\n");
}

void mul(){
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,errOverflow\n");
    printf("\tmul ax,bx\n");
    printf("\tjmpe cx\n");
    printf("\tpush ax\n");
}


void division(){
    printf("\tpop bx\n");
    printf("\tpop ax\n");
    printf("\tconst cx,errDiv0\n");
    printf("\tconst dx,errDiv\n"); //----------
    printf("\tdiv ax,bx\n");
    printf("\tjmpz dx\n"); //-----------
    printf("\tjmpe cx\n");
    printf("\tpush ax\n");
}

void or(){
    printf("\tpop bx\n");
    printf("\tpop ax\n");
    printf("\tor ax,bx\n");
    printf("\tpush ax\n");
}

void and(){
    printf("\tpop bx\n");
    printf("\tpop ax\n");
    printf("\tand ax,bx\n"); // on peut utiliser mul ax,bx
    printf("\tpush ax\n");
}

void not(){
    printf("\tpop bx\n");
    printf("\tconst ax,1\n");
    printf("\tsub ax,bx\n"); // on peut utiliser not
    printf("\tpush ax\n");
}





void num(int number){
    printf("\tconst ax,%d\n",number);
    printf("\tpush ax\n");
}

void true_key_word(){
    printf("\tconst ax,1\n");
    printf("\tpush ax\n");
}
void false_key_word(){
    printf("\tconst ax,0\n");
    printf("\tpush ax\n");
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
        case t_INF:
            return "t_INF";
        case t_INF_EGAL:
            return "t_INF_EGAL";
        case t_SUP:
            return "t_SUP";
        case t_SUP_EGAL:
            return "t_SUP_EGAL";
        case t_NOT:
            return "t_NOT";
        case t_NUM:
            return "t_NUM";
        case t_FALSE:
            return "t_FALSE";
        case t_TRUE:
            return "t_TRUE";
        case nt_END_FUNCTIONS:
            return "nt_END_FUNCT";
        default:
            return "Unknown node";
    }
}





