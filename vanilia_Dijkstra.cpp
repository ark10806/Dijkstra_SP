#include <iostream>
#include <queue>
#include <stack>
#define UNS 0
#define TRE 1
#define FRN 2
#define END 0
using namespace std;

int INF = 100000000;

class Dijkstra{
public:
    int **adjMat;   // main에서 완성하고 넘겨받을 것임.
    int *stat;
    int *dist;
    int *pred;
    int *tmp_pred;
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
        tmp_pred = new int[n];

        for(int i=0; i<n; i++){
            stat[i] = UNS;    // 모든 노드를 UNSEEN으로 초기화
            dist[i] = INF;      // 모든 노드의 거리를 무한대로 초기화
            pred[i] = END;
            tmp_pred[i] = END;
        }
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
    void init(){
        stat[s] = TRE;
        dist[s] = 0;
        pred[s] = END;
        tmp_pred[s] = END;
        getAdj(s);
        int curr;
        while(!adj_que.empty()){
            curr = adj_que.front(); adj_que.pop();
            stat[curr] = FRN;
            pred[curr] = s;
            dist[curr] = getDist(s, curr);
        }
        prn();
    }
    
    void calcPath(){
        init();
        while(!isComplete()){
            string hi;
            getline(cin, hi);
            int min_FRN = getMinIdx();
            cout << endl << endl << "----------------------------" << endl;
            cout << "min: " << (char)(min_FRN+'A') << endl;
            stat[min_FRN] = TRE;
            // if(getDist(s,min_FRN) > getDist(tmp_pred[min_FRN], min_FRN)){
            //     pred[min_FRN] = tmp_pred[min_FRN];
            //     decreasekey(min_FRN, getDist(pred[min_FRN], min_FRN));
            // }
            // decreasekey(min_FRN, getDist(s, min_FRN));  //B의 dist를 //A~B 2
            getAdj(min_FRN);
            int curr;
            while(!adj_que.empty()){
                curr = adj_que.front(); adj_que.pop(); // curr은 min_FRN의 adjacent Nodes.
                if(stat[curr] == UNS)
                    stat[curr] = FRN;
                cout << (char)(tmp_pred[curr] + 'A') << (char)(curr + 'A') << ' '  << dist[curr] << " <> " << getDist(tmp_pred[curr], curr) << endl;
                if(dist[curr] > getDist(tmp_pred[curr], curr)){
                    decreasekey(curr, getDist(tmp_pred[curr], curr));
                    pred[curr] = tmp_pred[curr];
                }
            }
            prn();
        }
        
        cout << endl << "RESULT#########" << endl;
        prn();

        cout << "pred####"<< endl;
        for(char c='A'; c<='I'; c++){
            cout << c << '\t';
        }cout << endl;
        cout << endl;
        for(int i=0; i<n; i++)
            cout << (char)(pred[i] + 'A') << '\t';
        cout << endl;
        for(int i=0; i<n; i++)
            cout << pred[i] << '\t';
        cout << endl << endl;
        for(int i=0; i<n; i++){
            getPath(i);
        }
    }
    void getPath(int node){
        int curr = node;
        stack<int> path;
        cout << "PATH: A->" << (char)(node+'A') << ": "  << getDist(pred[node], node) << endl;
        while(curr != END){
            // cout << (char)(pred[curr] + 'A') << ' ';
            path.push(pred[curr]);
            curr = pred[curr];
        }
        while(!path.empty()){
            if(path.top() == s)
                cout << 'A' << "->";
            else
                cout << (char)(path.top()+'A') << "->";
            path.pop();
        }cout << (char)(node + 'A') << endl << endl;
        // cout << pred[curr] << endl << endl;
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
        int minIdx;
        bool isFRN = false;
        for(int i=0; i<n; i++){
            if(stat[i]==FRN){
                if(!isFRN){
                    minIdx = i;
                    isFRN = true;
                }
                if(dist[i] < dist[minIdx])
                    minIdx = i;
            }
        }
        return minIdx;
    }
    void getAdj(int node){
        clearQue();
        cout << "adjs: ";
        for(int i=0; i<n; i++){
            if(adjMat[node][i] != INF && i!=node && stat[i]!=TRE){
                adj_que.push(i);
                if(node != s)
                    tmp_pred[i] = node;
                cout << (char)(i+'A') << '\t';
            }
        }cout << endl;
    }
    int getDist(int src, int dst){
        //src: Terminal member of TRE     dst: FRN
        return dist[src] + adjMat[src][dst];
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

    int arr2d[9][9] =   {{0,2,INF,INF,INF,9,5,INF,INF},
                        {2,0,4,INF,INF,INF,6,INF,INF},
                        {INF,4,0,2,INF,INF,INF,5,INF},

                        {INF,INF,2,0,1,INF,INF,1,INF},
                        {INF,INF,INF,1,0,6,INF,INF,3},
                        {9,INF,INF,INF,6,0,INF,INF,1},

                        {5,6,INF,INF,INF,INF,0,5,2},
                        {INF,INF,5,1,INF,INF,5,0,4},
                        {INF,INF,INF,INF,3,1,2,4,0}};

    int cnt = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            adjMat[i][j] = arr2d[i][j];
            if(adjMat[i][j] == INF)
                cout << "F" << ' ';
            else{
                cout << adjMat[i][j] << ' ';
                if(adjMat[i][j]!=0){
                    cnt++;
                }
            }
        }cout << endl;
    }
    cout << "cnt: " << cnt << endl;
    cout <<"hii" << endl;

    Dijkstra di(adjMat, n, 0);
    di.calcPath();

    return 0;
}