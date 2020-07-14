#include <iostream>
#include <vector>

using namespace std;

vector<vector<long long> > dp(100002,vector<long long>(5,-1));

long long custo (const string &s, const vector<long long> &v, int j, int ultHard) {
	long long ans = 0;

	if(ultHard==-1) return 10E10;
	if(j==-1) return 0;

	if(dp[j][ultHard]!=-1) return dp[j][ultHard];

	if(s[j]!="hard"[ultHard])
		ans = custo(s,v,j-1,ultHard);
	else
		ans = min(custo(s,v,j-1,ultHard-1),v[j]+custo(s,v,j-1,ultHard));

	dp[j][ultHard] = ans;

	return ans;
}

int main() {

	int n;
	cin >> n;
	string s;
	cin >> s;

	vector<long long> v(n);
	for(int i=0;i<n;i++)
		cin >> v[i];

	cout << custo(s,v,n-1,3) << endl;

	return 0;
}

