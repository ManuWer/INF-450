#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;

#define infinito 10E8

long long int global_target;
string global_set;
string global_result;
bool stop = false;

bool equation(char v, char w, char x, char y, char z, long long int &target) {
	long long int resp;
	resp = (int(v)-64) - (int(w)-64)*(int(w)-64) + (int(x)-64)*(int(x)-64)*(int(x)-64) - (int(y)-64)*(int(y)-64)*(int(y)-64)*(int(y)-64) + (int(z)-64)*(int(z)-64)*(int(z)-64)*(int(z)-64)*(int(z)-64);

	if(resp==target) 
		return true;
	
	return false;
}

bool equation2(string str, long long int &target) {
	return equation(str[0], str[1], str[2], str[3], str[4], global_target);
}

void permuting(vector<vector<int> > &result, vector<int> nums, vector<int> perm) {
	if (!stop) {
	    if (perm.size() == 5) {
					string substring;
					for (int i = 0 ; i < perm.size(); i++) {
						substring.push_back(global_set[perm[i]]);
					}
					if (equation2(substring, global_target)) {
						global_result = substring;
						stop = true;
					}
	        result.push_back(perm);
	    } else {
	        for (int i = 0; i < nums.size(); i++) {
	                int aux = nums[i];
	                perm.push_back(nums[i]);
	                nums.erase(nums.begin() + i);
	                permuting(result, nums, perm);
	                nums.insert(nums.begin() + i, aux);
	                perm.pop_back();
	        }
  		}
	}
}

vector<vector<int> > permute(vector<int>& nums) {
    vector<vector<int> > result;
    vector<int> perm;
    permuting(result, nums, perm);

    return result;
}

void combs(long long int &target, string set) {
	vector<int> nums;
	stop = false;
	for (int i = set.size()-1; i >= 0; i--) {
		nums.push_back(i);
	}
	vector< vector<int> > combination_indexes = permute(nums);

	return;
}

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

bool balanced(const string &str, stack<int> &erro) {
    stack<char> s;
    
    for(int i=0;i<str.size();i++) {
        char c = str[i];
        
        switch(c) {
            case '(':
				erro.push(i+1);
				if(i<str.size()-1 && str[i+1]=='*') {
					s.push(':');
					i++;
				}
				else s.push(c);
				break;
            case '[':
            case '{':
			case '<':
                s.push(c);
				erro.push(i+1);
                break;
            case ')':
                if(s.empty() || s.top()!='(') {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
            case ']':
                if(s.empty() || s.top()!='[')  {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
            case '}': 
                if(s.empty() || s.top()!='{')  {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
			case '>': 
                if(s.empty() || s.top()!='<') {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
			case '*':
				if(s.empty()) return false;
				if(i==str.size()-1 && s.top()==':')  {
					erro.push(i+1);
					return false;
				}
				if(i<str.size()-1 && str[i+1]==')') {
					if(s.top()!=':')  {
						erro.push(i+1);
						return false;
					}
					s.pop();
					erro.pop(); 
					i++;
				}
				break;
        }
    }

    if(!s.empty()) return false;
    return true;
}
		
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

double dist_eucli(int x, int y, int x1, int y1) {
	return sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
}

void graphReverse(vector< vector<int> > &G, vector< vector<int> > &revG) {
	for(int i=0;i<G.size();i++)
		for(int v:G[i])
			revG[v].push_back(i);
}

void DFS(vector< vector<int> > &G, int v, vector<bool> &visit) {
    if(visit[v]) return;
    visit[v] = true;
    //cout << v << " ";
    for(int i:G[v]) 
        (DFS(G,i,visit));
}

int tree() {

	map<string,double> tree;
	string aux;
	int c;
	cin >> c;

	//le as duas primeiras linhas em branco e ignora
	getline(cin,aux);
	getline(cin,aux);

	while (c>0) {
		tree.clear();
		int cont = 0;

		while (getline(cin,aux) && !aux.empty()) {
			cont++;
			pair<map<string,double>::iterator,bool> it;
			it = tree.insert (pair<string,double> (aux,1));
			//se insert retorna falso o elemento ja esta no map
			if (!it.second)
				it.first->second++;
		}

		for (map<string,double>::iterator i = tree.begin();i!=tree.end();i++) {
			double a = i->second*100.0/cont;	
			i->second = a;
		}

		for (map<string,double>::iterator i = tree.begin();i!=tree.end();i++)
			cout << i->first << " " << fixed << setprecision(4) << i->second << endl;
		
		c--;
		if (c>0) cout << endl;
	}

	return c;
}

bool vit() {

	double a, r;
	string u, v;
	vector<string> insig;
	
	cin >> a >> u >> r;
	getline(cin, v);
	
	while (a>=0) {
		double p;
		
		p = (a*100.0)/r;

		v.erase(0,1);

		if (p<1.0)
			insig.push_back(v);
		else
			cout << v << " " << fixed << setprecision(1) << a << " " << u << " " << fixed << setprecision(0) << p << "%\n";

		cin >> a >> u >> r;
		getline(cin, v);
	}

	cout << "Provides no significant amount of:\n";

	for (int i=0; i<insig.size(); i++)
		cout << insig[i] << endl;

	return true;
}

void judge() {

	int n;
	int m;

	cin >> n;

	while (n!=0) {
		cin >> m;

		string sol[n];
		string input[m];
		bool equal = true;
		
		for (int i=0; i<n; i++)
			getline(cin,sol[i]);

		for (int i=0; i<m; i++)
			getline(cin,input[i]);
				
		for (int i=0; i<n; i++) {
			if (sol[i].compare(input[i])==0)
				cout << "Accepted" << endl;
			else {
				stringstream ssol(sol[i]);
				stringstream sinput(input[i]);
    			vector<int> vsol;
				vector<int> vinput;
    			int a;
    			while(ssol >> a)
        			vsol.push_back(a);
				while(sinput >> a)
					vinput.push_back(a);
				if (vsol.size()!=vinput.size()) equal = false;
				else {
					for (int i=0;i<vsol.size();i++)
						if (vsol[i]!=vinput[i])
							equal = false;
				}
				if (equal) 
					cout << "Presentation Error" << endl;
				else cout << "Wrong Answer" << endl;
			}
		}
	}

}

int DFS(vector< vector<char> > &G, int l, int c, vector <vector<bool> > &visit) {		
    if(visit[l][c]) return 0;
    visit[l][c] = true;
	if(G[l][c]!='W') return 0;

	return 1 + DFS(G,l-1,c-1,visit) +
			   DFS(G,l,c-1,visit) +
			   DFS(G,l+1,c-1,visit) +
			   DFS(G,l-1,c,visit) +
			   DFS(G,l+1,c,visit) +
			   DFS(G,l-1,c+1,visit) +
			   DFS(G,l,c+1,visit) +
			   DFS(G,l+1,c+1,visit);
}

int visitaMapa() {

	int t;
	string s;
	cin >> t;
	cin.ignore();
	getline(cin,s); 	//blank line

	while(t>0) {

		int l,c;
		vector< vector<char> > grid;
		getline(cin,s);
		vector<char> v(s.size()+2,'.');    //contorno da matriz para nao acessar pos invalida
		grid.push_back(v);

		do {
			if(isdigit(s[0])) break;
			vector<char> aux;
			aux.push_back('.');
			for(int i=0;i<s.size();i++)
				aux.push_back(s[i]);
			aux.push_back('.');
			grid.push_back(aux);
		} while(getline(cin,s));

		grid.push_back(v);
	
		do {
			stringstream ss(s);
			ss >> l;
			ss >> c;
			vector< vector<bool> > visit(grid.size(), vector<bool>(grid[0].size(),0));
			cout << DFS(grid,l,c,visit) << endl;
		} while(getline(cin,s) && !s.empty());
	
		if(t>1) cout << endl;
		t--;
		
	}
		
	return 1;
}

int path() {

    int n, m;
    int a, b, p;
    
	while(cin >> n >> m && (n!=0 || m!=0)) {
		vector< vector<int> > G(n);
		vector< vector<int> > revG(n);
		bool sc = true;

		for (int i=0;i<m;i++) {
		    cin >> a >> b >> p;
		    G[a-1].push_back(b-1);
		    if(p==2) G[b-1].push_back(a-1);
		}
   
    	vector<bool> visit(n,0);
    	DFS(G,0,visit);

		for(int i=0;i<visit.size();i++) {
			if(!visit[i]) sc = false;
			visit[i] = false;
		}

		graphReverse(G,revG);
		DFS(revG,0,visit);

		for(bool i:visit) 
			if(!i) sc = false;
		cout << sc << endl;
    }

    return m;
}

void merge (int arr[], int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L [] and R [] */
	for (i=0;i<n1;i++)
		L[i] = arr[l+i];
	for (j=0;j<n2;j++)
		R[j] = arr[m+1+j];

	/* Merge the temp arrays back into arr [ l .. r ] */
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i<n1 && j<n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remain elements of L[], if there
	are any */
	while (i<n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remain elements of R[], if there
	are any */
	while (j<n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

/* l is for left index and r is right index of the
sub - array of arr to be sorted */
void mergeSort (int arr[], int l, int r) {
	if (l<r) {
		// Same as (l+r)/2 , but avoids overflow for large l and h
		int m = l+(r-l)/2;

		// Sort first and second halves
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);
		merge(arr, l, m, r);
	}
}

void printArray (int A[], int size) {
	int i;
	for (i=0;i<size;i++)
		printf("%d ", A[i]);
	printf("\n");
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
				e.peso = dist_eucli(outposts[i].first,outposts[i].second,outposts[j].first,outposts[j].second);
				arestas.push_back(e);
			}
		}

		// Ordenando as arestas por peso
		sort(arestas.begin(),arestas.end(),ordPeso);

		vector<bool> arestasArvore(arestas.size(),false);
		double max = kruskal(arestas,arestasArvore,n,sat);

		cout << fixed << setprecision(2) << max << endl;
	}

	string s;

	while(getline(cin,s)) {
		stack<int> erro;
		bool ok = balanced(s,erro);
		if(ok) cout << "YES\n";
		else { 
			cout << "NO ";
			if(erro.empty()) cout << "0\n";
			else cout << erro.top() << endl;
		}	
	}

	long long int target;
	string set;
	cin >> target >> set;
	global_target = target;
	global_set = set;
	sort(global_set.begin(), global_set.end());


	while(set!="END") {
		combs(target, set);
		if (!stop) {
			cout << "no solution" << endl;
		} else {
			cout << global_result << endl;
		}
		cin >> target >> set;
		global_target = target;
		global_set = set;
		sort(global_set.begin(), global_set.end());

	}

	cin >> t;

	while(t>0) {

		vector<string> v;
		map<string,int> map;
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

	int x;

	for(int i=0;i<100;i++) {
		if(i&1) {
			cout << i << endl;
			x = 4;
		}
		else {
			cout << 3*i << endl;
			x = 9;
		}
	}

	cout << x << endl;

	return 0;
}
