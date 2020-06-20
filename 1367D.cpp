#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int q;
	cin >> q;
	while(q--) {
		string s;
		int m;
		cin >> s >> m;
		vi b(m), q, q2;
		string ans(m, '.');
		for(int i = 0; i < m; ++i) {
			cin >> b[i];
			if(b[i] == 0) q.push_back(i);
		}
		sort(s.begin(), s.end());
		int i = s.size()-1;
		while(!q.empty()) {
			int j;
			while(true) {
				j = i-1;
				while(j >= 0 && s[j] == s[i]) --j;
				if(i-j >= q.size()) break;
				i = j;
			}
			q2.clear();
			for(int k : q) {
				ans[k] = s[i];
				for(int l = 0; l < m; ++l) if(b[l] > 0) {
					b[l] -= abs(k-l);
					if(b[l] == 0) q2.push_back(l);
				}
			}
			i = j;
			swap(q, q2);
		}
		cout << ans << "\n";
	}

	return 0;
}