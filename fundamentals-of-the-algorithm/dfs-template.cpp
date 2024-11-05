/*************************************************************************
    > File Name: dfs-template.cpp
    > Author: 
    > Mail: 
    > Created Time: 2024/11/5 9:07:04
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

void print_one_result(vector<int> &buff) {
    for (int i = 0; i < buff.size(); i++) {
        if (i) cout << " ";
        cout << buff[i];
    }
    cout << endl;
    return ;
}

void dfs(int k, int n, vector<int> &buff, vector<int> &vis) {
    if (k == n) {
        print_one_result(buff);
        return ;
    }
    for (int i = 1; i <= n; i++) {
        if (vis[i] == 1) continue;
        vis[i] = 1;
        buff.push_back(i);
        dfs(k + 1, n, buff, vis);
        vis[i] = 0;
        buff.pop_back();
    }
    return ;
}

int main() {
    int n;
    cin >> n;
    vector<int> buff, vis(n + 5);
    dfs(0, n, buff, vis);
    return 0;
}
