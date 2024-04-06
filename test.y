%{
#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
extern FILE* yyin;

pile_parsing_t* pile ;
pile_parsing_t* head ;

int param_number,local_number;
int label_number = 0;
char* int_to_string(int num);
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
%token DIF AND EGAL OR NOT;

%left EGAL DIF
%left OR
%left AND
%left NOT
%left ADD SUB
%left MULT DIV

%start S

%%

S :  ALGO ALGOLIST CALLS_TRANSITION{printf("S\n");push_pile_parsing(pile , "" , nt_S);};

ALGO : BEG OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO IDFS_TRANSITION CLOSE_ACCO COMMANDS_TRANSITION END 
       {printf("ALGO %s \n",$3); pile = push_pile_parsing(pile , $3 , nt_ALGO);
 };

ALGOLIST : {printf("ALGOLIST\n");pile = push_pile_parsing(pile , "" , nt_ALGOLIST);} 
        |ALGO ALGOLIST {printf("ALGOLIST\n");pile = push_pile_parsing(pile , "" , nt_ALGOLIST);}    


IDFS_TRANSITION : IDFS 
{printf("IDFS OF PARAMS OF\n");
pile = push_pile_parsing(pile , "" , nt_ALL_IDF_PARAM_FUNCTION);}
/* idfs params de fonction */
IDFS : IDF VIRGULE IDFS 
{printf("IDFS %s\n" , $1);pile = push_pile_parsing(pile , $1 , t_IDF_PARAM_FUNCTION);}
     | IDF {printf("IDFS LAST %s\n" , $1);pile = push_pile_parsing(pile , $1 , t_IDF_PARAM_FUNCTION);}

COMMANDS_TRANSITION : COMMANDS 
{printf("FIN DES COMMANDS\n");pile = push_pile_parsing(pile , "" , nt_ALL_COMMANDS);} 
COMMANDS: COMMAND COMMANDS {printf("COMMAND\n");pile = push_pile_parsing(pile , "" , nt_COMMAND);} 
| COMMAND {printf("NOT LAST COMMAND\n");pile = push_pile_parsing(pile , "" , nt_COMMAND);} 

COMMAND : SET_COMMAND | INCR_COMMAND | DECR_COMMAND 
    | OUT_COMMAND | DOFORI_COMMAND | DOWHILE_COMMAND
    | RETURN_COMMAND | IF_COMMAND;

/*######################## COMMANDS ####################################*/

SET_COMMAND    : SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO 
{printf("SET %s \n",$3); pile = push_pile_parsing(pile , $3 , nt_SET_COMMAND);}

INCR_COMMAND   : INCR OPEN_ACCO IDF CLOSE_ACCO 
{printf("INCR %s \n",$3);pile = push_pile_parsing(pile , $3 , nt_INCR_COMMAND);}

DECR_COMMAND   : DECR OPEN_ACCO IDF CLOSE_ACCO 
{printf("DECR %s \n",$3);pile = push_pile_parsing(pile , $3 , nt_DECR_COMMAND);}

OUT_COMMAND    : OUT OPEN_ACCO EXPR CLOSE_ACCO 
{printf("OUT \n");pile = push_pile_parsing(pile , "" , nt_OUT_COMMAND);}

DOFORI_COMMAND : DOFORI OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO COMMANDS_TRANSITION OD 
{printf("DOFORI%s \n",$3);pile = push_pile_parsing(pile , $3 , nt_DOFORI_COMMAND);}

DOWHILE_COMMAND: DOWHILE OPEN_ACCO EXPR  CLOSE_ACCO COMMANDS_TRANSITION OD 
{printf("DOWHILE\n");pile = push_pile_parsing(pile , "" , nt_DOWHILE_COMMAND);}

RETURN_COMMAND : RETURN OPEN_ACCO EXPR CLOSE_ACCO 
{printf("RETURN\n");pile = push_pile_parsing(pile , "" , nt_RETURN_COMMAND);}

IF_COMMAND     : IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS_TRANSITION FI 
{printf("IF \n");pile = push_pile_parsing(pile , "" , nt_IF_COMMAND);}
| IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS_TRANSITION ELSE  COMMANDS_TRANSITION FI 
{printf("IF WITH ELSE\n");pile = push_pile_parsing(pile , "" , nt_IF_ELSE_COMMAND);}

CALLS_TRANSITION : CALLS {pile = push_pile_parsing(pile , "" ,nt_ALL_CALLS );}
CALLS : CALL_FUNC CALLS
{printf("NOT LAST FUNC call\n");}
    | CALL_FUNC 
{printf("LAST FUNC call\n");}

CALL_FUNC: CALL OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO PARAMS_TRANSITION CLOSE_ACCO 
{printf("CALL FUNC %s\n", $3); pile = push_pile_parsing(pile , $3 , nt_CALL_FUNC);} 


PARAMS_TRANSITION : PARAMS {pile = push_pile_parsing(pile , "" , nt_ALL_PARAMS);}
PARAMS : EXPR {pile = push_pile_parsing(pile , "" , nt_PARAMS);}
    | EXPR VIRGULE PARAMS{pile = push_pile_parsing(pile , "" , nt_PARAMS);}


EXPR: EXPR ADD EXPR{printf("ADD\n");pile = push_pile_parsing(pile , "" , t_ADD);}
    | EXPR SUB EXPR {printf("sub\n");pile = push_pile_parsing(pile , "" , t_SUB);}
    | EXPR MULT EXPR {printf("Mul\n");pile = push_pile_parsing(pile , "" , t_MULT);}
    | EXPR DIV EXPR {printf("div\n");pile = push_pile_parsing(pile , "" , t_DIV);}
    | EXPR AND EXPR {printf("and\n");pile = push_pile_parsing(pile , "" , t_AND);}
    | EXPR OR EXPR {printf("or\n");pile = push_pile_parsing(pile , "" , t_OR);}
    | EXPR DIF EXPR {printf("dif\n");pile = push_pile_parsing(pile , "" , t_DIF);}
    | EXPR EGAL EXPR {printf("eq\n");pile = push_pile_parsing(pile , "" , t_EGAL);}
    | NOT EXPR {printf("not\n");pile = push_pile_parsing(pile , "" , t_NOT);}

    | OPEN_PARENT EXPR CLOSE_PARENT{}
    | NUM {printf("%d\n" , $1);pile = push_pile_parsing(pile , int_to_string($1) , t_NUM);}
    | FALSE {printf("%d\n" , $1);pile = push_pile_parsing(pile , int_to_string((int)$1) , t_FALSE);}
    | TRUE {printf("%d\n" , $1);pile = push_pile_parsing(pile , int_to_string((int)$1) , t_TRUE);}
    | IDF {printf("EXPR %s\n" , $1);pile = push_pile_parsing(pile , $1 , t_IDF);}
    | CALL_FUNC {}
%%

char* int_to_string(int num) {
    // Alloue dynamiquement de la mémoire pour la chaîne de caractères
    char* str = (char*)malloc(10 * sizeof(char));
    if (str == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }
    // Convertit l'entier en chaîne de caractères
    sprintf(str, "%d", num);
    return str;
}

int main(void) {


    pile = init_pile_parsing();
    head = pile;

    if (yyparse() == 0) {
        printf("Parsing successful!\n");
    } else {
        printf("Parsing failed!\n");
    }

    head = pop_pile_parsing(head);
    parcourir_pile(head);
    /* pile = pop_pile_parsing(pile);
    printf("Element : id = %s, noeud = %d\n", pile->id, pile->noeud); */

    return 0;
}


