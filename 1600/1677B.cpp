#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m; cin >> n >> m;
		string s; cin >> s;
		int N = n*m;
		vector<bool> col(m, false);
		vi row(m, 0);
		int c = 0, r = 0, last = -1e7;
		for(int i = 0; i < N; ++i) {
			int x = i%m;
			if(s[i] == '1') {
				if(!col[x]) {
					col[x] = true;
					++ c;
				}
				if(i < last+m) {
					-- row[x];
					++ row[last%m];
				}
				++ r;
				last = i;
			}
			r += row[x];
			cout << c+r << ' ';
		}
		cout << '\n';
	}

	return 0;
}