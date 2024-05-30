#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>
#include <queue>
#include <fstream>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main(int argc, char* argv[]) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, p;
	cin >> n >> m >> p;
	vector<vi> w(n, vi(n, 0));
	while(m--) {
		int a, b, c;
		cin >> a >> b >> c;
		--a;
		--b;
		w[a][b] += c;
		w[b][a] += c;
	}
	const vector<vi> w0 = w;

	vector<vi> A(n);
	for(int i = 0; i < n; ++i) A[i].push_back(i);
	priority_queue<array<int, 3>> Q;
	for(int i = 0; i < n; ++i) for(int j = 0; j < i; ++j) {
		if(w[i][j]) Q.push({w[i][j], i, j});
	}
	ll U = 0;
	int G = n;
	while(!Q.empty()) {
		auto [k, i, j] = Q.top();
		Q.pop();
		if(A[i].empty()) continue;;
		if(A[j].empty()) continue;;
		if(k != w[i][j]) continue;
		if(A[i].size() + A[j].size() <= p) {
			U += k;
			--G;
			A[i].insert(A[i].end(), A[j].begin(), A[j].end());
			A[j].clear();
			for(int l = 0; l < n; ++l) if(l != i && !A[l].empty() && (int) A[i].size() + A[j].size() <= p) {
				w[l][i] = w[i][l] += w[j][l];
				if(w[l][i]) Q.push({w[i][l], i, l});
			}
		}
	}
	cerr << U << '\n';


	vector<vi> Gs;
	for(auto &g : A) if(!g.empty()) {
		Gs.push_back(g);
	}

	for(int t = 0; t < 1000; ++t)
	for(int u = 0; u < Gs.size(); ++u) for(int i = 0; u < Gs.size() && i < Gs[u].size(); ++i) {
		for(int v = 0; v < u; ++v) {
			for(int j = 0; j < Gs[v].size(); ++j) {
				int ds = 0;
				for(int k = 0; k < Gs[u].size(); ++k) if(k != i) {
					ds += w0[Gs[v][j]][Gs[u][k]] - w0[Gs[u][i]][Gs[u][k]];
				}
				for(int k = 0; k < Gs[v].size(); ++k) if(k != j) {
					ds += w0[Gs[u][i]][Gs[v][k]] - w0[Gs[v][j]][Gs[v][k]];
				}
				if(ds >= 0) {
					U += ds;
					swap(Gs[u][i], Gs[v][j]);
				}
			}
		}
		for(int v = 0; v < (int) Gs.size(); ++v) if(u != v && (int) Gs[v].size() < p) {
			int ds = 0;
			for(int k = 0; k < Gs[u].size(); ++k) if(k != i) {
				ds -= w0[Gs[u][i]][Gs[u][k]];
			}
			for(int k = 0; k < Gs[v].size(); ++k) {
				ds += w0[Gs[u][i]][Gs[v][k]];
			}
			if((ds > 0) && (ds >= 0 || t > 50)) {
				U += ds;
				Gs[v].push_back(Gs[u][i]);
				Gs[u][i] = Gs[u].back();
				Gs[u].pop_back();
				if(Gs[u].empty()) {
					--G;
					Gs[u] = Gs.back();
					Gs.pop_back();
				}
				break;
			}
		}
	}

	ifstream old(argv[1]);
	ll U0;
	old >> U0;
	if(U0 >= U) return 0;


	ofstream out(argv[1]);
	cerr << U << '\n';
	out << U << ' ' << G << '\n';
	for(auto &g : Gs) {
		out << g.size();
		for(int i : g) out << ' ' << i+1;
		out << '\n';
	}

	cerr << "BETTER\n";

	return 0;
}