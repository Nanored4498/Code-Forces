#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, tmp;
		cin >> n;
		vi a(n);
		for(int &x: a) cin >> x;
		vector<vi> ans(n), stack(n+1);
		ans[0].push_back(1);
		stack[1].push_back(0);
		for(int i = 1; i < n; ++i) {
			int x = a[i];
			stack[x].push_back(i);
			if(x == 1) {
				ans[i] = ans[i-1];
				ans[i].push_back(1);
			} else {
				do {
					int j = stack[x-1].back();
					stack[x-1].pop_back();
					ans[i] = ans[j];
					ans[i].back() = x;
				} while(ans[i] <= ans[i-1]);
			}
		}
		for(const vi &l : ans) {
			cout << l[0];
			for(int j = 1; j < l.size(); ++j) cout << '.' << l[j];
			cout << '\n';
		}
	}

	return 0;
}