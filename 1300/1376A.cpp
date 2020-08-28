#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi a(n);
	for(int i = 0; i < n; ++i) cin >> a[i];
	sort(a.begin(), a.end());
	for(int i = 0; i < n; ++i) cout << a[i] << " \n"[i+1==n];

	return 0;
}