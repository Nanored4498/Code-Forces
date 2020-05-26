//https://codeforces.com/contest/1358/problem/F
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<ll> vl;

#define MNP 200000

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vl A(n), B(n);
	for(int i = 0; i < n; ++i) cin >> A[i];
	for(int i = 0; i < n; ++i) cin >> B[i];
	string res = "";
	if(n == 1) {
		if(A[0] == B[0]) cout << "SMALL\n0\n";
		else cout << "IMPOSSIBLE\n";
	} else if(n == 2) {
		ll np = 0;
		ll miA = min(A[0], A[1]), maA = max(A[0], A[1]);
		while(A[0] != B[0] || A[1] != B[1]) {
			cerr << B[0] << " " << B[1] << endl;
			if((A[0] == B[1] && A[1] == B[0]) || B[0] > B[1]) {
				res += 'R';
				swap(B[0], B[1]);
			} else if(B[0] < miA){
				break;
			} else {
				ll k = B[1] / B[0];
				if(B[0] == miA) {
					if(B[1] < maA || (B[1]-maA) % B[0] != 0) break;
					k = (B[1]-maA) / B[0];
				}
				B[1] -= k*B[0];
				np += k;
				if(np <= MNP) for(int i = 0; i < k; ++i) res += 'P';
			}
		}
		if(A[0] != B[0] || A[1] != B[1]) cout << "IMPOSSIBLE\n";
		else if(np <= MNP) {
			cout << "SMALL\n" << res.size() << "\n";
			reverse(res.begin(), res.end());
			cout << res << "\n";
		} else cout << "BIG\n" << np << "\n";
	} else {
		ll np = 0;
		bool eq = false;
		while(B[0] >= 1) {
			eq = true;
			for(int i = 0; i < n; ++i) if(A[i] != B[i]) { eq = false; break; }
			if(eq) break;
			eq = true;
			for(int i = 0; i < n; ++i) if(A[n-1-i] != B[i]) { eq = false; break; }
			if(eq) { res += 'R'; break; }
			bool inc = true;
			for(int i = 1; i < n; ++i) if(B[i-1] >= B[i]) { inc = false; break; }
			if(inc) {
				for(int i = n-1; i > 0; --i) B[i] -= B[i-1];
				++ np;
				if(np <= MNP) res += 'P';
			} else if(res.empty() || res.back() != 'R') {
				res += 'R';
				reverse(B.begin(), B.end());
			} else break;
		}
		if(!eq) cout << "IMPOSSIBLE\n";
		else if(np <= MNP) {
			cout << "SMALL\n" << res.size() << "\n";
			reverse(res.begin(), res.end());
			cout << res << "\n";
		} else cout << "BIG\n" << np << "\n";
	}

	return 0;
}