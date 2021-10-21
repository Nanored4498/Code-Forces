#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

string s;
int pos = 0;

bool solve(int k) {
	int N = s.size();
	const ll p = 29;
	const ll M = 1e9+7;
	ll h = 0, pk = 1;
	for(int i = 0; i < k; ++i) {
		h = (p*h + (s[i]-'A')) % M;
		pk = (pk * p) % M;
	}
	vector<pair<ll, int>> hs;
	hs.reserve(N-k+2);
	hs.emplace_back(h, k-1);
	for(int i = k; i < N; ++i) {
		h = (p*h - pk*(s[i-k]-'A') + (s[i]-'A')) % M;
		if(h < 0) h += M;
		hs.emplace_back(h, i);
	}
	sort(hs.begin(), hs.end());
	pos = N;
	int i = 0;
	while(i < hs.size()) {
		if(i+1 == hs.size() || hs[i].first != hs[i+1].first)
			pos = min(pos, hs[i++].second - k + 1);
		else {
			while(hs[i].first == hs[i+1].first) ++i;
			++ i;
		}
	}
	if(pos < N) return true;
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> s;
	int N = s.size();
	int a = 1, b = N;
	while(a < b) {
		int c = (a+b)/2;
		if(solve(c)) b = c;
		else a = c+1;
	}
	solve(a);
	cout << s.substr(pos, a) << '\n';

	return 0;
}