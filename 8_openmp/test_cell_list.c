//
//  test_cell_list.c
//  総当たり版とセルリスト版で、力とポテンシャルエネルギーが一致するかを確かめる
//  usage : ./test_cell_list init_file cutoff
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cal_force.h"
#include "cell_list.h"
#include "initial_condition.h"

int main(int argc, char ** argv){

    if(argc < 3){
        fprintf(stderr,"usage : %s init_file cutoff\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    double cutoff = atof(argv[2]);

    int i,npa = 0;
    double side = 0.0;
    double *cd = NULL, *vl = NULL, *nvl = NULL, *fc = NULL;
    double *fc_cell = NULL;
    int *head = NULL, *next = NULL;
    double pot, pot_cell, err_f = 0.0, err;

    read_initial_configuration(argv[1],&npa,&side,&cd,&vl,&nvl,&fc);
    fc_cell = (double *)malloc(3*npa*sizeof(double));
    allocate_cell_list(npa,side,cutoff,&head,&next);

    // 初期配置は格子点なので、粒子を少しずらしてから比べる
    srand(1);
    for(i = 0; i < 3*npa; i++){ cd[i] += 0.1*((double)rand()/RAND_MAX - 0.5); }
    for(i = 0; i < 3*npa; i++){ cd[i] -= side*floor(cd[i]/side); }  // 箱の中に戻す

    pot      = calc_force_pot_LJ(npa,cd,fc,side,cutoff);
    pot_cell = calc_force_pot_LJ_cell(npa,cd,fc_cell,side,cutoff,head,next);

    for(i = 0; i < 3*npa; i++){
        err = fabs(fc[i] - fc_cell[i]);
        if(err > err_f){ err_f = err; }
    }
    printf("npa = %d, side = %f, cutoff = %f\n",npa,side,cutoff);
    printf("potential : %.12f (all pairs) %.12f (cell list)\n",pot,pot_cell);
    printf("max |force error| = %.3e, |potential error| = %.3e\n",err_f,fabs(pot - pot_cell));

    if(err_f > 1e-10 || fabs(pot - pot_cell) > 1e-10){
        printf("NG\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
