#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
typedef long long ll;

template<typename A, typename B>
struct std::hash<pair<A, B>> {
	size_t operator()(const pair<A, B> &x) const {
		return (hash<A>{}(x.first) << 3) ^ (hash<B>{}(x.second)+1);
	}
};
template<typename A>
struct std::hash<array<A, 3>> {
	size_t operator()(const array<A, 3> &x) const {
		return (hash<A>{}(x[0]) << 3) ^ (hash<A>{}(x[1]) << 1) ^ (hash<A>{}(x[2]) + 1);
	}
};

struct Node {
	int l, r;
	ll ones;
	Node(int l, int r, ll o): l(l), r(r), ones(o) {}
};
vector<Node> nodes{{0,0,0}, {0,0,1}};
inline int L(int a) { return nodes[a].l; }
inline int R(int a) { return nodes[a].r; }

unordered_map<pair<int, int>, int> S;
int get(int l, int r) {
	auto [it, add] = S.emplace(make_pair(l,r), nodes.size());
	if(add) nodes.emplace_back(l, r, nodes[l].ones+nodes[r].ones);
	return it->second;
}

unordered_map<array<int, 3>, int> nxt;
int step(int a, int b, int c) {
	auto [it, add] = nxt.emplace(array<int, 3>{a,b,c}, 0);
	if(add) {
		int d = step(L(a), R(a), L(b));
		int e = step(R(a), L(b), R(b));
		int f = step(L(b), R(b), L(c));
		int g = step(R(b), L(c), R(c));
		it->second = get(step(d, e, f), step(e, f, g));
	}
	return it->second;
}

int main() {
	nxt[{0,0,0}] = 0;
	nxt[{0,0,1}] = 1;
	nxt[{0,1,0}] = 1;
	nxt[{0,1,1}] = 1;
	nxt[{1,0,0}] = 0;
	nxt[{1,0,1}] = 1;
	nxt[{1,1,0}] = 1;
	nxt[{1,1,1}] = 0;

	string s;
	ll N;
	cin >> s >> N;
	vector<int> conf, conf2;
	for(char c : s) conf.push_back(c-'0');

	int z = 0;
	for(; N; N>>=1) {
		if(N&1) {
			conf2.clear();
			conf2.push_back(step(z, z, conf[0]));
			for(int i = 0; i < conf.size(); ++i)
				conf2.push_back(step(i?conf[i-1]:z, conf[i], i+1<conf.size()?conf[i+1]:z));
			conf2.push_back(step(conf.back(), z, z));
			swap(conf, conf2);
		}
		conf.push_back(z);
		for(int i = 1; i < conf.size(); ++i) conf[i/2] = get(conf[i-1], conf[i]);
		conf.resize(conf.size()/2);
		z = get(z, z);
	}
	ll ans = 0;
	for(int a : conf) ans += nodes[a].ones;
	cout << ans << endl;
	cerr << nxt.size() << ' ' << S.size() << endl;

	return 0;
}