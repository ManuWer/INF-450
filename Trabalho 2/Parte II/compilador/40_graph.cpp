#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void DFS(vector< vector<int> > &G, int v, vector<bool> &visit) {
    if(visit[v]) return;
    visit[v] = true;
    cout << v+1 << " ";
    for(int i:G[v]) 
        (DFS(G,i,visit));
}

int main() {
    int n, m;
    int a, b;
    cin >> n >> m;
    vector< vector<int> > G(n);
    
    for (int i=0;i<m;i++) {
        cin >> a >> b;
        G[a-1].push_back(b-1);
        G[b-1].push_back(a-1);
    }
    
    while(cin >> a && a!=0) {
    	vector<bool> visit(n,0);
    	cout << a << ": ";
    	DFS(G,a-1,visit);
    	cout << endl;
    }
   
    return 0;
}
