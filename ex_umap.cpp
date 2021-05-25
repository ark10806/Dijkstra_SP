#include <iostream>
#include <unordered_map>
using namespace std;

int main(){
    unordered_map<int, string> um;
    
    um.insert(make_pair(1, "A"));
    um.insert(make_pair(2, "B"));
    um.insert(make_pair(3, "C"));
    um.insert(make_pair(4, "D"));
    um.insert(make_pair(5, "E"));

    // unordered_map<int, string>::iterator iter = um.find(5);
    int key = 5;
    cout << um.find(key)->first << ' ' << um.find(key)->second << endl;

    return 0;
}