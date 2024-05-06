#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int l, b;
vi L;
vector<vi> b2l;
vector<pair<int, int>> l2b;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> l >> b;
	l2b.assign(l, {-1, -1});
	string s;
	cin >> s;
	L.resize(l);
	for(int i = 0; i < l; ++i) L[i] = s[i] == 'R' ? 0 : (s[i] == 'G' ? 1 : 2);
	b2l.resize(b);
	for(int j = 0; j < b; ++j) {
		int k;
		cin >> k;
		b2l[j].resize(k);
		for(int &i : b2l[j]) {
			cin >> i;
			--i;
			if(l2b[i].first == -1) l2b[i].first = j;
			else l2b[i].second = j;
		}
	}

	int ans = 0;

	vi Q;
	for(int i = 0; i < l; ++i) if(l2b[i].second == -1) Q.push_back(i);
	while(!Q.empty()) {
		int i = Q.back();
		Q.pop_back();
		int j = l2b[i].first;
		if(j == -1) {
			if(L[i] != 0) {
				cout << "impossible\n";
				return 0;
			}
		} else {
			int o = (3-L[i]) % 3;
			ans += o;
			for(int k : b2l[j]) {
				L[k] = (L[k]+o)%3;
				if(l2b[k].first == j) l2b[k].first = l2b[k].second;
				l2b[k].second = -1;
				Q.push_back(k);
			}
		}
	}

	for(int i = 0; i < b; ++i) if(off[i] == -1) {
		fill_n(cnt, 3, 0);
	}

	cout << ans << '\n';

	return 0;
}