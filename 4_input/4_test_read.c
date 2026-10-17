#include <stdio.h>
#include <stdlib.h>


void read_input_file(char **input_file, int *nla, double *density, double *h, double *target_temp, int *seed, double *finish_time, int *norm_interval, int *output_interval);

int main( int argc, char** argv ){

    int nla;             // 面心立方格子の単位格子の数
    double density;      // 密度
    double h;            // 時間刻み幅
    double target_temp;  // 設定温度
    int seed;            // 乱数のシード
    double finish_time;  // 終了時刻
    int norm_interval;   // 温度の補正を行う間隔
    int output_interval; // 出力を行う間隔

    char *input_file;    // input fileの名前を管理
    int i;


    // argv の中身をチェック (実際は不要)
    for(i=0; i<argc; i++){
        printf("argv[%d] = %s \n",i,argv[i]);
    }

    // input file の中身を与える。
    input_file = argv[1];
    printf("input file : %s\n",input_file);

    read_input_file(&input_file,&nla,&density,&h,&target_temp,&seed,&finish_time,&norm_interval,&output_interval);

    // confirmation
    printf("nla = %d\n",nla);
    printf("density = %lf\n",density);
    printf("h = %lf\n",h);
    printf("target_temp = %lf\n",target_temp);
    printf("seed = %d\n",seed);
    printf("finish_time = %lf\n",finish_time);
    printf("norm_interval = %d\n",norm_interval);
    printf("output_interval = %d\n",output_interval);

    return 0;
}


void read_input_file(char **input_file, int *nla, double *density, double *h, double *target_temp, int *seed, double *finish_time, int *norm_interval, int *output_interval){

    FILE *fp;

    fp = fopen(*input_file,"r");

    // 各引数はポインタなので、`&`をつける必要はない
    fscanf(fp,"nla = %d\n",nla);
    fscanf(fp,"density = %lf\n",density);
    fscanf(fp,"h = %lf\n",h);
    fscanf(fp,"target_temp = %lf\n",target_temp);
    fscanf(fp,"seed = %d\n",seed);
    fscanf(fp,"finish_time = %lf\n",finish_time);
    fscanf(fp,"norm_interval = %d\n",norm_interval);
    fscanf(fp,"output_interval = %d\n",output_interval);

    fclose(fp);

}