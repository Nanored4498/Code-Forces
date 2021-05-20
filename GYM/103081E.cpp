#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, ans = 10'000;
	cin >> N;
	for(int i = 0; i < N; ++i) {
		int a, b;
		cin >> a >> b;
		ans = min(ans, b/a);
	}
	cout << ans << '\n';

	return 0;
}