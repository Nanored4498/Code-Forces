#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, M, T, a, b;
	cin >> N >> M >> T;
	vector<vector<int>> P(N);
	vector<int> start, start2, to(N, -1);
	while(M --> 0) {
		cin >> a >> b;
		if(b == T) {
			start.push_back(a);
			to[a] = a;
		} else P[b].push_back(a);
	}
	while(!start.empty()) {
		start2.clear();
		for(int i : start) for(int j : P[i]) if(to[j] != to[i]) {
			to[j] = to[j]==-1 ? to[i] : -2;
			start2.push_back(j);
		}
		swap(start, start2);
	}
	vector<int> ans;
	for(int i = 0; i < N; ++i) if(to[i] == i) ans.push_back(i);
	cout << ans.size() << "\n";
	for(int i : ans) cout << i << "\n";

	return 0;
}