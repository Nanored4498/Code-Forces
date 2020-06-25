#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <map>
#include <queue>
#include <set>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int T = 40;
vector<vi> G;
vector<bool> seen;
vector<vi> Cs;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vi x(n+1), best(n+1);
	int ans = 0;
	srand(time(nullptr));
	seen.resize(n+1);
	G.resize(n+1);
	vi deg(n+1, 0);
	while(m--) {
		int a, b;
		cin >> a >> b;
		G[a].push_back(b);
		G[b].push_back(a);
	}

	fill(seen.begin(), seen.end(), false);
	for(int i = 1; i <= n; ++i) if(!seen[i]) {
		Cs.emplace_back();
		vi st = {i};
		seen[i] = true;
		while(!st.empty()) {
			int k = st.back();
			st.pop_back();
			Cs.back().push_back(k);
			for(int j : G[k]) if(!seen[j]) {
				seen[j] = true;
				st.push_back(j);
			}
		}
	}
	cerr << "nb clus: " << Cs.size() << endl;

	vector<set<int>> bad(n+1);
	vi invC(n+1);
	for(vi &C : Cs) {
		int bk = 0;
		for(int t = 0; t < T; ++t) {
			if(t%3 == 0) cerr << "test: " << t << "  (best: " << bk << ")" << endl;
			random_shuffle(C.begin(), C.end());
			priority_queue<pii, vector<pii>, greater<pii>> Q;
			for(int j = 0; j < C.size(); ++j) {
				int i = C[j];
				invC[i] = j;
				seen[i] = false;
				x[i] = 1;
				bad[i].clear();
				deg[i] = G[i].size() * double(30 + (rand()%5)) / 30.;
				Q.emplace(deg[i], j);
			}
			int k = 0;
			while(!Q.empty()) {
				int d = Q.top().first, i = C[Q.top().second];
				Q.pop();
				if(d != deg[i]) continue;
				if(x[i] == 1) {
					seen[i] = true;
					++k;
					for(int j : G[i]) {
						if(x[j] == 1) {
							x[j] = 0;
							for(int l : G[j]) if(x[l] == 1 && !seen[l]) {
								-- deg[l];
								Q.emplace(deg[l], invC[l]);
							}
						}
						bad[j].insert(i);
					}
				}
			}
			cerr << "k: " << k << endl;
			while(true) {
				bool better = false;
				for(int i : C) if(x[i] == 0) Q.emplace(bad[i].size(), invC[i]);
				while(!Q.empty()) {
					int b = Q.top().first, i = C[Q.top().second];
					Q.pop();
					if(b != bad[i].size()) {
						if(x[i] == 0) Q.emplace(bad[i].size(), invC[i]);
						continue;
					}
					map<int, int> r;
					for(int j : bad[i]) for(int l : G[j]) ++ r[l];
					vector<int> ri;
					for(const pii &p : r) ri.push_back(p.first);
					sort(ri.begin(), ri.end(), [&](int a, int b) { return G[a].size() < G[b].size(); });
					vector<int> add;
					set<int> rm;
					for(int l : ri) if(r[l] == bad[l].size()) {
						add.push_back(l);
						for(int j : G[l]) {
							if(r.count(j)) r[j] = -1;
							if(x[j] == 1) rm.insert(j);
						}
					}
					if(add.size() > rm.size()) {
						better = true;
						k += add.size() - rm.size();
						for(int j : add) {
							x[j] = 1;
							for(int l : G[j]) bad[l].insert(j);
						}
						for(int j :rm) {
							x[j] = 0;
							for(int l : G[j]) bad[l].erase(j);
							Q.emplace(bad[i].size(), invC[i]);
						}
					}
				}
				if(!better) break;
				cerr << "k2: " << k << endl;
			}
			if(k > bk) {
				for(int i : C) best[i] = x[i];
				bk = k;
			}
		}
		ans += bk;
	}
	cerr << ans << endl;
	cout << ans << "\n";
	for(int i = 1; i <= n; ++i) cout << best[i] << " \n"[i==n];

	return 0;
}