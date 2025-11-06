#include <iostream>
#include <vector>

using namespace std;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string run;
	cin >> run;
	if(run == "first") {
		int n;
		cin >> n;
		while(n--) {
			int a;
			cin >> a;
			--a;
			while(a) {
				int r = a%25;
				a /= 25;
				cout << (char)(r+'a');
			}
			cout << 'z';
		}
		cout << '\n';
	} else {
		string s;
		cin >> s;
		vi a;
		int mul = 1, x = 0;
		for(char c : s) {
			if(c == 'z') {
				a.push_back(x+1);
				mul = 1;
				x = 0;
			} else {
				x += (c-'a')*mul;
				mul *= 25;
			}
		}
		cout << a.size() << '\n';
		for(int x : a) cout << x << ' ';
		cout << '\n';
	}

	return 0;
}