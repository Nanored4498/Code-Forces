#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, m;
	cin >> t;
	while(t--) {
		cin >> n >> m;
		vector<bool> col(n, false);
		int nr=n, nc=m;
		for(int i = 0; i < n; ++i) {
			bool row = false;
			for(int j = 0; j < m; ++j) {
				int a;
				cin >> a;
				if(a) {
					row = true;
					if(!col[j]) {
						col[j] = true;
						--nc;
					}
				}
			}
			if(row) --nr;
		}
		n = min(nr, nc);
		if(n&1) cout << "Ashish\n";
		else cout << "Vivek\n";
	}

	return 0;
}