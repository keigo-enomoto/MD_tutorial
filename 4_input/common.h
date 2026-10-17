
#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H
//------------------------------------------------

enum {
  NUM_FCC_PARTICLE = 4  // 面心立方格子の単位格子の内の粒子の数
};

//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------
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

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------

//------------------------------------------------
#endif

