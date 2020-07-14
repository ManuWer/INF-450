#include <iostream>
#include <string>
using namespace std;

string seven[6] = {"jolteon" ,"flareon", "umbreon", "leafeon", "glaceon", "sylveon"};
bool ok[6] = {1,1,1,1,1,1};

void verify(string s) {

	for(int i=0;i<4;i++) {
		if(s[i]!='.') {
			for(int j=0;j<6;j++) 
				if(s[i]!=seven[j][i]) ok[j] = 0;
		}			
	}
	for(int i=0;i<6;i++) {
		if(ok[i]) {
			cout << seven[i] << endl;
			break;
		}
	}
}

int main() {

	int n;
	string s;

	cin >> n; cin.ignore();
	cin >> s;

	if(n==6) 
		cout << "espeon\n";
	else if(n==8) 
		cout << "vaporeon\n";
	else
		verify(s);

	return 0;
}
			
	
	

	
