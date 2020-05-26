#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
typedef pair<int, int> pii;
typedef vector<pii> vii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi p[2] = {vi(n), vi(n)}, h[2] = {vi(n), vi(n)};
	vi r[2] = {vi(n), vi(n)};
	for(int i = 0; i < 2; ++i) {
		for(int j = 0; j < n; ++j) cin >> p[i][j];
		for(int j = 0; j < n; ++j) cin >> h[i][j];
		for(int j = 0; j < n; ++j) r[i][j] = j;
		sort(r[i].begin(), r[i].end(), [&](int a, int b) { return p[i][a] < p[i][b]; });
	}
	int i = 0, k = 0, l = 0;
	while(true) {
		i = min(k, l);
		if(i >= n) break;
		while(k < n && p[0][r[0][k]] == p[0][r[0][i]]) ++k;
		while(l < n && p[1][r[1][l]] == p[1][r[1][i]]) ++l;
		if(k < l) {
			while(true) {
				int k2 = k;
				while(k2 < n && p[0][r[0][k2]] == p[0][r[0][k]]) ++k2;
				if(k2 > l || k2==k) break;
				k = k2;
			}
			vi r0(k-i), r1(k-i), r2;
			for(int j = i; j < k; ++j) r0[j-i] = j;
			sort(r0.begin(), r0.end(), [&](int a, int b) { return h[0][r[0][a]] > h[0][r[0][b]]; });
			sort(r[1].begin()+i, r[1].begin()+l, [&](int a, int b) { return h[1][a] > h[1][b]; });
			int j2 = i;
			for(int j = 0; j < k-i; ++j) {
				int hj = h[0][r[0][r0[j]]];
				while(j2 < l && h[1][r[1][j2]] >= hj) r2.push_back(r[1][j2++]);
				if(j2 < l) r1[j] = r[1][j2++];
				else { cout << "impossible\n"; return 0; }
			}
			for(int j = 0; j < k-i; ++j) r[1][r0[j]] = r1[j];
			for(int j = 0; j < r2.size(); ++j) r[1][k+j] = r2[j];
		} else {
			while(true) {
				int l2 = l;
				while(l2 < n && p[1][r[1][l2]] == p[1][r[1][l]]) ++l2;
				if(l2 > k || l2==n) break;
				l = l2;
			}
			vi r0(l-i), r1(l-i), r2;
			for(int j = i; j < l; ++j) r0[j-i] = j;
			sort(r0.begin(), r0.end(), [&](int a, int b) { return h[1][r[1][a]] < h[1][r[1][b]]; });
			sort(r[0].begin()+i, r[0].begin()+k, [&](int a, int b) { return h[0][a] < h[0][b]; });
			int j2 = i;
			for(int j = 0; j < l-i; ++j) {
				int hj = h[1][r[1][r0[j]]];
				while(j2 < k && h[0][r[0][j2]] <= hj) r2.push_back(r[0][j2++]);
				if(j2 < k) r1[j] = r[0][j2++];
				else { cout << "impossible\n"; return 0; }
			}
			for(int j = 0; j < l-i; ++j) r[0][r0[j]] = r1[j];
			for(int j = 0; j < r2.size(); ++j) r[0][l+j] = r2[j];
		}
	}

	for(int j = 0; j < 2; ++j) {
		cout << r[j][0]+1;
		for(int i = 1; i < n; ++i) cout << " " << r[j][i]+1;
		cout << "\n";
	}

	return 0;
}