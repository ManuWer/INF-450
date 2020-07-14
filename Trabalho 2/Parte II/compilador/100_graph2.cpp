#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// Classe para conjuntos disjuntos
class UnionFind {
	private:
		vector<int> pai;

	public:
		UnionFind(int n);
		int find(int v);
		void join(int u,int v) { pai[find(u)] = find(v); }
		bool sameSet(int u, int v) { return find(u)==find(v); }
};

UnionFind::UnionFind(int n) {
	pai.resize(n);
	for(int i=0;i<n;i++)
		pai[i] = i;
}

// Retona o pai do conjunto que contem v
int UnionFind::find(int v) {
	if(pai[v]==v) return v;
	// Atualiza o pai de v antes de retornar por eficiencia...
	pai[v] = find(pai[v]);
	return pai[v];
}

struct Edge {
	int u, v;
	double peso;
};

bool ordPeso(const Edge &a,const Edge &b) {
	return a.peso<b.peso;
}

// Coloca arestas ordenadas por menor prioridade na floresta gerdaora minima e retorna
// o maior peso da uma aresta que esta na floresta
double kruskal(vector<Edge> &arestas, vector<bool> &arestasArvore, int numV, int sat) {
	UnionFind conjuntos(numV);
	int nArestas = 0;
	double max = 0;

	for(int i=0;i<arestas.size();i++) {
		if(!conjuntos.sameSet(arestas[i].v, arestas[i].u)) {
			arestasArvore[i] = true;
			nArestas++;

			if(arestas[i].peso>max) max = arestas[i].peso;
			if(nArestas==numV-sat) return max;

			conjuntos.join(arestas[i].v, arestas[i].u);
		}
	}

	return max;
}

double dist(int x, int y, int x1, int y1) {
	return sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
}

int main() {
	int t;
	cin >> t;

	while(t--) {
		int sat, n;
		cin >> sat >> n;
		vector<pair<int,int> > outposts(n);
		for(int i=0;i<n;i++)
			cin >> outposts[i].first >> outposts[i].second;

		vector<Edge> arestas;
		for(int i=0;i<n;i++) {
			for(int j=i+1;j<n;j++) {
				Edge e;
				e.u = i;
				e.v = j;
				e.peso = dist(outposts[i].first,outposts[i].second,outposts[j].first,outposts[j].second);
				arestas.push_back(e);
			}
		}

		// Ordenando as arestas por peso
		sort(arestas.begin(),arestas.end(),ordPeso);

		vector<bool> arestasArvore(arestas.size(),false);
		double max = kruskal(arestas,arestasArvore,n,sat);

		cout << fixed << setprecision(2) << max << endl;
	}

	return 0;
}
