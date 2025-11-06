#include <iostream>

using namespace std;

int main() {
	string run;
	int t;
	cin >> run >> t;
	while(t--) {
		if(run == "first") {
			int n;
			cin >> n;
			bool one = false;
			for(int i = 0; i < n; ++i) {
				int p;
				cin >> p;
				if(p == n) cout << "01"[one] << '\n';
				else if(p == 1) one = true;
			}
		} else {
			int n, x;
			cin >> n >> x;
			int l = 1, r = n;
			if(x) {
				for(int add = 1<<13; add; add >>= 1) {
					int r2 = r-add, ans;
					if(r2 <= l) continue;
					cout << "? " << l << ' ' << r2 << endl;
					cin >> ans;
					if(ans == n-1) r = r2;
				}
				cout << "! " << r << endl;
			} else {
				for(int add = 1<<13; add; add >>= 1) {
					int l2 = l+add, ans;
					if(l2 >= r) continue;
					cout << "? " << l2 << ' ' << r << endl;
					cin >> ans;
					if(ans == n-1) l = l2;
				}
				cout << "! " << l << endl;
			}
		}
	}
	return 0;
}