#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "symbol_table.h"


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
    ,nt_RETURN_COMMAND
    ,nt_ALL_CALLS
    ,nt_CALL_FUNC
    ,nt_ALL_PARAMS
    ,nt_PARAMS
    ,nt_EXPR
    ,nt_END_FUNCTIONS

    ,nt_START_IF_COMMAND
    ,nt_IF_COMMAND

    ,nt_ELSE_DETECTION
    ,nt_IF_ELSE_COMMAND
    ,nt_START_IF_ELSE_COMMAND
    // else detection

    ,nt_START_DOWHILE_COMMAND
    ,nt_DOWHILE_COMMAND
    ,nt_END_DOWHILE_COMMAND

    ,nt_DOFORI_COMMAND
    ,nt_BEGIN_DOFORI_COMMAND
    ,nt_END_DOFORI_COMMAND

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
    ,t_INF
    ,t_SUP
    ,t_SUP_EGAL
    ,t_INF_EGAL
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
int make_asm(pile_parsing_t** p,functions_hash_list *ts);


// debuging tools 
const char* get_node_name(noeud_t noeud) ;


void start();
void main_function();
void functioncall(const char *name , functions_hash_list *ts);
void end();
void add();
void sub();
void division();
void mul();
void or();
void and();
void not();
void num(int number);

void true_key_word();
void false_key_word();


void return_function();
void set_command(functions_hash_list *ts  , char *varname , char *function_name);
void idf(functions_hash_list *ts  , char *varname , char *function_name );

void increment(functions_hash_list *ts  , char *varname , char *function_name);

void decrement(functions_hash_list *ts  , char *varname , char *function_name);

void equal(int eq_label_number);
void lessthen(int eq_label_number);
void supthen(int eq_label_number);
void different(int diff_label_number);
void supthenOrEqual(int eq_label_number);
void lessthenOrEqual(int eq_label_number);


void out_command();

void end_if(int nbrif);
void else_detection(int nbrif);
void if_detection(int nbrif);
void end_if_else(int nbrif);


void start_while(int nbr_while);
void end_while(int nbr_while);
void end_expr_while(int nbr_while);







#endif