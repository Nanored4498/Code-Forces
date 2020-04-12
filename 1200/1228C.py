MOD = 1000000007

x, n = map(int, input().split())

def po(n, p):
	if p == 0: return 1
	if p == 1: return n % MOD
	p2 = p//2
	if p % 2 == 0: return (po(n, p2)**2) % MOD
	else: return (n * po(n, p2)**2) % MOD

def f(s, x, n):
	if x == 1: return 1
	sx = int(x**0.5)
	for p in range(s, sx+1):
		if x % p == 0:
			while x % p == 0:
				x //= p
			res = f(p+1, x, n)
			m = 0
			pp = p
			while n // pp > 0:
				m = (m + n // pp)
				pp *= p
			pm = po(p, m)
			res = (res*pm) % MOD
			return res
	m = 0
	pp = x
	while n // pp > 0:
		m = (m + n // pp)
		pp *= x
	return po(x, m) % MOD

res = f(2, x, n)
print(res)