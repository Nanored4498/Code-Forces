#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace std;
using ll = long long;
using vi = vector<int>;

template<typename T>
struct Point {
	using Vec = Point;
	T x, y;
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

	bool operator==(const Point &p) { return x == p.x && y == p.y; }
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
vector<pt> convex_hull(vector<pt>& pts) {
	if(pts.size() < 3) return pts;
	pt p0 = *min_element(pts.begin(), pts.end(), [](const pt &a, const pt &b) {
		return make_pair(a.y, a.x) < make_pair(b.y, b.x);
	});
	for(pt &a : pts) a -= p0; 
	sort(pts.begin(), pts.end(), [&p0](const pt& a, const pt& b) {
		auto o = det(a, b);
		if(o == 0) return norm2(a) < norm2(b);
		return o < 0;
	});
	for(pt &a : pts) a += p0; 
	vector<pt> st;
	for(const pt &a : pts) {
		while(st.size() > 1 && orientation(st[st.size()-2], st.back(), a) >= 0)
			st.pop_back();
		st.push_back(a);
	}
	return st;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<int> a(n);
	for(int &x : a) cin >> x;
	sort(a.begin(), a.end());
	cin >> n;
	vector<int> b(n);
	for(int &x : b) cin >> x;
	sort(b.begin(), b.end());

	{ // Maybe swap
		ll score = 0;
		int i = 0, i2 = 0;
		for(int f : b) {
			while(i < a.size() && a[i] < f) ++i;
			while(i2 < a.size() && a[i2] <= f) ++i2;
			score += i + i2;
		}
		if(score > a.size()*b.size()) swap(a, b);
	} // a is D1

	vi ab;
	for(vi *a : {&a, &b}) for(int x : *a) {
		if(x > 1) ab.push_back(x-1);
		ab.push_back(x);
		ab.push_back(x+1);
	}
	sort(ab.begin(), ab.end());
	ab.erase(unique(ab.begin(), ab.end()), ab.end());
	vector<Point<ll>> X;
	int i = 0, j = 0;
	for(int f : ab) {
		while(i < a.size() && a[i] < f) ++i;
		int i2 = i;
		while(i2 < a.size() && a[i2] <= f) ++i2;
		while(j < b.size() && b[j] < f) ++j;
		int j2 = j;
		while(j2 < b.size() && b[j2] <= f) ++j2;
		Point<ll> p = {i+i2, j+j2};
		if(!X.empty() && X.back() == p) continue;
		X.push_back(p);
	}

	auto ch = convex_hull(X);
	double ans0 = 1., ans1 = 0.;
	for(int i = 0; i < (int) ch.size(); ++i) {
		const auto &u = ch[i];
		const auto &v = ch[(i+1)%(int)ch.size()];
		// First value
		const ll dux = u.x - (ll) a.size();
		const ll dvx = v.x - (ll) a.size();
		if(dux >= 0) ans0 = min(ans0, (double)u.y/(2*b.size()));
		if(dux*dvx < 0) {
			double t = dvx / double(v.x - u.x);
			ans0 = min({ans0, (t * u.y + (1.-t) * v.y) / (2*b.size())});
		}
		// Second value
		const ll duy = u.y - (ll) b.size();
		const ll dvy = v.y - (ll) b.size();
		if(duy <= 0) ans1 = max(ans1, (double)u.x/(2*a.size()));
		if(duy*dvy < 0) {
			double t = dvy / double(v.y - u.y);
			ans1 = max({ans1, (t * u.x + (1.-t) * v.x) / (2*a.size())});
		}
	}
	cout << fixed << setprecision(7) << ans0 << ' ' << ans1 << '\n';

	return 0;
}