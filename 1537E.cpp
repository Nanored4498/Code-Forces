#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	string s;
	cin >> n >> k >> s;
	n = min(n, k);
	s.resize(n);

	int j = 1;
	while(j < n) {
		int i = 0;
		while(j+i < n && s[i] == s[j+i]) ++i;
		if(i+j >= n || s[i] < s[j+i]) break;
		j += i+1;
	}
	s.resize(j);
	while(s.size() < k) s += s;
	s.resize(k);

	cout << s << endl;

	return 0;	
}