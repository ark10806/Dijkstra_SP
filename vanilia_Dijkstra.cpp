#include <iostream>
#include <vector>
#include <queue>
#define UNS 0
#define TRE 1
#define FRN 2
#define INF -1
using namespace std;

// (1, 2, 10)

class Dijkstra{
    int **adjMat;   // main에서 완성하고 넘겨받을 것임.
    int *status_arr;
    int *dist_arr;
    int n;
    queue<int> adj_que;

    Dijkstra(int **adjMat, int n){
        this->adjMat = adjMat;
        this->n = n;
        status_arr = new int[n];
        dist_arr = new int[n];

        for(int i=0; i<n; i++){
            status_arr[i] = UNS;
            dist_arr[i] = INF;
        }
    }
    
    void get_path(){
        status_arr[0] = TRE;
        int curr = 0;
        while(!isComplete()){

        }
    }
    void getAdj(int node){
        queue<int> adj_que;
        for(int i=0; i<n; i++){
            if(adjMat[node][i] != 0)
                adj_que.push(adjMat[node][i]);
        }
    }
    bool isComplete(){
        for(int i=0; i<n; i++){
            if(status_arr[i] == FRN || status_arr[i] == UNS)
                return false;
        }
        return true;
    }
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