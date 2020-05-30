#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef vector<vector<pair<char, int>>> dic;

dic a(1), b(1);

void add_words(dic &a, int A) {
	string s;
	while(A --> 0) {
		cin >> s;
		int n = 0;
		for(char c : s) {
			int k = 0;
			while(k < a[n].size() && a[n][k].first != c) ++k;
			if(k >= a[n].size()) {
				a[n].emplace_back(c, a.size());
				a.emplace_back();
			}
			n = a[n][k].second;
		}
	}
	for(auto &v : a) sort(v.begin(), v.end());
}

int N, A, M, B, nc[4]{0, 0, 0, 0}, nr=0;
int f(int i, int j) {
	if(j >= M) {
		++ i;
		j = 0;
		nr = 0;
		if(i >= N) return 1;
	}
	int nc0 = nc[j], nr0 = nr;
	int x = 0, y = 0;
	int res = 0;
	while(x < a[nc0].size() && y < b[nr0].size()) {
		if(a[nc0][x].first == b[nr0][y].first) {
			nc[j] = a[nc0][x].second;
			nr = b[nr0][y].second;
			res += f(i, j+1);
			++ x; ++y;
		} else if(a[nc0][x].first < b[nr0][y].first) ++x;
		else ++y;
	}
	nc[j] = nc0;
	nr = nr0;
	return res;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> N >> A >> M >> B;
	add_words(a, A);
	add_words(b, B);
	cout << f(0, 0) << "\n";

	return 0;
}