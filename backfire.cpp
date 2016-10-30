#include "backfire.h"

BackFire::BackFire(){
    name = "BackFire Algorithm";
}

void BackFire::run(){
    srand ( time (0) );
    //初始化相关变量
    const double INIT_TEMP = 1000 ;//系统初始温度
    const double MIN_TEMP = 0.001 ;//系统最低温度
    const double COOLINGRATE = 0.98 ;//冷却速度
    double temp = INIT_TEMP ;//初始化当前温度最高
    double minTemp = MIN_TEMP ;//初始化最低温度
    double coolingRate = COOLINGRATE ;//初始化冷却速度
    int citySize = get_city_size() ;//初始化城市个数
    int MarkovChain = 10 * citySize ; //初始化Markov链

    //随机初始化路径
    vector <int> bestState ;//设置当前路径为最好路径
    vector <int> nextState ;//设置当前路径为最好路径
    bestState.clear() ;
    for ( int i = 0 ; i < citySize ; i ++ )
        bestState.push_back( i ) ;
    for ( int i = 0 ; i < citySize ; i ++ )
    {
        int x = rand() % citySize ;
        int y = rand() % citySize ;
        if ( x != y )
        {
            int tt = bestState[x] ;
            bestState[x] = bestState[y] ;
            bestState[y] = tt ;
        }
    }

    vector <int> curState = bestState ;//初始化当前路径

    while( temp > minTemp )
    {
        for ( int i = 0 ; i < MarkovChain ; i ++ )
        {
            nextState = curState ;

            //得到下一状态
            int x = rand() % citySize ;
            int y = rand() % citySize ;
            if ( x != y )
            {
                if ( x > y )
                {
                    int tt = x ;
                    x = y ;
                    y = tt ;
                }
                int choose = rand() % 3 ;
                if ( choose == 0 ) // 交换编号为x和y的城市
                {
                    int tt = nextState[x] ;
                    nextState[x] = nextState[y] ;
                    nextState[y] = tt ;
                }
                else if ( choose == 1 )// 编号为x和y的城市之间左移一位，第一位移到最后
                {
                    int tmp = nextState[x] ;
                    for ( int k = x ; k < y ; k ++ )
                    {
                        nextState[k] = nextState[k+1] ;
                    }
                    nextState[y] = tmp ;
                }
                else if ( choose == 2 ) // 编号为x和y的城市之间置逆
                {
                    reverse ( nextState.begin() + x , nextState.begin() + y ) ;
                }
            }

            double deltaEk = get_path_length ( nextState ) - get_path_length ( curState ) ;
            if ( deltaEk <= 0 )                        //如果移动后得到更优解，则总是接受移动
            {
                curState = nextState ;
                if ( get_path_length ( bestState ) >= get_path_length ( curState ) ) //记录最好结果
                    bestState = curState ;
            }
            else //如果移动后没有得到更优解，则根据一定概率移动
            {
                if ( 1000.0 / ( 1.0 + exp( - deltaEk / temp ) ) < rand() % 1001 ) // P( k ) = 1 / ( 1 + e ( - delta(Ek) / T ) )
                {
                    curState = nextState ;              //接受的移动
                }
            }
        }
        temp *= coolingRate ;  //降温退火
    }
    SetBestPath(bestState); //最后设置最好状态
}
