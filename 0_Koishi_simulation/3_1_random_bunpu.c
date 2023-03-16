//
//  3_1_random_bumpu.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program3/index.php
//



#include <stdio.h>
#include <stdlib.h>
int main()
{
    int i;
    double d;
    int d_int;
    int bunpu[5];
    srand(1);
    for(i = 0; i < 5; i++){
        bunpu[i] = 0;
    }
    for(i = 0; i < 10; i++){
        d = (double)rand()/RAND_MAX*5;
        printf("d = %f\n",d);

        d_int = (int)d;  // C言語ではdoubleをintに変換すると小数点以下は切り捨てされる
        bunpu[d_int]++;
    }

    for(i = 0; i < 5; i++){
        printf("bunpu[%d]=%d\n",i,bunpu[i]);
    }
    return 0;
}