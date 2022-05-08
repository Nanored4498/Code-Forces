#include <iostream>

using namespace std;

int x[128];
int main() {
	string a;
	getline(cin, a); for(char c : a) --x[c];
	getline(cin, a); for(char c : a) ++x[c];
	for(char c = 0; c < 127; ++c) if(x[c]) cout << c;
	cout << endl;
	return 0;
}