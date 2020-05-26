#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<bool> vb;

int n, k;
vi d, res, add, loop;
vvi id;
vb seen;

int f(int i, int m) {
	seen[i] = true;
	if(m+1 < loop.size() && loop[m+1] == i) ++m;
	-- add[max(m, (int)add.size()-1-k)];
	add.push_back(1);
	for(int j : id[i]) add.back() += f(j, m);
	int a = add.back(); add.pop_back();
	res[i] += a;
	return a;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> k;
	d.resize(n+1);
	id.resize(n+1);
	res.assign(n+1, 0);
	seen.assign(n+1, false);
	for(int i = 1; i <= n; ++i) {
		cin >> d[i];
		id[d[i]].push_back(i);
	}

	for(int i = 1; i <= n; ++i) if(!seen[i]) {
		int j = i;
		while(!seen[j]) {
			seen[j] = true;
			j = d[j];
		}
		loop.clear();
		int l = j;
		do {
			l = d[l];
			loop.push_back(l);
		} while(l != j);
		reverse(loop.begin(), loop.end());
		add.assign(loop.size(), 0);
		j = d[l];
		id[j].erase(find(id[j].begin(), id[j].end(), l));
		int a = f(l, 0);
		while(!loop.empty()) {
			a += add.back(); add.pop_back();
			res[loop.back()] += a; loop.pop_back();
		}
	}

	for(int i = 1; i <= n; ++i) cout << res[i] << "\n";

	return 0;
}