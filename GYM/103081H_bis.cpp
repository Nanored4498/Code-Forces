#include <iostream>
#include <vector>
#include <map>
#include <sstream>

using namespace std;
#define LSB(i) ((i)&(-(i)))

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N; cin >> N;
	vector<map<int, int>> m(N+1);
	vector<int> cd(N+1, 0);
	{ string line; getline(cin, line); }
	for(int d = 1; d <= N; ++d) {
		string line; getline(cin, line);
		stringstream ss(line); char c;
		cd[d] = cd[d-1];
		while(ss >> c) {
			int i; ss >> i;
			int a = c=='+'?1:-1;
			for(++i; i <= N; i += LSB(i)) m[i][d] += a;
			cd[d] += a;
		}
	}
	for(auto &s : m) {
		int cum = 0;
		for(auto &[i, n] : s) n = cum += n;
	}
	int x = 0;
	for(int i = 0; i < N; ++i) {
		int d; cin >> d;
		for(int y = x; y; y -= LSB(y)) {
			auto it = m[y].upper_bound(d);
			if(it == m[y].begin()) continue;
			x -= (--it)->second;
		}
		x = (x + cd[d]) % N;
	}
	cout << x << endl;

	return 0;
}