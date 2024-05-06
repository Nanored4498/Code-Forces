#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

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

	friend Point turn90(const Vec &v) { return {-v.y, v.x}; }
	friend T dot(const Vec &u, const Vec &v) { return u.x*v.x + u.y*v.y; }
	friend T norm2(const Vec &u) { return dot(u, u); }
	friend T norm(const Vec &u) { return sqrt(norm2(u)); }

	friend bool intersect(const Point &a, const Point &b, const Point &c, const Point &d) {
		const Vec u = b-a;
		const Vec v = d-c;
		const Vec u_orth = turn90(u);
		const Vec v_orth = turn90(v);

		const T ac_v_orth = dot(c-a, v_orth);
		const T u_v_orth = dot(u, v_orth);
		if(u_v_orth == 0) {
			if(ac_v_orth != 0) return false; 
		} else if(u_v_orth > 0) {
			if(ac_v_orth < 0 || ac_v_orth > u_v_orth) return false;
		} else {
			if(ac_v_orth > 0 || ac_v_orth < u_v_orth) return false;
		}

		const T ca_u_orth = dot(a-c, u_orth);
		const T v_u_orth = - u_v_orth;
		if(v_u_orth == 0) {
			if(ca_u_orth != 0) return false; 
		} else if(v_u_orth > 0) {
			if(ca_u_orth < 0 || ca_u_orth > v_u_orth) return false;
		} else {
			if(ca_u_orth > 0 || ca_u_orth < v_u_orth) return false;
		}

		return true;
	}
};

int main() {
	pair<double, vector<Point<double>>> rs[2];
	for(auto &[h,r] : rs) {
		double x, y, x2, y2;
		cin >> x >> y >> x2 >> y2 >> h;
		r.emplace_back(x, y);
		r.emplace_back(x2, y2);
		double dx = x2-x, dy = y2-y;
		r.emplace_back(x2-dy, y2+dx);
		r.emplace_back(x-dy, y+dx);
	}

	double ans = 1e9;
	for(int i = 0; i < 4; ++i) for(int j = 0; j < 4; ++j) {
		double ah = rs[0].first;
		auto a = rs[0].second[i];
		auto a2 = rs[0].second[(i+1)%4];
		double bh = rs[1].first;
		auto b = rs[1].second[j];
		auto b2 = rs[1].second[(j+1)%4];
		auto av = a2-a;
		double la = norm(av);
		auto ac = (a+a2)/2. + (sqrt(ah*ah + la*la/4.) / la) * turn90(av);
		auto bv = b2-b;
		double lb = norm(bv);
		auto bc = (b+b2)/2. + (sqrt(bh*bh + lb*lb/4.) / lb) * turn90(bv);
		ans = min(ans, norm(a-ac) + norm(b-a) + norm(bc-b));
		if(intersect(a, a2, ac, b)) ans = min(ans, norm(b-ac) + norm(bc-b));
		if(intersect(b, b2, bc, a)) ans = min(ans, norm(a-ac) + norm(bc-a));
		if(intersect(a, a2, bc, ac) && intersect(b, b2, bc, ac)) ans = min(ans, norm(bc-ac));
	}
	cout << fixed << setprecision(9) << ans << endl;

	return 0;
}