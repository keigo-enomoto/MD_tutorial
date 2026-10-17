//
//  mk_init.c
//  初期配置 (FCC 格子 + ランダム速度) を作ってファイルに書き出す
//  usage : ./mk_init nla density target_temp seed output_file
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "initial_condition.h"
#include "temp.h"
#include "utility.h"
#include "common.h"

int main(int argc, char ** argv){

    if(argc < 6){
        fprintf(stderr,"usage : %s nla density target_temp seed output_file\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    int nla = atoi(argv[1]);
    double density = atof(argv[2]);
    double target_temp = atof(argv[3]);
    int seed = atoi(argv[4]);
    char *output_file = argv[5];

    int npa = NUM_FCC_PARTICLE*nla*nla*nla;
    double side_unit = pow(NUM_FCC_PARTICLE / density, 1./3.);
    double side = side_unit * nla;
    double *cd = NULL, *vl = NULL, *nvl = NULL, *fc = NULL;

    allocate_arrays(npa,&cd,&vl,&nvl,&fc);

    init_position_FCC(nla, side_unit, cd);
    init_velocity(seed,npa,vl);
    modify_temp(npa,target_temp,vl,nvl);  // 温度補正後の速度が nvl に入る

    write_configuration(output_file,npa,side,cd,nvl);

    free(cd); free(vl); free(nvl); free(fc);
    return 0;
}
