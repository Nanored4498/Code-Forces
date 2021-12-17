#include <iostream>
#include <vector>

using namespace std;

bool querry(int a, int b, int c) {
	cout << "? " << a+1 << ' ' << b+1 << ' ' << c+1 << endl;
	bool r; cin >> r; return r;
}

int main() {
	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<bool> ans(n/3), cr(n);
		for(int i = 0; i < n; i += 3)
			ans[i/3] = querry(i, i+1, i+2);
		int a=0, b=0, A, B;
		while(ans[b]==ans[a]) ++b;
		if(ans[a]) swap(a, b);
		bool x = querry(3*a, 3*a+1, 3*b), y = querry(3*a, 3*a+1, 3*b+1);
		if(x) {
			A = y ? 3*a+2 : 3*b+1;
			B = 3*b;
		} else if(y) {
			A = 3*b;
			B = 3*b+1;
		} else {
			A = 3*a;
			B = querry(A, 3*b, 3*b+1) ? 3*b : 3*b+2;
		}
		for(int i = 0; i < n; i += 3) {
			if(i/3 == A/3 || i/3 == B/3) {
				for(int j = 0; j < 3; ++j)
					if(i+j == A) cr[i+j] = false;
					else if(i+j == B) cr[i+j] = true;
					else cr[i+j] = querry(A, B, i+j);
			} else {
				for(int j = 0; j < 3; ++j) cr[i+j] = ans[i/3];
				bool x = querry(cr[i] ? A : B, i, i+1), y = querry(cr[i] ? A : B, i, i+2);
				if(x == cr[i]) cr[i+2] = y;
				else if(y == x) cr[i] = x;
				else cr[i+1] = x;
			}
		}
		int k = 0;
		for(bool x : cr) if(!x) ++k;
		cout << "! " << k;
		for(int i = 0; i < n; ++i) if(!cr[i]) cout << ' ' << i+1;
		cout << endl;
	}
	return 0;
}