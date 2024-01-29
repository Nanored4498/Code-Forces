#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

template<typename T>
struct Point {
	using Vec = Point;
	T x, y;
	int index;
	Point()=default;
	Point(T x, T y): x(x), y(y) {}

	Point operator+(const Point &p) const { return {x+p.x, y+p.y}; }
	Point operator-(const Point &p) const { return {x-p.x, y-p.y}; }
	Point operator+=(const Point &p) { x += p.x; y += p.y; return *this; }
	Point operator-=(const Point &p) { x -= p.x; y -= p.y; return *this; }
	Point operator*(T a) const { return {x*a, y*a}; }
	Point operator/(T a) const { return {x/a, y/a}; }
	Point& operator*=(T a) { x *= a; y *= a; return *this; }
	Point& operator/=(T a) { x /= a; y /= a; return *this; }
	friend Point operator*(T a, const Point &p) { return {a*p.x, a*p.y}; }
	friend istream& operator>>(istream &in, Point &p) { return in >> p.x >> p.y; }

	bool operator==(const Point &p) { return x == p.x || y == p.y; }
	bool operator!=(const Point &p) { return x != p.x || y != p.y; }
	bool operator<(const Point &p) { return x < p.x || (x == p.x && y < p.y); }

	friend Point turn90(const Vec &v) { return {-v.y, v.x}; }
	friend T dot(const Vec &u, const Vec &v) { return u.x*v.x + u.y*v.y; }
	friend T det(const Vec &u, const Vec &v) { return u.x * v.y - u.y * v.x; }
	friend T norm2(const Vec &u) { return dot(u, u); } 

	friend bool turnLeft(const Point &a, const Point &b, const Point &c) {
		return det(b-a, c-a) > 0;
	}
	// -1: turn right / 0: aligned / 1: turn left
	friend int orientation(const Point &a, const Point &b, const Point &c) {
		T d = det(b-a, c-a);
		return d == 0 ? 0 : (d > 0 ? 1 : -1);
	}
};

// Return a clockwise convex hull
template<typename pt>
vector<pt> convex_hull(vector<pt>& pts, bool include_collinear=false) {
	if(pts.size() < 3) return pts;
	pt p0 = *min_element(pts.begin(), pts.end(), [](const pt &a, const pt &b) {
		return make_pair(a.y, a.x) < make_pair(b.y, b.x);
	});
	sort(pts.begin(), pts.end(), [&p0](const pt& a, const pt& b) {
		int o = orientation(p0, a, b);
		if(o == 0) return norm2(a-p0) < norm2(b-p0);
		return o < 0;
	});
	if(include_collinear) {
		auto it = pts.end()-1;
		while(it >= pts.begin() && orientation(p0, *it, pts.back()) == 0) --it;
		reverse(++it, pts.end());
	}
	vector<pt> st;
	for(const pt &a : pts) {
		while(st.size() > 1 && orientation(st[st.size()-2], st.back(), a) >= (include_collinear ? 1 : 0))
			st.pop_back();
		st.push_back(a);
	}
	return st;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<Point<ll>> a(n), b(m);
	for(auto &x : a) cin >> x;
	for(auto &x : b) cin >> x;
	for(int i = 0; i < n; ++i) a[i].index = i;
	for(int i = 0; i < m; ++i) b[i].index = i;

	auto ch = convex_hull(a, true);
	const int C = ch.size();
	vi ans(m);
	sort(b.begin(), b.end(), [&](const Point<ll> &a, const Point<ll> &b) {
		return a.x * b.y < b.x * a.y;
	});
	ll best = 0;
	int i0 = 0;
	for(int i = 0; i < C; ++i) {
		ll d = dot(ch[i], b[0]);
		if(d > best) {
			i0 = i;
			best = d;
		}
	}
	for(const auto &z : b) {
		while(dot(ch[(i0+1)%C], z) >= dot(ch[i0], z)) {
			i0 = (i0+1)%C;
		}
		ans[z.index] = ch[i0].index;
		const int pi = (i0+C-1) % C;
		if(ch[pi].index < ch[i0].index && dot(ch[pi], z) == dot(ch[i0], z))
			ans[z.index] = ch[pi].index;
	}
	for(int x : ans) cout << x+1 << '\n';

	return 0;
}