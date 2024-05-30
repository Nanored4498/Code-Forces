#include <vector>
#include <algorithm>
#include <array>
#include <numeric>
#include <cmath>
#include <tuple>
#include <iomanip>
#include <random>
#include <cstdio>
#include <queue>

#pragma GCC diagnostic ignored "-Wunused-result"

using namespace std;

struct Frame {
	int size, user, t0, t1;
};

// 1 <= N <= 100: number of users
// 1 <= K <= 10: number of cells
// 1 <= T <= 1000: number of time steps
// 1 <= R <= 10: number of RBG
// 1 <= J <= 5000: number of frames
constexpr int MAXN = 100, MAXK = 10, MAXT = 1000, MAXR = 10, MAXJ = 5000;
int N, K, T, R, J;
array<array<array<array<double, MAXN>, MAXR>, MAXK>, MAXT> s0;
array<array<array<array<double, MAXN>, MAXR>, MAXK>, MAXT> p;
array<array<array<array<double, MAXN>, MAXN>, MAXR>, MAXK> d;
array<array<double, MAXK>, MAXT> Rk;
array<Frame, MAXJ> frames;
array<bool, MAXJ> success;
array<array<bool, MAXR>, MAXT> usedR;

constexpr double W = 192.;

void read() {
	scanf("%d %d %d %d", &N, &K, &T, &R);
	for(int t = 0; t < T; ++t) {
		for(int k = 0; k < K; ++k) {
			for(int r = 0; r < R; ++r) {
				for(int n = 0; n < N; ++n) {
					scanf("%lf", &s0[t][k][r][n]);
				}
			}
		}
	}
	for(int k = 0; k < K; ++k) {
		for(int r = 0; r < R; ++r) {
			for(int m = 0; m < N; ++m) {
				for(int n = 0; n < N; ++n) {
					scanf("%lf", &d[k][r][m][n]);
				}
			}
		}
	}
	scanf("%d", &J);
	for(int ind = 0; ind < J; ++ind) {
		int j;
		scanf("%d", &j);
		Frame &f = frames[j];
		scanf("%d %d %d %d", &f.size, &f.user, &f.t0, &f.t1);
		f.t1 += f.t0;
	}
	for(int t = 0; t < T; ++t) for(int k = 0; k < K; ++k) {
		Rk[t][k] = R-1e-5;
	}
}

int main() {
	read();
	int score = 0;

	vector<vector<int>> fst(T);
	for(int j = 0; j < J; ++j) {
		for(int t = frames[j].t0; t < frames[j].t1; ++t) {
			fst[t].push_back(j);
		}
	}

	for(int t = 0; t < T; ++t) if(!fst[t].empty()) {
		using QEl = tuple<int, int, vector<pair<int, double>>, double>;
		const auto cmp = [](const QEl &a, const QEl &b) {
			return get<2>(a).size() < get<2>(b).size()
				|| (get<2>(a).size() == get<2>(b).size() && get<3>(a) > get<3>(b));
		};
		priority_queue<QEl, vector<QEl>, decltype(cmp)> Q(cmp);
		vector<vector<pair<int, vector<pair<int, double>>>>> horizon(K);
		const auto addKR = [&](int k, int r) {
			vector<pair<int,double>> fs;
			vector<int> rfs;
			for(int j : fst[t]) if(!success[j]) rfs.push_back(j);
			while(true) {
				int bi = -1;
				double bpp;
				pair<double, double> brp {-1., -1.};
				for(int i = 0; i < (int) rfs.size(); ++i) {
					const Frame &f = frames[rfs[i]];
					const int n = f.user;
					double s = s0[t][k][r][n];
					double led = 0.;
					double rp = min(4., Rk[t][k]);
					for(auto [j, pj] : fs) {
						led += d[k][r][n][frames[j].user];
						rp -= pj * exp(-d[k][r][n][frames[j].user]);
					}
					s *= exp(led);
					const double pp = (pow(2., (f.size+.1)/W) - 1.) / s;
					if(pp > rp) {
						rfs[i] = rfs.back();
						rfs.pop_back();
						--i;
						continue;
					}
					rp -= pp;
					pair<double, double> rp21 = {-1., rp};
					for(int i2 = 0; i2 < (int) rfs.size(); ++i2) if(i2 != i) {
						const Frame &f2 = frames[rfs[i2]];
						const int n2 = f2.user;
						double s2 = s0[t][k][r][n2];
						double led2 = d[k][r][n][n2];
						double rp2 = min(4., Rk[t][k]) - pp * exp(-d[k][r][n][n2]);
						for(auto [j, pj] : fs) {
							led2 += d[k][r][n2][frames[j].user];
							rp2 -= pj * exp(-d[k][r][n][frames[j].user] -d[k][r][n2][frames[j].user]);
						}
						s2 *= exp(led2);
						const double pp2 = (pow(2., (f2.size+.1)/W) - 1.) / s2;
						if(pp2 > rp2) continue;
						rp2 -= pp2;
						rp21.first = max(rp21.first, rp2);
					}
					if(rp21 > brp) {
						bi = i;
						bpp = pp;
						brp = rp21;
					}
				}
				if(bi == -1) break;
				const int j = rfs[bi];
				rfs[bi] = rfs.back();
				rfs.pop_back();
				for(auto &[j2, pj2] : fs) pj2 *= exp(-d[k][r][frames[j].user][frames[j2].user]);
				fs.emplace_back(j, bpp);
			}
			if(fs.size() <= 1) return;
			double sp = 0.;
			for(const auto &[j, pj] : fs) sp += pj;
			Q.emplace(k, r, fs, sp);
		};
		for(int k = 0; k < K; ++k)
			for(int r = 0; r < R; ++r)
				addKR(k, r);
		while(!Q.empty()) {
			const auto [k, r, fs, sp] = move(Q.top());
			Q.pop();
			if(usedR[t][r]) continue;
			if(sp > Rk[t][k]) {
				addKR(k, r);
				continue;
			}
			bool bad = false;
			for(auto [j, pj] : fs) if(success[j]) {
				bad = true;
				break;
			}
			if(bad) {
				addKR(k, r);
				continue;
			}
			usedR[t][r] = true;
			for(auto [j, pj] : fs) {
				++ score;
				success[j] = true;
				p[t][k][r][frames[j].user] = pj;
			}
			Rk[t][k] -= sp;
			horizon[k].emplace_back(r, fs);
		}

		vector<int> rfs;
		for(int j : fst[t]) if(!success[j]) rfs.push_back(j);
		vector<vector<tuple<int, int, double>>> singles(K);
		for(int r = 0; r < R; ++r) if(!usedR[t][r]) {
			for(int k = 0; k < K; ++k) for(int j : rfs) {
				const Frame &f = frames[j];
				const int n = f.user;
				double s = s0[t][k][r][n];
				const double rp = min(4., Rk[t][k]);
				const double pp = (pow(2., (f.size+.1)/W) - 1.) / s;
				if(pp > rp) continue;
				singles[k].emplace_back(r, j, pp);
			}
		}
		struct QEl3 {
			double sp, p1, p2, p3, p4;
			int k, r, k2, r2, j, j2, j3;
		};
		vector<QEl3> Q3;
		for(int k = 1; k < K; ++k) for(const auto &[r, j, pp] : singles[k]) {
			for(int k2 = 0; k2 < k; ++k2) for(const auto &[r2, j2, pp2] : singles[k2]) if(j != j2 && r != r2) {
				for(int j3 : rfs) if(j != j3 && j2 != j3) {
					const Frame &f3 = frames[j3];
					const double ed = exp(d[k][r][frames[j].user][f3.user]);
					const double ppd = pp/ed;
					const double rp = min(4., Rk[t][k]) - ppd;
					if(rp <= 0.) continue;
					const double ed2 = exp(d[k2][r2][frames[j2].user][f3.user]);
					const double ppd2 = pp2/ed2;
					const double rp2 = min(4., Rk[t][k2]) - ppd2;
					if(rp2 <= 0.) continue;
					const double s = s0[t][k][r][f3.user] * ed;
					const double s2 = s0[t][k2][r2][f3.user] * ed2;
					if(W * (log2(1. + s*rp) + log2(1. + s2*rp2)) < f3.size+.1) continue;
					double C = (f3.size+.1) / W - log2(s*s2);
					double Lambda = pow(2., C/2);
					double ppp = Lambda - 1/s, ppp2;
					if(ppp > rp) {
						Lambda *= Lambda * s / (1. + s*rp);
						ppp = rp;
						ppp2 = Lambda - 1/s2;
					} else if((ppp2 = Lambda - 1/s2) > rp2) {
						Lambda *= Lambda * s2 / (1. + s2*rp2);
						ppp = Lambda - 1/s;
						ppp2 = rp2;
					}
					const double sp = ppp + ppp2 + ppd + ppd2;
					Q3.push_back({sp, ppd, ppd2, ppp, ppp2, k, r, k2, r2, j, j2, j3});
				}
			}
		}
		ranges::sort(Q3, less<double>{}, [&](const QEl3 a) { return a.sp; });
		for(const auto &[sp, ppd, ppd2, ppp, ppp2, k, r, k2, r2, j, j2, j3] : Q3) if(!success[j] && !success[j2] && !success[j3] && !usedR[t][r] && !usedR[t][r2]) {
			if(ppp + ppd > Rk[t][k]) continue;
			if(ppp2 + ppd2 > Rk[t][k2]) continue;
			success[j] = success[j2] = success[j3] = true;
			score += 3;
			usedR[t][r] = usedR[t][r2] = true;
			Rk[t][k] -= (p[t][k][r][frames[j3].user] = ppp) + (p[t][k][r][frames[j].user] = ppd);
			Rk[t][k2] -= (p[t][k2][r2][frames[j3].user] = ppp2) + (p[t][k2][r2][frames[j2].user] = ppd2);
			horizon[k].emplace_back(r, vector<pair<int, double>>{{j, ppd}, {j3, ppp}});
			horizon[k2].emplace_back(r2, vector<pair<int, double>>{{j2, ppd2}, {j3, ppp2}});
		}

		for(int j : rfs) if(!success[j]) {
			const Frame &f = frames[j];
			const int n = f.user;
			double g = 0.;
			vector<tuple<int, int, double, double>> KI;
			for(int k = 0; k < K; ++k) {
				double g_add = 0.;
				KI.emplace_back(k, -1, 0., 0.);
				for(int i = 0; i < (int) horizon[k].size(); ++i) {
					const auto &[r, fs] = horizon[k][i];
					double rp = Rk[t][k];
					for(auto [j2, pj2] : fs) rp += pj2;
					rp = min(4., rp);
					double s = s0[t][k][r][n];
					double led = 0.;
					for(auto [j2, pj2] : fs) {
						rp -= pj2 * exp(-d[k][r][n][frames[j2].user]);
						led += d[k][r][n][frames[j2].user];
					}
					s *= exp(led);
					if(rp < 0.) continue;
					const double gg = log2(1. + s*rp);
					if(gg <= g_add) continue;
					g_add = gg;
					KI.back() = {k, i, s, rp};
				}
				if(get<1>(KI.back()) == -1) {
					KI.pop_back();
					continue;
				} 
				g += g_add;
			}
			if(W*g < f.size+.1) continue;
			vector<double> ps(K);
			vector<bool> clampedL(K, true), clampedH(K, false);
			double C = (f.size+.1)/W;
			int K2 = 0;
			bool newClamp;
			double Lambda;
			clamp_start9:
			for(const auto &[k, i, s, rp] : KI) if(clampedL[k]) {
				C -= log2(s);
				++ K2;
				clampedL[k] = false;
			}
			clamp_low9:
				newClamp = false;
				Lambda = pow(2., C / K2);
				for(const auto &[k, i, s, rp] : KI) if(!clampedL[k] && !clampedH[k]) {
					const double pp = Lambda - 1./s;
					if(pp <= 0.) {
						newClamp = clampedL[k] = true;
						C += log2(s);
						-- K2;
						ps[k] = 0.;
					}
				}
				if(newClamp) goto clamp_low9;
			clamp_high9:
				for(const auto &[k, i, s, rp] : KI) if(!clampedL[k] && !clampedH[k]) {
					const double pp = Lambda - 1./s;
					if(pp > rp) {
						clampedH[k] = true;
						C += log2(s / (1. + s * rp));
						-- K2;
						ps[k] = rp;
						goto clamp_start9;
					}
				}
			set_other9:
				for(const auto &[k, i, s, rp] : KI) if(!clampedL[k] && !clampedH[k]) {
					const double pp = Lambda - 1./s;
					ps[k] = pp;
				}
			++score;
			success[j] = true;
			for(const auto &[k, i, s, rp] : KI) if(ps[k] > 0.) {
				auto &[r, fs] = horizon[k][i];
				for(auto &[j2, pj2] : fs) {
					Rk[t][k] += pj2;
					p[t][k][r][frames[j2].user] = pj2 *= exp(-d[k][r][n][frames[j2].user]);
					Rk[t][k] -= pj2;
				}
				Rk[t][k] -= p[t][k][r][n] = ps[k];
				fs.emplace_back(j, ps[k]);
			}
		}

		using QEl2 = tuple<double, int, int, vector<double>>;
		const auto cmp2 = [](const QEl2 &a, const QEl2 &b) {
			return get<0>(a) > get<0>(b);
		};
		priority_queue<QEl2, vector<QEl2>, decltype(cmp2)> Q2(cmp2);
		const auto addJR = [&](int j, int r) {
			const Frame &f = frames[j];
			const int n = f.user;
			double g = 0.;
			int nr = 0;
			for(int k = 0; k < K; ++k) {
				const double mp = min(4., Rk[t][k]);
				g += log2(1. + s0[t][k][r][n] * mp);
			}
			if(W*g < f.size+.1) return;
			double sp = 0.;
			vector<double> ps(K);
			array<bool, MAXK> clampedL, clampedH;
			fill_n(clampedL.begin(), K, true);
			fill_n(clampedH.begin(), K, false);
			double C = (f.size+.1)/W;
			int K2 = 0;
			bool newClamp;
			double Lambda;
			clamp_start1:
			for(int k = 0; k < K; ++k) if(clampedL[k]) {
				C -= log2(s0[t][k][r][n]);
				++ K2;
				clampedL[k] = false;
			}
			clamp_low1:
				newClamp = false;
				Lambda = pow(2., C / K2);
				for(int k = 0; k < K; ++k) if(!clampedL[k] && !clampedH[k]) {
					const double pp = Lambda - 1./s0[t][k][r][n];
					if(pp <= 0.) {
						newClamp = clampedL[k] = true;
						C += log2(s0[t][k][r][n]);
						-- K2;
						ps[k] = 0.;
					}
				}
				if(newClamp) goto clamp_low1;
			clamp_high1:
				for(int k = 0; k < K; ++k) if(!clampedL[k] && !clampedH[k]) {
					const double mp = min(4., Rk[t][k]);
					const double pp = Lambda - 1./s0[t][k][r][n];
					if(pp > mp) {
						clampedH[k] = true;
						C += log2(s0[t][k][r][n] / (1. + s0[t][k][r][n] * mp));
						-- K2;
						ps[k] = mp;
						sp += mp;
						goto clamp_start1;
					}
				}
			set_other1:
				for(int k = 0; k < K; ++k) if(!clampedL[k] && !clampedH[k]) {
					const double pp = Lambda - 1./s0[t][k][r][n];
					ps[k] = pp;
					sp += pp;
				}
			Q2.emplace(sp, j, r, ps);
		};
		for(int j : rfs) if(!success[j])
			for(int r = 0; r < R; ++r) if(!usedR[t][r])
				addJR(j, r);
		while(!Q2.empty()) {
			const auto [sp, j, r, ps] = move(Q2.top());
			Q2.pop();
			if(success[j] || usedR[t][r]) continue;
			bool bad = false;
			for(int k = 0; k < K; ++k) {
				const double mp = min(4., Rk[t][k]);
				if(ps[k] > mp) {
					bad = true;
					break;
				}
			}
			if(bad) {
				addJR(j, r);
				continue;
			}
			++ score;
			success[j] = true;
			usedR[t][r] = true;
			for(int k = 0; k < K; ++k) {
				Rk[t][k] -= ps[k];
				p[t][k][r][frames[j].user] = ps[k];
			}
		}
	
	}
	fprintf(stderr, "%d\n", score);

	vector<int> orderJ;
	for(int j = 0; j < J; ++j) if(!success[j]) orderJ.push_back(j);
	ranges::sort(orderJ, less<int>{}, [&](int j) { return frames[j].t1; });
	vector<tuple<vector<int>, double, double>> addt(K), addt2(K);
	for(int i = 0; i < orderJ.size(); ++i) {
		pair<int, double> bnrsp {1e9, 1e9};
		int bj = -1;
		vector<tuple<int, int, int, double>> badd;
		int t1 = frames[orderJ[i]].t0 + min(3, (frames[orderJ[i]].t1 - frames[orderJ[i]].t0)/5);
		int tested = 0;
		for(int i2 = 0; i2 < orderJ.size() && frames[orderJ[i2]].t0 <= t1 && tested < 80; ++i2) if(!success[orderJ[i2]]) {
			const int j = orderJ[i2];
			const Frame &f = frames[j];
			const int &n = f.user;
			double g = 0.;
			vector<tuple<int, int, int, double>> add;
			pair<int, double> bnrspi {frames[orderJ[i2]].t0-frames[orderJ[i]].t0, 0};
			bool impossible = true;
			for(int t = f.t0; t < f.t1; ++t) {
				vector<int> rs;
				for(int r = 0; r < R; ++r) if(!usedR[t][r]) rs.push_back(r);
				const int R2 = rs.size();
				if(!R2) continue;
				double g_add = 0.;
				for(int k = 0; k < K; ++k) {
					ranges::sort(rs, greater<double>{}, [&](int r) { return s0[t][k][r][n]; });
					int bestU = 0;
					double best = 0.;
					double mr = 1.;
					for(int u = 1; u <= R2; ++u) {
						mr *= s0[t][k][rs[u-1]][n];
						const double p = min(4., Rk[t][k] / u);
						const double ms0k = pow(mr, 1./u);
						const double gk = u * log2(1 + ms0k * p);
						if(gk > best) {
							bestU = u;
							best = gk;
						}
					}
					get<0>(addt[k]).assign(rs.begin(), rs.begin()+bestU);
					g_add += best;
				}
				if(W*(g+g_add) >= f.size+.1) {
					impossible = false;
					double bsp = 1e9;
					int brs2 = -1;
					for(int nr = 1; nr <= R2; ++nr) {
						if(pair<int, double>{bnrspi.first+nr, bnrspi.second} >= bnrsp) {
							break;
						}
						const auto nxt = [](int rs2) {
							const int c = rs2 & (-rs2);
							const int s = rs2 + c;
							return (((s^rs2) >> 2) / c) | s;
						};
						for(int rs2 = (1<<nr)-1; rs2 < (1<<R2); rs2 = nxt(rs2)) {
							vector<int> orderR;
							for(int i = 0; i < R2; ++i) if((rs2>>i&1)) orderR.push_back(rs[i]);
							g_add = 0.;
							for(int k = 0; k < K; ++k) {
								ranges::sort(orderR, greater<double>{}, [&](int r) { return s0[t][k][r][n]; });
								int bestU = 0;
								double best = 0.;
								double mr = 1.;
								get<1>(addt[k]) = 1.;
								for(int u = 1; u <= nr; ++u) {
									mr *= s0[t][k][orderR[u-1]][n];
									const double p = min(4., Rk[t][k] * nr / (u*R2));
									const double ms0k = pow(mr, 1./u);
									const double gk = u * log2(1 + ms0k * p);
									if(gk > best) {
										bestU = u;
										best = gk;
										get<1>(addt[k]) = ms0k;
									}
								}
								get<0>(addt[k]).assign(orderR.begin(), orderR.begin()+bestU);
								g_add += best;
							}
							if(W*(g+g_add) < f.size+.1) continue;
							double sp = 0.;
							vector<bool> clampedL(K, true), clampedH(K, false);
							double C = (f.size+.1)/W - g;
							int SU = 0;
							bool newClamp;
							double Lambda;
							clamp_start:
							for(int k = 0; k < K; ++k) if(clampedL[k]) {
								const int u = get<0>(addt[k]).size();
								C -= u * log2(get<1>(addt[k]));
								SU += u;
								clampedL[k] = false;
							}
							clamp_low:
								newClamp = false;
								Lambda = pow(2., C / SU);
								for(int k = 0; k < K; ++k) if(!clampedL[k] && !clampedH[k]) {
									const double pp = Lambda - 1./get<1>(addt[k]);
									if(pp <= 0.) {
										const int u = get<0>(addt[k]).size();
										newClamp = clampedL[k] = true;
										C += u * log2(get<1>(addt[k]));
										SU -= u;
										get<2>(addt[k]) = 0.;
									}
								}
								if(newClamp) goto clamp_low;
							clamp_high:
								for(int k = 0; k < K; ++k) if(!clampedL[k] && !clampedH[k]) {
									const int u = get<0>(addt[k]).size();
									const double mp = min(4., Rk[t][k] / u);
									const double pp = Lambda - 1./get<1>(addt[k]);
									if(pp > mp) {
										clampedH[k] = true;
										C += u * log2(get<1>(addt[k]) / (1. + get<1>(addt[k]) * mp));
										SU -= u;
										get<2>(addt[k]) = mp;
										sp += u * mp;
										goto clamp_start;
									}
								}
							set_other:
								for(int k = 0; k < K; ++k) if(!clampedL[k] && !clampedH[k]) {
									const int u = get<0>(addt[k]).size();
									const double pp = Lambda - 1./get<1>(addt[k]);
									get<2>(addt[k]) = pp;
									sp += u * pp;
								}
							if(sp < bsp) {
								bsp = sp;
								brs2 = rs2;
								swap(addt, addt2);
							}
						}
						if(bsp != 1e9) break;
					}
					if(brs2 == -1) break;
					for(int k = 0; k < K; ++k)
						for(int r : get<0>(addt2[k])) {
							add.emplace_back(t, k, r, get<2>(addt2[k]));
							bnrspi.second += get<2>(addt2[k]);
						}
					for(int i = 0; i < R2; ++i) if((brs2>>i)&1) ++bnrspi.first;
					if(bnrspi < bnrsp) {
						badd = add;
						bj = j;
						bnrsp = bnrspi;
					}
					break;
				}
				for(int k = 0; k < K; ++k) {
					const int u = get<0>(addt[k]).size();
					const double p = min(4., Rk[t][k] / u);
					for(int r : get<0>(addt[k])) add.emplace_back(t, k, r, p);
					bnrspi.second += u*p;
				}
				bnrspi.first += rs.size();
				g += g_add;
			}
			if(impossible) success[j] = true;
			else ++tested;
		}
		if(bj != -1) {
			++ score;
			success[bj] = true;
			for(const auto &[t, k, r, pp] : badd) {
				p[t][k][r][frames[bj].user] = pp;
				Rk[t][k] -= pp;
				usedR[t][r] = true;
			}
		}
	}
	fprintf(stderr, "%d\n", score);

	// OUTPUT
	for(int t = 0; t < T; ++t) {
		for(int k = 0; k < K; ++k) {
			for(int r = 0; r < R; ++r) {
				for(int n = 0; n < N; ++n) {
					printf("%.7lf ", p[t][k][r][n]);
				}
				printf("\n");
			}
		}
	}
	
	return 0;
}