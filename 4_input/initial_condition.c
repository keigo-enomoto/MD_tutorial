//
//  initial_condition.c
//
//
//  Created by Keigo Enomoto on 2022/3/16.
//  ref : https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/mol_sim.php#sim_program
//  github : https://github.com/keigo-enomoto/MD_tutorial
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>




// ==============================================================

/*====================
    初期配置の設定
=====================*/

// from 4_6_force_Ar.c
void init_position_FCC(int nla, double side_unit, double *cd)
{
int i = 0;
int ix, iy, iz;

for(iz = 0; iz < nla; iz++){
    for(iy = 0; iy < nla; iy++){
        for(ix = 0; ix < nla; ix++){
            cd[i]   = side_unit*ix;
            cd[i+1] = side_unit*iy;
            cd[i+2] = side_unit*iz;
            i += 3;
            cd[i]   = side_unit*(ix+0.5);
            cd[i+1] = side_unit*(iy+0.5);
            cd[i+2] = side_unit*iz;
            i += 3;
            cd[i]   = side_unit*(ix+0.5);
            cd[i+1] = side_unit*iy;
            cd[i+2] = side_unit*(iz+0.5);
            i += 3;
            cd[i]   = side_unit*ix;
            cd[i+1] = side_unit*(iy+0.5);
            cd[i+2] = side_unit*(iz+0.5);
            i += 3;
        }
    }
}

}

/*====================
    初期速度の設定
=====================*/

// from 3_3_4_5_init_velocity.c
void init_velocity(int seed, int npa, double *vl){

    int i,j;
    double u0,u1;
    double sum[3];

    srand(seed);
    /* 速度の設定 */
    for(i = 0; i < npa*3; i++){
        u0 = (double)rand()/RAND_MAX;
        u1 = (double)rand()/RAND_MAX;
        vl[i] = sqrt(-2*log(u0))*cos(2*M_PI*u1);
    }

    /* 速度の和の計算 */
    for(i = 0; i < 3; i++){ sum[i] = 0; }
    for(i = 0; i < npa*3; i += 3){
        for(j = 0; j < 3; j++){
            sum[j] += vl[i+j];  //vx->sum[0], vy->sum[1], vz->sum[2]
        }
    }

    /* 速度の和をゼロになるようにする */
    for(i = 0; i < 3; i++){ sum[i] /= npa; }
    for(i = 0; i < npa*3; i += 3){
        for(j = 0; j < 3; j++){
            vl[i+j] -= sum[j];
        }
    }

    /* 速度の和がゼロになっているかどうかの確認 */
    for(i = 0; i < 3; i++){ sum[i] = 0; }
    for(i = 0; i < npa*3; i += 3){
        for(j = 0; j < 3; j++){
            sum[j] += vl[i+j];
        }
    }

    printf("sum of initial speed x:%f y:%f z:%f\n",sum[0],sum[1],sum[2]);

}
