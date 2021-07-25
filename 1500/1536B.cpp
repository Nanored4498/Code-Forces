#include <iostream>
#include <vector>
#include <set>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	vector<string> p = { "a" };
	while(p.back() != "z") {
		p.push_back(p.back());
		++ p.back()[0];
	}
	for(int l = 0; l < 2; ++l) {
		int n = p.size();
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < 26; ++j)
				p.push_back(p[i] + p[j]);
	}

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		set<string> S;
		string s;
		cin >> s;
		for(int i = 0; i < n; ++i) {
			S.insert(s.substr(i, 1));
			if(i == 0) continue;
			S.insert(s.substr(i-1, 2));
			if(i == 1) continue;
			S.insert(s.substr(i-2, 3));
		}
		int i = 0;
		while(S.count(p[i])) ++ i;
		cout << p[i] << '\n';
	}

	return 0;
}