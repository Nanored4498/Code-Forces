#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

string s;
int f(int i, int j, char c) {
	if(j == i+1) return s[i] != c;
	int k = i + (j-i)/2;
	int ans = f(i, k, c+1), ans2 = f(k, j, c+1);
	for(int l = k; l < j; ++l) ans += s[l] != c;
	for(int l = i; l < k; ++l) ans2 += s[l] != c;
	return min(ans, ans2);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n;
	cin >> t;
	while(t--) {
		cin >> n >> s;
		cout << f(0, n, 'a') << "\n";
	}

	return 0;
}