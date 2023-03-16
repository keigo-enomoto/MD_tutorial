//
//  md_simulation_base.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  ref : https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/mol_sim.php#sim_program
//

/******** 全体の流れのイメージ ************
main(){

（変数の初期化）
（位置の初期化）
（速度の初期化）
（温度の補正）

for(メインループの回数){ // whileにしてbreak等を使用してもOK

    （速度の更新(1)）
    （位置の更新）
    （周期境界条件処理）
    for(i = 0; i < 粒子数; i++){
        for(j = i+1; j < 粒子数; j++){
        （力の計算）
        }
    }
    （速度の更新(2)）
    （温度の計算）
    （温度の補正）
}
}
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


enum {
  NUM_FCC_PARTICLE = 4  // 面心立方格子の単位格子の内の粒子の数
};


// Declaration of functions ===============================================================
void non_dimensionalize_parameters(double *n_density, double *n_h, double *n_target_temp);
void init_position_FCC(int nla, double side_unit, double *cd);
void init_velocity(int seed, int npa, double *vl);
double calc_temp(int npa, double *vl);
void norm_temp(int npa, double target_temp, double T, double *nvl);
void modify_temp(int npa, double target_temp, double *vl, double *nvl);
void zero_force(int npa, double *fc);
void update_vel_velret(int npa, double dt, double *nvl, double *fc);
void update_position(int npa, double dt, double *cd, double *nvl);
void apply_PBC(int npa, double side, double *cd);
double calc_force_pot_LJ(int npa, double *cd, double *fc, double side);
void check_sum_force(int npa, double *fc, double *sum_fc);
// ========================================================================================


int main(int argc, char ** argv){

    // 設定パラメータ ------------
    int nla = 2;  // 面心立方格子の単位格子の数
    double density = 0.0;      // 密度
    double h = 0.0;            // 時間刻み幅
    double target_temp = 0.0;  // 設定温度
    int seed = 1;              // 乱数のシード
    double finish_time = 1.0;  // 終了時刻
    int norm_interval = 10;    // 温度の補正を行う間隔
    int output_interval = 100; // 出力を行う間隔

    // 変数 ---------------------
    int npa = NUM_FCC_PARTICLE*nla*nla*nla;   // 全粒子数
    int step = 0;         // 時間ステップ
    double side, side_unit;
    double time = 0.0;    // 時間
    double pot_erg = 0.0; // ポテンシャルエネルギー
    // double knt_erg = 0.0; // 運動エネルギー
    double T = 0.0;       // 温度
    double cd[npa*3];   // 位置
    double vl[npa*3];   // 速度
    double nvl[npa*3];  // 規格化速度
    double fc[npa*3];   // 力
    double sum_fc[3] = {0.0};

    // -----------------------------------------------------------

    non_dimensionalize_parameters(&density,&h,&target_temp);
    printf("density : %f, h : %f, target_temp : %f\n",density,h,target_temp);
    side_unit = pow(NUM_FCC_PARTICLE / density, 1./3.);
    side = side_unit * nla;

    init_position_FCC(nla, side_unit, cd);
    init_velocity(seed,npa,vl);
    modify_temp(npa,target_temp,vl,nvl);
    zero_force(npa,fc);

    // =============== main loop ===============
    printf("******* START SIMULATION *********\n");
    printf("time potential T \n");
    while(1){
        time += h;
        if(time > finish_time) break;

        update_vel_velret(npa,h,nvl,fc);  // v(t) -> v(t+0.5dt)
        update_position(npa,h,cd,nvl);    // r(t) -> r(t+dt)
        apply_PBC(npa,side,cd);

        pot_erg = calc_force_pot_LJ(npa, cd, fc, side);
        update_vel_velret(npa,h,nvl,fc);  // v(t+dt)
        T = calc_temp(npa,nvl);
        if(step%norm_interval==0){ norm_temp(npa,target_temp,T,nvl); }
        if(step%output_interval==0){
            printf("%8.4f %8.4f %8.4f \n",time,pot_erg,T);
            // check_sum_force(npa,fc,sum_fc);
        }
        step ++;
    }
    printf("******* END SIMULATION *********\n");
    printf("time (step) potential T \n");
    printf("%8.4f (%d) %8.4f %8.4f \n",time,step,pot_erg,T);

}



/******************
 * FUNCTIONS
*******************/

// 変数のポインタを渡す -> ポインタ経由で変数の中身を更新する
void non_dimensionalize_parameters(double *n_density, double *n_h, double *n_target_temp)
{
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
    double h = 1.0e-15;                  /* 時間刻み幅(s) */

    *n_density = density / (unit_mass/(unit_length * unit_length * unit_length)) ;
    *n_h = h / unit_time;
    *n_target_temp = target_temp * (kb/unit_erg_eV);
}


// ==============================================================

/*====================
    初期配置の設定
=====================*/

// from 4_6_force_Ar.c
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

/*====================
    初期速度の設定
=====================*/

// from 3_3_4_5_init_velocity.c
void init_velocity(int seed, int npa, double *vl){

    int i,j;
    double u0,u1;
    double sum[3];

    srand(seed);
    /* 速度の設定 */
    for(i = 0; i < npa*3; i++){
        u0 = (double)rand()/RAND_MAX;
        u1 = (double)rand()/RAND_MAX;
        vl[i] = sqrt(-2*log(u0))*cos(2*M_PI*u1);
    }

    /* 速度の和の計算 */
    for(i = 0; i < 3; i++){ sum[i] = 0; }
    for(i = 0; i < npa*3; i += 3){
        for(j = 0; j < 3; j++){
            sum[j] += vl[i+j];  //vx->sum[0], vy->sum[1], vz->sum[2]
        }
    }

    /* 速度の和をゼロになるようにする */
    for(i = 0; i < 3; i++){ sum[i] /= npa; }
    for(i = 0; i < npa*3; i += 3){
        for(j = 0; j < 3; j++){
            vl[i+j] -= sum[j];
        }
    }

    /* 速度の和がゼロになっているかどうかの確認 */
    for(i = 0; i < 3; i++){ sum[i] = 0; }
    for(i = 0; i < npa*3; i += 3){
        for(j = 0; j < 3; j++){
            sum[j] += vl[i+j];
        }
    }

    printf("sum of initial speed x:%f y:%f z:%f\n",sum[0],sum[1],sum[2]);

}

/*====================
    温度の補正
=====================*/

// from 3_3_4_5_init_velocity.c
double calc_temp(int npa, double *vl){
    int k ;
    double vl2;
    double T;

    vl2 = 0.0;
    for(k=0; k<npa; k++){
        vl2 += vl[k*3]*vl[k*3] + vl[k*3+1]*vl[k*3+1] + vl[k*3+2]*vl[k*3+2];
    }
    T = vl2/(3*(npa-1));  // 前運動量が0なので自由度は一つ減っている

    return T;
}

//----------------------------------------------------------------------------
// used in main loop
void norm_temp(int npa, double target_temp, double T, double *nvl){

    double x;
    int i;
        //normalize
        x = sqrt(target_temp/T);
        for(i=0; i<npa*3; i++){
            nvl[i] = x * nvl[i];
        }
}

//----------------------------------------------------------------------------

// from 3_3_4_5_init_velocity.c
void modify_temp(int npa, double target_temp, double *vl, double *nvl){

    int i;
    double x, T;
    //initialize nvl
    for(i=0; i<npa*3; i++){
        nvl[i] = 0.0;
    }

    //calculation temprature
    T = calc_temp(npa,vl);  //from "init_velocity.h"

    //normalize
    x = sqrt(target_temp/T);
    for(i=0; i<npa*3; i++){
        nvl[i] = x * vl[i];
    }

    T = calc_temp(npa,nvl);  // from "init_velocity.h"
    printf("target temp: %f normalized temp: %f\n",target_temp, T);

}

/*====================
    力の初期化
=====================*/

void zero_force(int npa, double *fc){

    int i;
    for(i=0; i<3*npa; i++){ fc[i] = 0.0; }

}

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

