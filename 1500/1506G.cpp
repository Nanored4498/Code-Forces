#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string s, r;
		cin >> s;
		vector<int> count(256, 0);
		vector<bool> used(256, false);
		for(char c : s) ++ count[c];
		for(char c : s) {
			if(used[c]) {
				-- count[c];
				continue;
			}
			while(!r.empty() && r.back() < c && count[r.back()] > 1) {
				-- count[r.back()];
				used[r.back()] = false;
				r.pop_back();
			}
			used[c] = true;
			r.push_back(c);
		}
		cout << r << '\n';
	}

	return 0;
}