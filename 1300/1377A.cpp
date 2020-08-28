#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <tuple>
#include <map>
#include <ctime>
#include <cmath>

using namespace std;
typedef pair<int, int> pii;
typedef pair<double, int> pdi;
typedef tuple<int, int, int> t3i;
typedef vector<int> vi;

int N, M=0;
vector<vi> G;

vector<set<int>> f(int n) {
	vector<set<int>> Cs(n);
	vi ds(N, N), cc(N, -1), sd(n);
	vector<set<int>> D(N+1);
	for(int i = 0; i < N; ++i) D[N].insert(i);
	int C = 1, last=rand()%N, md=N;
	Cs[0].insert(last);
	cc[last] = 0;
	sd[0] = G[last].size();
	while(C < n) {
		D[ds[last]].erase(last);
		vi Q = {last}, Q2;
		D[0].insert(last);
		int d = ds[last] = 0;
		while(!Q.empty()) {
			++ d;
			Q2.clear();
			for(int i : Q) for(int j : G[i]) if(d < ds[j]) {
				if(!D[ds[j]].count(j)) exit(1);
				D[ds[j]].erase(j);
				ds[j] = d;
				Q2.push_back(j);
				D[d].insert(j);
			}
			swap(Q, Q2);
		}
		while(D[md].empty()) --md;
		last = *D[md].begin();
		cc[last] = C;
		sd[C] = G[last].size();
		Cs[C++].insert(last);
	}
	if(!D[N].empty()) cerr << "BUG" << endl;

	vector<priority_queue<pdi, vector<pdi>, greater<pdi>>> to_add(n);
	vi to(N, -1);
	priority_queue<pii, vector<pii>, greater<pii>> Q;
	vi neig(n, 0);
	for(int i = 0; i < N; ++i) if(cc[i] < 0) {
		for(int j : G[i]) if(cc[j] >= 0) ++ neig[cc[j]];
		int bc, nc=0, dc=0;
		for(int j : G[i]) if(cc[j] >= 0) {
			int nj = neig[cc[j]], dj = sd[cc[j]];
			if(nj > nc || (nj == nc && dj < dc)) {
				bc = cc[j];
				nc = nj;
				dc = dj;
			}
			neig[cc[j]] = 0;
		}
		if(nc > 0) {
			if(to_add[bc].empty()) Q.emplace(dc, bc);
			to_add[bc].emplace(double(nc)/double(G[i].size()), i);
			to[i] = bc;
		}
	}
	while(!Q.empty()) {
		int deg = Q.top().first, c = Q.top().second;
		Q.pop();
		if(deg != sd[c]) {
			Q.emplace(sd[c], c);
			continue;
		}
		while(!to_add[c].empty()) {
			int i = to_add[c].top().second;
			to_add[c].pop();
			if(cc[i] >= 0 || to[i] != c) continue;
			cc[i] = c;
			Cs[c].insert(i);
			sd[c] += G[i].size();
			for(int j : G[i]) if(cc[j] < 0) {
				for(int k : G[j]) if(cc[k] >= 0) ++ neig[cc[k]];
				int bc, nc=0, dc=0;
				for(int k : G[j]) if(cc[k] >= 0) {
					int nk = neig[cc[k]], dk = sd[cc[k]];
					if(nk > nc || (nk == nc && dk < dc)) {
						bc = cc[k];
						nc = nk;
						dc = dk;
					}
					neig[cc[k]] = 0;
				}
				if(nc > 0 && bc != to[j]) {
					if(to_add[bc].empty() && bc != c) Q.emplace(dc, bc);
					to_add[bc].emplace(double(nc)/double(G[j].size()), j);
					to[j] = bc;
				}
			}
			break;
		}
		if(!to_add[c].empty()) Q.emplace(sd[c], c);
	}

	return Cs;
}

double score(vector<set<int>> &Cs) {
	int n = Cs.size();
	double mod = 0, reg = 0;
	vector<double> in(n, 0), out(n, 0);
	for(int c = 0; c < n; ++c) {
		for(int i : Cs[c]) for(int j : G[i])
			if(Cs[c].count(j)) in[c] += .5;
			else out[c] += 1.;
		mod += in[c] / M - pow((2.*in[c]+out[c]) / (2.*M), 2.);
		int cc = Cs[c].size();
		double d = cc == 1 ? 1. : in[c] / (.5 * cc*(cc-1));
		reg += .5 * d/n;
	}
	reg -= n / N;
	return (mod + reg + 1.) * 1e5;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	srand(time(nullptr));

	int a, b;
	while(cin >> a >> b) {
		++ M;
		int m = max(a, b);
		if(m >= G.size()) G.resize(m+1);
		G[a].push_back(b);
		G[b].push_back(a);
	}
	N = G.size();
	cerr << "size of V: " << N << endl;
	cerr << "size of E: " << M << endl;

	vector<set<int>> ans = f(80000);
	for(set<int> &c : ans) {
		for(int i : c) cout << i << " ";
		cout << "\n";
	}
	cerr << "score: " << score(ans) << endl;

	return 0;
}