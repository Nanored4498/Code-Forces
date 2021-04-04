#include <iostream>
#include <vector>

using namespace std;

int main() {
	int n;
	cin >> n;
	int i = 1;
	vector<bool> found(n+1, false);
	vector<int> p(n);
	for(int j = 2; j <= n; ++j) {
		int a, b;
		cout << "? " << i << " " << j << endl;
		cin >> a;
		cout << "? " << j << " " << i << endl;
		cin >> b;
		if(a > b) {
			found[a] = true;
			p[i-1] = a;
			i = j;
		} else {
			found[b] = true;
			p[j-1] = b;
		}
	}
	-- i;
	p[i] = 1;
	while(found[p[i]]) ++p[i];
	cout << "!";
	for(int a : p) cout << " " << a;
	cout << endl;

	return 0;
}