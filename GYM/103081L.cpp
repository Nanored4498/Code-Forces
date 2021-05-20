#include <iostream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, M;
	cin >> N >> M;
	vector<vi> pref(N), score(N);
	vi c(M), m(N, 0);
	vector<priority_queue<pii>> Q(M);

	for(int i = 0; i < M; ++i) cin >> c[i];

	string line;
	getline(cin, line);
	for(int i = 0; i < N; ++i) {
		getline(cin, line);
		stringstream ss(line);
		int x;
		while(ss >> x) pref[i].push_back(x-1);
		score[i].resize(pref[i].size());
	}

	for(int i = 0; i < M; ++i) {
		string line;
		getline(cin, line);
		stringstream ss(line);
		int x, sc = 0;
		while(ss >> x) {
			if(x == 0) continue;
			int j = 0;
			-- x;
			if(i == 9907) cerr << x << ' ';
			while(pref[x][j] != i) ++j;
			score[x][j] = ++ sc;
		}
			if(i == 9907) cerr << endl;
	}

	for(int i0 = 0; i0 < N; ++i0) {
		int i = i0;
		while(m[i] < pref[i].size()) {
			int r = pref[i][m[i]];
			int s = score[i][m[i]];
			if(Q[r].size() < c[r]) {
				Q[r].emplace(s, i);
				break;
			}
			auto [s2, j] = Q[r].top();
			if(s < s2) {
				Q[r].pop();
				Q[r].emplace(s, i);
				i = j;
			}
			++ m[i];
		}
	}

	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < m[i]; ++j) {
			int r = pref[i][j];
			if(Q[r].size() < c[r] || score[i][j] < Q[r].top().first)
				cerr << "bug " << i << ' ' << j << endl;
		}
	}
	for(int i : {9906, 9907}) {
		cerr << i << ' ' << m[i] << ' ' << pref[i].size() << ' ' << pref[i].back() << ' ' << score[i].back() << endl;
		for(int j : pref[i]) cerr << j << ' ';
		cerr << endl;
	}

	for(int i = 0; i < N; ++i)
		if(m[i] < pref[i].size())
			cout << i+1 << '\n';

	return 0;
}