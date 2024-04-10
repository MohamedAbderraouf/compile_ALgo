%{
#include "symbol_table.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
extern FILE* yyin;

pile_parsing_t* pile ;
pile_parsing_t* head ;
char current_function_name[50];
functions_hash_list *ts;
func_tab* tmp_func_tab;
int count_params =1;

int param_number,local_number;
int label_number = 0;
void intToString(int number, char str[]);
int notSameType(TYPE_SYNTH a , TYPE_SYNTH b , TYPE_SYNTH c);
int notSameType2(TYPE_SYNTH a , TYPE_SYNTH b);


int yylex();  
void yyerror(const char* s){
    fprintf(stderr,"%s\n",s);
}

%}

%define parse.error verbose

%union{
    int entier;
    char *idf;
    boolean_t boolean;
    TYPE_SYNTH synth;
}

%type<synth> EXPR

%token<boolean> TRUE FALSE
%token<entier> NUM
%token<idf> IDF

%token BEG END SET INCR DECR OUT CALL DOFORI OD DOWHILE RETURN IF ELSE FI;
%token OPEN_ACCO CLOSE_ACCO VIRGULE OPEN_PARENT CLOSE_PARENT;
%token ADD SUB MULT DIV;
%token DIF AND EGAL OR NOT INF INF_EGAL SUP SUP_EGAL;

%left EGAL DIF INF INF_EGAL SUP SUP_EGAL
%left OR
%left AND
%left NOT
%left ADD SUB
%left MULT DIV

%start S

%%

S :  ALGO ALGOLIST INDICATE_END CALLS_TRANSITION
    {
        push_pile_parsing(pile , "" , nt_S);
    }

INDICATE_END : 
    {
        pile = push_pile_parsing(pile , "" , nt_END_FUNCTIONS);
    }


ALGO_DECRLARATION : BEG OPEN_ACCO IDF CLOSE_ACCO
    {   
        count_params = 1;
        strcpy(current_function_name , $3);
        hlist_add_function(&ts , $3 ,0 ,0);
    };
CONTINUATION_ALGO : OPEN_ACCO IDFS_TRANSITION CLOSE_ACCO COMMANDS_TRANSITION END

ALGO : ALGO_DECRLARATION CONTINUATION_ALGO  
    {
        pile = push_pile_parsing(pile , tmp_func_tab->nom_func , nt_ALGO);
    }
ALGOLIST : 
    {
        pile = push_pile_parsing(pile , "" , nt_ALGOLIST);
    } 
| ALGO ALGOLIST 
    {
        pile = push_pile_parsing(pile , "" , nt_ALGOLIST);
    }    

// cette continuation pour s'assurer que la fonction soit declarer 


IDFS_TRANSITION : IDFS 
    {
        pile = push_pile_parsing(pile , current_function_name , nt_ALL_IDF_PARAM_FUNCTION);
        count_params = 1;
    }

/* idfs params de fonction */
IDFS : IDF VIRGULE IDFS 
    {
        pile = push_pile_parsing(pile , $1 , t_IDF_PARAM_FUNCTION);
        tmp_func_tab = hlist_get_function(ts , current_function_name );
        tmp_func_tab->nbr_params++;
        //ajout de la variable
        function_add_var(&tmp_func_tab , $1 , PARAM_VAR , count_params++ );

    }
    | IDF 
    {
        pile = push_pile_parsing(pile , $1 , t_IDF_PARAM_FUNCTION);
        tmp_func_tab = hlist_get_function(ts , current_function_name );
        if (tmp_func_tab == NULL) {
            printf("cant find Function\n" );
            exit(EXIT_FAILURE);
        }
        tmp_func_tab->nbr_params++;
        //ajout de la variable
        function_add_var(&tmp_func_tab , $1 , PARAM_VAR , count_params++ );
    }

COMMANDS_TRANSITION : COMMANDS 
    {
        pile = push_pile_parsing(pile , "" , nt_ALL_COMMANDS);
    } 
COMMANDS: COMMAND COMMANDS {pile = push_pile_parsing(pile , "" , nt_COMMAND);} 
| COMMAND {pile = push_pile_parsing(pile , "" , nt_COMMAND);} 

COMMAND : SET_COMMAND | INCR_COMMAND | DECR_COMMAND 
    | OUT_COMMAND | DOFORI_COMMAND | DOWHILE_COMMAND
    | RETURN_COMMAND | IF_COMMAND | CALL_FUNC;

/*######################## COMMANDS ####################################*/
// declarations de la variable 
// i ajout dans la liste de variable de la fonction courant
SET_COMMAND    : SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO 
    {
        pile = push_pile_parsing(pile , $3 , nt_SET_COMMAND);
        
        if(element_exists(tmp_func_tab->table , $3)==0){
            function_add_var(&tmp_func_tab , $3 , LOCAL_VAR , count_params++ );
            tmp_func_tab->nbr_locals++;
        }
        tmp_func_tab = hlist_get_function(ts , current_function_name );
        if (tmp_func_tab == NULL) {
            printf("cant find Function\n" );
            exit(EXIT_FAILURE);
        }
        
        sym_tab* sym =  function_get_var(tmp_func_tab,$3);
        if (sym == NULL) {
            printf("cant find Variable %s\n" , $3);
            exit(EXIT_FAILURE);
        }

        sym->type_synth = $6;

    }

INCR_COMMAND   : INCR OPEN_ACCO IDF CLOSE_ACCO 
    {   
        if(element_exists(tmp_func_tab->table , $3)==0){
            fprintf(stderr,"%s not inialized and used in \\INCR ",$3);
            exit(EXIT_FAILURE);
        }
        pile = push_pile_parsing(pile , $3 , nt_INCR_COMMAND);
    }

DECR_COMMAND   : DECR OPEN_ACCO IDF CLOSE_ACCO 
    {
        if(element_exists(tmp_func_tab->table , $3)==0){
            fprintf(stderr,"%s not inialized and used in \\INCR ",$3);
            exit(EXIT_FAILURE);
        }
        pile = push_pile_parsing(pile , $3 , nt_DECR_COMMAND);
    }

OUT_COMMAND : OUT OPEN_ACCO EXPR CLOSE_ACCO 
    {  
        pile = push_pile_parsing(pile , "" , nt_OUT_COMMAND);
    }

// separation pour la detection de l'idf avant de rantree dans les commands 
CONTINUATION_DOFORI : OPEN_ACCO EXPR CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO COMMANDS_TRANSITION OD
DOFORI_COMMAND : DOFORI OPEN_ACCO IDF CLOSE_ACCO CONTINUATION_DOFORI  
    {

        pile = push_pile_parsing(pile , $3 , nt_DOFORI_COMMAND);

        if(element_exists(tmp_func_tab->table , $3)==0){
            function_add_var(&tmp_func_tab , $3 , LOCAL_VAR , count_params++ );
            tmp_func_tab->nbr_locals++;
        }
        
        tmp_func_tab = hlist_get_function(ts , current_function_name );
        if (tmp_func_tab == NULL) {
            printf("cant find Function\n" );
            exit(EXIT_FAILURE);
        }
        
        sym_tab* sym =  function_get_var(tmp_func_tab,$3);
        if (sym == NULL) {
            printf("cant find Variable %s\n" , $3);
            exit(EXIT_FAILURE);
        }

        sym->type_synth = NUM_T;

    }


DOWHILE_COMMAND: DOWHILE OPEN_ACCO EXPR  CLOSE_ACCO COMMANDS_TRANSITION OD 
    {
        pile = push_pile_parsing(pile , "" , nt_DOWHILE_COMMAND);
    }

RETURN_COMMAND : RETURN OPEN_ACCO EXPR CLOSE_ACCO 
    {
        pile = push_pile_parsing(pile , "" , nt_RETURN_COMMAND);
    }

IF_COMMAND : IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS_TRANSITION FI 
    {
        pile = push_pile_parsing(pile , "" , nt_IF_COMMAND);
    }
| IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS_TRANSITION ELSE  COMMANDS_TRANSITION FI 
    {
        pile = push_pile_parsing(pile , "" , nt_IF_ELSE_COMMAND);
    }

CALLS_TRANSITION : CALLS 
    {
        pile = push_pile_parsing(pile , "" ,nt_ALL_CALLS );
    }

CALLS : CALL_FUNC CALLS
    {
    }
|CALL_FUNC 
    {
    }

CALL_FUNC: CALL OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO PARAMS_TRANSITION CLOSE_ACCO 
    {

        if(hlist_get_function(ts , $3 )== NULL){
            printf("Function called but not declared");
            exit(EXIT_FAILURE);
        }

        pile = push_pile_parsing(pile , $3 , nt_CALL_FUNC);
    } 


PARAMS_TRANSITION : PARAMS 
    {
        pile = push_pile_parsing(pile , "" , nt_ALL_PARAMS);
    }
PARAMS : EXPR 
    {
        pile = push_pile_parsing(pile , "" , nt_PARAMS);
    }
    | EXPR VIRGULE PARAMS
    {
        pile = push_pile_parsing(pile , "" , nt_PARAMS);
    }

/*#################### EXPR ############################*/

EXPR: EXPR ADD EXPR
    {   
        if(notSameType($1 , $3 , NUM_T)==0){
            $$=ERR_T;
            printf("ERROR: Adition only for ints\n");
            exit(EXIT_FAILURE);
        }
        $$=NUM_T;
        pile = push_pile_parsing(pile , "" , t_ADD);
    }
| EXPR SUB EXPR 
    {
        if(notSameType($1 , $3 , NUM_T)==0){
            $$=ERR_T;
            printf("ERROR: Substraction only for ints\n");
            exit(EXIT_FAILURE);
        }
        $$=NUM_T;
        pile = push_pile_parsing(pile , "" , t_SUB);
    }
| EXPR MULT EXPR 
    {
        if(notSameType($1 , $3 , NUM_T)==0){
            $$=ERR_T;
            printf("ERROR: Multiplication only for ints\n");
            exit(EXIT_FAILURE);
        }
        $$=NUM_T;
        pile = push_pile_parsing(pile , "" , t_MULT);
    }
    | EXPR DIV EXPR 
    {
        if(notSameType($1 , $3 , NUM_T)==0){
            $$=ERR_T;
            printf("ERROR: Division only for ints\n");
            exit(EXIT_FAILURE);
        }
        $$=NUM_T;
        pile = push_pile_parsing(pile , "" , t_DIV);
    }
    | EXPR AND EXPR 
    {
        if(notSameType($1 , $3 , BOOL_T)==0){
            $$=ERR_T;
            printf("ERROR: AND only for BOOL\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_AND);
    }
    | EXPR OR EXPR 
    {
        if(notSameType($1 , $3 , BOOL_T)==0){
            $$=ERR_T;
            printf("ERROR: OR only for BOOL\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_OR);
    }
    | EXPR DIF EXPR 
    {
        if(notSameType2($1 , $3 )==0){
            $$=ERR_T;
            printf("ERROR: cant compare bool with int\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_DIF);
    }
    | EXPR EGAL EXPR 
    {
        if(notSameType2($1 , $3 )==0){
            $$=ERR_T;
            printf("ERROR: cant compare bool with int\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_EGAL);
    }
    | EXPR INF EXPR 
    {
        if(notSameType2($1 , $3 )==0){
            $$=ERR_T;
            printf("ERROR: cant compare bool with int\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_INF);
    }
    | EXPR INF_EGAL EXPR 
    {
        if(notSameType2($1 , $3 )==0){
            $$=ERR_T;
            printf("ERROR: cant compare bool with int\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_INF_EGAL);
    }
    | EXPR SUP EXPR 
    {
        if(notSameType2($1 , $3 )==0){
            $$=ERR_T;
            printf("ERROR: cant compare bool with int\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_SUP);
    }
    | EXPR SUP_EGAL EXPR 
    {
        if(notSameType2($1 , $3 )==0){
            $$=ERR_T;
            printf("ERROR: cant compare bool with int\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_SUP_EGAL);
    }
    | NOT EXPR 
    {
        if($2 != BOOL_T || $2 != NOT_INITIALIZED){
            printf("ERROR: Not operation only for bools\n");
            exit(EXIT_FAILURE);
            $$=ERR_T;
        }
        $$=BOOL_T;
        pile = push_pile_parsing(pile , "" , t_NOT);
    }

    | OPEN_PARENT EXPR CLOSE_PARENT
    {
        $$=$2;
    }
    | NUM 
        {
            $$=NUM_T;
            char tmp[20];intToString($1 , tmp);
            pile = push_pile_parsing(pile , tmp , t_NUM);
        }
    | FALSE 
    {
        char tmp[20];intToString((int)$1 , tmp);
        $$=BOOL_T;
        pile = push_pile_parsing(pile , tmp , t_FALSE);
    }
    | TRUE 
    {   
        char tmp[20];intToString((int)$1 , tmp);
        $$=BOOL_T;
        pile = push_pile_parsing(pile , tmp , t_TRUE);
    }
    | IDF 
    {   
        tmp_func_tab = hlist_get_function(ts , current_function_name );
        if (tmp_func_tab == NULL) {
            printf("cant find Function\n" );
        }
        
        sym_tab* sym =  function_get_var(tmp_func_tab,$1);
        if (sym == NULL) {
            printf("cant find Variable %s\n" , $1);
            exit(EXIT_FAILURE);
        }

        $$ = sym->type_synth;
        pile = push_pile_parsing(pile , $1 , t_IDF);
    }
    | CALL_FUNC {
        $$ = NOT_INITIALIZED;
    }
%%

int notSameType(TYPE_SYNTH a , TYPE_SYNTH b , TYPE_SYNTH c){

    return 1? ((a==NOT_INITIALIZED || a==c) && (b==NOT_INITIALIZED || b==c) ):0;
}

int notSameType2(TYPE_SYNTH a , TYPE_SYNTH b){

    return 1? ((a==b && a!=ERR_T) || (a!=b && (b==NOT_INITIALIZED || a==NOT_INITIALIZED) && a!=ERR_T && b!=ERR_T) ):0;
}

void intToString(int number, char str[]) {
    int i = 0;

    // Handle the case of a negative number
    if (number < 0) {
        str[i++] = '-';
        number = -number;
    }

    // Extract digits one by one
    while (number != 0) {
        int digit = number % 10;
        str[i++] = digit + '0';
        number /= 10;
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0, end = i - 1;
    while (start < end) {
        // Swap characters at start and end
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        start++;
        end--;
    }
}

int main(void) {

    // hash list table des symboles 
    ts = NULL;
    hlist_init(&ts);


    pile = init_pile_parsing();
    head = pile;

    if (yyparse() == 0) {
        /* printf("Parsing successful!\n"); */
    } else {
        printf("Parsing failed!\n");
        exit(EXIT_FAILURE);
    }

    head = pop_pile_parsing(head);

    make_asm(&head , ts );

    /* parcourir_pile(head); */
    
    /* pile = pop_pile_parsing(pile);
    printf("Element : id = %s, noeud = %d\n", pile->id, pile->noeud); */

    /* print_functions_hash_list(ts); */

    free_hash_lists(ts);
    
    return 0;
}


