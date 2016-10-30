#include "greedy.h"

Greedy::Greedy(){
    name = "Greedy Algorithm";
}

void Greedy::run(){
    while(1){
        ++iter_time;
        int n = get_city_size();
        vector<bool> visd(n, false);
        visd[0] = true;
        int last = 0;
        vector<int> path;
        path.push_back(0);
        for (int i = 1;i < n;++i){
            bool first = true;
            double minDis;
            int bestP;
            for (int j = 0; j < n;++j){
                if (!visd[j]){
                    double dis = get_dis(last, j);
                    if (dis < minDis || first){
                        first = false;
                        minDis = dis;
                        bestP = j;
                    }
                }
            }
            if (first)break;
            visd[bestP] = true;
            last = bestP;
            path.push_back(bestP);
        }
        SetBestPath(path);
    }
}
