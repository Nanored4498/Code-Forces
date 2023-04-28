#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		vector<int> a(n+1, 0);
		a[0] = 1;
		while(--n) {
			int p; cin >> p;
			++ a[p];
		}
		vector<int> c;
		for(int x : a) if(x) c.push_back(x);
		sort(c.begin(), c.end());
		vector<int> b;
		int ans = 0;
		while(!c.empty() || !b.empty()) {
			++ans;
			int i = 0;
			while(i < b.size()) {
				if(--b[i] == 0) {
					b[i] = b.back();
					b.pop_back();
				} else ++i;
			}
			if(!c.empty()) {
				int x = c.back()-1;
				c.pop_back();
				if(x) b.push_back(x);
			} else if(!b.empty()) {
				auto it = max_element(b.begin(), b.end());
				*it -= 1;
				if(*it == 0) {
					*it = b.back();
					b.pop_back();
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}