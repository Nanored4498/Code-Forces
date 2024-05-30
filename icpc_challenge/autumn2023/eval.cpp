#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <numeric>
#include <cmath>
#include <tuple>
#include <iomanip>
#include <fstream>

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
array<Frame, MAXJ> frames;

constexpr double W = 192.;

int main(int argc, char* argv[]) {


	// INPUT
	ifstream in(argv[1]);
	in >> N >> K >> T >> R;
	for(int t = 0; t < T; ++t) {
		for(int k = 0; k < K; ++k) {
			for(int r = 0; r < R; ++r) {
				for(int n = 0; n < N; ++n) {
					in >> s0[t][k][r][n];
				}
			}
		}
	}
	for(int k = 0; k < K; ++k) {
		for(int r = 0; r < R; ++r) {
			for(int m = 0; m < N; ++m) {
				for(int n = 0; n < N; ++n) {
					in >> d[k][r][m][n];
				}
			}
		}
	}
	in >> J;
	for(int ind = 0; ind < J; ++ind) {
		int j;
		in >> j;
		Frame &f = frames[j];
		in >> f.size >> f.user >> f.t0 >> f.t1;
		f.t1 += f.t0;
	}

	// OUTPUT
	ifstream out(argv[2]);
	for(int t = 0; t < T; ++t) {
		for(int k = 0; k < K; ++k) {
			for(int r = 0; r < R; ++r) {
				for(int n = 0; n < N; ++n) {
					out >> p[t][k][r][n];
				}
			}
		}
	}

	// CHECK
	for(int t = 0; t < T; ++t) {
		for(int k = 0; k < K; ++k) {
			for(int r = 0; r < R; ++r) {
				for(int n = 0; n < N; ++n) {
					if(p[t][k][r][n] < 0.) {
						cerr << "Neg p " << p[t][k][r][n] << endl;
						exit(0);
					}
				}
			}
		}
	}
	for(int t = 0; t < T; ++t) {
		for(int k = 0; k < K; ++k) {
			double sp = 0.;
			for(int r = 0; r < R; ++r) {
				double sp2 = 0.;
				for(int n = 0; n < N; ++n) sp2 += p[t][k][r][n];
				if(sp2 > 4.00001) {
					cerr << "SP2 > 4\n";
					cerr << t << ' ' << k << ' ' << r << ' ' << sp2 << endl;
					exit(0);
				}
				sp += sp2;
			}
			if(sp > R+1e-5) {
				cerr << "SP > R\n";
				cerr << t << ' ' << k << ' ' << sp << ' ' << R << endl;
				exit(0);
			}
		}
	}

	int score = 0;
	for(int j = 0; j < J; ++j) {
		const Frame &f = frames[j];
		double g = 0.;
		const int n = f.user;
		for(int t = f.t0; t < f.t1; ++t) {
			for(int k = 0; k < K; ++k) {
				double snt = 1;
				int sb = 0;
				for(int r = 0; r < R; ++r) {
					if(p[t][k][r][n] > 0.) {
						++sb;
						double srnt = s0[t][k][r][n] * p[t][k][r][n];
						for(int m = 0; m < N; ++m) if(m != n && p[t][k][r][m] > 0.) {
							// cerr << "Case 1" << endl;
							srnt *= exp(d[k][r][m][n]);
						}
						double div = 1.;
						for(int l = 0; l < K; ++l) if(l != k) {
							for(int m = 0; m < N; ++m) if(m != n && p[t][l][r][m] > 0.) {
								// cerr << "Case 2" << endl;
								div += s0[t][l][r][n] * p[t][l][r][m] * exp(-d[l][r][m][n]);
							}
						}
						snt *= srnt / div;
					}
				}
				snt = pow(snt, 1./sb);
				g += sb * log2(1. + snt);
			}
		}
		g *= W;
		cout << g << ' ' << f.size << ' ' << (g >= f.size) << endl;
		if(g >= f.size) {
			++score;
		}
	}

	double sp = 0.;
	for(int t = 0; t < T; ++t) {
		for(int k = 0; k < K; ++k) {
			for(int r = 0; r < R; ++r) {
				for(int n = 0; n < N; ++n) {
					sp += p[t][k][r][n];
				}
			}
		}
	}

	cout << (score - 1e-6*sp) << endl;
	
	return 0;
}