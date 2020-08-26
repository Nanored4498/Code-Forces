#include <iostream>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll k;
	cin >> k;
	string s = "codeforces";
	ll n[10], c=1;
	for(int i = 0; i < 10; ++i) n[i] = 1; 
	int i = 0;
	while(c < k) {
		c /= n[i];
		++ n[i];
		c *= n[i];
		i = (i+1)%10;
	}
	string ans;
	for(int i = 0; i < 10; ++i) while(n[i]--) cout << s[i];
	cout << "\n";

	return 0;
}