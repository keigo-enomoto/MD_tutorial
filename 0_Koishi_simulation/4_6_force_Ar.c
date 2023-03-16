//
//  4_6_force_Ar.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program4/index.php
//
/* Results
0  4.042242  4.042242  4.042242
1  0.849328  0.849328  7.660279
2  0.849328  7.660279  0.849328
3  7.660279  0.849328  0.849328
4 -0.735526  5.794359  5.794359
5 -5.794359  0.735526  5.794359
6 -5.794359  5.794359  0.735526
7 -1.076932  1.076932  1.076932
8  5.794359 -0.735526  5.794359
9  0.735526 -5.794359  5.794359
10  1.076932 -1.076932  1.076932
11  5.794359 -5.794359  0.735526
12 -0.849328 -0.849328  7.660279
13 -4.042242 -4.042242  4.042242
14 -7.660279 -0.849328  0.849328
15 -0.849328 -7.660279  0.849328
16  5.794359  5.794359 -0.735526
17  1.076932  1.076932 -1.076932
18  0.735526  5.794359 -5.794359
19  5.794359  0.735526 -5.794359
20 -0.849328  7.660279 -0.849328
21 -7.660279  0.849328 -0.849328
22 -4.042242  4.042242 -4.042242
23 -0.849328  0.849328 -7.660279
24  7.660279 -0.849328 -0.849328
25  0.849328 -7.660279 -0.849328
26  0.849328 -0.849328 -7.660279
27  4.042242 -4.042242 -4.042242
28 -1.076932 -1.076932 -1.076932
29 -5.794359 -5.794359 -0.735526
30 -5.794359 -0.735526 -5.794359
31 -0.735526 -5.794359 -5.794359
sum of force: -0.000000 -0.000000 -0.000000
*/


#include <stdio.h>
#include <math.h>

enum {
  NUM_FCC_PARTICLE = 4  // 面心立方格子の単位格子の内の粒子の数
};


void init_position_FCC(int nla, double side_unit, double *cd);
double calc_force_pot(int npa, double *cd, double *fc, double side);

int main(){

    int npa = 32;
    int nla = 2;
    double density = 0.817657;

    int i=0;
    double cd[npa*3];
    double fc[npa*3];
    double sum_fc[3] = {0.0};
    double pot_erg = 0.0;
    double side, side_unit;

    side_unit = pow(NUM_FCC_PARTICLE / density, 1./3.);
    side = 6.0;

    init_position_FCC(nla, side_unit, cd);
    pot_erg = calc_force_pot(npa, cd, fc, side);

    for(i = 0; i < npa; i++){
        sum_fc[0] += fc[i*3];
        sum_fc[1] += fc[i*3+1];
        sum_fc[2] += fc[i*3+2];
        printf("%d % f % f % f\n",i,fc[i*3],fc[i*3+1],fc[i*3+2]);
    }

    printf("sum of force: %f %f %f",sum_fc[0],sum_fc[1],sum_fc[2]);


    return 0;

}


/***************
 * FUNCTIONS
****************/

void init_position_FCC(int nla, double side_unit, double *cd)
{
int i = 0;
int ix, iy, iz;

for(iz = 0; iz < nla; iz++){
    for(iy = 0; iy < nla; iy++){
        for(ix = 0; ix < nla; ix++){
            cd[i]   = side_unit*ix;
            cd[i+1] = side_unit*iy;
            cd[i+2] = side_unit*iz;
            i += 3;
            cd[i]   = side_unit*(ix+0.5);
            cd[i+1] = side_unit*(iy+0.5);
            cd[i+2] = side_unit*iz;
            i += 3;
            cd[i]   = side_unit*(ix+0.5);
            cd[i+1] = side_unit*iy;
            cd[i+2] = side_unit*(iz+0.5);
            i += 3;
            cd[i]   = side_unit*ix;
            cd[i+1] = side_unit*(iy+0.5);
            cd[i+2] = side_unit*(iz+0.5);
            i += 3;
        }
    }
}

}

// --------------------------------------------------------------

double calc_force_pot(int npa, double *cd, double *fc, double side){

    int i,j;
    double pot_erg;
    double r2,r6,r12;
    double dphi,phi;
    double sideh = 0.5 * side;
    double cut_off2 = sideh * sideh;

    double dd[3] = {0.0};  // distance
    double df[3] = {0.0};  // force

    // initialize ---------
    pot_erg = 0;
    for(i = 0; i < npa*3; i++){ fc[i] = 0; }

    // calc force and potential -------------
    for(i = 0; i < npa*3; i += 3){
        for(j = i + 3; j < npa*3; j += 3){
            dd[0] = cd[i]   - cd[j];
            dd[1] = cd[i+1] - cd[j+1];
            dd[2] = cd[i+2] - cd[j+2];
            if(dd[0] < -sideh) dd[0] += side; if(dd[0] >  sideh) dd[0] -= side;
            if(dd[1] < -sideh) dd[1] += side; if(dd[1] >  sideh) dd[1] -= side;
            if(dd[2] < -sideh) dd[2] += side; if(dd[2] >  sideh) dd[2] -= side;
            r2  = dd[0]*dd[0] + dd[1]*dd[1] + dd[2]*dd[2];
            if(r2 < cut_off2){
            r6 = r2*r2*r2;
            r12 = r6*r6;
            phi = 4.0*(1.0/(r12) - 1.0/(r6));
            dphi = 4.0*(12.0/(r12*r2) - 6.0/(r6*r2));
            pot_erg += phi;
            df[0] = dphi*dd[0]; df[1] = dphi*dd[1]; df[2] = dphi*dd[2];
            fc[i]   += df[0];   fc[i+1] += df[1];   fc[i+2] += df[2];
            fc[j]   -= df[0];   fc[j+1] -= df[1];   fc[j+2] -= df[2];
            }
        }
    }

    return pot_erg;
}