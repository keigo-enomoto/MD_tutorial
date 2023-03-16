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




// 変数のポインタを渡す -> ポインタ経由で変数の中身を更新する
void non_dimensionalize_parameters(double *n_density, double *n_h, double *n_target_temp)
{
    double avogadro  = 6.0221367e+23;    /* アボガドロ数 (mol^-1) */
    double kb = 8.617080363e-5;          /* ボルツマン定数 (eV K^-1) */

    double unit_length = 3.4e-10;                 /* 長さ(m) */
    double unit_mass   = 39.948/avogadro*1e-3;    /* 質量(kg) */
    double unit_erg_eV = 120.*kb;                 /* エネルギー(eV) */
    double unit_erg_J  = unit_erg_eV*1.60219e-19; /* エネルギー(J) */
    double unit_time2 = (unit_mass * unit_length * unit_length)/unit_erg_J; /* unit_timeのルートの中身 (2乗なので変数名に2とつけた)*/
    double unit_time   = sqrt(unit_time2);   /* 時間(s) */

    double density  = 1.38e+3;           /* 密度 (kg/m^3) */
    double target_temp = 86.64;          /* 設定温度 (K) */
    double h = 1.0e-15;                  /* 時間刻み幅(s) */

    *n_density = density / (unit_mass/(unit_length * unit_length * unit_length)) ;
    *n_h = h / unit_time;
    *n_target_temp = target_temp * (kb/unit_erg_eV);
}


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
