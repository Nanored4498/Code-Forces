#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	cin >> N;
	string s;
	cin >> s;
	vector<vi> d(N, vi(N));
	for(int i = 0; i < N; ++i) d[i][i] = 1;
	for(int l = 2; l <= N; ++l) {
		for(int i = 0; i <= N-l; ++i) {
			int j = i+l-1;
			d[i][j] = 1 + d[i+1][j];
			for(int c = 1; 2*c <= l; ++c) {
				int a = i, b = i+c;
				while(b <= j && s[a] == s[b]) {
					++a;
					++b;
				}
				int n = (b-i)/c;
				d[i][j] = min(d[i][j], d[i][i+c-1] + (i+n*c <= j ? d[i+n*c][j] : 0));
			}
		}
	}
	cout << d[0][N-1] << "\n";

	return 0;
}