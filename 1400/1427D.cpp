#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<int> a(n), b(n);
	for(int &x : a) cin >> x;
	int x = 1, y = n;
	int l=0, r=0;
	bool inf = n&1;
	// for(int x : a) cerr << x << ' '; cerr << endl;
	int q = n;
	if((inf && a[0]==1) || (!inf && a[0]==n)) {
		inf = !inf;
		--q;
	}
	cout << q << '\n';
	while(q--) {
		int z = inf ? x : y;
		int i = 0;
		while(a[i] != z) ++i;
		cout << i+1+r << ' ';
		for(int j = 0; j < i; ++j) {
			cout << "1 ";
			b[n-1-j] = a[j];
		}
		cout << n-r-i;
		for(int j = i; j < n-r; ++j) {
			b[r+j-i] = a[j];
		}
		for(int j = 0; j < r; ++j) {
			cout << " 1";
			b[j] = a[n-1-j];
		}
		cout << '\n';
		if(inf) ++x;
		else --y;
		++ r;
		swap(l, r);
		swap(a, b);
		inf = !inf;
		// for(int x : a) cerr << x << ' '; cerr << '[' << l << ' ' << r << ']' << endl;
	}

	return 0;
}