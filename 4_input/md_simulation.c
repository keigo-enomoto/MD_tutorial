//
//  md_simulation.c
//
//
//  Created by Keigo Enomoto on 2022/3/16.
//  ref : https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/mol_sim.php#sim_program
//  github : https://github.com/keigo-enomoto/MD_tutorial
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cal_force.h"
#include "initial_condition.h"
#include "temp.h"
#include "update.h"
#include "utility.h"
#include "common.h"



int main(int argc, char ** argv){

    // 設定パラメータ (input file から読み込む) ------------
    simulation_parameters *parameters = NULL;
    char *input_file = NULL;

    if(argc < 2){
        fprintf(stderr,"usage : %s input_file\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    input_file = argv[1];
    parameters = read_parameters_from_inputfile(&input_file);

    int nla = parameters->nla;
    double density = parameters->density;
    double h = parameters->h;
    double target_temp = parameters->target_temp;
    int seed = parameters->seed;
    double finish_time = parameters->finish_time;
    int norm_interval = parameters->norm_interval;
    int output_interval = parameters->output_interval;

    // 変数 ---------------------
    int npa = NUM_FCC_PARTICLE*nla*nla*nla;   // 全粒子数
    int step = 0;         // 時間ステップ
    double side, side_unit;
    double time = 0.0;    // 時間
    double pot_erg = 0.0; // ポテンシャルエネルギー
    // double knt_erg = 0.0; // 運動エネルギー
    double T = 0.0;       // 温度
    double *cd = NULL;    // 位置
    double *vl = NULL;    // 速度
    double *nvl = NULL;   // 規格化速度
    double *fc = NULL;    // 力
    // double sum_fc[3] = {0.0};

    // -----------------------------------------------------------

    printf("density : %f, h : %f, target_temp : %f\n",density,h,target_temp);
    side_unit = pow(NUM_FCC_PARTICLE / density, 1./3.);
    side = side_unit * nla;

    // cd = (double *)malloc(3*npa*sizeof(double));
    // vl = (double *)malloc(3*npa*sizeof(double));
    // nvl = (double *)malloc(3*npa*sizeof(double));
    // fc = (double *)malloc(3*npa*sizeof(double));

    allocate_arrays(npa,&cd,&vl,&nvl,&fc);

    init_position_FCC(nla, side_unit, cd);
    init_velocity(seed,npa,vl);
    modify_temp(npa,target_temp,vl,nvl);
    zero_force(npa,fc);

    // =============== main loop ===============
    printf("******* START SIMULATION *********\n");
    printf("time potential T \n");
    while(1){
        time += h;
        if(time > finish_time) break;

        update_vel_velret(npa,h,nvl,fc);  // v(t) -> v(t+0.5dt)
        update_position(npa,h,cd,nvl);    // r(t) -> r(t+dt)
        apply_PBC(npa,side,cd);

        pot_erg = calc_force_pot_LJ(npa, cd, fc, side);
        update_vel_velret(npa,h,nvl,fc);  // v(t+dt)
        T = calc_temp(npa,nvl);
        if(step%norm_interval==0){ norm_temp(npa,target_temp,T,nvl); }
        if(step%output_interval==0){
            printf("%8.4f %8.4f %8.4f \n",time,pot_erg,T);
            // check_sum_force(npa,fc,sum_fc);
        }
        step ++;
    }
    printf("******* END SIMULATION *********\n");
    printf("time (step) potential T \n");
    printf("%8.4f (%d) %8.4f %8.4f \n",time,step,pot_erg,T);

    free(cd); free(vl); free(nvl); free(fc);
    free(parameters);

}