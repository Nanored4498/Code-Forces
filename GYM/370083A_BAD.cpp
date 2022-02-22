#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef vector<ll> vi;

ll sumDiv(ll n, ll d) {
	ll q = n/d, r = n%d;
	return d * (q*(q+1))/2 - q * (d-1-r);
}

ll sum2Div(ll n, ll d) {
	ll q = n/d, r = n%d;
	return d * (q*(q+1)*(2*q+1))/6 - q*q * (d-1-r);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int X = 50, Y = 50;
	vector<vi> F(X, vi(Y, -1));
	vector<pair<int, int>> Q, Q2;
	F[0][0] = 0;
	Q.emplace_back(0, 0);
	while(!Q.empty()) {
		Q2.clear();
		for(auto [i, j] : Q) {
			for(int dx = -2; dx <= 2; ++dx) if(dx != 0)
				for(int dy = -2; dy <= 2; ++dy) if(abs(dx)+abs(dy)==3) {
					int x = i+dx, y = j+dy;
					if(x < 0 || x >= X) continue;
					if(y < 0 || y >= Y) continue;
					if(F[x][y] != -1) continue;
					F[x][y] = F[i][j]+1;
					Q2.emplace_back(x, y);
				}
		}
		swap(Q, Q2);
	}

	/*
	for(ll i = 0; i < X; ++i) {
		for(ll j = 0; j < Y; ++j) {
			ll x = F[i][j] - (i+j)/3 - ((i+j)%3);
			ll k = j - 2*i - 3;
			if(k >= 0) x -= 2 * (k/12 + 1);
			if(k >= 0 && ((k%12) == 1 || (k%12) == 2 || (k%12) == 5)) x += 2;
			k = i - 2*j - 3;
			if(k >= 0) x -= 2 * (k/12 + 1);
			if(k >= 0 && ((k%12) == 1 || (k%12) == 2 || (k%12) == 5)) x += 2;
			cout << x << ' ';
		}
		cout << endl;
	}
	*/

	for(ll i = 0; i < X; ++i) for(ll j = 0; j < Y; ++j) {
		F[i][j] = (i+j)/3;
	}
	for(ll i = 0; i < X; ++i) for(ll j = 1; j < Y; ++j) F[i][j] += F[i][j-1];
	// for(int i = 1; i < X; ++i) for(int j = 0; j < Y; ++j) F[i][j] += F[i-1][j];
	for(ll i = 0; i < X; ++i) {
		for(ll j = 0; j < Y; ++j) {
			ll x = F[i][j];
			x -= sumDiv(i+j, 3) - sumDiv(i-1, 3);
			cout << x << ' ';
		}
		cout << endl;
	}

	return 0;
}