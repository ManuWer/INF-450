#include <iostream>
#include <cmath>
#include <vector>

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

int main() {

	cout << "Geometria computacional\n";

	return 0;
}