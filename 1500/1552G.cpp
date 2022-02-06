#include <iostream>
#include <vector>

using namespace std;

int n, k;
vector<vector<int>> a;
bool f(int i, const string &v) {
	if(i == k) {
		int j = 0;
		while(j < n && v[j] == 0) ++j;
		if(j < n && v[j] == 2) ++j;
		while(j < n && v[j] == 1) ++j;
		return j == n;
	}
	int count[3] {};
	for(int j : a[i]) ++ count[v[j]];
	string w = v;
	for(int j = 0; j < count[0]; ++j) w[a[i][j]] = 0;
	for(int j = count[0]; j < a[i].size(); ++j) w[a[i][j]] = 1;
	for(int k = 0; k < count[2]; ++k) {
		if(!f(i+1, w)) return false;
		w[a[i][count[0]+k]] = 0;
	}
	return f(i+1, w);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> k;
	a.resize(k);
	for(auto &v : a) {
		int q; cin >> q;
		v.resize(q);
		for(int &x : v) { cin >> x; --x; }
	}
	cout << (f(0, string(n, 2)) ? "ACCEPTED" : "REJECTED") << endl;

	return 0;
}