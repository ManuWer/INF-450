#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include <map>
#include <sstream>

using namespace std;

#define infinito 10E8

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

int main () {

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

			
