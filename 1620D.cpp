#include <iostream>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, a[100];
	cin >> t;
	while(t--) {
		int n, m[3]={};
		cin >> n;
		for(int i = 0; i < n; ++i) {
			cin >> a[i];
			m[a[i]%3] = 1;
		}
		sort(a, a+n);
		int add = m[1]+m[2], j = n-1;
		while(j > 0 && a[j] == a[n-1]) --j;
		if(add==2 && (a[n-1]%3==0 || (a[n-1]%3==1 && a[0] != 1 && a[j]!=a[n-1]-1))) --add;
		cout << a[n-1]/3+add << '\n';
	}

	return 0;	
}