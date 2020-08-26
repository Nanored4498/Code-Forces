#include <iostream>
#include <vector>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vii;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, m;
	cin >> t;
	while(t--) {
		cin >> n >> m;
		vi deg_out(n+1, 0);
		vector<vi> in(n+1);
		vii E(m);
		bool ti;
		for(int i = 0; i < m; ++i) {
			cin >> ti >> E[i].first >> E[i].second;
			if(ti) {
				++ deg_out[E[i].first];
				in[E[i].second].push_back(E[i].first);
			}
		}
		int s = 0;
		vi Q, ind(n+1);
		for(int i = 1; i <= n; ++i) if(deg_out[i] == 0) Q.push_back(i);
		while(!Q.empty()) {
			int i = Q.back();
			Q.pop_back();
			ind[i] = s++;
			for(int j : in[i]) if(--deg_out[j] == 0) Q.push_back(j);
		}
		if(s < n) {
			cout << "NO\n";
		} else {
			cout << "YES\n";
			for(pii &e : E) {
				if(ind[e.first] < ind[e.second]) swap(e.first, e.second);
				cout << e.first << ' ' << e.second << "\n";
			}
		}
	}

	return 0;
}