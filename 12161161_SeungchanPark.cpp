#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <map>
#define INF 1e6
#define END -1
using namespace std;

map<int, int> zip2idx;
string* idx2name;
stack<int> zip_stk;
bool* isChecked;
int* pred;
int* idx2zip;
int* dist;
int n;
int tree_cnt;

void dijkstra(int src_idx, int dst_idx, vector<pair<int,int> > *vec){
    tree_cnt = 0;
    fill(pred, pred+n, END);
    fill(dist, dist+n, INF);
    priority_queue<pair<int, int> >pq; // distance, dst_zip
    pq.push(make_pair(0, -idx2zip[src_idx]));
    dist[src_idx] = 0;

    fill(isChecked, isChecked+n, false);

    while(!pq.empty()){
        int distance = -pq.top().first; // 인접 노드 중 가장 가까운 거리
        int curr_idx = zip2idx.find(-pq.top().second)->second; // 해당 노드의 idx
        pq.pop();

        if( (dist[curr_idx] < distance) || (distance >= INF) )
            continue;
        
        tree_cnt++;
        if(curr_idx == dst_idx){
            break;
        }
        
        for(int i=0; i<(int)vec[curr_idx].size(); i++){ // 다음 선택할 가장 가까운 거리의 노드. (FRN)
            int cost = distance + vec[curr_idx][i].first;   // src_idx~curr + curr~FRN
            int FRN_idx = vec[curr_idx][i].second;
            if(cost < dist[FRN_idx]){
                if(FRN_idx != src_idx)
                    pred[FRN_idx] = curr_idx;
                dist[FRN_idx] = cost;
                int FRN_zip = idx2zip[FRN_idx];
                pq.push(make_pair(-cost, -FRN_zip));    // cost와 idx를 저장.
            }
        }
    }
}

void prnPred(int dst){
    zip_stk.push(idx2zip[dst]);
    int curr_idx = pred[dst];
    do{
        zip_stk.push(idx2zip[curr_idx]);
        curr_idx = pred[curr_idx];
    }
    while(curr_idx != END);

    while(!zip_stk.empty()){
        cout << zip_stk.top() << ' ';
        zip_stk.pop();
    }cout << endl;
}

string zip2name(int zip){
    int idx = zip2idx.find(zip)->second;
    return idx2name[idx];
}

int main(){
    int m, q;
    cin >> n >> m >> q;
    vector<pair<int, int> > edge[m];    // dist, idx
    dist = new int[n];
    fill(dist, dist+n, INF);
    pred = new int[n];
    idx2zip = new int[n];
    idx2name = new string[n];
    isChecked = new bool[n];


    
    int zip;
    string name;
    bool wet;
    bool* isImmersed = new bool[n];
    for(int i=0; i<n; i++){
        cin >> zip >> name >> wet;
        idx2name[i] = name;
        zip2idx.insert(make_pair(zip, i));
        idx2zip[i] = zip;
        if(wet){
            isImmersed[i] = true;
        }
    }

    int src_zip, dst_zip;
    int src_idx, dst_idx;
    int distance;
    for(int i=0; i<m; i++){
        cin >> src_zip >> dst_zip >> distance;
        src_idx = zip2idx.find(src_zip)->second;
        dst_idx = zip2idx.find(dst_zip)->second;

        if(isImmersed[src_idx] || isImmersed[dst_idx]){
            distance = INF;
        }

        edge[src_idx].push_back(make_pair(distance, dst_idx));
        edge[dst_idx].push_back(make_pair(distance, src_idx));
    }

    string oper;
    for(int i=0; i<q; i++){
        cin >> oper >> src_zip >> dst_zip;
        src_idx = zip2idx.find(src_zip)->second;
        dst_idx = zip2idx.find(dst_zip)->second;
        dijkstra(src_idx, dst_idx, edge);
        // for(int j=0; j<n; j++){
        //     cout << pred[j] << ' ';
        // }cout << endl;
        // cout << "\t####";
        distance = dist[dst_idx];
        if(dist[dst_idx] >= INF){
            cout << "None" << endl;
        }
        else if(oper == "A"){
            if (src_zip == dst_zip){
                cout << "1 0 " << zip2name(src_zip) << ' ' << zip2name(src_zip) << endl;
            }
            else{
                cout << tree_cnt << ' ' << dist[dst_idx] << ' ' << zip2name(src_zip) << ' ' << zip2name(dst_zip) << endl;
            }
        }
        else{
            if(src_zip == dst_zip){
                cout << "1 " << src_zip << endl;
            }
            else{
                cout << tree_cnt << ' ';
            }
            prnPred(dst_idx);
        }
    }
    return 0;
}