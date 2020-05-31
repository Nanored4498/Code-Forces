#include <iostream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

#define FOR(i, n) for(int i = 0; i < (n); ++i)
#define MAX_A 200000
typedef vector<int> VI;
typedef pair<int, int> pi;
typedef vector<pi> VPI;

int n, m, t;
string s[1000];
VI start[1000];
VPI Q, Q2;

void add(int i, int j, VPI &Q, int d) {
	if(i < 0 || i >= n || j < 0 || j >= m) return;
	if(start[i][j] < 0) { start[i][j] = d; Q.emplace_back(i, j); }
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m >> t;
	int d = 0;
	for(int i = 0; i < n; ++i) {
		cin >> s[i];
		start[i].assign(m, -1);
		for(int j = 0; j < m; ++j) {
			if(i > 0 && s[i][j] == s[i-1][j]) {
				add(i, j, Q, d);
				add(i-1, j, Q, d);
			}
			if(j > 0 && s[i][j] == s[i][j-1]) {
				add(i, j, Q, d);
				add(i, j-1, Q, d);
			}
		}
	}
	while(!Q.empty()) {
		Q2.clear();
		++ d;
		for(const auto &p : Q) {
			int i = p.first, j = p.second;
			add(i-1, j, Q2, d);
			add(i+1, j, Q2, d);
			add(i, j-1, Q2, d);
			add(i, j+1, Q2, d);
		}
		swap(Q, Q2);
	}

	int i, j;
	long long p;
	while(t --> 0) {
		cin >> i >> j >> p;
		--i; --j;
		if(start[i][j] < 0 || p < start[i][j]) cout << s[i][j] << endl;
		else if((p-start[i][j])&1LL) cout << ('1'-s[i][j]) << endl;
		else cout << s[i][j] << endl;
	}

	return 0;
}