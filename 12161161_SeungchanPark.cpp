#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <string.h>
#define UNS 0
#define TRE 1
#define FRN 2
#define END 0
using namespace std;

int INF = 1000000;


/*
1. mapping table: mat_idx와 지역번호, 지역번호와 지역 이름 엮기
    1) 지역번호로 지역이름 찾으려면 일단 BruteForce, 안되면 RB-tree
    2) 지역정보는 class?
2. adjMat생성 함수 생성.
    1) 최단거리: mat_idx로 지역 번호를 찾아 지역이름을 출력해야함.
    2) 최단경로: mat_idx로 지역번호를 찾아 출력. 
        -> 동적할당 이용.
    3) distance 를 adjMat에 기입하기 위해서 -> 지역번호로 mat_idx를 찾아야 함.
        -> map 이용.
3. starting vertex 설정 가능하게.
*/


class Geometry{ 
/*
1. insert
    1) 지역번호와 지역이름 쌍:   um[지역번호] = 지역이름     
    2) 지역번호를 idxs에 넣기: idxs[mat_idx] = 지역번호
    3) zip으로 mat_idx:    um[zip]=mat_idx
2. adjMat 생성
    1) 
2. 
*/
public:
    int **adjMat;
    unordered_map<int, string> zip2name;
    unordered_map<int, int> zip2idx;
    int* idx2zip;
    bool* isImmersed;
    int size;
    int cnt;
    Geometry(int size){
    /*
    n: 노드의 수
    */  
        this->size = size;
        adjMat = new int*[size];
        for(int i=0; i<size; i++){
            adjMat[i] = new int[size];
            memset(adjMat[i], INF, sizeof(int)*size);
            adjMat[i][i] = 0;
        }
        idx2zip = new int[size];
        isImmersed = new bool[size];
        cnt = 0;
    }
    void insert_places(int zip, string name, bool wet){
        zip2name.insert(make_pair(zip, name));
        zip2idx.insert(make_pair(zip, cnt));
        idx2zip[cnt] = zip;
        isImmersed[cnt] = wet;
        cnt++;
    }
    void make_roads(int src_zip, int dst_zip, int distance){
        int src = zip2idx.find(src_zip)->second;
        int dst = zip2idx.find(dst_zip)->second;
        if(isImmersed[src] || isImmersed[dst]){
            adjMat[src][dst] = INF;
            adjMat[dst][src] = INF;
        }
        else{
            adjMat[src][dst] = distance;
            adjMat[dst][src] = distance;
        }
    }
    string get_name(int idx){
        int zip = idx2zip[idx];
        return zip2name.find(zip)->second;
    }
    int** get_adjMat(){
        return adjMat;
    }
};

class Dijkstra{
public:
    int **adjMat;   // main에서 완성하고 넘겨받을 것임.
    int *stat;
    int *dist;
    int *pred;
    int *tmp_pred;
    int n;
    int s;
    int treeNum;
    queue<int> adj_que;
    Geometry* geo;

    Dijkstra(int **adjMat, int n, Geometry* geo){
        this->adjMat = adjMat;
        this->n = n;
        this->geo = geo;
    }
    void prn(){
        for(char c='A'; c<='I'; c++){
            cout << c << '\t';
        }cout << endl;
        for(int i=0; i<n; i++){
            if(stat[i]==UNS)
                cout << "UNS" << '\t';
            if(stat[i]==TRE)
                cout << "TRE" << '\t';
            if(stat[i]==FRN)
                cout << "FRN" << '\t';
        }cout << endl;

        for(int i=0; i<n; i++){
            if(dist[i] == INF)
                cout << "INF" << '\t';
            else
                cout << dist[i] << '\t';
        }
        cout << "----------------------" << endl;
    }
    void init(int s){
        stat = new int[n];
        dist = new int[n];
        pred = new int[n];
        tmp_pred = new int[n];

        for(int i=0; i<n; i++){
            stat[i] = UNS;    // 모든 노드를 UNSEEN으로 초기화
            dist[i] = INF;      // 모든 노드의 거리를 무한대로 초기화
            pred[i] = END;
            tmp_pred[i] = END;
        }
        this->s = s;
        stat[s] = TRE;
        dist[s] = 0;
        pred[s] = END;
        tmp_pred[s] = END;
        treeNum = 1;
        getAdj(s);
        int curr;
        while(!adj_que.empty()){
            curr = adj_que.front(); adj_que.pop();
            stat[curr] = FRN;
            pred[curr] = s;
            dist[curr] = getDist(s, curr);
        }
    }
    
    void calcPath(int s, int terminal){
        init(s);
        while(!isComplete()){
            int min_FRN = getMinIdx();
            stat[min_FRN] = TRE;
            treeNum++;
            if(min_FRN == terminal) return;
            getAdj(min_FRN);
            int curr;
            while(!adj_que.empty()){
                curr = adj_que.front(); adj_que.pop(); // curr은 min_FRN의 adjacent Nodes.
                if(stat[curr] == UNS)
                    stat[curr] = FRN;
                if(dist[curr] > getDist(tmp_pred[curr], curr)){
                    decreasekey(curr, getDist(tmp_pred[curr], curr));
                    pred[curr] = tmp_pred[curr];
                }
            }
        }
    }
    void getPath(int node){
        int curr = node;
        stack<int> path;
        cout << getTreeNum() << ' ';
        while(curr != s){
            path.push(pred[curr]);
            curr = pred[curr];
        }
        while(!path.empty()){
            cout << geo->idx2zip[path.top()] << ' ';
            path.pop();
        }cout << geo->idx2zip[node] << endl;
    }
    void decreasekey(int idx, int distance){
        dist[idx] = distance;
    }
    bool isComplete(){
        for(int i=0; i<n; i++){
            if(stat[i] == FRN)   //  || status[i] == UNS 는 없어도 되나?
                return false;
        }
        return true;
    }
    int getMinIdx(){
        int minIdx = 0;
        bool isFRN = false;
        for(int i=0; i<n; i++){
            if(stat[i]==FRN){
                if(!isFRN){
                    minIdx = i;
                    isFRN = true;
                }
                if(dist[i] < dist[minIdx])
                    minIdx = i;
                else if(dist[i] == dist[minIdx]){
                    int a = geo->idx2zip[i];
                    int b = geo->idx2zip[minIdx];
                    if(a < b) minIdx = i;
                }
            }
        }
        return minIdx;
    }
    void getAdj(int node){
        clearQue();
        for(int i=0; i<n; i++){
            if(adjMat[node][i] != INF && i!=node && stat[i]!=TRE){
                adj_que.push(i);
                if(node != s)
                    tmp_pred[i] = node;
            }
        }
    }
    int getDist(int src, int dst){
        //src: Terminal member of TRE     dst: FRN
        return dist[src] + adjMat[src][dst];
    }
    int getFinalDist(int dst){
        return dist[dst];
    }
    int getTreeNum(){
        return treeNum;
    }
    void clearQue(){
        queue<int> empty;
        swap(this->adj_que, empty);
    }
};

int main(){
    int n, m, q;
    int src, dst, distance;
    string oper, name;
    bool isWet;

    cin >> n >> m >> q;
    // n = 9;
    // m = 11;
    // q = 7;
    Geometry* geo = new Geometry(n);
    // geo->insert_places(0, "a", false);
    // geo->insert_places(1, "b", false);
    // geo->insert_places(2, "c", false);

    // geo->insert_places(3, "d", false);
    // geo->insert_places(4, "e", false);
    // geo->insert_places(5, "f", false);

    // geo->insert_places(6, "g", false);
    // geo->insert_places(7, "h", false);
    // geo->insert_places(8, "i", true);

    // geo->make_roads(0,6,450);
    // geo->make_roads(0,1,1000);
    // geo->make_roads(6,5,950);
    // geo->make_roads(5,1,500);
    // geo->make_roads(1,2,5000);
    // geo->make_roads(1,3,900);
    // geo->make_roads(2,3,3);
    // geo->make_roads(2,4,2);
    // geo->make_roads(7,0,3);
    // geo->make_roads(7,8,1);
    // geo->make_roads(8,5,1);

    for(int i=0; i<n; i++){
        cin.ignore();
        cin >> src >> name >> isWet;
        geo->insert_places(src, name, isWet);
    }

    for(int i=0; i<m; i++){
        cin >> src >> dst >> distance;
        geo->make_roads(src, dst, distance);
    }
    Dijkstra dik(geo->get_adjMat(), n, geo);
    for(int i=0; i<q; i++){
        cin >> oper >> src >> dst;
        src = geo->zip2idx.find(src)->second;
        dst = geo->zip2idx.find(dst)->second;
        if(geo->isImmersed[src] || geo->isImmersed[dst]){
            cout << "None" << endl;
            continue;
        }

        if(oper == "A"){
            dik.calcPath(src, dst);
            if(dik.getFinalDist(dst) > INF)
                cout << "None" << endl;
            else
                cout << dik.getTreeNum() << ' ' << dik.getFinalDist(dst) << ' ' << geo->get_name(src) << ' ' << geo->get_name(dst) << endl;
        }
        else if(oper == "B"){
            dik.calcPath(src, dst);
            if(dik.getFinalDist(dst) > INF)
                cout << "None" << endl;
            else
                dik.getPath(dst);
        }
        else
            cout << "wrong operations" << endl;
    }
    return 0;
}