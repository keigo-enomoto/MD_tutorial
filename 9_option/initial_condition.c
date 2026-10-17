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

#include "utility.h"




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

/*====================
    初期配置ファイルの書き出し・読み込み
=====================*/

// 粒子数・箱の大きさ・位置と速度を書き出す。%.17g は double を過不足なく復元できる桁数
void write_configuration(char *file_name, int npa, double side, double *cd, double *vl){

    int i;
    FILE *fp = fopen(file_name,"w");
    if(fp == NULL){
        fprintf(stderr,"cannot open file for writing : %s\n",file_name);
        exit(EXIT_FAILURE);
    }

    fprintf(fp,"npa = %d\n",npa);
    fprintf(fp,"side = %.17g\n",side);
    fprintf(fp,"# x y z vx vy vz\n");
    for(i = 0; i < npa*3; i += 3){
        fprintf(fp,"%.17g %.17g %.17g %.17g %.17g %.17g\n",
                cd[i],cd[i+1],cd[i+2],vl[i],vl[i+1],vl[i+2]);
    }

    fclose(fp);
}

// 初期配置ファイルを読み込む。粒子数がわかった時点で配列を確保する (呼び出し側で free すること)
void read_initial_configuration(char *file_name, int *npa, double *side, double **cd, double **vl, double **nvl, double **fc){

    int i;
    char buf[256];
    FILE *fp = fopen(file_name,"r");
    if(fp == NULL){
        fprintf(stderr,"cannot open initial configuration file : %s\n",file_name);
        exit(EXIT_FAILURE);
    }

    // 1, 2 行目: npa と side。3 行目はコメント行なので読み飛ばす
    if(fscanf(fp,"npa = %d\n",npa) != 1 || *npa <= 0
    || fscanf(fp,"side = %lf\n",side) != 1
    || fgets(buf,sizeof(buf),fp) == NULL){
        fprintf(stderr,"failed to read header : %s\n",file_name);
        exit(EXIT_FAILURE);
    }

    allocate_arrays(*npa,cd,vl,nvl,fc);

    for(i = 0; i < (*npa)*3; i += 3){
        if(fscanf(fp,"%lf %lf %lf %lf %lf %lf",
                  &(*cd)[i],&(*cd)[i+1],&(*cd)[i+2],&(*vl)[i],&(*vl)[i+1],&(*vl)[i+2]) != 6){
            fprintf(stderr,"failed to read particle %d : %s\n",i/3,file_name);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);
}
