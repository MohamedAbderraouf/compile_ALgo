#ifndef AST_H
#define AST_H

typedef enum {
 nt_S
,nt_ALGO
,nt_ALGOLIST
,nt_IDFS
,nt_COMMANDS
,nt_COMMAND
,nt_SET_COMMAND
,nt_INCR_COMMAND
,nt_DECR_COMMAND
,nt_OUT_COMMAND
,nt_DOFORI_COMMAND
,nt_DOWHILE_COMMAND
,nt_RETURN_COMMAND
,nt_IF_COMMAND
,nt_CALLS
,nt_CALL_FUNC
,nt_PARAMS
,nt_EXPR

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
} non_terminal_t;



#endif