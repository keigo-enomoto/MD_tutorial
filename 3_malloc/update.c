//
//  update.c
//
//
//  Created by Keigo Enomoto on 2022/3/16.
//  ref : https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/mol_sim.php#sim_program
//  github : https://github.com/keigo-enomoto/MD_tutorial
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*====================
    位置と速度の更新
=====================*/

void update_vel_velret(int npa, double dt, double *nvl, double *fc){

    int i;

    for(i=0; i<npa*3; i+=3){
        nvl[i]   += fc[i]  *dt*0.5;
        nvl[i+1] += fc[i+1]*dt*0.5;
        nvl[i+2] += fc[i+2]*dt*0.5;
    }

}

void update_position(int npa, double dt, double *cd, double *nvl){

    int i;

    for(i=0; i<npa*3; i+=3){
        cd[i]   += nvl[i]  *dt;
        cd[i+1] += nvl[i+1]*dt;
        cd[i+2] += nvl[i+2]*dt;
    }


}


/*====================
    周期境界条件の適用
=====================*/

/* シミュレーションboxはx : 0.0 -> side, y : 0.0 -> side, z : 0.0 -> side
　　のように各辺の長さは全て等しい(= side)とする */
void apply_PBC(int npa, double side, double *cd){

    int i;

    for(i=0;i<npa*3;i++){
        if(cd[i]>=side){
            cd[i]-=side;
        }
        else if(cd[i]<= 0.0){
            cd[i]+=side;
        }
    }
}

