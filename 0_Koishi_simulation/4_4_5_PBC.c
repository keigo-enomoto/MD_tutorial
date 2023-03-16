//
//  4_4_5_PBC.c
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

よって、以下ではr6等を使用した実装を行っています。
********************************/


#include <stdio.h>
#include <math.h>
enum {
  N = 4,
  L = 6
};

int main()
{
    int i,j,k;
    int n = N;
    double cd[N*3], fc[N*3];
    double df[3], dd[3];
    double rd,dphi;
    double r8,r14;
    cd[0] = 1.0;  cd[1] = 1.0; cd[2] = 1.0;
    cd[3] = 2.0;  cd[4] = 3.0; cd[5] = 1.0;
    cd[6] = 4.0;  cd[7] = 1.0; cd[8] = 1.0;
    cd[9] = 5.0;  cd[10] = 5.0; cd[11] = 1.0;
    for(i = 0; i < n*3; i++){
        fc[i] = 0;
    }

    for(i=0; i<N*3; i+=3){
        for(j=i+3; j<N*3;j+=3){
            dd[0] = cd[i]   - cd[j];
            dd[1] = cd[i+1] - cd[j+1];
            dd[2] = cd[i+2] - cd[j+2];
            // 4_4 : PBC ------------------
            for(k=0; k<3; k++){
                if(dd[k] < -L*0.5){
                    dd[k] += L;
                }
                else if(dd[k]>L*0.5){
                    dd[k] -= L;
                }
            }

            rd  = dd[0]*dd[0] + dd[1]*dd[1] + dd[2]*dd[2];
            if (rd < 9.0){
                r8 = rd * rd * rd * rd;
                r14 = r8 * rd * rd * rd;
                dphi = 4.0*(12.0/r14 - 6.0/r8);
                df[0] = dphi*dd[0];
                df[1] = dphi*dd[1];
                df[2] = dphi*dd[2];
                fc[i]   += df[0];
                fc[i+1] += df[1];
                fc[i+2] += df[2];
                fc[j]   -= df[0];
                fc[j+1] -= df[1];
                fc[j+2] -= df[2];
            }
        }
    }

    for(i = 0; i < n; i++){
        printf("%d % f % f % f\n",i,fc[i*3],fc[i*3+1],fc[i*3+2]);
    }

    return 0;
}