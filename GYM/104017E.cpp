#include <iostream>

using namespace std;

string f(const string &s) {
	int nb = 0;
	string ans;
	for(char c : s) {
		if(c == 'A' || c == 'C') {
			if(!ans.empty() && ans.back() == c) ans.pop_back();
			else ans.push_back(c);
		} else /* c == B */ {
			++nb;
		}
	}
	if(nb&1) ans.push_back('B');
	return ans;
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string u, v;
		cin >> u >> v;
		if(f(u) == f(v)) {
			cout << "YES\n";
		} else {
			cout << "NO\n";
		}
	}

	return 0;
}