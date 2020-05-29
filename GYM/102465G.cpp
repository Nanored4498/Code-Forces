#include <iostream>
#include <vector>

using namespace std;
typedef unsigned long long ull;
#define MOD 1000000007

string S0;
vector<ull> su0, len, sum;
vector<bool> leaf;
vector<int> X, Y;
void new_node(bool p_leaf, int x, int y) {
	leaf.push_back(p_leaf);
	X.push_back(x);
	Y.push_back(y);
	if(p_leaf) {
		len.push_back(y-x);
		sum.push_back(su0[y]-su0[x]);
	} else {
		len.push_back(len[x] + len[y]);
		sum.push_back((sum[x] + sum[y]) % MOD);
	}
}

int add(int n, ull l, bool left) {
	if(leaf[n]) {
		if(left) new_node(true, X[n], X[n]+l);
		else new_node(true, X[n]+l, Y[n]);
		return X.size()-1;
	} else {
		int nx = X[n], ny = Y[n];
		if(l < len[nx]) {
			if(left) return add(nx, l, left);
			else {
				int nxy = add(nx, l, left);
				new_node(false, nxy, ny);
				return X.size()-1;
			}
		} else if(l > len[nx]) {
			l -= len[nx];
			if(left) {
				int nyx = add(ny, l, left);
				new_node(false, nx, nyx);
				return X.size()-1;
			} else return add(ny, l, left);
		} else if(left) return nx;
		else return ny;
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, x, y;
	ull lo, hi;
	string name;
	cin >> N;
	cin >> S0;
	su0.assign(S0.size()+1, 0);
	for(int i = 0; i < S0.size(); ++i) su0[i+1] = su0[i] + S0[i]; 
	new_node(true, 0, S0.size());
	vector<int> nodes(N, 0);
	
	for(int i = 1; i < N; ++i) {
		cin >> name;
		if(name[0] == 'A') {
			cin >> x >> y;
			nodes[i] = X.size();
			new_node(false, nodes[x], nodes[y]);
		} else {
			cin >> x >> lo >> hi;
			int n = nodes[x];
			while(!leaf[n]) {
				int nx = X[n];
				if(hi <= len[nx]) {
					n = nx;
				} else if(lo >= len[nx]) {
					lo -= len[nx];
					hi -= len[nx];
					n = Y[n];
				} else break;
			}
			if(leaf[n]) {
				nodes[i] = X.size();
				new_node(true, X[n]+lo, X[n]+hi);
			} else {
				int nx = add(X[n], lo, false);
				int ny = add(Y[n], hi-len[X[n]], true);
				nodes[i] = X.size();
				new_node(false, nx, ny);
			}
		}
	}
	cout << sum[nodes[N-1]] << "\n";

	return 0;
}