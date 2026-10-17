#include <stdio.h>
#include <stdlib.h>


typedef struct simulation_parameters
{
    int nla;             // 面心立方格子の単位格子の数
    double density;      // 密度
    double h;            // 時間刻み幅
    double target_temp;  // 設定温度
    int seed;            // 乱数のシード
    double finish_time;  // 終了時刻
    int norm_interval;   // 温度の補正を行う間隔
    int output_interval; // 出力を行う間隔

} simulation_parameters;

simulation_parameters *read_parameters_from_inputfile(char **input_file );

int main( int argc, char** argv ){

    simulation_parameters *parameters;
    char *input_file;    // input fileの名前を管理
    int i;


    // argv の中身をチェック (実際は不要)
    for(i=0; i<argc; i++){
        printf("argv[%d] = %s \n",i,argv[i]);
    }

    // input file の中身を与える。
    input_file = argv[1];
    printf("input file : %s\n",input_file);

    parameters = read_parameters_from_inputfile(&input_file);

    // confirmation
    printf("nla = %d\n",parameters->nla);
    printf("density = %lf\n",parameters->density);
    printf("h = %lf\n",parameters->h);
    printf("target_temp = %lf\n",parameters->target_temp);
    printf("seed = %d\n",parameters->seed);
    printf("finish_time = %lf\n",parameters->finish_time);
    printf("norm_interval = %d\n",parameters->norm_interval);
    printf("output_interval = %d\n",parameters->output_interval);

    free(parameters);

    return 0;
}


simulation_parameters *read_parameters_from_inputfile(char **input_file ){

    FILE *fp;
    simulation_parameters *parameters;

    parameters = (simulation_parameters *)malloc(sizeof(simulation_parameters));
    fp = fopen(*input_file,"r");

    fscanf(fp,"nla = %d\n",&(parameters->nla));
    fscanf(fp,"density = %lf\n",&(parameters->density));
    fscanf(fp,"h = %lf\n",&(parameters->h));
    fscanf(fp,"target_temp = %lf\n",&(parameters->target_temp));
    fscanf(fp,"seed = %d\n",&(parameters->seed));
    fscanf(fp,"finish_time = %lf\n",&(parameters->finish_time));
    fscanf(fp,"norm_interval = %d\n",&(parameters->norm_interval));
    fscanf(fp,"output_interval = %d\n",&(parameters->output_interval));

    fclose(fp);
    return parameters;
}