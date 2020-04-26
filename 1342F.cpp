#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int T, n, nb, nc, k;
int a[16], b[16], c[16];
int ib[16], ic[16];

bool solve0(int i, int j) {
	if(b[i] >= b[i+1]) return false;
	if(i == 0 && nc == 0) {
		if(b[0] < b[1]) {
			cout << k << "\n";
			return true;
		}
		return false;
	}
	if(j >= nc) return solve0(i-1, 0);
	if(j < nc && b[i] + c[j] < b[i+1]) {
		b[i] += c[j];
		swap(c[j], c[--nc]);
		swap(ic[j], ic[nc]);
		if(solve0(i, j)) {
			swap(ic[j], ic[nc]);
			swap(c[j], c[nc++]);
			b[i] -= c[j];
			cout << ic[j]+1 << " " << ib[i]+1 << "\n";
			for(int k = 0; k < 15; ++k) {
				if(ib[k] > ic[j]) --ib[k];
				if(ic[k] > ic[j]) --ic[k];
			}
			return true;
		}
		swap(ic[j], ic[nc]);
		swap(c[j], c[nc++]);
		b[i] -= c[j];
	}
	if(i > 0 && solve0(i, j+1)) return true;
	return false;
}

bool solve(int i, int k) {
	if(i == n) return solve0(nb-1, 0);
	if(k+i < n) {
		ib[nb] = i;
		b[nb++] = a[i];
		if(solve(i+1, k)) return true;
		--nb;
	}
	if(k > 0) {
		ic[nc] = i;
		c[nc++] = a[i];
		if(solve(i+1, k-1)) return true;
		--nc;
	}
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cout.tie(nullptr);

	cin >> T;
	while(T --> 0) {
		cin >> n;
		for(int i = 0; i < n; ++i) cin >> a[i];
		k = 0;
		while(true) {
			nb = nc = 0;
			b[n-k] = 100000000;
			if(solve(0, k)) break;
			++ k;
		}
	}

	return 0;
}