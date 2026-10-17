//
//  cal_force.c
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
    力の初期化
=====================*/

void zero_force(int npa, double *fc){

    int i;
    for(i=0; i<3*npa; i++){ fc[i] = 0.0; }

}


/*====================
    力の計算
=====================*/

// from 4_6_force_Ar.c
double calc_force_pot_LJ(int npa, double *cd, double *fc, double side){

    int i,j;
    double pot_erg;
    double r2,r6,r12;
    double dphi,phi;
    double sideh = 0.5 * side;
    double cut_off2 = sideh * sideh;  // 本当はこれもインプットパラメーター

    double dd[3] = {0.0};  // distance
    double df[3] = {0.0};  // force

    // initialize ---------
    pot_erg = 0;
    zero_force(npa,fc);

    // calc force and potential -------------
    for(i = 0; i < npa*3; i += 3){
        for(j = i + 3; j < npa*3; j += 3){
            dd[0] = cd[i]   - cd[j];
            dd[1] = cd[i+1] - cd[j+1];
            dd[2] = cd[i+2] - cd[j+2];
            if(dd[0] < -sideh){ dd[0] += side;} else if(dd[0] >  sideh){ dd[0] -= side;}
            if(dd[1] < -sideh){ dd[1] += side;} else if(dd[1] >  sideh){ dd[1] -= side;}
            if(dd[2] < -sideh){ dd[2] += side;} else if(dd[2] >  sideh){ dd[2] -= side;}
            r2  = dd[0]*dd[0] + dd[1]*dd[1] + dd[2]*dd[2];
            if(r2 < cut_off2){
                r6 = r2*r2*r2;
                r12 = r6*r6;
                phi = 4.0*(1.0/(r12) - 1.0/(r6));
                dphi = 4.0*(12.0/(r12*r2) - 6.0/(r6*r2));
                pot_erg += phi;
                df[0] = dphi*dd[0]; df[1] = dphi*dd[1]; df[2] = dphi*dd[2];
                fc[i] += df[0];     fc[i+1] += df[1];   fc[i+2] += df[2];
                fc[j] -= df[0];     fc[j+1] -= df[1];   fc[j+2] -= df[2];
            }
        }
    }

    return pot_erg;
}


void check_sum_force(int npa, double *fc, double *sum_fc){
    int i;

    for(i = 0; i < npa; i++){
        sum_fc[0] += fc[i*3];
        sum_fc[1] += fc[i*3+1];
        sum_fc[2] += fc[i*3+2];
        // printf("%d % f % f % f\n",i,fc[i*3],fc[i*3+1],fc[i*3+2]);
    }

    printf("sum of force: %f %f %f\n",sum_fc[0],sum_fc[1],sum_fc[2]);

}
