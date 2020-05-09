#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int S, L, N;
	cin >> S >> L >> N;
	vector<string> i2s(S);
	for(int i = 0; i < S; ++i) cin >> i2s[i];
	sort(i2s.begin(), i2s.end());
	map<string, int> s2i;
	for(int i = 0; i < S; ++i) s2i[i2s[i]] = i;
	bool A[200][200];
	for(int i = 0; i < S; ++i)
		for(int j = 0; j < S; ++j)
			A[i][j] = false;
	string a, b;
	while(L --> 0) {
		cin >> a >> b;
		int i = s2i[a], j = s2i[b];
		A[i][j] = A[j][i] = true;
	}
	vector<int> in(N), out(N);
	for(int i = 0; i < N; ++i) {
		cin >> a;
		in[i] = s2i[a];
	}

	vector<int> com(S, 0);
	for(int i = 0; i < N; ++i) {
		for(int s = 0; s < S; ++s) {
			while((in[com[s]] < 0 || A[s][in[com[s]]]) && com[s]+1 < N) ++com[s];
			if(in[com[s]] == s) { in[com[s]] = -1; out[i] = s; break; }
		}
	}

	cout << i2s[out[0]];
	for(int i = 1; i < N; ++i) cout << " " << i2s[out[i]];
	cout << "\n";

	return 0;
}