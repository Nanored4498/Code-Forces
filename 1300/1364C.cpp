#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi a(n), b(n);
	set<int> vals;
	for(int i = 0; i < n; ++i) {
		cin >> a[i];
		if(a[i] > i+1) return cout << "-1\n", 0;
		vals.insert(a[i]);
	}
	int v = 0;
	for(int i = 0; i < n; ++i) {
		if(i > 0 && a[i] > a[i-1]) b[i] = a[i-1];
		else {
			while(vals.count(v)) ++v;
			b[i] = v++;
		}
		cout << b[i] << " \n"[i+1==n];
	}

	return 0;
}