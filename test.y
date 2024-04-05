%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

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

%token BEG END SET INCR DECR OUT CALL DOFORI OD DOWHILE RETURN IF ELSE;
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
    | RETURN_COMMAND |IF_COMMAND;

SET_COMMAND    : SET OPEN_ACCO IDF CLOSE_ACCO CALL OPEN_ACCO EXPR CLOSE_ACCO;
INCR_COMMAND   : INCR OPEN_ACCO IDF CLOSE_ACCO;
DECR_COMMAND   : DECR OPEN_ACCO IDF CLOSE_ACCO;
OUT_COMMAND    : OUT OPEN_ACCO EXPR CLOSE_ACCO;
DOFORI_COMMAND : DOFORI OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO COMMANDS OD;
DOWHILE_COMMAND: DOWHILE OPEN_ACCO CONDITION  CLOSE_ACCO COMMANDS OD
RETURN_COMMAND : RETURN OPEN_ACCO


ALGOLIST : ALGO ALGOLIST {}    
    | {};


CALLS : CALLS CALL_FUNC {}
    | CALL_FUNC;

CALL_FUNC: CALL OPEN_ACCO IDF CLOSE_ACCO CALL OPEN_ACCO PARAMS CLOSE_ACCO; 
PARAMS : EXPR {}
    | EXPR VIRGULE PARAMS;


EXPR: EXPR ADD EXPR{
    if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            addition();
        }
    }
    | EXPR SUB EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            soustraction();
        }
    }
    | EXPR MULT EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            multiplication();
        }
    }
    | EXPR DIV EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            division();
        }
    }
    | EXPR AND EXPR {
        if(test_expr_bool($1,$3) == ERR_T){
            $$=ERR_T;
        } else {
            $$=BOOL_T;
            multiplication();
        }
    }
    | OPEN_PARENT EXPR CLOSE_PARENT{$$=$2;}
    | NUM {$$=NUM_T;num($1);};
    | FALSE {$$=BOOL_T;num(0);}
    | TRUE {$$=BOOL_T;num(1);}
    | IDF {
        get_param_from_stack($1,current_fct->table);
        printf("\tpush dx\n");
        }
%%
/*
    callprintfd dx
	const ax,nl
	callprintfs ax
*/

int main(void){
    liste=NULL;
    /* ajouter_func("main",0,0,&liste); */
    

    start_asm();
    main_asm();
    yyparse();
    end_asm();
    printf(";Function : %s + %d params\n",liste->nom_func,liste->nbr_params);
    print_sym_tab(liste->table);
    return 0;
}