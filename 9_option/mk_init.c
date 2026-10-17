//
//  mk_init.c
//  初期配置 (FCC 格子 + ランダム速度) を作ってファイルに書き出す
//  usage : ./mk_init [-n nla] [-d density] [-t target_temp] [-s seed] [-o output_file]
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>   // getopt

#include "initial_condition.h"
#include "temp.h"
#include "utility.h"
#include "common.h"

int main(int argc, char ** argv){

    // 既定値 (オプションで上書きする)
    int nla = 5;
    double density = 0.817657464660018;
    double target_temp = 0.722;
    int seed = 1;
    char *output_file = "init.dat";
    int opt;

    while((opt = getopt(argc,argv,"n:d:t:s:o:h")) != -1){
        switch(opt){
            case 'n': nla = atoi(optarg); break;
            case 'd': density = atof(optarg); break;
            case 't': target_temp = atof(optarg); break;
            case 's': seed = atoi(optarg); break;
            case 'o': output_file = optarg; break;
            case 'h':
            default :
                fprintf(stderr,"usage : %s [-n nla] [-d density] [-t target_temp] [-s seed] [-o output_file]\n",argv[0]);
                exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
        }
    }

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
