#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <cmath>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <cstring>

#define maxN 500

using namespace std;

const double EPS = 1e-10;
int cmp(double x, double y=0, double tol=EPS) {
	return (x<=y+tol)?(x+tol<y)?-1:0:1;
}

struct Point {
	double x,y;
	
	Point(double x_=0, double y_=0) : x(x_), y(y_) {}
	Point operator+(Point q) { return Point(x+q.x,y+q.y); }
	Point operator-(Point q) { return Point(x-q.x,y-q.y); }
	Point operator*(double t) { return Point(x*t,y*t); }
	Point operator/(double t) { return Point(x/t,y/t); }
	double operator*(Point q) { return x*q.x + y*q.y; }
	double operator^(Point q) { return x*q.y - y*q.x; }

	int cmp(Point q) const {
		if(int t = ::cmp(x,q.x) ) return t;
		return ::cmp(y,q.y);
	}

	bool operator==(Point q) const { return cmp(q)==0; }
	bool operator!=(Point q) const { return cmp(q)!=0; }
	
	friend ostream& operator<<(ostream &out, Point p) {
		return out << "(" << p.x << ", " << p.y << ")";
	}

	void set(double x_, double y_) { x=x_; y=y_; }

	static Point pivot;
};

Point Point::pivot;

double area (Point a, Point b, Point c) {
	return (c-a)^(b-a)/2.0;
}

double polygonArea(vector< Point> &T) {
	double s = 0.0;
	int n = T.size();
	for(int i=0;i<n;i++)
		s += T[i]^T[(i+1)%n];
	return s/2.0;        //Retorna area com sinal
}

inline int ccw(Point &p, Point &q, Point &r) {
	return cmp((p-r)^(q-r));
}

bool pontoSobreReta(Point &p1, Point &p2, Point &p) {
	return ccw(p1,p2,p)==0;
}

bool between(Point &p1, Point &p, Point &p2) {
	return ccw(p1,p2,p)==0 && cmp((p1-p)*(p2-p))<=0;
}

bool distaciaReta(Point &p1, Point &p2, Point &p) {
	Point A = p1-p, B = p2-p1;
	return fabs(A^B)/sqrt(B*B);
}

bool distaciaSegReta(Point &p1, Point &p2, Point &p) {
	Point A = p1-p, B = p2-p1, C = p1-p;
	double a = A*A, b = B*B, c = C*C;
	if(cmp(a,b+c)>=0) return sqrt(c);
	else if(cmp(c,a+b)>=0) return sqrt(a);
	else return fabs(A^C)/sqrt(b);
}

double angle(Point &p, Point &q, Point &r) {
	Point u = p-r, w = q-r;
	return atan2(u^w,u*w);
}

int inpoly(Point &p, vector< Point > &T) {
	double a = 0.0; 
	int n = T.size();
	for(int i=0;i<n;i++) {
		if(between(T[i],p,T[(i+1)%n])) return -1;
		a += angle(T[i],T[(i+1)%n],p);
	}
	return cmp(a)!=0;
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

double dist(int x, int y, int x1, int y1) {
	return sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
}

// mapeia uma string de estado do jogo em uma sequencia de
// movimentos de como chegar neste estado
unordered_map<string,string> m;

// os movimentos verticais sao feitos para baixo...
string V1(const string &p) {
    string s(p);
    int s0 = s[0], s3 = s[3];
    s[0] = s[6];
    s[3] = s0;
    s[6] = s3;
    return s;
}

string V2(const string &p) {
    string s(p);
    int s1 = s[1], s4 = s[4];
    s[1] = s[7];
    s[4] = s1;
    s[7] = s4;
    return s;
}

string V3(const string &p) {
    string s(p);
    int s2 = s[2], s5 = s[5];
    s[2] = s[8];
    s[5] = s2;
    s[8] = s5;
    return s;
}

// os movimentos horizontais sao feitos para a esquerda...
string H1(const string &p) {
    string s(p);
    int s0 = s[0], s1 = s[1];
    s[0] = s1;
    s[1] = s[2];
    s[2] = s0;
    return s;
}

string H2(const string &p) {
    string s(p);
    int s3 = s[3], s4 = s[4];
    s[3] = s4;
    s[4] = s[5];
    s[5] = s3;
    return s;
}

string H3(const string &p) {
    string s(p);
    int s6 = s[6], s7 = s[7];
    s[6] = s7;
    s[7] = s[8];
    s[8] = s6;
    return s;
}

// coloca no mapa todos os estados que sao possiveis de alcancar a partir
// do estado consistente do jogo, fazendo os movimentos contrarios
void criaMapa() {
    queue<pair<string,string> > F;
    F.push({"123456789",""});
    string s, p;
    while(!F.empty()) {
        s = F.front().first; p = F.front().second; F.pop();   
        auto it = m.find(s);
        if(it==m.end()) { 
            m[s] = p;
            F.push({H1(s),p+"1H"});
            F.push({H2(s),p+"2H"});
            F.push({H3(s),p+"3H"});
            F.push({V1(s),p+"1V"});
            F.push({V2(s),p+"2V"});
            F.push({V3(s),p+"3V"});
        }
    }
}

void test(int t) {

	while(t>0) {
		int n;
		cin >> n;

		vector<int> tam;
		vector<int> unset;
		set<int> machine;
		pair<set<int>::iterator,bool> p;
		vector<int>::iterator it;
		int aux;
		int cont = 0;

		for (int i=0;i<n;i++) {
			cin >> aux;
			p = machine.insert(aux);
			unset.push_back(aux);
			if (p.second)
				cont++; 
			else {
				tam.push_back(cont);
				machine.clear();
				it = find(unset.begin(),unset.end(),aux);
				it++;
				for(vector<int>::iterator i=it;i!=unset.end();i++)
					machine.insert(*i);
				cont = machine.size();
				unset.erase(unset.begin(),it);
			}
		}

		tam.push_back(cont);

		cout << *max_element(tam.begin(),tam.end()) << endl;
		t--;
	}

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

bool BFS(int ways[maxN][maxN], int *parent, bool *visited, int n, int s, int t){
    queue<int> q;

    parent[s] = -1;
    q.push(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = 0; i < n ; ++i) {
            if (!visited[i] && ways[u][i] > 0) {
                q.push(i);
                parent[i] = u;
                visited[i] = true;
            }
        }
    }
    return visited[t];
}

bool DFS(int rGraph[maxN][maxN], int *parent, bool *visited, int n, int x, int t){

	if(x==t) return true;
	visited[x] = true;

	for(int i=0;i<n;++i){
		if(rGraph[x][i]>0 && !visited[i]){
			parent[i] = x;
			if(DFS(rGraph, parent, visited, n, i, t))
				return true;
		}
	}
	return false;
}

int FordFulkerson(int graph[maxN][maxN], int n, int s, int t) {

	int rGraph[maxN][maxN];
	int parent[maxN];
	bool visited[maxN];

	// Replicate the graph for residual one
	for(int i=0;i<n;++i)
		for(int j = 0; j < n; ++j)
			rGraph[i][j] = graph[i][j];

	for(int i=0;i<n;i++)
		visited[i] = false;

	int max_flow = 0;

	while(BFS(rGraph, parent, visited, n, s, t)) { 
		memset(visited, false, sizeof(visited));
		int path_flow = 0xfffffff; // Current flow to add on the max flow later
         
		for(int v = t; v != s; v = parent[v]){
			int u = parent[v];
             
			path_flow = min(path_flow, rGraph[u][v]);
		}

		for(int v = t; v != s; v = parent[v]){
			int u = parent[v];
			rGraph[u][v] -= path_flow;
			rGraph[v][u] += path_flow;
		}

		max_flow += path_flow;
	}

	return max_flow;
}

int graphFlow(){

    int n;

    while(cin >> n) {
        int graph[maxN][maxN];
        for(int i=0;i<2*n+2;i++) 
            for(int j=0;j<2*n+2;j++)
                graph[i][j]=0;
            
        vector<int>vec(n);
		int m;
        for(int i=0;i<n;i++) cin>>vec[i];
        cin >> m;
        for(int i=0;i<m;i++) {
            int a,b,c;
            cin >> a >> b >> c;
            graph[a][n+1+a]=vec[a-1];
            graph[n+1+a][b]=c;
        }

        int aux1,aux2;
        cin >> aux1 >> aux2;

        for(int i=0;i<aux1;i++){
            int aaa;
            cin >> aaa;
            graph[0][aaa]=1000000;
            graph[aaa][aaa+n+1] = vec[aaa-1];
        }

        for(int i=0;i<aux2;i++){
            int bbb;
            cin >> bbb;
            graph[bbb][bbb+n+1]=vec[bbb-1];
            graph[bbb+n+1][n+1]=1000000;
        }

        cout << FordFulkerson(graph, 2*n+2, 0, n+1) << endl;
    }
}

void blackBox() {

	int t, n, m;
	cin >> t;

	while (t>0) {
		cin >> m >> n;
		vector<int> A(m);
		vector<int> u(n);
		multiset<int> Aset;
		multiset<int>::iterator it;
		int i = 0;

		for (int j=0;j<m;j++)
			cin >> A[j];

		cin >> u[0];
		for (int k=0;k<u[0];k++)
				Aset.insert(A[k]);
		it = Aset.begin();
		cout << *it << endl;

		for (int j=1;j<n;j++) {
			cin >> u[j];
			for (int k=Aset.size();k<u[j];k++) {
				Aset.insert(A[k]);
				if (A[k]<(*it)) it--;
			}
			it++;
			cout << *it << endl;
		}

		t--;
		if (t>0) cout << endl;
	}
}

bool BFS(vector< vector<int> > &G, int v, vector<bool> &visit) {
	queue<int> F; F.push(v);
    visit[v] = true;
    int init = v;
    bool comeBack = false;
    while(!F.empty()) {
    	v = F.front(); F.pop();
    	for(int i:G[v]) {
            if(i==init) 
                comeBack = true;
    		if(!visit[i]) {
				F.push(i);
				visit[i] = true;
    		}
    	}
    }

    return comeBack;	
}

int catAndMouse() {

    int t;
    cin >> t;

    while(t--) {
        int n,m,c;
        cin >> n >> c >> m;
        cin.ignore();

        vector< vector<int> > cat(n);
        vector< vector<int> > mouse(n);
        vector<bool> visitCat(n,0);
        vector<bool> visitMouse(n,0);

        string s;
        while(getline(cin,s)) {
            stringstream ss(s);
            int a, b;
            ss >> a; ss >> b;
            if(a==-1) break;
            cat[a-1].push_back(b-1);
        }

        while(getline(cin,s)) {
            if(s.empty()) break;
            stringstream ss(s);
            int a, b;
            ss >> a; ss >> b;
            mouse[a-1].push_back(b-1);
        }

        BFS(cat,c-1,visitCat);
        BFS(mouse,m-1,visitMouse);

        vector< vector<int> > mouseSemCat(mouse);
        vector<bool> visitMSC(n,0);

        for(int i=0;i<n;i++)
            if(visitCat[i]) mouseSemCat[i].clear();

        bool q1 = false, q2 = false;

        for(int i=0;i<n;i++)
            if(visitCat[i] && visitMouse[i]) {
                q1 = true;
                break;
            }

        q2 = BFS(mouseSemCat,m-1,visitMSC);

        if(q1) cout << "Y ";
        else cout << "N ";
        
        if(q2) cout << "Y\n";
        else cout << "N\n";

        if(t) cout << "\n";
    }

    return 1;
}

int main() {
    
    string s, puzzle;
    criaMapa();   
    
    while(1) {
        for(int i=0;i<3;i++) {
            getline(cin,s);
            if(s[0]=='0') return 0;
            puzzle += s[0]; puzzle += s[2]; puzzle += s[4];
        }

        // se o estado de entrada nao esta no mapa, nao tem solucao
        if(m.find(puzzle)==m.end()) cout << "Not solvable\n";
        // se estiver, imprime o numero de movimentos e a ordem ao contrario
        else {
            s = m[puzzle];
            reverse(s.begin(),s.end());
            cout << s.size()/2 << " " << s << endl;
        }
        
        puzzle.clear();
        s.clear();
    }

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

    return 0;
}

