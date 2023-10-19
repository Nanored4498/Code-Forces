#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, M;
	cin >> N >> M;
	unordered_map<int, int> m;
	vector<int> a(N), b(M);
	for(int &x : a) cin >> x;
	for(int &x : b) cin >> x;

	for(int x : a) for(int y : b) {
		int d = y - x;
		if(d >= 0) m[d] ++;
	}

	int ans = 0;
	for(auto &[diff, count] : m) {
		if(count > m[ans] || (count == m[ans] && diff < ans)) {
			ans = diff;
		}
	}
	cout << ans << '\n';

	return 0;
}