#include <iostream>
#include <vector>

using namespace std;

int n, k;
string c;
vector<int> s;
vector<bool> good;
vector<pair<int, int>> G;

void f0(int i) {
	if(i == -1) return;
	f0(G[i].first-1);
	s.push_back(i);
	f0(G[i].second-1);
}

bool f(int i, int np=0) {
	if(i == -1) return false;
	bool d = false;
	if(f(G[i].first-1, np+1)) d = true;
	else if(np+1 <= k && good[i]) {
		k -= np+1;
		d = true;
	} else np = k;
	if(d) {
		cout << c[i];
		np = -1;
	}
	cout << c[i];
	f(G[i].second-1, np+1);
	return d;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> k >> c;
	G.resize(n);
	for(auto &x : G) cin >> x.first >> x.second;
	f0(0);
	good.resize(n, false);
	for(int i = n-1; i > 0; --i)
		if(c[s[i-1]] < c[s[i]]) good[s[i-1]] = true;
		else if(c[s[i-1]] == c[s[i]]) good[s[i-1]] = good[s[i]];
	f(0);
	cout << endl;

	return 0;
}