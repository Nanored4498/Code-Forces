#include <iostream>
#include <map>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	map<string, int> m;
	cin >> n;
	for(int i = 0; i < n; ++i) {
		string s;
		cin >> s;
		++ m[s];
		if(m[s] > n/2) {
			cout << s << '\n';
			return 0;
		}
	}
	for(auto [k, v] : m) {
		
	}
	cout << "NONE\n";

	return 0;
}