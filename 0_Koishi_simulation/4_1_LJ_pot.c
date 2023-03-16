//
//  4_1_LJ_pot.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program4/index.php
//
//
/********* CAUTION *************

LAMMPS等のOSSでも必ずそうですが、
LJポテンシャル等の整数乗の計算の際にはpow()は非推奨です。
(精度面と速度面から)

以下のプログラムではHPに従って、pow()を使用していますが、
実際には例えば

for(r = 0.9; r < 4; r += 0.01){
    r2 = r * r;
    r6 = r2 * r2 * r2;
    r12 = r6 * r6;
    phi = 4.0*(1.0/r12 - 1.0/r6);
    printf("%f %f\n",r,phi);
}

のように計算します。
(掛け算と割り算は速度が落ちるので、できるだけ回数を減らすために
r2やr6といった一次的な変数を作成します。)
********************************/


#include <stdio.h>
#include <math.h>

int main()
{
    double r;
    double phi, dphi;

    FILE *fp_phi;
    FILE *fp_dphi;

    fp_phi = fopen("output/4_1_LJ_pot.txt","w");
    // LJ potential
    for(r = 0.9; r < 4; r += 0.01){
        phi = 4.0*(pow(r,-12)-pow(r,-6));
        fprintf(fp_phi,"%f %f\n",r,phi);
    }
    fclose(fp_phi);

    fp_dphi = fopen("output/4_1_d_LJ_pot.txt","w");
    // derivative of LJ potential
    for(r = 0.9; r < 4; r += 0.01){
        dphi = -24.0*(2.0*pow(r,-13)-pow(r,-7));
        fprintf(fp_dphi,"%f %f\n",r,dphi);
    }
    fclose(fp_dphi);

    return 0;
}