#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int T, n;
	string s, t;
	int ms[256][2001], mt[256][2001];
	cin >> T;
	while(T--) {
		cin >> n >> s >> t;
		for(int c = 0; c < 256; ++c) ms[c][n] = mt[c][n] = 0;
		for(int i = n-1; i >= 0; --i) {
			for(int c = 0; c < 256; ++c) {
				ms[c][i] = ms[c][i+1];
				mt[c][i] = mt[c][i+1];
			}
			++ ms[s[i]][i];
			++ mt[t[i]][i];
		}
		bool ok = true;
		for(int c = 0; c < 256; ++c)
			if(ms[c][0] != mt[c][0])
				ok = false;
		if(ok) {
			vi curr(n, 0), pred(n);
			for(int i = 0; i < n; ++i) {
				swap(curr, pred);
				for(int j = i; j < n; ++j) {
					curr[j] = 1 + pred[j];
					char x = t[j];
					if(s[i] == x)
						curr[j] = min(curr[j], j > 0 ? pred[j-1] : 0);
					if(i < j && j+1 < n && ms[x][i+1] > mt[x][j+1])
						curr[j] = min(curr[j], curr[j-1]);
				}
			}
			cout << curr[n-1] << "\n";
		} else {
			cout << "-1\n";
		}
	}

	return 0;
}