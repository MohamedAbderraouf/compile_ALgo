%{
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
extern FILE* yyin;

func_tab *liste;
func_tab *current_fct;
int param_number,local_number;
int label_number = 0;

int yylex();  
void yyerror(const char* s){
    fprintf(stderr,"%s\n",s);
}

%}

%define parse.error verbose

%union{
    int entier;
    char *idf;
    TYPE_SYNTH synth;
}

%type<synth> EXPR

%token<entier> NUM
%token<idf> IDF

%token BEG END SET INCR DECR OUT CALL DOFORI OD DOWHILE RETURN IF ELSE FI;
%token OPEN_ACCO CLOSE_ACCO VIRGULE OPEN_PARENT CLOSE_PARENT;
%token ADD SUB MULT DIV;
%token DIF AND EGAL OR NOT TRUE FALSE;

%left EGAL DIF
%left OR
%left AND
%left NOT
%left ADD SUB
%left MULT DIV

%start S

%%

S :  ALGO ALGOLIST CALLS{};

ALGO : BEG OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO IDFS CLOSE_ACCO COMMANDS END {printf("end of %s\n" , $3);};

ALGOLIST : {} |ALGO ALGOLIST {}    


/* idfs params de fonction */
IDFS : IDF VIRGULE IDFS 
     | IDF;


COMMANDS: COMMAND COMMANDS | COMMAND; 

COMMAND : SET_COMMAND | INCR_COMMAND | DECR_COMMAND 
    | OUT_COMMAND | DOFORI_COMMAND | DOWHILE_COMMAND
    | RETURN_COMMAND | IF_COMMAND;

SET_COMMAND    : SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO {printf("SET\n");}
INCR_COMMAND   : INCR OPEN_ACCO IDF CLOSE_ACCO {printf("INCR\n");}
DECR_COMMAND   : DECR OPEN_ACCO IDF CLOSE_ACCO {printf("DECR\n");}
OUT_COMMAND    : OUT OPEN_ACCO EXPR CLOSE_ACCO {printf("OUT\n");}
DOFORI_COMMAND : DOFORI OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO COMMANDS OD {printf("DOFORI\n");}
DOWHILE_COMMAND: DOWHILE OPEN_ACCO EXPR  CLOSE_ACCO COMMANDS OD {printf("DOWHILE\n");}
RETURN_COMMAND : RETURN OPEN_ACCO EXPR CLOSE_ACCO {printf("RETURN\n");}
IF_COMMAND     : IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS FI {printf("IF\n");}
                | IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS ELSE  COMMANDS FI {printf("IF WITH ELSE\n");}


CALLS : CALLS CALL_FUNC {}
    | CALL_FUNC{}

CALL_FUNC: CALL OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO PARAMS CLOSE_ACCO {printf("CALL FUNC %s\n", $3);} 
PARAMS : EXPR {}
    | EXPR VIRGULE PARAMS{}


EXPR: EXPR ADD EXPR{}
    | EXPR SUB EXPR {}
    | EXPR MULT EXPR {}
    | EXPR DIV EXPR {}
    | EXPR AND EXPR {}
    | EXPR OR EXPR {}
    | EXPR DIF EXPR {}
    | EXPR EGAL EXPR {}
    | NOT EXPR {}

    | OPEN_PARENT EXPR CLOSE_PARENT{}
    | NUM {};
    | FALSE {}
    | TRUE {}
    | IDF {}
    | CALL_FUNC {}
%%

int main(void) {

    if (yyparse() == 0) {
        printf("Parsing successful!\n");
        // Traitement supplémentaire si l'analyse est réussie
    } else {
        printf("Parsing failed!\n");
        // Gestion des erreurs si l'analyse échoue
    }

    return 0;
}
