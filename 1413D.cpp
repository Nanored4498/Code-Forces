#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi down(2*n);
	vi up(2*n);
	vi events(2*n);
	vi indices(n);
	vi ans(n);
	int add = 0;
	for(int i = 0; i < 2*n; ++i) {
		down[i] = max(0, i-1);
		up[i] = min(2*n-1, i+1);
		char c;
		cin >> c;
		if(c == '-') {
			int x;
			cin >> x;
			events[i] = -x;
			indices[x-1] = i;
		} else events[i] = add++;
	}

	for(int i = 0; i < n; ++i) {
		int j = indices[i];
		int d = down[j];
		if(events[d] < 0) return cout << "NO\n", 0;
		ans[events[d]] = i+1;
		d = down[d];
		int u = up[j];
		down[u] = d;
		up[d] = u;
	}

	cout << "YES\n";
	for(int x : ans) cout << x << " ";
	cout << "\n";

	return 0;
}