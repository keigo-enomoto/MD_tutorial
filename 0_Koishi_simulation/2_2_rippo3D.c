//
//  2_2_rippo3D.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program2/index.php
//



#include <stdio.h>
enum {
N = 4
};

int main(int argc, char **argv)
{
    int i,j,k;
    int npa = 0;
    double cd[N*N*N*3];
    double l = 1.0;

    for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
    for(k = 0; k < N; k++){
        cd[npa*3]   = l*i;
        cd[npa*3+1] = l*j;
        cd[npa*3+2] = l*k;
        npa++;
    }
    }
    }

    for(i = 0; i < npa*3; i += 3){
    printf("%d %f %f %f\n",i/3,cd[i],cd[i+1],cd[i+2]);
    }
    return 0;
}