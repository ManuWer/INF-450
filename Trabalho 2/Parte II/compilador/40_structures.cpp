#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

int main () {

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
	
	return 0;
}
