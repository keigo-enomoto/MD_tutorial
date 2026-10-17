//
//  md_simulation.c
//
//
//  Created by Keigo Enomoto on 2022/3/16.
//  ref : https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/mol_sim.php#sim_program
//  github : https://github.com/keigo-enomoto/MD_tutorial
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cal_force.h"
#include "initial_condition.h"
#include "output.h"
#include "temp.h"
#include "update.h"
#include "utility.h"
#include "common.h"



int main(int argc, char ** argv){

    // 設定パラメータ (input file から読み込む) ------------
    simulation_parameters *parameters = NULL;
    char *input_file = NULL;

    if(argc < 2){
        fprintf(stderr,"usage : %s input_file\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    input_file = argv[1];
    parameters = read_parameters_from_inputfile(&input_file);

    double h = parameters->h;
    double target_temp = parameters->target_temp;
    double finish_time = parameters->finish_time;
    int norm_interval = parameters->norm_interval;
    int output_interval = parameters->output_interval;
    int snapshot_interval = parameters->snapshot_interval;
    char *output_dir = parameters->output_dir;

    // 変数 ---------------------
    int npa = 0;          // 全粒子数 (初期配置ファイルから読み込む)
    int step = 0;         // 時間ステップ
    double side = 0.0;    // 箱の大きさ (初期配置ファイルから読み込む)
    double time = 0.0;    // 時間
    double pot_erg = 0.0; // ポテンシャルエネルギー
    double knt_erg = 0.0; // 運動エネルギー
    double T = 0.0;       // 温度
    double *cd = NULL;    // 位置
    double *vl = NULL;    // 速度
    double *nvl = NULL;   // 未使用 (4章までの規格化速度。mk_init と配列確保を共有するため残している)
    double *fc = NULL;    // 力
    // double sum_fc[3] = {0.0};

    char path[512];
    FILE *fp_energy = NULL;

    // -----------------------------------------------------------

    // 初期配置の読み込み (npa が決まるので配列の確保もここで行われる)
    read_initial_configuration(parameters->init_file,&npa,&side,&cd,&vl,&nvl,&fc);
    zero_force(npa,fc);

    // 出力ディレクトリを作り、実行時のパラメータを記録する
    make_output_dir(output_dir);
    write_parameters(output_dir,parameters);
    fp_energy = open_energy_file(output_dir);
    write_xyz(output_dir,0,npa,time,cd);      // step 0 のスナップショット
    write_vtu(output_dir,0,npa,cd,vl);

    printf("npa : %d, side : %f, h : %f, target_temp : %f\n",npa,side,h,target_temp);

    // =============== main loop ===============
    printf("******* START SIMULATION *********\n");
    printf("time potential T \n");
    while(1){
        time += h;
        if(time > finish_time) break;

        update_vel_velret(npa,h,vl,fc);  // v(t) -> v(t+0.5dt)
        update_position(npa,h,cd,vl);    // r(t) -> r(t+dt)
        apply_PBC(npa,side,cd);

        pot_erg = calc_force_pot_LJ(npa, cd, fc, side);
        update_vel_velret(npa,h,vl,fc);  // v(t+dt)
        T = calc_temp(npa,vl);
        knt_erg = calc_kinetic(npa,vl);  // 温度補正の前の値 (T と同じ速度から計算する)
        if(step%norm_interval==0){ norm_temp(npa,target_temp,T,vl); }
        if(step%output_interval==0){
            write_energy(fp_energy,time,pot_erg,knt_erg,T);
            printf("%8.4f %8.4f %8.4f \n",time,pot_erg,T);
            // check_sum_force(npa,fc,sum_fc);
        }
        step ++;
        if(step%snapshot_interval==0){
            write_xyz(output_dir,step/snapshot_interval,npa,time,cd);
            write_vtu(output_dir,step/snapshot_interval,npa,cd,vl);
        }
    }
    printf("******* END SIMULATION *********\n");
    printf("time (step) potential T \n");
    printf("%8.4f (%d) %8.4f %8.4f \n",time,step,pot_erg,T);

    // 最終配置を書き出す (init_file に指定すればリスタートできる)
    snprintf(path,sizeof(path),"%s/final.dat",output_dir);
    write_configuration(path,npa,side,cd,vl);
    fclose(fp_energy);

    free(cd); free(vl); free(nvl); free(fc);
    free(parameters);

}