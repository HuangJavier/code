#include "tsp.h"

vector<Point> TSP::points;
vector<vector<double> > TSP::dis;

TSP::TSP():sem(1){
    iter_time = 0;
    bestDis = DBL_MAX;
}

void TSP::read(string filename){
    points.clear();
    ifstream fin(filename.c_str());
    int n;
    int id;
    double x,y;
    fin >> n;
    for (int i = 0;i < n;++i){
        fin >> id >> x >> y;
        points.push_back(Point(x,y));
    }
    dis = vector<vector<double> >(n, vector<double>(n, 0));
    for (int i = 0;i < n;++i){
        for (int j = i + 1;j < n;++j){
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            double d = sqrt(dx*dx + dy*dy);
            dis[i][j] = d;
            dis[j][i] = d;
        }
    }
}

//节点i到节点j的距离
double TSP::get_dis(int i, int j){
    return dis[i][j];
}
//计算路径总长度
double TSP::get_path_length(const vector<int> &path){
    double s = 0;
    for (size_t i = 0;i < path.size() - 1;++i){
        s += dis[path[i]][path[i + 1]];
    }
    s += dis[path[path.size() - 1]][path[0]];
    return s;
}
//保存路径
void TSP::save(string filename, const vector<int> &path){
    ofstream fout(filename.c_str());
    for (size_t i = 0;i < path.size();++i){
        fout << path[i] << " ";
    }
    fout << endl;
    fout << "Cost: " << get_path_length(path) << endl;
}

int TSP::get_city_size(){
    return points.size();
}

void TSP::SetBestPath(const vector<int> &path){
    sem.acquire();
    int n = get_city_size();
    vector<bool> used(n, 0);
    bool err = false;
    if (path.size() == n){
        for (int p:path){
            if (p < 0 || p >= n || used[p]){
                err = true;
                break;
            }
            used[p] = true;
        }
    }else{
        err = true;
    }
    if (err){
        bestPath.clear();
        bestDis = DBL_MAX;
    }else{
        bestPath = path;
        bestDis = get_path_length(bestPath);
    }
    sem.release();
}

double TSP::GetBestPath(vector<int> &path){
    sem.acquire();
    path = bestPath;
    sem.release();
    return bestDis;
}
