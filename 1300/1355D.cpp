#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, S;
	cin >> N >> S;
	if(2*N <= S) {
		cout << "YES\n";
		for(int i = 0; i < N-1; ++i) cout << "2\n";
		cout << S-2*(N-1) << "\n";
		cout << "1\n";
	} else cout << "NO\n";

	return 0;
}