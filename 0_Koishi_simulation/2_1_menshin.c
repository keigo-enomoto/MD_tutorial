//
//  2_1_menshin.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program2/index.php
//


#include <stdio.h>
#include <math.h>

enum {
  NUM_FCC_PARTICLE = 4  // 面心立方格子の単位格子の内の粒子の数
};


int main(int argc, char **argv){

    int nla = 2;                              // 面心立方格子の単位格子の数
    int npa = NUM_FCC_PARTICLE*nla*nla*nla;   // 全粒子数

    double side, side_unit;
    double density = 0.817657;

    side_unit = pow(NUM_FCC_PARTICLE / density, 1./3.);
    side = side_unit*nla;


    printf("Number of particles : %d\n",npa);
    printf("side_unit : %lf\n",side_unit);
    printf("side : %lf\n",side);

    return 0;
}
