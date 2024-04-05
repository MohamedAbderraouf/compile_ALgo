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

ALGO : BEG COMMANDS END;

COMMANDS: COMMAND COMMANDS | COMMAND; 

COMMAND : SET_COMMAND | INCR_COMMAND | DECR_COMMAND 
    | OUT_COMMAND | DOFORI_COMMAND | DOWHILE_COMMAND
    | RETURN_COMMAND | IF_COMMAND;

SET_COMMAND    : SET OPEN_ACCO IDF CLOSE_ACCO CALL OPEN_ACCO EXPR CLOSE_ACCO;
INCR_COMMAND   : INCR OPEN_ACCO IDF CLOSE_ACCO;
DECR_COMMAND   : DECR OPEN_ACCO IDF CLOSE_ACCO;
OUT_COMMAND    : OUT OPEN_ACCO EXPR CLOSE_ACCO;
DOFORI_COMMAND : DOFORI OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO COMMANDS OD;
DOWHILE_COMMAND: DOWHILE OPEN_ACCO EXPR  CLOSE_ACCO COMMANDS OD
RETURN_COMMAND : RETURN OPEN_ACCO EXPR CLOSE_ACCO
IF_COMMAND     : IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS FI 
                | IF OPEN_ACCO EXPR CLOSE_ACCO COMMANDS ELSE OPEN_ACCO COMMANDS CLOSE_ACCO;

ALGOLIST : ALGO ALGOLIST {}    
    | {}



CALLS : CALLS CALL_FUNC {}
    | CALL_FUNC{}

CALL_FUNC: CALL OPEN_ACCO IDF CLOSE_ACCO CALL OPEN_ACCO PARAMS CLOSE_ACCO; 
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

    | OPEN_PARENT EXPR CLOSE_PARENT{}
    | NUM {};
    | FALSE {}
    | TRUE {}
    | IDF {}
    | CALL_FUNC {}
%%

int main(void) {

    printf("Parsing failed!\n");
    yyparse();
    

    return 0;
}
