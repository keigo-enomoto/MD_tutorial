//
//  3_3_4_5_init_velocity.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program3/index.php
//



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//----------------------------------------------------------------------------

/*速度から温度を計算し上記の結果を得るプログラムを作成せよ。*/

double calc_temp(int npa, double *vl){
    int k ;
    double vl2;
    double T;

    vl2 = 0;
    for(k=0; k<npa; k++){
        vl2 += vl[k*3]*vl[k*3] + vl[k*3+1]*vl[k*3+1] + vl[k*3+2]*vl[k*3+2];
    }
    T = vl2/(3*(npa-1));

    return T;
}

//----------------------------------------------------------------------------

int main()
{
    int i,j;
    int npa = 32;
    double vl[npa*3];
    double u0,u1;
    double sum[3];

    // 3_4
    double T;
    double target_temp = 0.722000;

    // 3_5
    double x;
    double nvl[npa*3];


    srand(1);
    /* 速度の設定 */
    for(i = 0; i < npa*3; i++){
      u0 = (double)rand()/RAND_MAX;
      u1 = (double)rand()/RAND_MAX;
      vl[i] = sqrt(-2*log(u0))*cos(2*M_PI*u1);
    }
    /* 速度の和の計算 */
    for(i = 0; i < 3; i++){
      sum[i] = 0;
    }
    for(i = 0; i < npa*3; i += 3){
      for(j = 0; j < 3; j++){
        sum[j] += vl[i+j];
      }
    }

    /* 速度の和をゼロになるようにする */
    for(i = 0; i < 3; i++){
      sum[i] /= npa;
    }
    for(i = 0; i < npa*3; i += 3){
      for(j = 0; j < 3; j++){
        vl[i+j] -= sum[j];
      }
    }

    /* 速度の和がゼロになっているかどうかの確認 */
    for(i = 0; i < 3; i++){
      sum[i] = 0;
    }
    for(i = 0; i < npa*3; i += 3){
      for(j = 0; j < 3; j++){
        sum[j] += vl[i+j];
      }
      printf("%3d % f % f % f\n",i/3,vl[i],vl[i+1],vl[i+2]);
    }
    printf("sum % f % f % f\n",sum[0],sum[1],sum[2]);

    // 3_4 : calculation temprature
    T = calc_temp(npa,vl);
    printf("%f %f\n",target_temp, T);

    // 3_5 : normalize temperature
    x = sqrt(target_temp/T);
    for(i=0; i<npa*3; i++){
        nvl[i] = x * vl[i];
    }

    T = calc_temp(npa,nvl);
    printf("Normalize %f %f\n",target_temp, T);


    return 0;
}