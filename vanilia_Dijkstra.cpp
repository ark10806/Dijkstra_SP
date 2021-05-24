#include <iostream>
#include <vector>
#include <queue>
#define UNS 0
#define TRE 1
#define FRN 2
#define INF 1000000000
using namespace std;

// (1, 2, 10)

class Dijkstra{
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
    void calcPath(int s){
        init(s);
        while(!isComplete()){
            int min_FRN = getMinIdx();
            stat[min_FRN] = TRE;
            if(dist[min_FRN] > getDist(s, ))
        }
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
        int min = 0;
        for(int i=0; i<n; i++){
            if(stat[i]==FRN && dist[i]<min){
                min = dist[i];
            }
        }
    }
    int getAdj(int node){
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

    
    // void get_path(){
    //     status_arr[0] = TRE;
    //     int curr = 0;
    //     while(!isComplete()){
    //         getAdj(curr);

    //     }
    // }
    // void getFrn(int node){

    // }
    // void getAdj(int node){
    //     queue<int> adj_que;
    //     for(int i=0; i<n; i++){
    //         if(adjMat[node][i] != 0)
    //             adj_que.push(adjMat[node][i]);
    //     }

    //     for(int i=0; i<n; i++){
    //         if(adjMat[node][i] != 0)
    //     }
    // }
    // bool isComplete(){
    //     for(int i=0; i<n; i++){
    //         if(status_arr[i] == FRN || status_arr[i] == UNS)
    //             return false;
    //     }
    //     return true;
    // }
};

int main(){
    int n, m, q;
    cin >> n >> m >> q;
    int **adjMat = new int*[n];
    for(int i=0; i<n; i++){
        adjMat[i] = new int[n];
    }

    int x, y, w;
    for(int i=0; i<m; i++){
        cin >> x >> y >> w;
        adjMat[x][y] = w;
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << adjMat[i][j] << ' ';
        }
        cout << endl;
    }


    return 0;
}