T = int(input())
for _ in range(T):
	n, k = map(int, input().split())
	a = list(map(int, input().split()))
	p = [True]*100
	bad = False
	for v in a:
		i, pk = 0, 1
		while pk <= v:
			i += 1
			pk *= k
		while v > 0 and i >= 0:
			if pk <= v:
				v -= pk
				if p[i]: p[i] = False
				else:
					bad = True
					break
			pk //= k
			i -= 1
		if v > 0: bad = True
		if bad: break
	print("NO" if bad else "YES")