#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vector<bool> a(n+1, false);
	int count = 0;
	int res = 0;
	while(k --> 0) {
		int i;
		cin >> i;
		for(int j = i; j <= n; j += i) {
			a[j] = !a[j];
			if(a[j]) ++count;
			else --count;
		}
		res = max(res, count);
	}
	cout << res << "\n";

	return 0;
}