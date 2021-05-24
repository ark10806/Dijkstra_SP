#include <iostream>
#include <vector>
#include <queue>
#define UNS 0
#define TRE 1
#define FRN 2
// #define INF 1000000000
using namespace std;

// (1, 2, 10)

int INF = 100000000;

class Dijkstra{
public:
    int **adjMat;   // main에서 완성하고 넘겨받을 것임.
    int *stat;
    int *dist;
    int *pred;
    int n;
    int s;
    queue<int> adj_que;

    Dijkstra(int **adjMat, int n, int s){
        this->adjMat = adjMat;
        this->n = n;
        this->s = s;
        stat = new int[n];
        dist = new int[n];
        pred = new int[n];

        for(int i=0; i<n; i++){
            stat[i] = UNS;    // 모든 노드를 UNSEEN으로 초기화
            dist[i] = INF;      // 모든 노드의 거리를 무한대로 초기화
            pred[i] = -1;
        }
    }
    void init(){
        stat[s] = TRE;
        dist[s] = 0;
        getAdj(s);
        int curr;
        while(!adj_que.empty()){
            curr = adj_que.front(); adj_que.pop();
            stat[curr] = FRN;
            pred[curr] = s;
            dist[curr] = getDist(s, curr);
        }
    }
    void prn(){
        int hi;
        cin >> hi;
        cout << endl << "----------------------" << endl;
        for(int i=0; i<n; i++){
            if(stat[i]==UNS){
                cout << "UNS" << '\t';
            }
            if(stat[i]==TRE){
                cout << "TRE" << '\t';
            }
            if(stat[i]==FRN){
                cout << "FRN" << '\t';
            }
        }cout << endl;


        cout << "----------------------" << endl;
    }
    void calcPath(){
        init();
        while(!isComplete()){
            prn();
            int min_FRN = getMinIdx();
            stat[min_FRN] = TRE;
            if(dist[min_FRN] > getDist(s, min_FRN)){
                decreasekey(min_FRN, getDist(s, min_FRN));
                getAdj(min_FRN);
                while(!adj_que.empty()){
                    int curr = adj_que.front(); adj_que.pop();
                    stat[curr] = FRN;
                    pred[curr] = min_FRN;
                    dist[curr] = getDist(s, curr);
                }
            }
        }
        for(char c='A'; c<='I'; c++){
            cout << c << '\t';
        }cout << endl;
        for(int i=0; i<n; i++){
            cout << dist[i] << '\t';
        }cout << endl;
    }
    void decreasekey(int idx, int distance){
        dist[idx] = distance;
    }
    bool isComplete(){
        for(int i=0; i<n; i++){
            if(stat[i] == FRN){   //  || status[i] == UNS 는 없어도 되나?
                return false;
            }
        }
        return true;
    }
    int getMinIdx(){
        int min;
        bool isFRN = false;
        for(int i=0; i<n; i++){
            if(stat[i]==FRN){
                if(!isFRN){
                    min = dist[i];
                    isFRN = true;
                }
                if(dist[i] < min)
                    min = dist[i];
            }
        }
        return min;
    }
    void getAdj(int node){
        clearQue();
        for(int i=0; i<n; i++){
            if(adjMat[node][i] != 0)
                adj_que.push(i);
        }
    }
    int getDist(int src, int dst){
        // stV: starting V   src: Terminal member of TRE     dst: FRN
        int pred_idx = pred[dst];
        if(pred_idx == -1){
            pred_idx = src;
        }
        return dist[pred_idx] + adjMat[src][dst];
    }
    void clearQue(){
        queue<int> empty;
        swap(this->adj_que, empty);
    }
};

int main(){
    int n = 9;
    int **adjMat = new int*[n];
    for(int i=0; i<n; i++){
        adjMat[i] = new int[n];
        memset(adjMat[i], 0, sizeof(int)*n);
    }

    int arr2d[9][9] = {{0,2,INF,INF,INF,9,5,INF,INF},
        {2,0,4,INF,INF,INF,6,INF,INF},
        {INF,4,0,2,INF,INF,INF,5,INF},

        {INF,INF,2,0,1,INF,INF,1,INF},
        {INF,INF,INF,1,0,6,INF,INF,3},
        {9,INF,INF,INF,6,0,INF,INF,1},

        {5,6,INF,INF,INF,INF,0,5,2},
        {INF,INF,5,1,INF,INF,5,0,4},
        {INF,INF,INF,INF,3,1,2,4,0}};

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            adjMat[i][j] = arr2d[i][j];
            if(adjMat[i][j] == INF)
                cout << "INF" << '\t';
            else
                cout << adjMat[i][j] << '\t';
        }cout << endl;
    }

    Dijkstra di(adjMat, n, 0);
    di.calcPath();

    return 0;
}