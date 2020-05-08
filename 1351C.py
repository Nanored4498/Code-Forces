#https://codeforces.com/contest/1351/problem/C
t = int(input())
M = int(10**5)
for _ in range(t):
	p = input()
	e = set()
	x, y = 0, 0
	res = 0
	for d in p:
		a, b = x, y
		if d == 'N': b += 1
		elif d == 'S': b -= 1
		elif d == 'E': a += 1
		else: a -= 1
		u, v = x*M+y, a*M+b
		if (u, v) in e or (v, u) in e:
			res += 1
		else:
			e.add((u, v))
			res += 5
		x, y = a, b
	print(res)