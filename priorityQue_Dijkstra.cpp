#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <map>
#define INF 1e6 // 최대 거리의 상한 설정
#define END -1  // predecessor를 담을 pred에 사용. 해당 값을 pred에 가지고 있는 원소는 시작점임.
using namespace std;


/*
-용어 설명
    zip: 지역번호
    idx: 각 지역은 입력된 순서대로 0~n으로 할당됨
    name: 지역이름
*/
map<int, int> zip2idx;  // zip으로 idx를 찾기 위함. 탐색시간이 빨라야하므로 Red Black Tree를 활용하는 map 사용.
string* idx2name;   // idx로 지역이름을 찾는 배열. idx를 알고 있다면 접근시간이 O(1)-time이다.
int* idx2zip;   // idx로 zip을 찾는 배열. 위의 idx2name과 같은 맥락으로 접근시간이 O(1)-time이다.

bool* isChecked;    // 방문하는 노드가 이전에 방문한 적이 있나 확인하기 위한 배열. 
stack<int> zip_stk; // 시작점부터 현재까지의 경로를 저장하기 위한 stack 자료구조. pred배열을 활용하여 나탄냄.
int* pred;      // src~dst간의 경로를 구하기 위해 각 노드의 predecessor를 저장.
int* dist;  // 시작 노드부터 현재 노드까지의 최단경로를 저장할 배열
int n;
int tree_cnt;   // 도착지를 구하기 까지 TREE set에 포함된 노드의 수

void dijkstra(int src_idx, int dst_idx, vector<pair<int,int> > *vec){
    tree_cnt = 0;
    fill(pred, pred+n, END);
    fill(dist, dist+n, INF);
    // maxPriority인 pq에는 최단거리를 반환하고, 특히 최단거리가 같을 경우 지역번호가 낮은 것을 선택해야 한다.
    // 따라서 {-distance, -dst_zip} 의 형태로 저장한다.
    priority_queue<pair<int, int> >pq; // {-distance, -dst_zip}
    pq.push(make_pair(0, -idx2zip[src_idx]));   
    dist[src_idx] = 0;  // 시작점의 distance를 0으로 설정

    fill(isChecked, isChecked+n, false);

    while(!pq.empty()){
        int distance = -pq.top().first; // 인접 노드 중 가장 가까운 거리
        int curr_idx = zip2idx.find(-pq.top().second)->second; // 해당 노드의 idx
        pq.pop();

        if( (dist[curr_idx] < distance) || (distance >= INF) )
            continue;   // distance가 상한인 INF 이상이거나 기존 최단거리보다 큰 값일 경우 업데이트하지 않는다.
        
        tree_cnt++;
        if(curr_idx == dst_idx){
            break;
        }
        
        for(int i=0; i<(int)vec[curr_idx].size(); i++){ // 다음 선택할 가장 가까운 거리의 노드. (Fringe를 모두 탐색한다.)
            int cost = distance + vec[curr_idx][i].first;   // src_idx~curr + curr~FRN 즉 시작점부터 현재 노드까지의 최단거리를 계산.
            int FRN_idx = vec[curr_idx][i].second;          // Fringe노드의 idx를 구한다.

            if(cost < dist[FRN_idx]){   // 새로 구한 거리(cost)가 기존 거리(dist[FRN_idx])보다 작으면 값을 갱신.
                if(FRN_idx != src_idx)
                    pred[FRN_idx] = curr_idx;   // FRN노드의 predecessor를 저장한다. 계속해서 최단거리로 갱신되므로, 현재 임시적으로 저장한 pred가 최단이 아니어도 된다.
                dist[FRN_idx] = cost;
                int FRN_zip = idx2zip[FRN_idx];
                pq.push(make_pair(-cost, -FRN_zip));    // cost와 zip을 저장.
            }
        }
    }
}

void prnPred(int dst){
    // 출발지(src) ~ 도착지(dst) 경로 상에 있는 노드를 출력.
    zip_stk.push(idx2zip[dst]);
    int curr_idx = pred[dst];
    do{
        zip_stk.push(idx2zip[curr_idx]);
        curr_idx = pred[curr_idx];
    }
    while(curr_idx != END);

    // pred를 이용하면 dst->src의 올라가는 순서이므로, LIFO구조를 따르는 Stack을 이용하여 의도한대로 나타냄.
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

        if(isImmersed[src_idx] || isImmersed[dst_idx]){ // 출발지 혹은 도착지가 침수지역이라면 distance를 INF로 설정.
            distance = INF;
        }

        edge[src_idx].push_back(make_pair(distance, dst_idx));  // 각각 인접 정보를 저장.
        edge[dst_idx].push_back(make_pair(distance, src_idx));  // 무향그래프이므로 출발지, 도착지에 각각 인접정보를 부여한다.
    }

    string oper;
    for(int i=0; i<q; i++){
        cin >> oper >> src_zip >> dst_zip;
        src_idx = zip2idx.find(src_zip)->second;
        dst_idx = zip2idx.find(dst_zip)->second;
        dijkstra(src_idx, dst_idx, edge);

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