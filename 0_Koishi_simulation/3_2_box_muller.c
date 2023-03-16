//
//  3_2_box_muller.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program3/index.php
//



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum {
    N_ARRAY = 20,
    N_LOOP = 1000
};

int main()
{
    int i,j;
    double d;
    int d_int;
    double u0, u1, S;
    int bunpu[N_ARRAY];
    srand(1);

    for(i = 0; i < N_ARRAY; i++){
        bunpu[i] = 0;
    }

    for(i = 0; i < N_LOOP; i++){

        u0 = (double)rand()/RAND_MAX;
        u1 = (double)rand()/RAND_MAX;

        S = sqrt(-2*log(u0)) * cos(2*M_PI*u1);
        // printf("d=%f\n", S);

        if (S>= -5.0 && S <= 5.0){
            d = S*2 + 10;    // 0 < d < 20
            d_int = (int)d;  // C言語ではdoubleをintに変換すると小数点以下は切り捨てされる
            bunpu[d_int]++;
        }
    }

    for(i = 0; i < N_ARRAY; i++){
        printf("%d: ",i);
        for(j = 0; j < bunpu[i]/10; j++){
            printf("*");
        }
        printf("\n");
    }

    return 0;
}