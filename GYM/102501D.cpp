#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef long long ll;
#define P0 50000

ll P1 = P0;
map<pair<ll, ll>, ll> P;
pair<ll, ll> LR[P0];

struct Prot {
	bool failed=false;
	vector<ll> p;
	ll n = 0;
	ll pop() {
		if(p.empty()) return n++;
		else { ll r = p.back(); p.pop_back(); return r; }
	}
	void push(ll a) { p.push_back(a); }
};

Prot apply(const string &ops) {
	Prot chain;
	ll a, b;
	pair<ll, ll> p;
	for(char op : ops) {
		switch(op) {
		case 'C':
			a = chain.pop();
			chain.push(a);
			chain.push(a);
			break;
		case 'D':
			chain.pop();
			break;
		case 'L':
			a = chain.pop();
			if(a < P0 || a >= P1) { chain.failed = true; return chain; }
			chain.push(LR[a-P0].first);
			break;
		case 'P':
			a = chain.pop();
			b = chain.pop();
			p = {a, b};
			if(!P.count(p)) { LR[P1-P0] = p; P[p] = P1++; }
			chain.push(P[p]);
			break;
		case 'R':
			a = chain.pop();
			if(a < P0 || a >= P1) { chain.failed = true; return chain; }
			chain.push(LR[a-P0].second);
			break;
		case 'S':
			a = chain.pop();
			b = chain.pop();
			chain.push(a);
			chain.push(b);
			break;
		case 'U':
			a = chain.pop();
			if(a < P0 || a >= P1) { chain.failed = true; return chain; }
			chain.push(LR[a-P0].second);
			chain.push(LR[a-P0].first);
			break;
		}
	}
	return chain;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string a, b;
	cin >> a >> b;
	Prot pa = apply(a), pb = apply(b);
	if(pa.failed) cout << (pb.failed ? "True\n" : "False\n");
	else if(pb.failed) cout << "False\n";
	else {
		bool good = true;
		ll n = max(pa.n, pb.n), i, j;
		do {
			i = pa.pop();
			j = pb.pop();
			if(i != j) {
				good = false;
				break;
			}
		} while(i != n);
		cout << (good ? "True\n" : "False\n");
	}

	return 0;
}