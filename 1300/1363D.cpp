#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main() {

	int t, n, k;
	cin >> t;
	string ans;
	while(t --> 0) {
		cin >> n >> k;
		vector<set<int>> S(k);
		for(int i = 0; i < k; ++i) {
			int c, s;
			cin >> c;
			for(int j = 0; j < c; ++j) {
				cin >> s;
				S[i].insert(s);
			}
		}
		cout << "? " << n;
		for(int i = 1; i <= n; ++i) cout << " " << i;
		cout << endl;
		int m, x;
		cin >> m;
		int a = 1, b = n;
		while(a < b) {
			int c = (a+b-1)/2;
			cout << "? " << c;
			for(int i = 1; i <= c; ++i) cout << " " << i;
			cout << endl;
			cin >> x;
			if(x == -1) return 1;
			if(x == m) b = c;
			else a = c+1;
		}
		vector<int> P(k, m);
		for(int i = 0; i < k; ++i) {
			if(S[i].count(a)) {
				cout << "? " << n-S[i].size();
				for(int j = 1; j <= n; ++j) if(!S[i].count(j)) cout << " " << j;
				cout << endl;
				cin >> P[i];
				if(P[i] == -1) return 1;
			}
		}
		cout << "!";
		for(int i = 0; i < k; ++i) cout << " " << P[i];
		cout << endl;
		cin >> ans;
		if(ans[0] == 'I') return 1;
	}

	return 0;
}