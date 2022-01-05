#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		if(k > (n+1)/2) {
			cout << "-1\n";
			continue;
		}
		string l(n, '.');
		l.push_back('\n');
		for(int i = 0; i < n; ++i)
			if(i%2==0 && i/2<k) {
				l[i] = 'R';
				cout << l;
				l[i] = '.';
			} else cout << l;
	}

	return 0;
}