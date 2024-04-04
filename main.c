#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"


int main(int argc, char const *argv[])
{
    //table de symbole
    functions_hash_list *ts = NULL;

    hlist_init(&ts);

    char *name = "firstFunction";
    if(hlist_add_function(&ts , name ,3 ,7) == -1){
        printf("%s allready declared\n" , name);
    }

    char *name2 = "secondFunction";
    if(hlist_add_function(&ts , name2 ,4 ,5) == -1){
        printf("%s allready declared\n" , name2);
    }

    char *name3 = "theardFunction";
    if(hlist_add_function(&ts , name3 ,2 ,2) == -1){
        printf("%s allready declared\n" , name3);
    }
    

    if(hlist_add_function(&ts , name ,3 ,0) == -1){
        printf("%s allready declared\n" , name);
    }


    // FINDING A FUNCTION
    char *sname = "firstFunction";
    func_tab* tab = hlist_get_function(ts , sname , 3);
    if(tab == NULL){
        //not found
        printf("%s not found\n" , sname);
    } else {
        //found
        tab->nbr_locals = 2222;

        printf("%s found  nbrparams = %d , nbr locals = %d\n" 
                ,tab->nom_func , tab->nbr_params , tab->nbr_locals);
    }


    // function_add_var(&tab ,  )
    
    // FINDING A FUNCTION NOT EXIST
    char *sname2 = "firstINGFunction";
    func_tab* tab2 = hlist_get_function(ts , sname2 , 3);
    if(tab2 == NULL){
        //not found
        printf("%s not found\n" , sname2);
    } else {
        //found
        tab2->nbr_locals = 2222;
        printf("%s found  nbrparams = %d , nbr locals = %d\n"
                ,tab2->nom_func , tab2->nbr_params , tab2->nbr_locals);
    }

    func_tab *function = hlist_get_function(ts , name2 , 4 );
    if(function == NULL){
        //not found
        printf("name2 not found\n");
        exit(EXIT_FAILURE);
    }
    char *var1 = "v1";
    function_add_var(&function , var1 , PARAM_VAR , 1 );
    char *var2 = "v2";
    function_add_var(&function , var2 , PARAM_VAR , 2 );
    char *var3 = "v3";
    function_add_var(&function , var3 , PARAM_VAR , 3 );


    sym_tab* v1 = function_get_var(function , var1);
    printf("%s found  num var = %d , nbr locals = %d\n"
        ,v1->nom_idf , v1->num_var , v1->type);

    v1->type=LOCAL_VAR;

    v1 = function_get_var(function , var1);
    printf("%s found  num var = %d , nbr locals = %d\n"
        ,v1->nom_idf , v1->num_var , v1->type);

    sym_tab* v2 = function_get_var(function , var2);
    printf("%s found  num var = %d , nbr locals = %d\n"
        ,v2->nom_idf , v2->num_var , v2->type);
    sym_tab* v3 = function_get_var(function , var3);
    printf("%s found  num var = %d , nbr locals = %d\n"
        ,v3->nom_idf , v3->num_var , v3->type);

    free_hash_lists(ts);

    


    return 0;
}

