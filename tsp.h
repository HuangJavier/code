#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <set>
#include <algorithm>
#include <cfloat>
#include <QThread>
#include <QSemaphore>
using namespace std;

struct Point{
    double x,y;
    Point(){}
    Point(double x, double y):x(x),y(y){}
};


class TSP : public QThread{
public:
    //读取tsp文件，第一行为节点数
    //第二行开始，节点(ID+1), x, y
    //节点编号从0开始
    static void read(string filename);
    //节点i到节点j的距离
    static double get_dis(int i, int j);
    //计算路径总长度
    static double get_path_length(const vector<int> &path);
    // 返回城市数量
    static int get_city_size();
    //保存路径
    void save(string filename, const vector<int> &path);
    void SetBestPath(const vector<int> &path);
    double GetBestPath(vector<int> &path);
    TSP();
public:
    string name;
public:
    static vector<Point> points;
private:
    static vector<vector<double> > dis;
private:
    vector<int> bestPath;
    double bestDis;
    QSemaphore sem;
public:
    int iter_time;
};


#endif // TSP_H
