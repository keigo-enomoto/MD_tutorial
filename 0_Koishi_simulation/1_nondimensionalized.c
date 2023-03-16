//
//  1_nondimensionalized.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program1/index.php
//

#include <stdio.h>
#include <math.h>

int main(void){

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
    double h = 2.0e-15;                  /* 時間刻み幅(s) */

    density /=  unit_mass/(unit_length * unit_length * unit_length) ;
    h /= unit_time;
    target_temp *= kb/unit_erg_eV;

    printf("単位長さ %e(m)\n",unit_length);
    printf("単位質量 %e(kg)\n",unit_mass);
    printf("単位エネルギー %e(eV)\n",unit_erg_eV);
    printf("単位時間 %e(s)\n",unit_time);

    printf("無次元数密度 %f\n",density);
    printf("無次元時間刻み幅 %f\n",h);
    printf("無次元温度 %f\n",target_temp);

    return 0;



}
