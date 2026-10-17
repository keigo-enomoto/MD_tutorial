
#include <stdio.h>
#include <stdlib.h>

#include "utility.h"



// https://programming-place.net/ppp/contents/c/035.html
static void* xmalloc(size_t size)
{
    void* p = malloc( size );
    if( p == NULL ){
        fprintf(stderr,"\nout of memory allocating %lu bytes\n",(unsigned long)size);
        exit( EXIT_FAILURE );
    }
    return p;
}


void allocate_arrays(int npa, double **cd, double **vl, double **nvl, double **fc){

    *cd = (double *)xmalloc(3*npa*sizeof(double));
    *vl = (double *)xmalloc(3*npa*sizeof(double));
    *nvl = (double *)xmalloc(3*npa*sizeof(double));
    *fc = (double *)xmalloc(3*npa*sizeof(double));

}


// input file を読み込み、ヒープ上の構造体へのポインタを返す (呼び出し側で free すること)
simulation_parameters *read_parameters_from_inputfile(char **input_file){

    FILE *fp;
    simulation_parameters *parameters;

    parameters = (simulation_parameters *)xmalloc(sizeof(simulation_parameters));

    fp = fopen(*input_file,"r");
    if(fp == NULL){
        fprintf(stderr,"cannot open input file : %s\n",*input_file);
        exit(EXIT_FAILURE);
    }

    // fscanf は読み込めた項目の数を返す。1 でなければ書式が合っていない
    if(fscanf(fp,"h = %lf\n",&(parameters->h)) != 1
    || fscanf(fp,"target_temp = %lf\n",&(parameters->target_temp)) != 1
    || fscanf(fp,"finish_time = %lf\n",&(parameters->finish_time)) != 1
    || fscanf(fp,"norm_interval = %d\n",&(parameters->norm_interval)) != 1
    || fscanf(fp,"output_interval = %d\n",&(parameters->output_interval)) != 1
    || fscanf(fp,"init_file = %255s\n",parameters->init_file) != 1){
        fprintf(stderr,"failed to read input file : %s\n",*input_file);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    return parameters;
}
