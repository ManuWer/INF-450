#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

#define infinito 10E8

int grafo[200][200]; // Arestas do grafo
int dist[200]; // Distancia da origem aos vertices

void dijkstra(int n, int origem, int destino, bool visit[]) {
	for(int i=0;i<n;i++)
		dist[i]=infinito;

	dist[origem]=0;
	int atual;

	while(!visit[destino]) {
		int menor=infinito,menori;	
		for(int i=0;i<n;i++) {
			if(!visit[i] && dist[i]<menor) {
				menor=dist[i];
				menori=i;
			}
		}
		atual=menori;
		visit[menori]=true;

		for(int i=0;i<n;i++) {
			if(dist[atual]+grafo[atual][i] < dist[i])
				dist[i]=dist[atual]+grafo[atual][i];
		}
	}
}

bool oneChar(const string& s, const string& p) {
	int dif = 0;
	if(s.size()!=p.size()) return false;
	
	for(int i=0;i<s.size();i++) {
		if(s[i]!=p[i]) {
			dif++;
			if(dif>1) return false;
		}
	}
	
	if(dif==1) return true;
	return false;
}

int main() {

	int t;
	cin >> t;

	while(t>0) {

		vector<string> v;
		map<string,int> map;
		string s;
		string a,b;
		int vert = 0;

		for(int i=0;i<200;i++)
			for(int j=0;j<200;j++)
				grafo[i][j]=infinito;

		while(cin >> s && s!="*") {
			v.push_back(s);
			map[s] = vert;
			for(int i=0;i<v.size();i++) {			//pra cada palavra no grafo
				if(oneChar(v[i],s)) {
					grafo[i][vert]=grafo[vert][i]=1;
				}
			}
			vert++;
		}

		cin.ignore();

		while(getline(cin,s) && !s.empty()) {
			bool visit[200] = {0};
			stringstream ss(s);
			ss >> a;
			ss >> b;
			cout << a << " " << b << " ";
			dijkstra(vert,map[a],map[b],visit);
			cout << dist[map[b]] << endl;
		}

		if(t>1) cout << endl;
		t--;
	}

	return 0;
}
				
