#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <list>
#include <cmath>
#include <set>

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

void DFS(vector< list<int> > &G, int v, vector<bool> &visit) {
	if(visit[v]) return;
	visit[v] = true;
	for(int i:G[v])
		DFS(G,i,visit);
}

void printTour(int G[][50], int v) {
	for(int i=0;i<50;i++) {
		if(G[v][i]) {
			G[v][i]--;
			G[i][v]--;
			printTour(G,i);
			cout << i+1 << " " << v+1 << endl;
		}
	}
}

int eulerTour() {
	int t, k=1;
	cin >> t;
	
	while(t--) {
		int n;
		cin >> n;
		int adj[50][50] = {0};
		int grau[50] = {0};

		int a, b;

		for(int i=0;i<n;i++) {
			cin >> a >> b;
			adj[a-1][b-1]++; adj[b-1][a-1]++;
			grau[a-1]++; grau[b-1]++;
		}

		bool euler = 1;
		for(int i=0;i<50;i++)
			if(grau[i]%2!=0) euler = 0;

		cout << "Case #" << k << endl; k++;

		if(!euler) cout << "some beads may be lost\n";
		else {
			for(int i=0;i<50;i++)
				if(grau[i]!=0) {
					printTour(adj,i);
					break;
				}
		}

		if(t>0) cout << endl;
	}

	return 1;
}

void criticalLinks() {

	int n;
	while(cin >> n) {

		if(n==0) break;
		vector< list<int> > G(n);
		int v,w;
		string s;
		cin.ignore();
		while(getline(cin,s)) {
			stringstream ss(s);
			ss >> v;
			if(v==0) break;
			for(int i=1;i<s.size()/2+1;i++) {
				ss >> w;
				G[v-1].push_back(w-1);
				G[w-1].push_back(v-1);
			}
		}
	
		vector< list<int> > aux(G);
		int critical = 0;	

		for(int v=0;v<n;v++) {
			vector<bool> visit(n,0);
			visit[v] = 1;
			int a = (v==0)?1:0;
			aux[v].clear();
			DFS(aux,a,visit);
			for(int i=0;i<n;i++)
				if(!visit[i]) {
					critical++;
					break;
				}
			aux[v] = G[v];
		}

		cout << critical << endl;		
	}
}

void insertion(int s[], int n) {
    int i,j;
    int temp;
    for (i=1; i<n; i++) {
        j=i;
        while ((j>0) && (s[j] < s[j-1])) {
            temp = s[j];
            s[j] = s[j-1];
            s[j-1] = temp;
            j = j-1;
        }
    }
}

int ponte() {

	int t;
	int n;
	cin >> t;

	while (t>0) {
		
		cin >> n;
		deque<int> start(n);
		deque<int> end;

		for (int i=0; i<n; i++)
			cin >> start[i];

		sort(start.begin(), start.end());

		for (int i=0; i<n; i++) {
			end.push_front(start[i]);
			end.push_front(start[i+1]);
			start.erase(start.begin(), start.begin()+1);
		}
	}

	return t;
}

bool troca() {

	int n, k;
	cin >> n >> k;

	int init = n-k;
	int i=2, j=0;

	cout << "1";

	while(init>=i) {
		cout << " " << i;
		i++;
	}

	bool change = 0;
	for(int c=i;c<=n;c++) {
		if(!change) {
			cout << " " << n-j;
			change = 1;
			j++;
		} else {
			cout << " " << i;
			change = 0;
			i++;
		}
	}
	
	cout << endl;
	
	return change;
}

set<int> s;

void BFS(vector< vector<char> > &maze, int &i, int &j, vector< vector<bool> > &visit, int n, int m, vector< vector<int> > &ordem, const vector< vector<int> > &ordem2) {
    queue< pair<int,int> > F;
    F.push(make_pair(i,j));
    visit[i][j] = true;
    ordem[i][j] = 0;

    while(!F.empty()) {
        i = F.front().first; j = F.front().second; F.pop();
        if(i==0 || j==0 || i==n-1 || j==m-1) {
            if(ordem[i][j]<ordem2[i][j]) s.insert(ordem[i][j]);
        }
        if(i!=n-1 && !visit[i+1][j] && maze[i+1][j]!='F' && maze[i+1][j]!='#') {
            F.push(make_pair(i+1,j));
            visit[i+1][j] = true;
            ordem[i+1][j] = ordem[i][j]+1;
        }
        if(i!=0 && !visit[i-1][j] && maze[i-1][j]!='F' && maze[i-1][j]!='#') {
            F.push(make_pair(i-1,j));
            visit[i-1][j] = true;
            ordem[i-1][j] = ordem[i][j]+1;
        }
        if(j!=m-1 && !visit[i][j+1] && maze[i][j+1]!='F' && maze[i][j+1]!='#') {
            F.push(make_pair(i,j+1));
            visit[i][j+1] = true;
            ordem[i][j+1] = ordem[i][j]+1;
        }
        if(j!=0 && !visit[i][j-1] && maze[i][j-1]!='F' && maze[i][j-1]!='#') {
            F.push(make_pair(i,j-1));
            visit[i][j-1] = true;
            ordem[i][j-1] = ordem[i][j]+1;
        }
    }
}

void fire_BFS(vector< vector<char> > &maze, queue< pair<int,int> > &fire, vector< vector<bool> > &visit, int n, int m, vector< vector<int> > &ordem) {
    queue< pair<int,int> > F(fire);
    int i, j;
    while(!fire.empty()) {
        i = fire.front().first; j = fire.front().second; fire.pop();
        visit[i][j] = true;
        ordem[i][j] = 0;
    }
    while(!F.empty()) {
        i = F.front().first; j = F.front().second; F.pop();
        if(i!=n-1 && !visit[i+1][j] && maze[i+1][j]!='F' && maze[i+1][j]!='#') {
            F.push(make_pair(i+1,j));
            visit[i+1][j] = true;
            ordem[i+1][j] = ordem[i][j]+1;
        }
        if(i!=0 && !visit[i-1][j] && maze[i-1][j]!='F' && maze[i-1][j]!='#') {
            F.push(make_pair(i-1,j));
            visit[i-1][j] = true;
            ordem[i-1][j] = ordem[i][j]+1;
        }
        if(j!=m-1 && !visit[i][j+1] && maze[i][j+1]!='F' && maze[i][j+1]!='#') {
            F.push(make_pair(i,j+1));
            visit[i][j+1] = true;
            ordem[i][j+1] = ordem[i][j]+1;
        }
        if(j!=0 && !visit[i][j-1] && maze[i][j-1]!='F' && maze[i][j-1]!='#') {
            F.push(make_pair(i,j-1));
            visit[i][j-1] = true;
            ordem[i][j-1] = ordem[i][j]+1;
        }
    }
}

int fire() {

    int t;
    cin >> t;
    while(t--) {
        int n, m;
        cin >> n >> m;
        vector< vector<char> > maze(n, vector<char> (m));
        int jx, jy; // posicao do joe
        queue< pair<int,int> > fire; // posicoes iniciais onde tem fogo
        s.clear();

        for(int i=0;i<n;i++) {
            for(int j=0;j<m;j++) {
                cin >> maze[i][j];
                if(maze[i][j]=='J') {
                    jx = i; jy = j;
                }
                if(maze[i][j]=='F') 
                    fire.push(make_pair(i,j));
            }
        }     

        vector< vector<int> > ordem(n, vector<int>(m,-1));
        vector< vector<int> > ordem2(n, vector<int>(m,1000));
        vector< vector<bool> > visit(n, vector<bool>(m,0));
        vector< vector<bool> > visit2(n, vector<bool>(m,0));

        fire_BFS(maze,fire,visit2,n,m,ordem2);
        BFS(maze,jx,jy,visit,n,m,ordem,ordem2);

        if(!s.empty()) cout << *(s.begin())+1 << endl;
        else cout << "IMPOSSIBLE\n";
    }

    return 1;
}

void caminho() {

	int R, d, n;
	cin >> R >> d >> n;
	vector<vector<int> > v(n,vector<int>(3));

	for(int i=0;i<n;i++)
		cin >> v[i][0] >> v[i][1] >> v[i][2];

	int c = 0;

	for(int i=0;i<n;i++) {
		int x = v[i][0]; int y = v[i][1]; int r = v[i][2];
		double dist = sqrt(x*x + y*y);
		if(dist >= (R-d+r) && (dist+r) <= R) c++;
	}

	cout << c << endl;

}

bool stack() {

	string input;

	while((getline(cin,input)) && !cin.eof()) {
	
		stringstream ss(input);
		vector<int> s;
		int a;
		while (ss>>a)
			s.push_back(a);

		vector<int>::iterator it = s.end();
		int m = 0;
	
		while(m<s.size()) {
			
			vector<int>::iterator max;
			vector<int>::iterator min;

			max = max_element(s.begin(),s.end()-m);
			min = min_element(s.begin(),s.end()-m);
			
			if (max==s.end()-m-1 && min==s.begin())
				break;

			int pos = s.end() - max;

			while(max!=s.begin()) {
			
				reverse(s.begin(),max+1);
				cout << pos << " ";
			
				max = max_element(s.begin(),s.end()-m);
				pos = s.end() - max;
			}

			if (max==s.begin()) {
				reverse(s.begin(),it-m);
				cout << m+1 << " ";
			}
		}

		cout << "0" << endl;
	}

	return false;
}

void tarjan(int u, const vector<vector<int> > &adj, vector<int> &dfsParent, vector<int> &dfsNum, vector<int> &dfsLow, vector<bool> &isArticulation,vector<pair<int,int> > &bridges, int &dfsCount, int &rootChildren, int dfsRoot) {
	dfsLow[u] = dfsNum[u] = dfsCount++; // dfsLow[u] <= dfsNum[u]
	for(int v:adj[u]) {
		//vertices nao descobertos tem tempo de descoberto (dfsNum) 0 
		if (dfsNum[v] == 0) { 
			dfsParent[v] = u; //pai de v e' u (v foi descoberto por u)
			if (u == dfsRoot) //caso especial... precisamos contar quantas vezes descobrimos vertices a partir da raiz para descobrir se raiz e' articulacao...
				rootChildren++; 

			tarjan(v,adj,dfsParent,dfsNum,dfsLow,isArticulation,bridges,dfsCount,rootChildren,dfsRoot);

			if (dfsLow[v] >= dfsNum[u]) //apos visitar v recursivamente, conseguimos chegar em um vertice em v ou antes passando por u? (sem voltar pela aresta v,u)
				isArticulation[u] = true; // store this information first
			if (dfsLow[v] > dfsNum[u]) // ponte?
				bridges.push_back(make_pair(u,v));
			dfsLow[u] = min(dfsLow[u], dfsLow[v]); // update dfsLow[u]
		}
		else if (v != dfsParent[u]) //cheguei de volta em um vertice ja descoberto... se nao passei pela aresta v,u de volta --> ciclo...
			dfsLow[u] = min(dfsLow[u], dfsNum[v]); //atualize dfsLow...
	}
}

void tarjan(const vector<vector<int> > &adj, vector<bool> &isArticulation, vector<pair<int,int> > &bridges) {
	int nv = adj.size();
	isArticulation = vector<bool>(nv,false);
	bridges.resize(0);

	vector<int> dfsParent(nv,-1);
	vector<int> dfsLow(nv,0);
	vector<int> dfsNum(nv,0);	
	int dfsCount = 0;

	//para multiplos CCs ...
	for(int i=0;i<nv;i++) {
		if(dfsNum[i]==0) {
			int rootChildren = 0;  //resete para cada CC!!
			tarjan(i,adj,dfsParent,dfsNum,dfsLow,isArticulation,bridges,dfsCount,rootChildren,i);
			isArticulation[i] = rootChildren>1;
		}
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

unordered_map<string,string> m;

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

int main () {

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

	for(int i=0;i<100;i++) {
		if(i&1) 
			cout << i << endl;
		else 
			cout << 3*i << endl;
	}

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

	return 0;
}

			
