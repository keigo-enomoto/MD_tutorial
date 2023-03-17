//
//  temp.c
//
//
//  Created by Keigo Enomoto on 2022/3/16.
//  ref : https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/mol_sim.php#sim_program
//  github : https://github.com/keigo-enomoto/MD_tutorial
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*====================
    温度の計算、補正
=====================*/

// from 3_3_4_5_init_velocity.c
double calc_temp(int npa, double *vl){
    int k ;
    double vl2;
    double T;

    vl2 = 0.0;
    for(k=0; k<npa; k++){
        vl2 += vl[k*3]*vl[k*3] + vl[k*3+1]*vl[k*3+1] + vl[k*3+2]*vl[k*3+2];
    }
    T = vl2/(3*(npa-1));  // 前運動量が0なので自由度は一つ減っている

    return T;
}

//----------------------------------------------------------------------------
// used in main loop
void norm_temp(int npa, double target_temp, double T, double *nvl){

    double x;
    int i;
        //normalize
        x = sqrt(target_temp/T);
        for(i=0; i<npa*3; i++){
            nvl[i] = x * nvl[i];
        }
}

//----------------------------------------------------------------------------

// from 3_3_4_5_init_velocity.c
void modify_temp(int npa, double target_temp, double *vl, double *nvl){

    int i;
    double x, T;
    //initialize nvl
    for(i=0; i<npa*3; i++){
        nvl[i] = 0.0;
    }

    //calculation temperature
    T = calc_temp(npa,vl);  //from "init_velocity.h"

    //normalize
    x = sqrt(target_temp/T);
    for(i=0; i<npa*3; i++){
        nvl[i] = x * vl[i];
    }

    T = calc_temp(npa,nvl);  // from "init_velocity.h"
    printf("target temp: %f normalized temp: %f\n",target_temp, T);

}
