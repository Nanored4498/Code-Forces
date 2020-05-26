t = int(input())

for _ in range(t):
	n = int(input())
	b, m = 1, 1
	res = []
	while True:
		if 2*b+m >= n:
			res.append(n - (m+b))
			break
		add = min(b, (n - (2*b+m)) // 2)
		res.append(add)
		b += add
		m += b
	d = len(res)
	print(d)
	print(" ".join(str(a) for a in res))