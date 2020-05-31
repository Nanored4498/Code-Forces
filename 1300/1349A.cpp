#include <iostream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

#define FOR(i, n) for(int i = 0; i < (n); ++i)
#define MAX_A 200000
typedef vector<int> VI;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, a;
	cin >> n;
	VI mi0(MAX_A+1, MAX_A+1), mi1(MAX_A+1, MAX_A+1), count(MAX_A+1, 0);
	vector<map<int, int>> ps(MAX_A+1);
	for(int i = 2; i <= MAX_A; ++i) if(ps[i].empty()) {
		ps[i][i] = 1;
		for(int j = 2*i, k=2; j <= MAX_A; j+=i, ++k)
			ps[j][i] = ps[k].count(i) ? ps[k][i]+1 : 1;
	}
	for(int i = 0; i < n; ++i) {
		cin >> a;
		for(const auto &ppo : ps[a]) {
			int p = ppo.first, po = ppo.second;
			++ count[p];
			if(po <= mi0[p]) {
				mi1[p] = mi0[p];
				mi0[p] = po;
			} else if(po <= mi1[p]) mi1[p] = po;
		}
	}
	long long res = 1;
	for(int p = 2; p <= MAX_A; ++p) {
		if(count[p] < n-1) continue;
		if(count[p] == n-1) res *= pow(p, mi0[p]);
		else res *= pow(p, mi1[p]);
	}
	cout << res << "\n";

	return 0;
}