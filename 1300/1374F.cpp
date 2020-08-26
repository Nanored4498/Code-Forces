#include <iostream>
#include <queue>

using namespace std;
typedef pair<int, int> pii;

int a[501], ans[250000];

void move(int i) {
	swap(a[i], a[i+1]);
	swap(a[i], a[i+2]);
}
void upd(int m, int &k) {
	int i = 1;
	for(int j = 2; j <= m; ++j) if(a[j] >= a[i]) i = j;
	if(i == 1 && m == 2) {
		int t = 0;
		while(t < 2 && (a[1] > a[2] || a[2] > a[3])) {
			ans[k++] = 1;
			move(1);
			++ t;
		}
		return;
	}
	while(i < m) {
		bool first = i==1;
		if(first) ++i;
		ans[k++] = i-1;
		move(i-1);
		if(!first) ++i;
	}
}

int main() {
	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		for(int i = 1; i <= n; ++i) cin >> a[i];
		int k = 0;
		bool same = false;
		int m2, k2;
		for(int m = n; m > 0; --m) {
			upd(m, k);
			if(m < n && a[m] == a[m+1]) {
				same = true;
				m2 = m;
				k2 = k;
			}
		}
		if(a[1] <= a[2] && a[2] <= a[3]) {
			cout << k << "\n";
			for(int i = 0; i < k ; ++i) cout << ans[i] << " ";
			cout << "\n";
		} else if(same) {
			while(k > k2) {
				int i = ans[--k];
				move(i); move(i);
			}
			ans[k++] = m2-1;
			move(m2-1);
			for(int m = m2; m > 0; --m) upd(m, k);
			cout << k << "\n";
			for(int i = 0; i < k ; ++i) cout << ans[i] << " ";
			cout << "\n";
		} else cout << "-1\n";
	}
}